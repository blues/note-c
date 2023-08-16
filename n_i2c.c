/*!
 * @file n_i2c.c
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

// Forwards
static void _DelayIO(void);
static const char * _I2cNoteQueryLength(uint32_t * available);

/**************************************************************************/
/*!
  @brief  We've noticed that there's an instability in some cards'
  implementations of I2C, and as a result we introduce an intentional
  delay before each and every I2C I/O.The timing was computed
  empirically based on a number of commercial devices.
*/
/**************************************************************************/
static void _DelayIO(void)
{
    if (!cardTurboIO) {
        _DelayMs(6);
    }
}

/**************************************************************************/
/*!
  @brief  Query the Notecard for the length of cached data.

  @details  It is necessary to send a priming I2C transaction to understand
             the amount of data the Notecard is prepared to send before an
             I2C read request can be issued.
*/
/**************************************************************************/
static const char * _I2cNoteQueryLength(uint32_t * available)
{
    uint8_t dummy_buffer = 0;
    const char *err = _I2CReceive(_I2CAddress(), &dummy_buffer, 0, available);
    if (err) {
#ifdef ERRDBG
        _Debug(err);
#endif
        return err;
    }
    return NULL;
}

/**************************************************************************/
/*!
  @brief  Given a JSON string, perform an I2C transaction with the Notecard.
  @param   request
            A c-string containing the JSON request object.
  @param   response
            An out parameter c-string buffer that will contain the JSON
            response from the Notercard.
  @returns a c-string with an error, or `NULL` if no error occurred.
*/
/**************************************************************************/
const char *i2cNoteTransaction(char *request, char **response)
{
    const char *err = NULL;
    const size_t nullIndex = strlen(request);
    const size_t requestLen = (strlen(request) + 1);

    // Swap NULL terminator ('\0') with newline during transmission ('\n')
    request[nullIndex] = '\n';

    // Lock over the entire transaction
    _LockI2C();

    err = i2cChunkedTransmit((uint8_t *)request, requestLen, true);
    request[nullIndex] = '\0';  // Restore the transmit buffer
    if (err) {
        _UnlockI2C();
        return err;
    }

    // If no reply expected, we're done
    if (response == NULL) {
        _UnlockI2C();
        return NULL;
    }

    _DelayIO();

    // Allocate a buffer for input, noting that we always put the +1 in the
    // alloc so we can be assured that it can be null-terminated. This must be
    // the case because json parsing requires a null-terminated string.
    uint32_t available = 0;
    err = _I2cNoteQueryLength(&available);
    if (err) {
#ifdef ERRDBG
        _Debug("failed to query Notecard\n");
#endif
        _UnlockI2C();
        return err;
    }
    size_t jsonbufAllocLen = (ALLOC_CHUNK * ((available / ALLOC_CHUNK) + ((available % ALLOC_CHUNK) > 0)));
    uint8_t *jsonbuf = NULL;
    if (jsonbufAllocLen) {
        jsonbuf = (uint8_t *)_Malloc(jsonbufAllocLen + 1);
        if (jsonbuf == NULL) {
#ifdef ERRDBG
            _Debug("transaction: jsonbuf malloc failed\n");
#endif
            _UnlockI2C();
            return ERRSTR("insufficient memory",c_mem);
        }
    }

    // Receive the Notecard response
    size_t jsonbufLen = 0;
    do {
        size_t jsonbufAvailLen = (jsonbufAllocLen - jsonbufLen);

        // Append into the json buffer
        const char *err = i2cChunkedReceive((uint8_t *)(jsonbuf + jsonbufLen), &jsonbufAvailLen, true, (NOTECARD_TRANSACTION_TIMEOUT_SEC * 1000), &available);
        if (err) {
            if (jsonbuf) {
                _Free(jsonbuf);
            }
#ifdef ERRDBG
            _Debug("error occured during receive\n");
#endif
            _UnlockI2C();
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
#ifdef ERRDBG
                _Debug("transaction: jsonbuf grow malloc failed\n");
#endif
                if (jsonbuf) {
                    _Free(jsonbuf);
                }
                _UnlockI2C();
                return ERRSTR("insufficient memory", c_mem);
            }
            if (jsonbuf) {
                memcpy(jsonbufNew, jsonbuf, jsonbufLen);
                _Free(jsonbuf);
            }
            jsonbuf = jsonbufNew;
        }
    } while (available);

    // Done with the bus
    _UnlockI2C();

    // Null-terminate it, using the +1 space that we'd allocated in the buffer
    if (jsonbuf) {
        jsonbuf[jsonbufLen] = '\0';
    }

    // Return it
    *response = (char *)jsonbuf;
    return NULL;
}

//**************************************************************************/
/*!
  @brief  Initialize or re-initialize the I2C subsystem, returning false if
  anything fails.
  @returns a boolean. `true` if the reset was successful, `false`, if not.
*/
/**************************************************************************/
bool i2cNoteReset()
{

    // Reset the I2C subsystem and exit if failure
    _LockI2C();
    bool success = _I2CReset(_I2CAddress());
    if (!success) {
        _UnlockI2C();
        return false;
    }

    // Synchronize by guaranteeing not only that I2C works, but that after we
    // send \n that we drain the remainder of any pending partial reply from a
    // previously-aborted session. If we get a failure on transmitting the \n,
    // it means that the notecard isn't even present.
    _DelayIO();
    const char *transmitErr = _I2CTransmit(_I2CAddress(), (uint8_t *)"\n", 1);
    if (!cardTurboIO) {
        _DelayMs(CARD_REQUEST_I2C_SEGMENT_DELAY_MS);
    }

    // This outer loop does retries on I2C error, and is simply here for robustness.
    bool notecardReady = false;
    int retries;
    for (retries=0; transmitErr==NULL && !notecardReady && retries<3; retries++) {

        // Loop to drain all chunks of data that may be ready to transmit to us
        uint16_t chunkLen = 0;
        while (true) {

            // Read the next chunk of available data
            uint32_t available;
            uint8_t buffer[128];
            chunkLen = (chunkLen > sizeof(buffer)) ? sizeof(buffer) : chunkLen;
            chunkLen = (chunkLen > _I2CMax()) ? _I2CMax() : chunkLen;
            _DelayIO();
            const char *err = _I2CReceive(_I2CAddress(), buffer, chunkLen, &available);
            if (err) {
                break;
            }

            // If nothing left, we're ready to transmit a command to receive the data
            if (available == 0) {
                notecardReady = true;
                break;
            }

            // Read the minimum of the available bytes left to read and what
            // will fit into a 16-bit unsigned value (_I2CReceive takes the
            // buffer size as a uint16_t).
            chunkLen = (available > 0xFFFF) ? 0xFFFF : available;

        }

        // Exit loop if success
        if (notecardReady) {
            break;
        }

    }

    // Reinitialize i2c if there's no response
    if (!notecardReady) {
        _I2CReset(_I2CAddress());
        _Debug(ERRSTR("notecard not responding\n", "no notecard\n"));
    }

    // Done with the bus
    _UnlockI2C();

    // Done
    return notecardReady;
}

/**************************************************************************/
/*!
  @brief  Receive bytes over I2C from the Notecard.
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
  @param   available (in/out)
            - (in) The amount of bytes to request. Sending zero (0) will
                   initiate a priming query when using the I2C interface.
            - (out) The amount of bytes unable to fit into the provided buffer.
  @returns  A c-string with an error, or `NULL` if no error ocurred.
*/
/**************************************************************************/
const char *i2cChunkedReceive(uint8_t *buffer, size_t *size, bool delay, size_t timeoutMs, uint32_t *available)
{
    // Load buffer with chunked I2C values
    size_t received = 0;
    uint16_t requested = 0;
    bool overflow = false;
    const size_t startMs = _GetMs();

    for (bool eop = false ; !overflow ; overflow = ((received + requested) >= *size)) {
        // Request all available bytes, up to the maximum request size
        requested = (*available > 0xFFFF) ? 0xFFFF : *available;
        requested = (requested > _I2CMax()) ? _I2CMax() : requested;

        // Read a chunk of data from I2C
        // The first read will request zero bytes to query the amount of data
        // available to receive from the Notecard.
        const char *err = _I2CReceive(_I2CAddress(), (buffer + received), requested, available);
        if (err) {
#ifdef ERRDBG
            _Debug(err);
#endif
            return err;
        }

        // Add requested bytes to received total
        received += requested;

        // Look for end-of-packet marker
        if (received > 0 && !eop) {
            eop = (buffer[received-1] == '\n');
        }

        // If the last byte of the chunk is `\n`, then we have received a
        // complete message. However, everything pending from the Notecard must
        // be pulled. This loop will only exit when a newline is received AND
        // there are no more bytes available from the Notecard, OR if the buffer
        // is full and cannot receive more bytes (i.e. overflow condition).

        // If there's something available on the Notecard for us to receive, do it
        if (*available > 0) {
            continue;
        }

        // If there's nothing available AND we've received a newline, we're done
        if (eop) {
            break;
        }

        // Exit on timeout
        if (timeoutMs && (_GetMs() - startMs >= timeoutMs)) {
            if (buffer) {
                if (*size > received) {
                    buffer[received] = '\0';
                } else {
                    buffer[(received - 1)] = '\0';
                }
            }
            *size = received;
#ifdef ERRDBG
            if (received) {
                _Debug("received only partial reply after timeout:\n");
                _Debug((char *)buffer);
                _Debug("\n^^ partial buffer contents ^^\n");
            }
#endif
            return ERRSTR("timeout: transaction incomplete {io}",c_iotimeout);
        }

        // Delay, simply waiting for the Note to process the request
        if (delay) {
            _DelayMs(50);
        }
    }

    *size = received;
    return NULL;
}

/**************************************************************************/
/*!
  @brief  Transmit bytes over I2C to the Notecard.
  @param   buffer
            A buffer of bytes to transmit.
  @param   size
            The count of bytes in the buffer to send
  @param   delay
            Respect standard processing delays.
  @returns  A c-string with an error, or `NULL` if no error ocurred.
*/
/**************************************************************************/
const char *i2cChunkedTransmit(uint8_t *buffer, size_t size, bool delay)
{
    // Transmit the request in chunks, but also in segments so as not to
    // overwhelm the notecard's interrupt buffers
    const char *estr;
    uint8_t *chunk = buffer;
    uint16_t sentInSegment = 0;
    while (size > 0) {
        // Constrain chunkLen to fit into 16 bits (_I2CTransmit takes the buffer
        // size as a uint16_t).
        uint16_t chunkLen = (size > 0xFFFF) ? 0xFFFF : size;
        // Constrain chunkLen to be <= _I2CMax().
        chunkLen = (chunkLen > _I2CMax()) ? _I2CMax() : chunkLen;

        _DelayIO();
        estr = _I2CTransmit(_I2CAddress(), chunk, chunkLen);
        if (estr != NULL) {
            _I2CReset(_I2CAddress());
#ifdef ERRDBG
            _Debug("i2c transmit: ");
            _Debug(estr);
            _Debug("\n");
#endif
            return estr;
        }
        chunk += chunkLen;
        size -= chunkLen;
        sentInSegment += chunkLen;
        if (sentInSegment > CARD_REQUEST_I2C_SEGMENT_MAX_LEN) {
            sentInSegment = 0;
            if (delay) {
                _DelayMs(CARD_REQUEST_I2C_SEGMENT_DELAY_MS);
            }
        }
        if (delay) {
            _DelayMs(CARD_REQUEST_I2C_CHUNK_DELAY_MS);
        }
    }

    return NULL;
}
