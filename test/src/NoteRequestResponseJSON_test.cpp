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
        char cmd1[] = "{\"cmd\":\"card.version\"}\n";
        char cmd2[] = "{\"cmd\":\"card.sleep\"}\n";
        char req[sizeof(cmd1) + sizeof(cmd2) - 1] = {0};
        strlcat(req, cmd1, sizeof(req));
        strlcat(req, cmd2, sizeof(req));

        const size_t command_count = 2;

        WHEN("NoteRequestResponseJSON is called") {
            char *rsp = NoteRequestResponseJSON(req);

            THEN("NoteJSONTransaction is called once for each command") {
                CHECK(NoteJSONTransaction_fake.call_count == command_count);
            }

            THEN("NoteJSONTransaction is called with the correct pointer and "
                 "length for each command") {
                CHECK(NoteJSONTransaction_fake.arg0_history[0] == req);
                CHECK(NoteJSONTransaction_fake.arg1_history[0] ==
                      (sizeof(cmd1) - 1));
                CHECK(NoteJSONTransaction_fake.arg0_history[1] ==
                      (req + sizeof(cmd1) - 1));
                CHECK(NoteJSONTransaction_fake.arg1_history[1] ==
                      (sizeof(cmd2) - 1));
            }
        }
    }

    GIVEN("The request is not a command") {
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

    GIVEN("The request is not a command but it has an inner object with a "
          "\"cmd\" field") {
        char req[] = "{\"req\":\"note.add\",\"body\":{\"cmd\":\"fire\"}}\n";

        WHEN("NoteRequestResponseJSON is called") {
            char *rsp = NoteRequestResponseJSON(req);

            THEN("The request is still treated as a regular request and not a "
                 "command") {
                REQUIRE(NoteJSONTransaction_fake.call_count > 0);
                // The response parameter is non-NULL, meaning we expect a
                // response (i.e. it's not a command).
                CHECK(NoteJSONTransaction_fake.arg2_history[0] != NULL);
            }
        }
    }

    GIVEN("The request doesn't have a terminating newline") {
        char req[] = "{\"req\":\"card.version\"}";

        WHEN("NoteRequestResponseJSON is called") {
            char *rsp = NoteRequestResponseJSON(req);

            THEN("NoteJSONTransaction isn't called") {
                CHECK(NoteJSONTransaction_fake.call_count == 0);
            }

            THEN("NULL is returned") {
                CHECK(rsp == NULL);
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
