/*!
 * @file NoteRequestResponseJSON_test.cpp
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
FAKE_VALUE_FUNC(bool, NoteTransactionStart, uint32_t)
FAKE_VOID_FUNC(NoteTransactionStop)
FAKE_VOID_FUNC(NoteLockNote)
FAKE_VOID_FUNC(NoteUnlockNote)
FAKE_VALUE_FUNC(const char *, NoteJSONTransaction, const char *, size_t, char **, size_t)

namespace
{

SCENARIO("NoteRequestResponseJSON")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    NoteTransactionStart_fake.return_val = true;

    GIVEN("The request is NULL") {
        WHEN("NoteRequestResponseJSON is called") {
            char *rsp = NoteRequestResponseJSON(NULL);

            THEN("The response is NULL") {
                CHECK(rsp == NULL);
            }
        }
    }

    GIVEN("The request is a command") {
        char req[] = "{\"cmd\":\"card.sleep\"}\n";

        WHEN("NoteRequestResponseJSON is called") {
            char *rsp = NoteRequestResponseJSON(req);

            THEN("NoteJSONTransaction is called with NULL for the response "
                 "parameter") {
                REQUIRE(NoteJSONTransaction_fake.call_count > 0);
                CHECK(NoteJSONTransaction_fake.arg2_history[0] == NULL);
            }
        }
    }

    GIVEN("The request is a command pipeline") {
        char req[] = "{\"cmd\":\"card.version\"}\n{\"cmd\":\"card.sleep\"}\n";
        const size_t command_count = 2;

        WHEN("NoteRequestResponseJSON is called") {
            char *rsp = NoteRequestResponseJSON(req);

            THEN("NoteJSONTransaction is called once for each command") {
                CHECK(NoteJSONTransaction_fake.call_count == command_count);
            }
        }
    }

    GIVEN("The request is not a command)") {
        char req[] = "{\"req\":\"card.version\"}\n";

        WHEN("NoteRequestResponseJSON is called") {
            char *rsp = NoteRequestResponseJSON(req);

            THEN("NoteJSONTransaction is called with a valid pointer for the "
                 "response parameter") {
                REQUIRE(NoteJSONTransaction_fake.call_count > 0);
                CHECK(NoteJSONTransaction_fake.arg2_history[0] != NULL);
            }
        }
    }

    GIVEN("The request is valid") {
        char req[] = "{\"req\":\"card.version\"}\n";

        WHEN("NoteRequestResponseJSON is called") {
            char *rsp = NoteRequestResponseJSON(req);

            THEN("The Notecard is locked and unlocked") {
                REQUIRE(NoteJSONTransaction_fake.call_count > 0);
                CHECK(NoteLockNote_fake.call_count == 1);
                CHECK(NoteUnlockNote_fake.call_count == 1);
            }

            THEN("The transaction start/stop function are used to prepare the "
                 "Notecard for the transaction") {
                REQUIRE(NoteJSONTransaction_fake.call_count > 0);
                CHECK(NoteTransactionStart_fake.call_count == 1);
                CHECK(NoteTransactionStop_fake.call_count == 1);
            }
        }

        AND_GIVEN("The transaction with the Notecard fails to start") {
            NoteTransactionStart_fake.return_val = false;

            WHEN("NoteRequestResponseJSON is called") {
                char *rsp = NoteRequestResponseJSON(req);

                THEN("NULL is returned") {
                    REQUIRE(NoteTransactionStart_fake.call_count > 0);
                    CHECK(rsp == NULL);
                }
            }
        }
    }

    RESET_FAKE(NoteTransactionStart);
    RESET_FAKE(NoteTransactionStop);
    RESET_FAKE(NoteLockNote);
    RESET_FAKE(NoteUnlockNote);
    RESET_FAKE(NoteJSONTransaction);
}

}

#endif // NOTE_C_TEST
