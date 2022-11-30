/*!
 * @file NoteRequest_test.cpp
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

#include "note.h"
#include "n_lib.h"

DEFINE_FFF_GLOBALS;
// These note-c functions are mocked for the purposes of testing
// NoteRequest.
FAKE_VALUE_FUNC(J *, NoteTransaction, J *)

namespace
{

J *NoteTransactionValid(J *req)
{
    return JCreateObject();
}

J *NoteTransactionError(J *req)
{
    J *resp = JCreateObject();
    assert(resp != NULL);
    JAddStringToObject(resp, c_err, "An error.");

    return resp;
}

TEST_CASE("NoteRequest")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    RESET_FAKE(NoteTransaction);

    SECTION("Passing a NULL request returns false")
    {
        REQUIRE(!NoteRequest(NULL));
    }


    SECTION("NoteTransaction returns NULL")
    {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != nullptr);
        NoteTransaction_fake.return_val = NULL;

        REQUIRE(!NoteRequest(req));
        REQUIRE(NoteTransaction_fake.call_count == 1);
    }

    SECTION("NoteTransaction returns a response with an error")
    {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != nullptr);
        NoteTransaction_fake.custom_fake = NoteTransactionError;

        REQUIRE(!NoteRequest(req));
        REQUIRE(NoteTransaction_fake.call_count == 1);
    }

    SECTION("NoteTransaction returns a valid response")
    {
        J *req = NoteNewRequest("note.add");
        REQUIRE(req != nullptr);
        NoteTransaction_fake.custom_fake = NoteTransactionValid;

        REQUIRE(NoteRequest(req));
        REQUIRE(NoteTransaction_fake.call_count == 1);
    }
}

}
