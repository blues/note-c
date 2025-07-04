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

#include <catch2/catch_test_macros.hpp>
#include <fff.h>

#include "n_lib.h"

DEFINE_FFF_GLOBALS

FAKE_VALUE_FUNC(const char *, _noteJSONTransaction, const char *, size_t, char **, uint32_t)
FAKE_VOID_FUNC(_noteLockNote)
FAKE_VALUE_FUNC(bool, _noteTransactionStart, uint32_t)
FAKE_VOID_FUNC(_noteTransactionStop)
FAKE_VOID_FUNC(_noteUnlockNote)
FAKE_VALUE_FUNC(N_CJSON_PUBLIC(J *), JParse, const char *)
FAKE_VALUE_FUNC(char *, JPrintUnformatted, const J *)
FAKE_VOID_FUNC(NoteDebugWithLevel, uint8_t, const char *)
FAKE_VOID_FUNC(NoteFree, void *)
FAKE_VALUE_FUNC(void *, NoteMalloc, size_t)

extern mallocFn hookMalloc;
extern freeFn hookFree;
extern delayMsFn hookDelayMs;
extern getMsFn hookGetMs;

namespace
{

// Required to validate the value of the command
// pipeline request without a terminating newline.
char * call1_arg0_copy;

SCENARIO("NoteRequestResponseJSON")
{
    hookMalloc = malloc;
    hookFree = free;
    hookDelayMs = NULL;
    hookGetMs = NULL;

    JParse_fake.custom_fake = [](const char *value) -> J * {
        return JParseWithOpts(value,0,0);
    };
    JPrintUnformatted_fake.custom_fake = [](const J *json) -> char * {
        return (char *)_print(json, false, false);
    };
    NoteMalloc_fake.custom_fake = malloc;
    NoteFree_fake.custom_fake = free;
    _noteTransactionStart_fake.return_val = true;

    GIVEN("The request is NULL") {
        WHEN("NoteRequestResponseJSON is called") {
            char *rsp = NoteRequestResponseJSON(NULL);

            THEN("_noteJSONTransaction is not called") {
                CHECK(_noteJSONTransaction_fake.call_count == 0);
            }

            THEN("The response is NULL") {
                CHECK(rsp == NULL);
            }
        }
    }

    GIVEN("The request length is zero (0)") {
        WHEN("NoteRequestResponseJSON is called") {
            char *rsp = NoteRequestResponseJSON("");

            THEN("_noteJSONTransaction is not called") {
                CHECK(_noteJSONTransaction_fake.call_count == 0);
            }

            THEN("The response is NULL") {
                CHECK(rsp == NULL);
            }
        }
    }

    GIVEN("A valid, non-newline terminated command (cmd)") {
        char req[] = "{\"cmd\":\"card.sleep\"}";

        WHEN("NoteRequestResponseJSON is called") {
            char *rsp = NoteRequestResponseJSON(req);

            THEN("_noteJSONTransaction is called with NULL for the response "
                 "parameter") {
                REQUIRE(_noteJSONTransaction_fake.call_count > 0);
                CHECK(_noteJSONTransaction_fake.arg2_history[0] == NULL);
            }
        }

        WHEN("NoteRequestResponseJSON is called") {
            char *rsp = NoteRequestResponseJSON(req);

            THEN("The response is NULL") {
                CHECK(rsp == NULL);
            }
        }
    }

    GIVEN("A valid command (cmd)") {
        char req[] = "{\"cmd\":\"card.sleep\"}\n";

        WHEN("NoteRequestResponseJSON is called") {
            char *rsp = NoteRequestResponseJSON(req);

            THEN("_noteJSONTransaction is called with NULL for the response "
                 "parameter") {
                REQUIRE(_noteJSONTransaction_fake.call_count > 0);
                CHECK(_noteJSONTransaction_fake.arg2_history[0] == NULL);
            }
        }

        AND_GIVEN("The transaction with the Notecard encounters an error") {
            _noteJSONTransaction_fake.return_val = "an error occurred";

            WHEN("NoteRequestResponseJSON is called") {
                char *rsp = NoteRequestResponseJSON(req);

                THEN("The response is NULL") {
                    CHECK(rsp == NULL);
                }

                THEN("The error message is logged with LOG_LEVEL_ERROR") {
                    CHECK(NoteDebugWithLevel_fake.call_count > 0);
                    CHECK(NoteDebugWithLevel_fake.arg0_history[0] ==
                          NOTE_C_LOG_LEVEL_ERROR);
                }
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

            THEN("_noteJSONTransaction is called once for each command") {
                CHECK(_noteJSONTransaction_fake.call_count == command_count);
            }

            THEN("_noteJSONTransaction is called with the correct pointer and "
                 "length for each command") {
                CHECK(_noteJSONTransaction_fake.arg0_history[0] == req);
                CHECK(_noteJSONTransaction_fake.arg1_history[0] ==
                      (sizeof(cmd1) - 1));
                CHECK(_noteJSONTransaction_fake.arg0_history[1] ==
                      (req + sizeof(cmd1) - 1));
                CHECK(_noteJSONTransaction_fake.arg1_history[1] ==
                      (sizeof(cmd2) - 1));
            }
        }
    }

    GIVEN("The request is a command pipeline without a terminating newline") {
        _noteJSONTransaction_fake.custom_fake = [](const char *request, size_t reqLen, char **response, uint32_t timeoutMs) -> const char * {
            // Capture the value of the request on the second call, because the
            // `_noteJSONTransaction` function will add the missing newline to
            // the end of the request.
            if (_noteJSONTransaction_fake.call_count == 2)
            {
                call1_arg0_copy = (char *)malloc(reqLen + 1);
                strncpy(call1_arg0_copy, request, reqLen);
                call1_arg0_copy[reqLen] = '\0';
            }

            return NULL;
        };

        char cmd1[] = "{\"cmd\":\"card.version\"}\n";
        char cmd2[] = "{\"cmd\":\"card.sleep\"}";
        char req[sizeof(cmd1) + sizeof(cmd2) - 1] = {0};
        strlcat(req, cmd1, sizeof(req));
        strlcat(req, cmd2, sizeof(req));

        const size_t command_count = 2;

        WHEN("NoteRequestResponseJSON is called") {
            char *rsp = NoteRequestResponseJSON(req);

            THEN("_noteJSONTransaction is called once for each command") {
                CHECK(_noteJSONTransaction_fake.call_count == command_count);
            }

            THEN("_noteJSONTransaction is called with the correct pointer and "
                 "length for each command") {
                CHECK(_noteJSONTransaction_fake.arg0_history[0] == req);
                CHECK(_noteJSONTransaction_fake.arg1_history[0] == strlen(cmd1));

                // Special checks for added newline
                CHECK(strstr(call1_arg0_copy, (req + strlen(cmd1))));
                CHECK(_noteJSONTransaction_fake.arg1_history[1] == (strlen(cmd2) + sizeof('\n')));
            }

            NoteFree(call1_arg0_copy);
        }
    }

    GIVEN("The request is not a command (req)") {
        char req[] = "{\"req\":\"card.version\"}\n";

        WHEN("NoteRequestResponseJSON is called") {
            char *rsp = NoteRequestResponseJSON(req);

            THEN("_noteJSONTransaction is called with a valid pointer for the "
                 "response parameter") {
                REQUIRE(_noteJSONTransaction_fake.call_count > 0);
                CHECK(_noteJSONTransaction_fake.arg2_history[0] != NULL);
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
                REQUIRE(_noteJSONTransaction_fake.call_count > 0);
                // The response parameter is non-NULL, meaning we expect a
                // response (i.e. it's not a command).
                CHECK(_noteJSONTransaction_fake.arg2_history[0] != NULL);
            }
        }
    }

    GIVEN("A malformed, non-newline terminated command") {
        char req[] = "{\"cmd\":\"card.version}";

        AND_GIVEN("The request fails to parse") {
            JParse_fake.custom_fake = nullptr;
            JParse_fake.return_val = NULL;

            WHEN("NoteRequestResponseJSON is called") {
                char *rsp = NoteRequestResponseJSON(req);

                THEN("_noteJSONTransaction is not called") {
                    REQUIRE(NoteMalloc_fake.call_count > 0);
                    CHECK(_noteJSONTransaction_fake.call_count == 0);
                }

                THEN("NoteFree is called") {
                    REQUIRE(NoteMalloc_fake.call_count > 0);
                    CHECK(NoteFree_fake.call_count == NoteMalloc_fake.call_count);
                }

                THEN("NoteRequestResponseJSON returns NULL") {
                    CHECK(rsp == NULL);
                }
            }
        }
    }

    GIVEN("A malformed, newline-terminated command") {
        char req[] = "{\"cmd\":\"card.version}\n";

        AND_GIVEN("The request fails to parse") {
            JParse_fake.custom_fake = nullptr;
            JParse_fake.return_val = NULL;

            WHEN("NoteRequestResponseJSON is called") {
                char *rsp = NoteRequestResponseJSON(req);

                THEN("_noteJSONTransaction is not called") {
                    CHECK(_noteJSONTransaction_fake.call_count == 0);
                }

                THEN("NoteFree is not called") {
                    REQUIRE(NoteMalloc_fake.call_count == 0);
                    CHECK(NoteFree_fake.call_count == 0);
                }

                THEN("NoteRequestResponseJSON returns NULL") {
                    CHECK(rsp == NULL);
                }
            }
        }
    }

    GIVEN("The request doesn't have a terminating newline") {
        char req[] = "{\"req\":\"card.version\"}";

        WHEN("NoteMalloc fails to duplicate the request") {
            NoteMalloc_fake.custom_fake = nullptr;
            NoteMalloc_fake.return_val = NULL;
            char *rsp = NoteRequestResponseJSON(req);

            THEN("_noteJSONTransaction is not called") {
                REQUIRE(NoteMalloc_fake.call_count > 0);
                CHECK(_noteJSONTransaction_fake.call_count == 0);
            }

            THEN("NoteFree is not called") {
                REQUIRE(NoteMalloc_fake.call_count > 0);
                CHECK(NoteFree_fake.call_count == 0);
            }

            THEN("NoteRequestResponseJSON returns NULL") {
                REQUIRE(NoteMalloc_fake.call_count > 0);
                CHECK(rsp == NULL);
            }
        }

        WHEN("NoteMalloc is able to duplicate the request") {
            char *rsp = NoteRequestResponseJSON(req);

            THEN("NoteMalloc is called") {
                REQUIRE(_noteJSONTransaction_fake.call_count > 0);
                CHECK(NoteMalloc_fake.call_count > 0);
            }

            THEN("_noteJSONTransaction is called with the newline appended") {
                REQUIRE(_noteJSONTransaction_fake.call_count > 0);
                REQUIRE(_noteJSONTransaction_fake.arg0_val != NULL);
                // We cannot test the value of the string passed to
                // `_noteJSONTransaction` because it is a pointer to a string
                // freed by `NoteFree`. We can only check that the length
                // of the string is one longer than would normally be expected.
                CHECK(_noteJSONTransaction_fake.arg1_val == sizeof(req));
            }

            THEN("NoteFree is called to free the memory allocated by malloc") {
                REQUIRE(_noteJSONTransaction_fake.call_count > 0);
                CHECK(NoteFree_fake.call_count == NoteMalloc_fake.call_count);
            }
        }
    }

    GIVEN("A valid request with an identifier") {
        char req[] = "{\"req\":\"card.version\",\"id\":917}\n";

        WHEN("NoteRequestResponseJSON is called") {
            char *rsp = NoteRequestResponseJSON(req);

            THEN("The Notecard is locked and unlocked") {
                REQUIRE(_noteJSONTransaction_fake.call_count > 0);
                CHECK(1 == _noteLockNote_fake.call_count);
                CHECK(_noteUnlockNote_fake.call_count == _noteLockNote_fake.call_count);
            }

            THEN("The transaction start/stop function are used to prepare the "
                 "Notecard for the transaction") {
                REQUIRE(_noteJSONTransaction_fake.call_count > 0);
                CHECK(_noteTransactionStart_fake.call_count == 1);
                CHECK(_noteTransactionStop_fake.call_count == 1);
            }

            THEN("NoteMalloc is not called") {
                REQUIRE(_noteJSONTransaction_fake.call_count > 0);
                CHECK(NoteMalloc_fake.call_count == 0);
            }

            THEN("NoteFree is not called") {
                REQUIRE(_noteJSONTransaction_fake.call_count > 0);
                CHECK(NoteFree_fake.call_count == 0);
            }
        }

        AND_GIVEN("The transaction with the Notecard fails to start") {
            _noteTransactionStart_fake.return_val = false;

            WHEN("NoteRequestResponseJSON is called") {
                char *rsp = NoteRequestResponseJSON(req);

                THEN("NULL is returned") {
                    REQUIRE(_noteTransactionStart_fake.call_count > 0);
                    CHECK(rsp == NULL);
                }
            }
        }

        AND_GIVEN("The transaction with the Notecard encounters an error") {
            _noteJSONTransaction_fake.return_val = "an error occurred";

            WHEN("NoteRequestResponseJSON is called") {
                char *rsp = NoteRequestResponseJSON(req);

                THEN("The error is returned in a well-formed JSON error string") {
                    REQUIRE(rsp != NULL);
                    J *json = JParse(rsp);

                    CHECK(JContainsString(json, "err", _noteJSONTransaction_fake.return_val));

                    JDelete(json);
                }

                THEN("The id is returned in a well-formed JSON error string") {
                    REQUIRE(rsp != NULL);
                    J *json = JParse(rsp);

                    CHECK(JGetInt(json, "id") == 917);

                    JDelete(json);
                }

                THEN("The source is returned in a well-formed JSON error string") {
                    REQUIRE(rsp != NULL);
                    J *json = JParse(rsp);

                    CHECK(JContainsString(json, "src", "note-c"));

                    JDelete(json);
                }

                NoteFree(rsp);
            }

            AND_GIVEN("The request fails to parse") {
                JParse_fake.custom_fake = nullptr;
                JParse_fake.return_val = NULL;

                WHEN("NoteRequestResponseJSON is called") {
                    char *rsp = NoteRequestResponseJSON(req);

                    THEN("The error is returned in a well-formed JSON error string") {
                        REQUIRE(rsp != NULL);
                        J *json = JParseWithOpts(rsp, 0, 0);

                        CHECK(JContainsString(json, "err", _noteJSONTransaction_fake.return_val));

                        JDelete(json);
                    }

                    THEN("The id is not returned in a well-formed JSON error string") {
                        REQUIRE(rsp != NULL);
                        J *json = JParseWithOpts(rsp, 0, 0);

                        CHECK(!JHasObjectItem(json, "id"));

                        JDelete(json);
                    }

                    THEN("The source is returned in a well-formed JSON error string") {
                        REQUIRE(rsp != NULL);
                        J *json = JParseWithOpts(rsp, 0, 0);

                        CHECK(JContainsString(json, "src", "note-c"));

                        JDelete(json);
                    }

                    NoteFree(rsp);
                }
            }

            AND_GIVEN("_errDoc fails to allocate") {
                NoteMalloc_fake.custom_fake = nullptr;
                NoteMalloc_fake.return_val = NULL;

                WHEN("NoteRequestResponseJSON is called") {
                    char *rsp = NoteRequestResponseJSON(req);

                    THEN("NULL is returned") {
                        CHECK(rsp == NULL);
                    }
                }
            }

            AND_GIVEN("Unable to print the error document") {
                JPrintUnformatted_fake.custom_fake = nullptr;
                JPrintUnformatted_fake.return_val = NULL;

                WHEN("NoteRequestResponseJSON is called") {
                    char *rsp = NoteRequestResponseJSON(req);

                    THEN("NULL is returned") {
                        CHECK(rsp == NULL);
                    }
                }
            }

            AND_GIVEN("_Malloc fails to allocate rspJSON") {
                NoteMalloc_fake.custom_fake = [](size_t size) -> void * {
                    // Allow the first seventeen calls to malloc to succeed for
                    // JParse(6), JCreateObject(9) and JPrintUnformatted(2), but
                    // fail the eighteenth call.
                    if (NoteMalloc_fake.call_count > 17)
                    {
                        return NULL;
                    }
                    return malloc(size);
                };

                WHEN("NoteRequestResponseJSON is called") {
                    char *rsp = NoteRequestResponseJSON(req);

                    THEN("NULL is returned") {
                        CHECK(rsp == NULL);
                    }
                }
            }
        }
    }

    RESET_FAKE(_noteJSONTransaction);
    RESET_FAKE(_noteLockNote);
    RESET_FAKE(_noteTransactionStart);
    RESET_FAKE(_noteTransactionStop);
    RESET_FAKE(_noteUnlockNote);
    RESET_FAKE(JParse);
    RESET_FAKE(JPrintUnformatted);
    RESET_FAKE(NoteDebugWithLevel);
    RESET_FAKE(NoteFree);
    RESET_FAKE(NoteMalloc);
}

}
