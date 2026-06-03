/*!
 * @file NotePing_test.cpp
 *
 * Written by the Blues Inc. team.
 *
 * Copyright (c) 2026 Blues Inc. MIT License. Use of this source code is
 * governed by licenses granted by the copyright holder including that found in
 * the
 * <a href="https://github.com/blues/note-c/blob/master/LICENSE">LICENSE</a>
 * file.
 *
 */

#include <catch2/catch_test_macros.hpp>
#include <fff.h>

#include "n_lib.h"

#include <cstdlib>
#include <cstring>

DEFINE_FFF_GLOBALS
FAKE_VALUE_FUNC(bool, _noteTransactionStart, uint32_t)
FAKE_VOID_FUNC(_noteTransactionStop)
FAKE_VOID_FUNC(_noteLockNote)
FAKE_VOID_FUNC(_noteUnlockNote)
FAKE_VALUE_FUNC(const char *, _noteJSONTransaction, const char *, size_t, char **, uint32_t)
FAKE_VALUE_FUNC(bool, _noteSerialAvailable)
FAKE_VALUE_FUNC(char, _noteSerialReceive)
FAKE_VALUE_FUNC(bool, _noteHardReset)

extern volatile int hookActiveInterface;

namespace
{

enum class PingResponse {
    Echo,
    WrongNonce,
    Error,
    InvalidJson,
    NoResponse,
    TransactionError,
};

PingResponse pingResponse = PingResponse::Echo;
uint32_t currentMs = 1000;
size_t serialBytesRemaining = 0;
size_t serialBytesRemainingAtTransaction = 0;
size_t lastRequestLength = 0;
uint32_t lastTransactionTimeoutMs = 0;
bool lastRequestEndedWithNewline = false;
bool lastRequestHadCrc = false;

char *copyString(const char *src)
{
    const size_t len = strlen(src);
    char *dst = static_cast<char *>(malloc(len + 1));
    if (dst != NULL) {
        memcpy(dst, src, len + 1);
    }
    return dst;
}

uint32_t getMs()
{
    return currentMs;
}

void delayMs(uint32_t ms)
{
    currentMs += ms;
}

bool serialAvailable()
{
    return serialBytesRemaining > 0;
}

char serialReceive()
{
    if (serialBytesRemaining > 0) {
        --serialBytesRemaining;
    }
    return 'x';
}

char *makeResponse(const char *text)
{
    J *rsp = JCreateObject();
    if (rsp == NULL) {
        return NULL;
    }
    JAddStringToObject(rsp, "text", text);
    char *json = JPrintUnformatted(rsp);
    JDelete(rsp);
    return json;
}

const char *pingTransaction(const char *request, size_t reqLen, char **response, uint32_t timeoutMs)
{
    lastRequestLength = reqLen;
    lastTransactionTimeoutMs = timeoutMs;
    lastRequestEndedWithNewline = (reqLen > 0 && request[reqLen - 1] == '\n');
    serialBytesRemainingAtTransaction = serialBytesRemaining;

    char *requestCopy = static_cast<char *>(malloc(reqLen + 1));
    if (requestCopy == NULL) {
        return ERRSTR("malloc failed {mem}", c_mem);
    }
    memcpy(requestCopy, request, reqLen);
    requestCopy[reqLen] = '\0';
    lastRequestHadCrc = (strstr(requestCopy, "\"crc\"") != NULL);

    if (pingResponse == PingResponse::TransactionError) {
        free(requestCopy);
        return ERRSTR("transaction failed {io}", c_ioerr);
    }
    if (response == NULL || pingResponse == PingResponse::NoResponse) {
        free(requestCopy);
        return NULL;
    }
    if (pingResponse == PingResponse::InvalidJson) {
        *response = copyString("not-json");
        free(requestCopy);
        return NULL;
    }
    if (pingResponse == PingResponse::Error) {
        *response = copyString("{\"err\":\"failed\"}");
        free(requestCopy);
        return NULL;
    }

    if (reqLen > 0 && requestCopy[reqLen - 1] == '\n') {
        requestCopy[reqLen - 1] = '\0';
    }

    J *req = JParse(requestCopy);
    free(requestCopy);
    if (req == NULL) {
        return ERRSTR("parse failed {bad}", c_bad);
    }

    const char *nonce = JGetString(req, "text");
    *response = makeResponse(pingResponse == PingResponse::WrongNonce ? "wrong" : nonce);
    JDelete(req);

    return NULL;
}

void resetTestState(void)
{
    RESET_FAKE(_noteTransactionStart);
    RESET_FAKE(_noteTransactionStop);
    RESET_FAKE(_noteLockNote);
    RESET_FAKE(_noteUnlockNote);
    RESET_FAKE(_noteJSONTransaction);
    RESET_FAKE(_noteSerialAvailable);
    RESET_FAKE(_noteSerialReceive);
    RESET_FAKE(_noteHardReset);

    pingResponse = PingResponse::Echo;
    currentMs = 1000;
    serialBytesRemaining = 0;
    serialBytesRemainingAtTransaction = 0;
    lastRequestLength = 0;
    lastTransactionTimeoutMs = 0;
    lastRequestEndedWithNewline = false;
    lastRequestHadCrc = false;
    resetRequired = false;

    NoteSetFnDefault(malloc, free, delayMs, getMs);
    RESET_FAKE(_noteLockNote);
    RESET_FAKE(_noteUnlockNote);

    hookActiveInterface = NOTE_C_INTERFACE_SERIAL;
    _noteTransactionStart_fake.return_val = true;
    _noteJSONTransaction_fake.custom_fake = pingTransaction;
    _noteSerialAvailable_fake.custom_fake = serialAvailable;
    _noteSerialReceive_fake.custom_fake = serialReceive;
}

SCENARIO("NotePing")
{
    resetTestState();

    SECTION("returns true when echo response matches the nonce") {
        CHECK(NotePing());
        CHECK(_noteJSONTransaction_fake.call_count == 1);
        CHECK(lastRequestLength > 0);
        CHECK(lastRequestEndedWithNewline);
        CHECK(!lastRequestHadCrc);
        CHECK(lastTransactionTimeoutMs == 500);
        CHECK(_noteTransactionStart_fake.call_count == 1);
        CHECK(_noteTransactionStop_fake.call_count == 1);
        CHECK(_noteLockNote_fake.call_count == 1);
        CHECK(_noteUnlockNote_fake.call_count == 1);
    }

    SECTION("returns false when the echoed nonce does not match") {
        pingResponse = PingResponse::WrongNonce;

        CHECK(!NotePing());
        CHECK(_noteJSONTransaction_fake.call_count == 1);
    }

    SECTION("returns false when the response contains an error") {
        pingResponse = PingResponse::Error;

        CHECK(!NotePing());
        CHECK(_noteJSONTransaction_fake.call_count == 1);
    }

    SECTION("returns false when the response is not valid JSON") {
        pingResponse = PingResponse::InvalidJson;

        CHECK(!NotePing());
        CHECK(_noteJSONTransaction_fake.call_count == 1);
    }

    SECTION("returns false when the transaction fails and does not retry") {
        pingResponse = PingResponse::TransactionError;

        CHECK(!NotePing());
        CHECK(_noteJSONTransaction_fake.call_count == 1);
    }

    SECTION("returns false when no response is received and does not retry") {
        pingResponse = PingResponse::NoResponse;

        CHECK(!NotePing());
        CHECK(_noteJSONTransaction_fake.call_count == 1);
    }

    SECTION("does not reset even when resetRequired is set") {
        resetRequired = true;
        pingResponse = PingResponse::TransactionError;

        CHECK(!NotePing());
        CHECK(_noteHardReset_fake.call_count == 0);
        CHECK(resetRequired == true);
    }

    SECTION("returns false without locking or transacting when transaction start fails") {
        _noteTransactionStart_fake.return_val = false;

        CHECK(!NotePing());
        CHECK(_noteLockNote_fake.call_count == 0);
        CHECK(_noteJSONTransaction_fake.call_count == 0);
        CHECK(_noteTransactionStop_fake.call_count == 0);
    }

    SECTION("drains serial input before sending the ping") {
        serialBytesRemaining = 3;

        CHECK(NotePing());
        CHECK(_noteSerialReceive_fake.call_count == 3);
        CHECK(serialBytesRemainingAtTransaction == 0);
    }

    resetTestState();
}

}
