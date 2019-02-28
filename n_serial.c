// Copyright 2018 Inca Roads LLC.  All rights reserved.
// Use of this source code is governed by licenses granted by the
// copyright holder including that found in the LICENSE file.

#include "n_lib.h"

// Process a transaction over the serial port, returning NULL and a buffer if success, or an error string
const char *serialNoteTransaction(char *json, char **jsonResponse) {

    // Transmit the request in segments so as not to overwhelm the notecard's interrupt buffers
    uint32_t segOff = 0;
    uint32_t segLeft = strlen(json);
    while (true) {
        size_t segLen = segLeft;
        if (segLen > CARD_REQUEST_SEGMENT_MAX_LEN)
            segLen = CARD_REQUEST_SEGMENT_MAX_LEN;
        segLeft -= segLen;
        if (segLeft == 0) {
            _SerialWriteLine(&json[segOff]);
            break;
        }
        _SerialWrite((uint8_t *)&json[segOff], segLen);
        segOff += segLen;        
        _DelayMs(CARD_REQUEST_SEGMENT_DELAY_MS);
    }

    // Wait for something to become available, processing timeout errors up-front
    // because the json parse operation immediately following is subject to the
    // serial port timeout. We'd like more flexibility in max timeout and ultimately
    // in our error handling.
    for (int start = _GetMs(); !_SerialAvailable(); ) {
        if (_GetMs() >= start + (NOTECARD_TRANSACTION_TIMEOUT_SEC*1000)) {
            _Debug("reply to request didn't arrive from module in %dmS\n", _GetMs() - start);
            return "transaction timeout";
        }
        _DelayMs(10);
    }

    // Allocate a buffer for input, noting that we always put the +1 in the alloc so we can be assured
    // that it can be null-terminated.  This must be the case because json parsing requires a
    // null-terminated string.
    int jsonbufAllocLen = 1024;
    char *jsonbuf = (char *) _Malloc(jsonbufAllocLen+1);
    if (jsonbuf == NULL) {
        _Debug("transaction: jsonbuf malloc(%d) failed\n", jsonbufAllocLen);
        return "insufficient memory";
    }
    int jsonbufLen = 0;
    char ch = 0;
    while (ch != '\n') {
        if (!_SerialAvailable()) {
            _DelayMs(1);
            continue;
        }
        ch = _SerialRead();
        jsonbuf[jsonbufLen++] = ch;
        if (jsonbufLen >= jsonbufAllocLen) {
            jsonbufAllocLen += 512;
            char *jsonbufNew = (char *) _Malloc(jsonbufAllocLen+1);
            if (jsonbufNew == NULL) {
                _Debug("transaction: jsonbuf malloc(%d) grow failed\n", jsonbufAllocLen);
                _Free(jsonbuf);
                return "insufficient memory";
            }
            memcpy(jsonbufNew, jsonbuf, jsonbufLen);
            _Free(jsonbuf);
            jsonbuf = jsonbufNew;
        }
    }

    // Null-terminate it, using the +1 space that we'd allocated in the buffer
    jsonbuf[jsonbufLen] = '\0';

    // Return it
    *jsonResponse = jsonbuf;
    return NULL;

}

// Initialize or re-initialize the module, returning false if anything fails
bool serialNoteReset() {
    bool notecardReady = false;

    // Initialize, or re-initialize.  Because we've observed Arduino serial driver flakiness,
    // close the Arduino HardwareSerial port in order to re-initialize it.
    static bool first = true;
    if (first)
        first = false;
    else
        _DelayMs(500);
    _SerialReset();

    // The guaranteed behavior for robust resyncing is to send two newlines
    // and  wait for two echoed blank lines in return.
    for (int retries=0; !notecardReady && retries<10; retries++) {

        _Debug("notecard serial reset\n");

        // Attempt to synchronize serial
        for (int i=0; i<10; i++) {
            _SerialWrite((uint8_t *)"\n", 1);
            _SerialWrite((uint8_t *)"\n", 1);
            _DelayMs(500);
            bool somethingFound = false;
            bool nonControlCharFound = false;
            while (_SerialAvailable() > 0) {
                somethingFound = true;
                char ch = _SerialRead();
                if (ch >= ' ')
                    nonControlCharFound = true;
            }
            if (somethingFound && !nonControlCharFound) {
                notecardReady = true;
                break;
            }
            _DelayMs(1000);
        }

        // Did we successfully communicate with the notecard?
        if (notecardReady)
            break;

        _Debug("Warning: notecard not responding\n");
        _DelayMs(500);
        _SerialReset();
    }

    // Flush anything pending on input prior to sending the command
    while (_SerialAvailable())
        _SerialRead();

    // Done
    return notecardReady;
}
