// Copyright 2018 Inca Roads LLC.  All rights reserved.
// Use of this source code is governed by licenses granted by the
// copyright holder including that found in the LICENSE file.

#include "n_lib.h"

// Process a transaction over the serial port, returning NULL and a buffer if success, or an error string
char *serialNotecardTransaction(char *json, char **jsonResponse) {

    // Send the transaction to serial, including the newline that initiates it
    NotecardFnSerialPrintln(json);

    // Wait for something to become available, processing timeout errors up-front
    // because the json parse operation immediately following is subject to the
    // serial port timeout. We'd like more flexibility in max timeout and ultimately
    // in our error handling.
    for (int start = _millis(); !NotecardFnAvailable(); ) {
        if (_millis() >= start + (NOTECARD_TRANSACTION_TIMEOUT_SEC*1000)) {
            NotecardFnDebug("reply to request didn't arrive from module in %dmS\n", _millis() - start);
            return "transaction timeout";
        }
        _delay(10);
    }

    // If static mem, we read into a static buffer - else we dynamically grow as we read.  Note
    // that we always put the +1 in the alloc so we can be assured that it can be null-terminated,
    // which must be the case because json parsing requires a null-terminated string.
    int jsonbufAllocLen = 1024;
    char *jsonbuf = (char *) _malloc(jsonbufAllocLen+1);
    if (jsonbuf == NULL) {
        NotecardFnDebug("transaction: jsonbuf malloc(%d) failed\n", jsonbufAllocLen);
        return "insufficient memory";
    }
    int jsonbufLen = 0;
    char ch = 0;
    while (ch != '\n') {
        if (!NotecardFnAvailable()) {
            _delay(1);
            continue;
        }
        ch = NotecardFnSerialRead();
        jsonbuf[jsonbufLen++] = ch;
        if (jsonbufLen >= jsonbufAllocLen) {
            jsonbufAllocLen += 512;
            char *jsonbufNew = (char *) _malloc(jsonbufAllocLen+1);
            if (jsonbufNew == NULL) {
                NotecardFnDebug("transaction: jsonbuf malloc(%d) grow failed\n", jsonbufAllocLen);
                _free(jsonbuf);
                return "insufficient memory";
            }
            memcpy(jsonbufNew, jsonbuf, jsonbufLen);
            _free(jsonbuf);
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
bool serialNotecardReset() {
    bool notecardReady = false;

    // Initialize, or re-initialize.  Because we've observed Arduino serial driver flakiness,
    // close the Arduino HardwareSerial port in order to re-initialize it.
    static bool first = true;
    if (first)
        first = false;
    else
        _delay(500);
    NotecardFnSerialReset();

    // The guaranteed behavior for robust resyncing is to send two newlines
    // and  wait for two echoed blank lines in return.
    for (int retries=0; !notecardReady && retries<10; retries++) {

        NotecardFnDebug("notecard serial reset\n");

        // Attempt to synchronize serial
        for (int i=0; i<10; i++) {
            NotecardFnSerialWrite("\n");
            NotecardFnSerialWrite("\n");
            _delay(500);
            bool somethingFound = false;
            bool nonControlCharFound = false;
            while (NotecardFnAvailable() > 0) {
                somethingFound = true;
                char ch = NotecardFnSerialRead();
                if (ch >= ' ')
                    nonControlCharFound = true;
            }
            if (somethingFound && !nonControlCharFound) {
                notecardReady = true;
                break;
            }
            _delay(1000);
        }

        // Did we successfully communicate with the notecard?
        if (notecardReady)
            break;

        NotecardFnDebug("Warning: notecard not responding\n");
        _delay(500);
        NotecardFnSerialReset();
    }

    // Flush anything pending on input prior to sending the command
    while (NotecardFnAvailable())
        NotecardFnSerialRead();

    // Done
    return notecardReady;
}
