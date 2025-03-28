/*!
 * @file NoteLocationValid_test.cpp
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

#include <catch2/catch_test_macros.hpp>
#include <fff.h>

#include "n_lib.h"

DEFINE_FFF_GLOBALS
FAKE_VALUE_FUNC(J *, NoteNewRequest, const char *)
FAKE_VALUE_FUNC(J *, NoteRequestResponse, J *)

namespace
{

SCENARIO("NoteLocationValid")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    char errBuf[32];

    SECTION("No response") {
        NoteRequestResponse_fake.return_val = NULL;

        CHECK(!NoteLocationValid(errBuf, sizeof(errBuf)));
    }

    SECTION("Response") {
        J* resp = JCreateObject();
        REQUIRE(resp != NULL);
        NoteRequestResponse_fake.return_val = resp;

        SECTION("Error in response") {
            const char errMsg[] = "an error";
            JAddStringToObject(resp, "err", errMsg);

            CHECK(!NoteLocationValid(errBuf, sizeof(errBuf)));
            CHECK(!strcmp(errBuf, errMsg));
        }

        SECTION("Location off") {
            JAddStringToObject(resp, "mode", "off");

            CHECK(NoteLocationValid(errBuf, sizeof(errBuf)));
            CHECK(!strcmp(errBuf, ""));
        }

        SECTION("If previously valid, should stay valid if time-suppressed") {
            CHECK(NoteLocationValid(errBuf, sizeof(errBuf)));
            CHECK(!strcmp(errBuf, ""));

            CHECK(NoteLocationValidST(errBuf, sizeof(errBuf)));
            CHECK(!strcmp(errBuf, ""));

            // The valid result from the first NoteLocationValid call should be
            // cached and no subsequent NoteRequestResponse call should be made.
            CHECK(NoteRequestResponse_fake.call_count == 1);
        }
    }

    RESET_FAKE(NoteNewRequest);
    RESET_FAKE(NoteRequestResponse);
}

}


