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

/**************************************************************************/
/*!
  @brief  We've noticed that there's an instability in some cards'
  implementations of I2C, and as a result we introduce an intentional
  delay before each and every I2C I/O.The timing was computed
  empirically based on a number of commercial devices.
*/
/**************************************************************************/
static void _DelayIO()
{
    if (!cardTurboIO) {
        _DelayMs(6);
    }
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
    const size_t nullIndex = strlen(request);
    const size_t requestLen = (strlen(request) + 1);

    // Swap NULL terminator ('\0') with newline during transmission ('\n')
    request[nullIndex] = '\n';

    // Lock over the entire transaction
    _LockI2C();

    const char *err = i2cRawTransmit((uint8_t *)request, requestLen, true);
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

    // Dynamically grow the buffer as we read. Note that we always put the +1 in
    // the alloc so we can be assured that it can be null-terminated, which must
    // be the case because our json parser requires a null-terminated string.
    size_t jsonbufAllocLen = ALLOC_CHUNK;
    uint8_t *jsonbuf = (uint8_t *) _Malloc(jsonbufAllocLen+1);
    if (jsonbuf == NULL) {
#ifdef ERRDBG
        _Debug("transaction: jsonbuf malloc failed\n");
#endif
        _UnlockI2C();
        return ERRSTR("insufficient memory",c_mem);
    }

    size_t jsonbufLen = 0;
    for (size_t overflow = true ; overflow ;) {
        size_t jsonbufAvailLen = (jsonbufAllocLen - jsonbufLen);

        // Append into the json buffer
        const char *err = i2cRawReceive((uint8_t *)&jsonbuf[jsonbufLen], &jsonbufAvailLen, true, (NOTECARD_TRANSACTION_TIMEOUT_SEC * 1000), &overflow);
        if (err) {
            _Free(jsonbuf);
#ifdef ERRDBG
            _Debug("i2c receive error\n");
#endif
            _UnlockI2C();
            return err;
        }
        jsonbufLen += jsonbufAvailLen;

        if (overflow) {
            if (overflow > ALLOC_CHUNK) {
                jsonbufAllocLen += overflow;
            } else {
                jsonbufAllocLen += ALLOC_CHUNK;
            }
            uint8_t *jsonbufNew = (uint8_t *) _Malloc(jsonbufAllocLen+1);
            if (jsonbufNew == NULL) {
#ifdef ERRDBG
                _Debug("transaction: jsonbuf grow malloc failed\n");
#endif
                _Free(jsonbuf);
                _UnlockI2C();
                return ERRSTR("insufficient memory",c_mem);
            }
            memcpy(jsonbufNew, jsonbuf, jsonbufLen);
            _Free(jsonbuf);
            jsonbuf = jsonbufNew;
        }
    }

    // Done with the bus
    _UnlockI2C();

    // Null-terminate it, using the +1 space that we'd allocated in the buffer
    jsonbuf[jsonbufLen] = '\0';

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
  @brief  Receive bytes over I2C to the Notecard.
  @param   buffer
            A buffer to receive bytes into.
  @param   size (in/out)
            - (in) The size of the buffer in bytes.
            - (out) The length of the received data in bytes.
  @param   delay
            Respect delay standard transmission delays.
  @param   timeoutMs
            The maximum amount of time, in milliseconds, to wait for serial data
            to arrive. Passing zero (0) disables the timeout.
  @param   overflow (out)
            The contents did not fit inside the provided buffer.
  @returns  A c-string with an error, or `NULL` if no error ocurred.
*/
/**************************************************************************/
const char *i2cRawReceive(uint8_t *buffer, size_t *size, bool delay, size_t timeoutMs, size_t *overflow)
{
    // Check for special case
    if (!timeoutMs) {
        timeoutMs = (size_t)-1;
    }

    if (delay) {
        _DelayIO();
    }

    // Load buffer with chunked I2C values
    uint32_t available;
    size_t received = 0;
    uint16_t requested = 0;
    *overflow = (received >= *size);
    const size_t startMs = _GetMs();

    for (bool eop = false ; !(*overflow) ;) {
        // Read a chunk of data from I2C
        // The first read will request zero bytes to query the amount of data
        // available to receive from the Notecard.
        const char *err = _I2CReceive(_I2CAddress(), &buffer[received], requested, &available);
        if (err) {
#ifdef ERRDBG
            _Debug(err);
#endif
            return err;
        }

        // Add requested bytes to received total
        received += requested;

        // Request all available bytes, up to the maximum request size
        requested = (available > 0xFFFF) ? 0xFFFF : available;
        requested = (requested > _I2CMax()) ? _I2CMax() : requested;

        // Look for end-of-packet marker
        if (received > 0 && !eop) {
            eop = (buffer[received-1] == '\n');
        }

        // Check overflow condition
        *overflow = ((received + requested) >= *size);
        if (*overflow) {
            *overflow = available; // `available` should optimize allocation
            // *overflow = requested;
        }

        // If the last byte of the chunk is `\n`, then we have received a
        // complete message. However, to ensure we pull everything pending from
        // the Notecard, we will only exit when we've received a newline AND
        // there's no more bytes available from the Notecard.

        // If there's something available on the Notecard for us to receive, do it
        if (available > 0) {
            continue;
        }

        // If there's nothing available AND we've received a newline, we're done
        if (eop) {
            break;
        }

        // Exit on timeout
        if (_GetMs() - startMs >= timeoutMs) {
            *size = received;
            buffer[received] = '\0';
#ifdef ERRDBG
            _Debug("received only partial reply after timeout:\n");
            _Debug((char *)buffer);
            _Debug("^^ partial buffer contents ^^\n");
#endif
            return ERRSTR("transaction incomplete {io}",c_iotimeout);
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
            Respect delay standard transmission delays.
  @returns  A c-string with an error, or `NULL` if no error ocurred.
*/
/**************************************************************************/
const char *i2cRawTransmit(uint8_t *buffer, size_t size, bool delay)
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
