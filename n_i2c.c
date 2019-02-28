// Copyright 2018 Inca Roads LLC.  All rights reserved.
// Use of this source code is governed by licenses granted by the
// copyright holder including that found in the LICENSE file.

#include "n_lib.h"

// I2C Slave Robustness Testing
// Note that during these tests, in stm32l4xx_hal_i2c.c, neither of these two breakpoints was hit:
// I2C_ITError()
// HAL_I2C_ER_IRQHandler()
#define ROBUSTNESS_TEST_RECEIVE_WHEN_UNEXPECTED false       // Pulls both SDA/SCL low; I2C reset on notecard recovers
#define ROBUSTNESS_TEST_TRANSMIT_TOO_MUCH       false       // Puts SCL into low state and hangs I2C
#define ROBUSTNESS_TEST_TRANSMIT_TOO_LITTLE     false       // Smooth handling/recovery
#define ROBUSTNESS_TEST_RECEIVE_TOO_MUCH        false       // Puts SCL into low state and hangs I2C
#define ROBUSTNESS_TEST_RECEIVE_TOO_LITTLE      false       // Smooth handling/recovery

// Firmware parameters as per spec

// Initiate a transaction to the notecard using reqdoc, and return the result in rspdoc
char *i2cNotecardTransaction(char *json, char **jsonResponse) {

    // Transmit it in chunks
    char *errstr;
    uint8_t *chunk = (uint8_t *) json;
    int start = _GetMs();
    int jsonLen = strlen(json);
    while (jsonLen > 0) {
        uint8_t chunklen = (uint8_t) (jsonLen > _I2CMax() ? _I2CMax() : jsonLen);
#if ROBUSTNESS_TEST_RECEIVE_WHEN_UNEXPECTED
        uint8_t testbyte;
        _I2CReceive(_I2CAddress(), &testbyte, sizeof(testbyte), 0);
#endif
#if ROBUSTNESS_TEST_TRANSMIT_TOO_MUCH
        errstr = _I2CTransmit(_I2CAddress(), &chunklen, sizeof(chunklen)+10, 0);
#else
        errstr = _I2CTransmit(_I2CAddress(), &chunklen, sizeof(chunklen), 0);
#endif
        if (errstr != NULL) {
            _Debug("i2c transmit of length byte: %s\n", errstr);
            if (_GetMs() >= start + (NOTECARD_TRANSACTION_TIMEOUT_SEC*1000))
                return errstr;
            _DelayMs(2000);
            continue;
        }
#if ROBUSTNESS_TEST_TRANSMIT_TOO_LITTLE
        errstr = _I2CTransmit(_I2CAddress(), chunk, chunklen > 10 ? chunklen-5 : chunklen, 0);
#else
        errstr = _I2CTransmit(_I2CAddress(), chunk, chunklen, 0);
#endif
        if (errstr != NULL) {
            _Debug("i2c transmit: %s\n", errstr);
            return errstr;
        }
        chunk += chunklen;
        jsonLen -= chunklen;
    }

    // Write the command terminator
    uint8_t chunklen = sizeof(uint8_t);
    while (errstr = _I2CTransmit(_I2CAddress(), &chunklen, sizeof(chunklen), 0)) {
        _Debug("i2c transmit of length byte: %s\n", errstr);
        if (_GetMs() >= start + (NOTECARD_TRANSACTION_TIMEOUT_SEC*1000))
            return errstr;
        _DelayMs(2000);
    }
    errstr = _I2CTransmit(_I2CAddress(), (uint8_t *)"\n", chunklen, 0);
    if (errstr != NULL) {
        _Debug("i2c transmit of terminator: %s\n", errstr);
        return errstr;
    }

    // If static mem, we read into a static buffer - else we dynamically grow as we read.  Note
    // that we always put the +1 in the alloc so we can be assured that it can be null-terminated,
    // which must be the case because json parsing requires a null-terminated string.
    int jsonbufAllocLen = 1024;
    char *jsonbuf = (char *) _Malloc(jsonbufAllocLen+1);
    if (jsonbuf == NULL) {
        _Debug("transaction: jsonbuf malloc(%d) failed\n", jsonbufAllocLen);
        return "insufficient memory";
    }

    // Loop, building a reply buffer out of received chunks.  We'll build the reply in the same
    // buffer we used to transmit, and will grow it as necessary.
    bool receivedNewline = false;
    int jsonbufLen = 0;
    start = _GetMs();
    while (true) {

        // Issue an "get the next chunk len to read" request
        uint8_t cmdbyte = 0x80;
        char *err = _I2CTransmit(_I2CAddress(), &cmdbyte, sizeof(cmdbyte), 0);
        if (err != NULL) {
            if (_GetMs() < start + (NOTECARD_TRANSACTION_TIMEOUT_SEC*1000)) {
                _DelayMs(2000);
                continue;
            }
            _Free(jsonbuf);
            _Debug("%s: waited for %d sec\n", err, (_GetMs() - start)/1000);
            return err;
        }

        // Get the minimum amount that is waiting for us to receive
        uint8_t chunklen;
        err = _I2CReceive(_I2CAddress(), &chunklen, sizeof(chunklen), 0);
        if (err != NULL) {
            _Free(jsonbuf);
            _Debug("%s: receive error\n", err);
            return err;
        }

        // If the length byte is 0 before we've received a \n, it means that the module is
        // still processing the command and the reply isn't ready.  This is the NORMAL CASE
        // because it takes some number of milliseconds or seconds for the card to actually
        // process requests, and there may be an occasional delay between chunks just because
		// of normal task preemption.
        if (chunklen == 0) {
            if (receivedNewline)
                break;
            if (_GetMs() < start + (NOTECARD_TRANSACTION_TIMEOUT_SEC*1000)) {
                _DelayMs(100);
                continue;
            }
            _Free(jsonbuf);
            _Debug("reply to request didn't arrive from module in %d sec\n", (_GetMs() - start)/1000);
            return "module request or response was lost";
        }

        // Grow the buffer as necessary
        if (jsonbufLen + chunklen > jsonbufAllocLen) {
            uint8_t growlen = 1024;
            if (chunklen > growlen)
                jsonbufAllocLen += chunklen;
            else
                jsonbufAllocLen += growlen;
            char *jsonbufNew = (char *) _Malloc(jsonbufAllocLen+1);
            if (jsonbufNew == NULL) {
                _Debug("transaction: jsonbuf grow malloc(%d) failed\n", jsonbufAllocLen);
                _Free(jsonbuf);
                return "insufficient memory";
            }
            memcpy(jsonbufNew, jsonbuf, jsonbufLen);
            _Free(jsonbuf);
            jsonbuf = jsonbufNew;
        }

        // Issue a "read the next chunk" request
        if (chunklen > _I2CMax())
            chunklen = _I2CMax();
        uint8_t readlength = 0x80 + chunklen;
        err = _I2CTransmit(_I2CAddress(), &readlength, sizeof(readlength), 0);
        if (err != NULL) {
            _Free(jsonbuf);
            _Debug("%s: about to read %d bytes\n", err, chunklen);
            return err;
        }

        // Read the next chunk
#if ROBUSTNESS_TEST_RECEIVE_TOO_MUCH
        err = _I2CReceive(_I2CAddress(), (uint8_t *) &jsonbuf[jsonbufLen], chunklen<100 ? chunklen+10 : chunklen, 0);
#elif ROBUSTNESS_TEST_RECEIVE_TOO_LITTLE
        err = _I2CReceive(_I2CAddress(), (uint8_t *) &jsonbuf[jsonbufLen], chunklen>10 ? chunklen-5 : chunklen, 0);
#else
        err = _I2CReceive(_I2CAddress(), (uint8_t *) &jsonbuf[jsonbufLen], chunklen, 0);
#endif
        if (err != NULL) {
            _Free(jsonbuf);
            _Debug("%s: read of %d bytes\n", err, chunklen);
            return err;
        }

		// If the last byte of the chunk is \n, chances are that we're done.  However, just so
		// that we pull everything pending from the module, we only exit when we've received
		// a newline AND there's nothing left available from the module.
        if (jsonbuf[jsonbufLen+chunklen-1] == '\n')
            receivedNewline = true;

        // The data has now been appended to the JSON being accumulated
        jsonbufLen += chunklen;
    }

    // Null-terminate it, using the +1 space that we'd allocated in the buffer
    jsonbuf[jsonbufLen] = '\0';

    // Return it
    *jsonResponse = jsonbuf;
    return NULL;
}

// Initialize or re-initialize the module, returning false if anything fails
bool i2cNotecardReset() {
    bool notecardReady = false;

    // Reset the I2C subsystem
    _I2CReset();

    // Synchronize by guaranteeing not only that I2C works, but that we drain the remainder of any
    // pending partial reply from a previously-aborted session.
    for (int retries=0; !notecardReady && retries<10; retries++) {

        _Debug("notecard i2c reset\n");

        // Loop to drain all chunks of data that may be ready to transmit to us
        while (true) {

            // Get the minimum amount waiting for us to receive
            uint8_t getlen = 0x80;
            char *err = _I2CTransmit(_I2CAddress(), &getlen, sizeof(getlen), 0);
            if (err)
                break;
            uint8_t chunklen;
            err = _I2CReceive(_I2CAddress(), &chunklen, sizeof(chunklen), 0);
            if (err)
                break;

            // If nothing left, we're ready to transmit a command to receive the data
            if (chunklen == 0) {
                notecardReady = true;
                break;
            }

            // Read a chunk and discard it
            uint8_t buffer[128];
            uint8_t readlen = chunklen > _I2CMax() ? _I2CMax() : chunklen;
            uint8_t readcmd = 0x80 | readlen;
            if (_I2CTransmit(_I2CAddress(), &readcmd, sizeof(readcmd), 0) == NULL)
                _I2CReceive(_I2CAddress(), buffer, readlen, 0);

        }

        // Exit loop if success
        if (notecardReady)
            break;

        // Reinitialize i2c if there's no response
        _I2CReset();
        _Debug("Warning: notecard not responding\n");
        _DelayMs(2000);

    }

    // Done
    return notecardReady;
}

