/*!
 * @file NoteGetLocationMode_test.cpp
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

SCENARIO("NoteGetLocationMode")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    char modeBuf[32];

    SECTION("No response") {
        NoteRequestResponse_fake.return_val = NULL;

        CHECK(!NoteGetLocationMode(modeBuf, sizeof(modeBuf)));
    }

    SECTION("Response") {
        J* resp = JCreateObject();
        REQUIRE(resp != NULL);
        NoteRequestResponse_fake.return_val = resp;

        SECTION("Error in response") {
            const char errMsg[] = "an error";
            JAddStringToObject(resp, "err", errMsg);

            CHECK(!NoteGetLocationMode(modeBuf, sizeof(modeBuf)));
        }

        SECTION("Valid") {
            char mode[] = "continuous";
            JAddStringToObject(resp, "mode", mode);

            CHECK(NoteGetLocationMode(modeBuf, sizeof(modeBuf)));
            CHECK(!strcmp(modeBuf, modeBuf));
        }
    }

    RESET_FAKE(NoteNewRequest);
    RESET_FAKE(NoteRequestResponse);
}

}


