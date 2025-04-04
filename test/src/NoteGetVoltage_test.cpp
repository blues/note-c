/*!
 * @file NoteGetVoltage_test.cpp
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

SCENARIO("NoteGetVoltage")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    JNUMBER voltage = 1.0;
    J *req = JCreateObject();
    REQUIRE(req != NULL);

    SECTION("NoteNewRequest fails") {
        NoteNewRequest_fake.return_val = NULL;

        CHECK(!NoteGetVoltage(&voltage));
        CHECK(voltage == 0);
    }

    SECTION("NoteRequestResponse fails") {
        NoteNewRequest_fake.return_val = req;
        NoteRequestResponse_fake.return_val = NULL;

        CHECK(!NoteGetVoltage(&voltage));
        CHECK(voltage == 0);
    }

    SECTION("NoteRequestResponse succeeds") {
        NoteNewRequest_fake.return_val = req;
        J *rsp = JCreateObject();
        REQUIRE(rsp != NULL);
        NoteRequestResponse_fake.return_val = rsp;

        SECTION("Response has error") {
            JAddStringToObject(rsp, "err", "an error");

            CHECK(!NoteGetVoltage(&voltage));
            CHECK(voltage == 0);
        }

        SECTION("Response ok") {
            JNUMBER rspVoltage = 3.3;
            JAddNumberToObject(rsp, "value", rspVoltage);

            CHECK(NoteGetVoltage(&voltage));
            CHECK(voltage == rspVoltage);
        }
    }

    JDelete(req);

    RESET_FAKE(NoteNewRequest);
    RESET_FAKE(NoteRequestResponse);
}

}


