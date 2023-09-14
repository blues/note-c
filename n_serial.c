/*!
 * @file n_serial.c
 *
 * Written by Ray Ozzie and Blues Inc. team.
 *
 * Copyright (c) 2019 Blues Inc. MIT License. Use of this source code is
 * governed by licenses granted by the copyright holder including that found in
 * the
 * <a href="https://github.com/blues/note-c/blob/master/LICENSE">LICENSE</a>
 * file.
 *
 */

#include "n_lib.h"

/**************************************************************************/
/*!
  @brief  Given a JSON string, perform a serial transaction with the Notecard.
  @param   request
            A c-string containing the JSON request object.
  @param   response
            An out parameter c-string buffer that will contain the JSON
            response from the Notercard. If NULL, no response will be captured.
  @returns a c-string with an error, or `NULL` if no error ocurred.
*/
/**************************************************************************/
const char *serialNoteTransaction(char *request, char **response)
{
    const char *err = serialChunkedTransmit((uint8_t *)request, strlen(request), true);
    if (err) {
        NOTE_C_LOG_ERROR(err);
        return err;
    }

    // Append newline to the transaction
    _SerialTransmit((uint8_t *)c_newline, c_newline_len, true);

    // If no reply expected, we're done
    if (response == NULL) {
        return NULL;
    }

    // Wait for something to become available, processing timeout errors
    // up-front because the json parse operation immediately following is
    // subject to the serial port timeout. We'd like more flexibility in max
    // timeout and ultimately in our error handling.
    for (const uint32_t startMs = _GetMs(); !_SerialAvailable(); ) {
        if (_GetMs() - startMs >= NOTECARD_TRANSACTION_TIMEOUT_SEC*1000) {
#ifdef ERRDBG
            NOTE_C_LOG_ERROR(ERRSTR("reply to request didn't arrive from module in time", c_iotimeout));
#endif
            return ERRSTR("transaction timeout {io}", c_iotimeout);
        }
        if (!cardTurboIO) {
            _DelayMs(10);
        }
    }

    // Allocate a buffer for input, noting that we always put the +1 in the
    // alloc so we can be assured that it can be null-terminated. This must be
    // the case because json parsing requires a null-terminated string.
    uint32_t available = 0;
    uint32_t jsonbufAllocLen = ALLOC_CHUNK;
    uint8_t *jsonbuf = (uint8_t *)_Malloc(jsonbufAllocLen + 1);
    if (jsonbuf == NULL) {
        const char *err = ERRSTR("transaction: jsonbuf malloc failed", c_mem);
#ifdef ERRDBG
        NOTE_C_LOG_ERROR(err);
#endif
        return err;
    }

    // Receive the Notecard response
    uint32_t jsonbufLen = 0;
    do {
        uint32_t jsonbufAvailLen = (jsonbufAllocLen - jsonbufLen);

        // Append into the json buffer
        const char *err = serialChunkedReceive((uint8_t *)(jsonbuf + jsonbufLen), &jsonbufAvailLen, true, (NOTECARD_TRANSACTION_TIMEOUT_SEC * 1000), &available);
        if (err) {
            _Free(jsonbuf);
#ifdef ERRDBG
            NOTE_C_LOG_ERROR(ERRSTR("error occured during receive", c_iobad));
#endif
            return err;
        }
        jsonbufLen += jsonbufAvailLen;

        if (available) {
            // When more bytes are available than we have buffer to accommodate
            // (i.e. overflow), then we allocate blocks of size `ALLOC_CHUNK` to
            // reduce heap fragmentation.
            // NOTE: We always put the +1 in the allocation so we can be assured
            // that it can be null-terminated, because the json parser requires
            // a null-terminated string.
            jsonbufAllocLen += (ALLOC_CHUNK * ((available / ALLOC_CHUNK) + ((available % ALLOC_CHUNK) > 0)));
            uint8_t *jsonbufNew = (uint8_t *)_Malloc(jsonbufAllocLen + 1);
            if (jsonbufNew == NULL) {
                const char *err = ERRSTR("transaction: jsonbuf grow malloc failed", c_mem);
#ifdef ERRDBG
                NOTE_C_LOG_ERROR(err);
#endif
                _Free(jsonbuf);
                return err;
            }
            memcpy(jsonbufNew, jsonbuf, jsonbufLen);
            _Free(jsonbuf);
            jsonbuf = jsonbufNew;
        }
    } while (available);

    // Null-terminate it, using the +1 space that we'd allocated in the buffer
    jsonbuf[jsonbufLen] = '\0';

    // Return it
    *response = (char *)jsonbuf;
    return NULL;
}

//**************************************************************************/
/*!
    @brief  Initialize or re-initialize the Serial bus, returning false if
            anything fails.
    @returns a boolean. `true` if the reset was successful, `false`, if not.
*/
/**************************************************************************/
bool serialNoteReset()
{

    // Initialize, or re-initialize, because we've observed Arduino serial
    // driver flakiness.
    _DelayMs(250);
    if (!_SerialReset()) {
        return false;
    }

    // The guaranteed behavior for robust resyncing is to send two newlines
    // and  wait for two echoed blank lines in return.
    bool notecardReady = false;
    int retries;
    for (retries=0; retries<10; retries++) {

        // Send a newline to the module to clean out request/response processing
        // NOTE: This MUST always be `\n` and not `\r\n`, because there are some
        //       versions of the Notecard firmware will not respond to `\r\n`
        //       after communicating over I2C.
        _SerialTransmit((uint8_t *)"\n", 1, true);

        // Drain all serial for 500ms
        bool somethingFound = false;
        bool nonControlCharFound = false;
        for (const size_t startMs = _GetMs() ; _GetMs() - startMs < 500 ;) {
            while (_SerialAvailable()) {
                somethingFound = true;
                // The Notecard responds to a bare \n with \r\n. If we get any
                // other characters back, it means the host and Notecard aren't
                // synced up yet, and we need to transmit \n again.
                char ch = _SerialReceive();
                if (ch != '\n' && ch != '\r') {
                    nonControlCharFound = true;
                }
            }
            _DelayMs(1);
        }

        // If all we got back is newlines, we're ready
        if (somethingFound && !nonControlCharFound) {
            notecardReady = true;
            break;
        }

#ifdef ERRDBG
        NOTE_C_LOG_ERROR(somethingFound ? ERRSTR("unrecognized data from notecard", c_iobad) : ERRSTR("notecard not responding", c_iobad));
#else
        NOTE_C_LOG_ERROR(ERRSTR("notecard not responding", c_iobad));
#endif
        _DelayMs(500);
        if (!_SerialReset()) {
            return false;
        }
    }

    // Done
    return notecardReady;
}

/**************************************************************************/
/*!
  @brief  Receive bytes over Serial from the Notecard.
  @param   buffer
            A buffer to receive bytes into.
  @param   size (in/out)
            - (in) The size of the buffer in bytes.
            - (out) The length of the received data in bytes.
  @param   delay
            Respect standard processing delays.
  @param   timeoutMs
            The maximum amount of time, in milliseconds, to wait for serial data
            to arrive. Passing zero (0) disables the timeout.
  @param   available (out)
            The amount of bytes unable to fit into the provided buffer.
  @returns  A c-string with an error, or `NULL` if no error ocurred.
*/
/**************************************************************************/
const char *serialChunkedReceive(uint8_t *buffer, uint32_t *size, bool delay, size_t timeoutMs, uint32_t *available)
{
    size_t received = 0;
    bool overflow = (received >= *size);
    size_t startMs = _GetMs();
    for (bool eop = false ; !overflow && !eop ;) {
        while (!_SerialAvailable()) {
            if (timeoutMs && (_GetMs() - startMs >= timeoutMs)) {
                *size = received;
#ifdef ERRDBG
                if (received) {
                    NOTE_C_LOG_ERROR(ERRSTR("received only partial reply before timeout", c_iobad));
                }
#endif
                return ERRSTR("timeout: transaction incomplete {io}",c_iotimeout);
            }
            // Yield while awaiting the first byte (lazy). After the first byte,
            // start to spin for the remaining bytes (greedy).
            if (delay && received == 0) {
                _DelayMs(1);
            }
        }

        // Once we've received any character, we will no longer wait patiently
        timeoutMs = 1000;
        startMs = _GetMs();

        // Receive the next character
        char ch = _SerialReceive();

        // Place into the buffer
        buffer[received++] = ch;

        // Look for end-of-packet marker
        eop = (ch == '\n');

        // Check overflow condition
        overflow = ((received >= *size) && !eop);
        if (overflow) {
            // We haven't received a newline, so we're not done with this
            // packet. If the newline never comes, for whatever reason, when
            // this function is called again, we'll timeout. We don't just
            // use _SerialAvailable to set *available here because we're
            // typically reading faster than the serial buffer fills, and so
            // _SerialAvailable may return 0.
            *available = 1;
            break;
        } else {
            *available = 0;
        }
    }

    // Return it
    *size = received;
    return NULL;
}

/**************************************************************************/
/*!
  @brief  Transmit bytes over serial to the Notecard.
  @param   buffer
            A buffer of bytes to transmit.
  @param   size
            The count of bytes in the buffer to send.
  @param   delay
            Respect standard processing delays.
  @returns  A c-string with an error, or `NULL` if no error ocurred.
*/
/**************************************************************************/
const char *serialChunkedTransmit(uint8_t *buffer, uint32_t size, bool delay)
{
    // Transmit the request in segments so as not to overwhelm the Notecard's
    // interrupt buffers
    uint32_t segOff = 0;
    uint32_t segLeft = size;

    if (sizeof(size_t) != 4) { // Give the compiler a hint to eliminate the code
        // Ensure truncation does not occur on 16-bit microcontrollers
        const size_t castSize = (size_t)size;
        if (castSize != size) {
            const char *err = ERRSTR("Cannot transmit provided size; limit to `size_t`", c_iobad);
            NOTE_C_LOG_ERROR(err);
            return err;
        }
    }

    while (true) {
        size_t segLen = segLeft;
        if (segLen > CARD_REQUEST_SERIAL_SEGMENT_MAX_LEN) {
            segLen = CARD_REQUEST_SERIAL_SEGMENT_MAX_LEN;
        }
        _SerialTransmit(&buffer[segOff], segLen, false);
        segOff += segLen;
        segLeft -= segLen;
        if (segLeft == 0) {
            break;
        }
        if (delay) {
            _DelayMs(CARD_REQUEST_SERIAL_SEGMENT_DELAY_MS);
        }
    }

    return NULL;
}
