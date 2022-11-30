/*!
 * @file NoteTransaction_test.cpp
 *
 * Written by the Blues Inc. team.
 *
 * Copyright (c) 2022 Blues Inc. MIT License. Use of this source code is
 * governed by licenses granted by the copyright holder including that found in
 * the
 * <a href="https://github.com/blues/note-c/blob/master/LICENSE">LICENSE</a>
 * file.
 *
 */

#include <catch2/catch_test_macros.hpp>
#include "fff.h"

#include "n_lib.h"

DEFINE_FFF_GLOBALS;
// These note-c functions are mocked for the purposes of testing
// NoteTransaction.
FAKE_VALUE_FUNC(bool, NoteReset)
FAKE_VALUE_FUNC(const char *, NoteJSONTransaction, char *, char **)

namespace
{

const char *NoteJSONTransactionValid(char *req, char **resp)
{
    static char respString[] = "{ \"total\": 1 }";

    if (resp) {
        char* respBuf = reinterpret_cast<char *>(malloc(sizeof(respString)));
        memcpy(respBuf, respString, sizeof(respString));
        *resp = respBuf;
    }

    return NULL;
}

const char *NoteJSONTransactionError(char *req, char **resp)
{
    return "This is an error.";
}

const char *NoteJSONTransactionBadJSON(char *req, char **resp)
{
    static char respString[] = "Bad JSON";

    if (resp) {
        char* respBuf = reinterpret_cast<char *>(malloc(sizeof(respString)));
        memcpy(respBuf, respString, sizeof(respString));
        *resp = respBuf;
    }

    return NULL;
}

TEST_CASE("NoteTransaction")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    RESET_FAKE(NoteReset);
    RESET_FAKE(NoteJSONTransaction);

    // NoteReset's mock should succeed unless the test explicitly instructs
    // it to fail.
    NoteReset_fake.return_val = true;

    SECTION("Passing a NULL request returns NULL")
    {
        REQUIRE(NoteTransaction(NULL) == nullptr);
    }

    SECTION("A response is expected and the response is valid")
    {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != nullptr);
        NoteJSONTransaction_fake.custom_fake = NoteJSONTransactionValid;

        J *resp = NoteTransaction(req);

        REQUIRE(NoteJSONTransaction_fake.call_count == 1);
        REQUIRE(resp != nullptr);
        // Ensure there's no error in the response.
        REQUIRE(!NoteResponseError(resp));

        JDelete(req);
        JDelete(resp);
    }

    SECTION("A response is expected and the response has an error")
    {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != nullptr);
        NoteJSONTransaction_fake.custom_fake = NoteJSONTransactionError;

        J *resp = NoteTransaction(req);

        REQUIRE(NoteJSONTransaction_fake.call_count == 1);
        // Ensure there's an error in the response.
        REQUIRE(resp != nullptr);
        REQUIRE(NoteResponseError(resp));

        JDelete(req);
        JDelete(resp);
    }

    SECTION("A reset is required and it fails")
    {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != nullptr);
        NoteResetRequired();
        // Force NoteReset failure.
        NoteReset_fake.return_val = false;

        J *resp = NoteTransaction(req);

        REQUIRE(NoteReset_fake.call_count == 1);
        // The transaction shouldn't be attempted if reset failed.
        REQUIRE(NoteJSONTransaction_fake.call_count == 0);
        // The response should be null if reset failed.
        REQUIRE(resp == nullptr);

        JDelete(req);
    }

    SECTION("Serializing the JSON request fails")
    {
        // Create an invalid J object.
        J *req = reinterpret_cast<J *>(malloc(sizeof(J)));
        REQUIRE(req != nullptr);
        memset(req, 0, sizeof(J));

        J *resp = NoteTransaction(req);

        // The transaction shouldn't be attempted if the request couldn't be
        // serialized.
        REQUIRE(NoteJSONTransaction_fake.call_count == 0);
        // Ensure there's an error in the response.
        REQUIRE(resp != nullptr);
        REQUIRE(NoteResponseError(resp));

        JDelete(req);
        JDelete(resp);
    }

    SECTION("No response is expected")
    {
        J *req = NoteNewCommand("note.add");
        REQUIRE(req != nullptr);
        NoteJSONTransaction_fake.custom_fake = NoteJSONTransactionValid;

        J *resp = NoteTransaction(req);

        REQUIRE(NoteJSONTransaction_fake.call_count == 1);
        REQUIRE(resp != nullptr);
        // Ensure there's no error in the response.
        REQUIRE(!NoteResponseError(resp));
        // Ensure a blank object was returned.
        J *expectedResp = JCreateObject();
        REQUIRE(JCompare(resp, expectedResp, true));

        JDelete(req);
        JDelete(resp);
        JDelete(expectedResp);
    }

    SECTION("Parsing the JSON response fails")
    {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != nullptr);
        NoteJSONTransaction_fake.custom_fake = NoteJSONTransactionBadJSON;

        J *resp = NoteTransaction(req);

        REQUIRE(NoteJSONTransaction_fake.call_count == 1);
        REQUIRE(resp != nullptr);
        // Ensure there's an error in the response.
        REQUIRE(NoteResponseError(resp));

        JDelete(req);
        JDelete(resp);
    }
}

}
