/*!
 * @file NoteTransaction_test.cpp
 *
 * Written by the Blues Inc. team.
 *
 * Copyright (c) 2023 Blues Inc. MIT License. Use of this source code is
 * governed by licenses granted by the copyright holder including that found in
 * the
 * <a href="https://github.com/blues/note-c/blob/master/LICENSE">LICENSE</a>
 * file.
 *
 */

#ifdef NOTE_C_TEST

#include <catch2/catch_test_macros.hpp>
#include "fff.h"

#include "n_lib.h"
#include "test_static.h"

DEFINE_FFF_GLOBALS
FAKE_VALUE_FUNC(bool, NoteReset)
FAKE_VALUE_FUNC(const char *, NoteJSONTransaction, char *, char **, size_t)
FAKE_VALUE_FUNC(bool, NoteTransactionStart, uint32_t)
FAKE_VALUE_FUNC(J *, NoteUserAgent)
FAKE_VALUE_FUNC(bool, crcError, char *, uint16_t)

namespace
{

const char *NoteJSONTransactionValid(char *, char **resp, size_t)
{
    static char respString[] = "{ \"total\": 1 }";

    if (resp) {
        char* respBuf = reinterpret_cast<char *>(malloc(sizeof(respString)));
        memcpy(respBuf, respString, sizeof(respString));
        *resp = respBuf;
    }

    return NULL;
}

const char *NoteJSONTransactionBadJSON(char *, char **resp, size_t)
{
    static char respString[] = "Bad JSON";

    if (resp) {
        char* respBuf = reinterpret_cast<char *>(malloc(sizeof(respString)));
        memcpy(respBuf, respString, sizeof(respString));
        *resp = respBuf;
    }

    return NULL;
}

const char *NoteJSONTransactionIOError(char *, char **resp, size_t)
{
    static char respString[] = "{\"err\": \"{io}\"}";

    if (resp) {
        char* respBuf = reinterpret_cast<char *>(malloc(sizeof(respString)));
        memcpy(respBuf, respString, sizeof(respString));
        *resp = respBuf;
    }

    return NULL;
}

SCENARIO("NoteTransaction")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    // NoteReset's mock should succeed unless the test explicitly instructs
    // it to fail.
    NoteReset_fake.return_val = true;
    NoteTransactionStart_fake.return_val = true;
    crcError_fake.return_val = false;

    SECTION("Passing a NULL request returns NULL") {
        CHECK(NoteTransaction(NULL) == NULL);
    }

    SECTION("NoteTransactionStart fails") {
        NoteTransactionStart_fake.return_val = false;
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != NULL);

        CHECK(NoteTransaction(req) == NULL);

        JDelete(req);
    }

    SECTION("A response is expected and the response is valid") {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != NULL);
        NoteJSONTransaction_fake.custom_fake = NoteJSONTransactionValid;

        J *resp = NoteTransaction(req);

        CHECK(NoteJSONTransaction_fake.call_count == 1);
        CHECK(resp != NULL);
        // Ensure there's no error in the response.
        CHECK(!NoteResponseError(resp));

        JDelete(req);
        JDelete(resp);
    }

    SECTION("A response is expected and the response has an error") {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != NULL);
        NoteJSONTransaction_fake.return_val = "This is an error.";

        J *resp = NoteTransaction(req);

        // Ensure the mock is called at least once
        // Here the error causes multiple invocations by retries
        CHECK(NoteJSONTransaction_fake.call_count >= 1);

        // Ensure there's an error in the response.
        CHECK(resp != NULL);
        CHECK(NoteResponseError(resp));

        JDelete(req);
        JDelete(resp);
    }

    WHEN("A response is expected and the response has a {bad-bin} error") {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != NULL);
        NoteJSONTransaction_fake.custom_fake = [](char *, char **response, size_t) -> const char * {
            *response = (char *)malloc(20);
            strncpy(*response, "{\"err\":\"{bad-bin}\"}", 20);
            return nullptr;
        };

        J *resp = NoteTransaction(req);

        // Ensure the mock is called at least once
        // Here the error causes multiple invocations by retries
        REQUIRE(NoteJSONTransaction_fake.call_count >= 1);

        // Ensure there's an error in the response.
        REQUIRE(resp != NULL);

        THEN("The transaction is not retried") {
            CHECK(NoteJSONTransaction_fake.call_count == 1);
        }

        JDelete(req);
        JDelete(resp);
    }

    SECTION("Bad CRC") {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != NULL);
        NoteJSONTransaction_fake.custom_fake = NoteJSONTransactionValid;
        crcError_fake.return_val = true;

        J *resp = NoteTransaction(req);

        CHECK(resp != NULL);
        CHECK(NoteResponseError(resp));

        JDelete(req);
        JDelete(resp);
    }

    SECTION("I/O error") {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != NULL);
        NoteJSONTransaction_fake.custom_fake = NoteJSONTransactionIOError;

        J *resp = NoteTransaction(req);

        CHECK(resp != NULL);
        CHECK(NoteResponseError(resp));

        JDelete(req);
        JDelete(resp);
    }

    SECTION("A reset is required and it fails") {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != NULL);
        NoteResetRequired();
        // Force NoteReset failure.
        NoteReset_fake.return_val = false;

        J *resp = NoteTransaction(req);

        CHECK(NoteReset_fake.call_count == 1);
        // The transaction shouldn't be attempted if reset failed.
        CHECK(NoteJSONTransaction_fake.call_count == 0);
        // The response should be null if reset failed.
        CHECK(resp == NULL);

        JDelete(req);
    }

    SECTION("Serializing the JSON request fails") {
        // Create an invalid J object.
        J *req = reinterpret_cast<J *>(malloc(sizeof(J)));
        REQUIRE(req != NULL);
        memset(req, 0, sizeof(J));

        J *resp = NoteTransaction(req);

        // The transaction shouldn't be attempted if the request couldn't be
        // serialized.
        CHECK(NoteJSONTransaction_fake.call_count == 0);
        // Ensure there's an error in the response.
        CHECK(resp != NULL);
        CHECK(NoteResponseError(resp));

        JDelete(req);
        JDelete(resp);
    }

    SECTION("No response is expected") {
        J *req = NoteNewCommand("note.add");
        REQUIRE(req != NULL);
        NoteJSONTransaction_fake.custom_fake = NoteJSONTransactionValid;

        J *resp = NoteTransaction(req);

        CHECK(NoteJSONTransaction_fake.call_count == 1);
        CHECK(resp != NULL);
        // Ensure there's no error in the response.
        CHECK(!NoteResponseError(resp));
        // Ensure a blank object was returned.
        J *expectedResp = JCreateObject();
        CHECK(JCompare(resp, expectedResp, true));

        JDelete(req);
        JDelete(resp);
        JDelete(expectedResp);
    }

    SECTION("Parsing the JSON response fails") {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != NULL);
        NoteJSONTransaction_fake.custom_fake = NoteJSONTransactionBadJSON;

        J *resp = NoteTransaction(req);

        CHECK(NoteJSONTransaction_fake.call_count >= 1);
        CHECK(resp != NULL);
        // Ensure there's an error in the response.
        CHECK(NoteResponseError(resp));

        JDelete(req);
        JDelete(resp);
    }

#ifndef NOTE_DISABLE_USER_AGENT
    SECTION("hub.set with product adds user agent information") {
        J *req = NoteNewRequest("hub.set");
        REQUIRE(req != NULL);
        JAddStringToObject(req, "product", "a.b.c:d");
        NoteJSONTransaction_fake.custom_fake = NoteJSONTransactionValid;
        NoteUserAgent_fake.return_val = JCreateObject();

        J *resp = NoteTransaction(req);
        CHECK(resp != NULL);
        CHECK(NoteUserAgent_fake.call_count > 0);

        JDelete(req);
        JDelete(resp);
    }
#endif // !NOTE_DISABLE_USER_AGENT

    SECTION("Regular transactions have a timeout of CARD_INTER_TRANSACTION_TIMEOUT_SEC seconds") {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != NULL);

        J *resp = NoteTransaction(req);
        CHECK(NoteJSONTransaction_fake.arg2_val == (CARD_INTER_TRANSACTION_TIMEOUT_SEC * 1000));

        JDelete(req);
        JDelete(resp);
    }

    SECTION("`note.add` with `milliseconds` updates timeout value with `milliseconds`") {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != NULL);
        JAddIntToObject(req, "milliseconds", 9171979);

        J *resp = NoteTransaction(req);
        CHECK(NoteJSONTransaction_fake.arg2_val == 9171979);

        JDelete(req);
        JDelete(resp);
    }

    SECTION("`note.add` with `seconds` updates timeout value with `seconds`") {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != NULL);
        JAddIntToObject(req, "seconds", 917);

        J *resp = NoteTransaction(req);
        CHECK(NoteJSONTransaction_fake.arg2_val == (917 * 1000));

        JDelete(req);
        JDelete(resp);
    }

    SECTION("`note.add` with both `milliseconds` and `seconds` updates timeout value with `milliseconds`") {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != NULL);
        JAddIntToObject(req, "seconds", 917);
        JAddIntToObject(req, "milliseconds", 9171979);

        J *resp = NoteTransaction(req);
        CHECK(NoteJSONTransaction_fake.arg2_val == 9171979);

        JDelete(req);
        JDelete(resp);
    }

    SECTION("`web.post` with `milliseconds` updates timeout value with `milliseconds`") {
        J *req = NoteNewRequest("web.post");
        REQUIRE(req != NULL);
        JAddIntToObject(req, "milliseconds", 9171979);

        J *resp = NoteTransaction(req);
        CHECK(NoteJSONTransaction_fake.arg2_val == 9171979);

        JDelete(req);
        JDelete(resp);
    }

    SECTION("`web.post` with `seconds` updates timeout value with `seconds`") {
        J *req = NoteNewRequest("web.post");
        REQUIRE(req != NULL);
        JAddIntToObject(req, "seconds", 1979);

        J *resp = NoteTransaction(req);
        CHECK(NoteJSONTransaction_fake.arg2_val == (1979 * 1000));

        JDelete(req);
        JDelete(resp);
    }

    SECTION("`web.post` with both `milliseconds` and `seconds` updates timeout value with `milliseconds`") {
        J *req = NoteNewRequest("web.post");
        REQUIRE(req != NULL);
        JAddIntToObject(req, "seconds", 917);
        JAddIntToObject(req, "milliseconds", 9171979);

        J *resp = NoteTransaction(req);
        CHECK(NoteJSONTransaction_fake.arg2_val == 9171979);

        JDelete(req);
        JDelete(resp);
    }

    SECTION("`web.post` without `milliseconds` or `seconds` updates timeout value with 90 seconds") {
        J *req = NoteNewRequest("web.post");
        REQUIRE(req != NULL);

        J *resp = NoteTransaction(req);
        CHECK(NoteJSONTransaction_fake.arg2_val == (90 * 1000));

        JDelete(req);
        JDelete(resp);
    }

    RESET_FAKE(NoteReset);
    RESET_FAKE(NoteJSONTransaction);
    RESET_FAKE(NoteTransactionStart);
    RESET_FAKE(crcError);
}

}

#endif // NOTE_C_TEST
