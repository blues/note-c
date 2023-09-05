/*!
 * @file NoteBinaryRequiredRxMaxBuffer_test.cpp
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

DEFINE_FFF_GLOBALS
FAKE_VALUE_FUNC(J *, NoteRequestResponse, J *)

const size_t cobsLen = 10;

namespace
{

SCENARIO("NoteBinaryRequiredRxMaxBuffer")
{
    RESET_FAKE(NoteRequestResponse);

    NoteSetFnDefault(malloc, free, NULL, NULL);

    size_t size = 1;

    GIVEN("The card.binary request fails") {
        NoteRequestResponse_fake.custom_fake = [](J *req) -> J * {
            JDelete(req);

            return NULL;
        };

        WHEN("NoteBinaryRequiredRxMaxBuffer is called") {
            const char *err = NoteBinaryRequiredRxMaxBuffer(&size);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("The response to the card.binary request has an error") {
        NoteRequestResponse_fake.custom_fake = [](J *req) -> J * {
            JDelete(req);
            J *rsp = JCreateObject();
            JAddStringToObject(rsp, "err", "some error");

            return rsp;
        };

        WHEN("NoteBinaryRequiredRxMaxBuffer is called") {
            const char *err = NoteBinaryRequiredRxMaxBuffer(&size);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("The response to the card.binary request indicates there's nothing to"
          " read") {
        NoteRequestResponse_fake.custom_fake = [](J *req) -> J * {
            JDelete(req);
            J *rsp = JCreateObject();
            JAddIntToObject(rsp, "cobs", 0);

            return rsp;
        };

        WHEN("NoteBinaryRequiredRxMaxBuffer is called") {
            const char *err = NoteBinaryRequiredRxMaxBuffer(&size);

            THEN("An error is not returned") {
                CHECK(err == NULL);
            }

            THEN("The size out parameter is 0") {
                CHECK(size == 0);
            }
        }
    }

    GIVEN("The response to the card.binary request indicates there's data "
          "to read") {
        NoteRequestResponse_fake.custom_fake = [](J *req) -> J * {
            JDelete(req);
            J *rsp = JCreateObject();
            JAddIntToObject(rsp, "cobs", cobsLen);

            return rsp;
        };

        WHEN("NoteBinaryRequiredRxMaxBuffer is called") {
            const char *err = NoteBinaryRequiredRxMaxBuffer(&size);

            THEN("An error is not returned") {
                CHECK(err == NULL);
            }

            THEN("The size out parameter is the size in the card.binary "
                 "response, plus 1 for the trailing newline") {
                CHECK(size == cobsLen + 1);
            }
        }
    }
}

}

#endif // NOTE_C_TEST
