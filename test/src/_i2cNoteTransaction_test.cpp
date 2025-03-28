/*!
 * @file _i2cNoteTransaction_test.cpp
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
FAKE_VALUE_FUNC(void *, NoteMalloc, size_t)
FAKE_VOID_FUNC(NoteLockI2C)
FAKE_VOID_FUNC(NoteUnlockI2C)
FAKE_VALUE_FUNC(const char *, _i2cChunkedTransmit, uint8_t *, uint32_t, bool)
FAKE_VALUE_FUNC(const char *, _i2cNoteQueryLength, uint32_t *, uint32_t)
FAKE_VALUE_FUNC(const char *, _i2cChunkedReceive, uint8_t *, uint32_t *, bool,
                uint32_t, uint32_t *)

namespace
{

SCENARIO("_i2cNoteTransaction")
{
    NoteSetFnDefault(NULL, free, NULL, NULL);

    char req[] = "{\"req\": \"note.add\"}\n";
    char originalReq[sizeof(req)];
    strlcpy(originalReq, req, sizeof(req));
    char *rsp = NULL;
    const char *err = NULL;
    NoteMalloc_fake.custom_fake = malloc;
    uint32_t timeoutMs = CARD_INTER_TRANSACTION_TIMEOUT_SEC;

    GIVEN("_i2cChunkedTransmit returns an error") {
        _i2cChunkedTransmit_fake.return_val = "some error";

        WHEN("_i2cNoteTransaction is called") {
            err = _i2cNoteTransaction(req, strlen(req), &rsp, timeoutMs);

            THEN("An error is returned") {
                REQUIRE(_i2cChunkedTransmit_fake.call_count > 0);
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("The response parameter is NULL so no Notecard response is "
          "expected") {
        WHEN("_i2cNoteTransaction is called") {
            err = _i2cNoteTransaction(req, strlen(req), NULL, timeoutMs);

            THEN("No error is returned") {
                CHECK(err == NULL);
            }
        }
    }

    GIVEN("_i2cNoteQueryLength returns an error") {
        _i2cNoteQueryLength_fake.return_val = "some error";

        WHEN("_i2cNoteTransaction is called") {
            err = _i2cNoteTransaction(req, strlen(req), &rsp, timeoutMs);

            THEN("An error is returned") {
                REQUIRE(_i2cNoteQueryLength_fake.call_count > 0);
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("_i2cNoteQueryLength reports bytes are available to read from the"
          " Notecard") {
        _i2cNoteQueryLength_fake.custom_fake = [](uint32_t * available,
        uint32_t) -> const char* {
            *available = ALLOC_CHUNK;

            return NULL;
        };

        AND_GIVEN("Allocating a buffer for the Notecard response fails") {
            NoteMalloc_fake.custom_fake = NULL;
            NoteMalloc_fake.return_val = NULL;

            WHEN("_i2cNoteTransaction is called") {
                err = _i2cNoteTransaction(req, strlen(req), &rsp, timeoutMs);

                THEN("An error is returned") {
                    REQUIRE(NoteMalloc_fake.call_count > 0);
                    CHECK(err != NULL);
                }
            }
        }

        AND_GIVEN("_i2cChunkedReceive returns an error") {
            _i2cChunkedReceive_fake.return_val = "some error";

            WHEN("_i2cNoteTransaction is called") {
                err = _i2cNoteTransaction(req, strlen(req), &rsp, timeoutMs);

                THEN("An error is returned") {
                    REQUIRE(_i2cChunkedReceive_fake.call_count > 0);
                    CHECK(err != NULL);
                }
            }
        }

        AND_GIVEN("A single chunk is required to read the full response from "
                  "the Notecard") {
            // Write out the number of bytes reported available on the prior
            // call to _i2cNoteQueryLength and report no more bytes available.
            _i2cChunkedReceive_fake.custom_fake = [](uint8_t *buf, uint32_t *size, bool, uint32_t,
            uint32_t *available) -> const char* {
                memset(buf, 'a', *available - 1);
                buf[*available - 1] = '\n';
                *size = *available;
                *available = 0;

                return NULL;
            };

            WHEN("_i2cNoteTransaction is called") {
                err = _i2cNoteTransaction(req, strlen(req), &rsp, timeoutMs);

                // If _i2cChunkedReceive is called more than once, the response
                // took more than one chunk, and this test is no longer testing
                // what it intends to.
                REQUIRE(_i2cChunkedReceive_fake.call_count == 1);

                THEN("No error is returned") {
                    CHECK(err == NULL);
                }

                THEN("The response buffer contains exactly what was returned by"
                     " _i2cChunkedReceive") {
                    char expectedRsp[ALLOC_CHUNK + 1];
                    size_t expectedSize = sizeof(expectedRsp);
                    memset(expectedRsp, 'a', expectedSize - 2);
                    expectedRsp[expectedSize - 2] = '\n';
                    expectedRsp[expectedSize - 1] = '\0';

                    CHECK(strcmp(rsp, expectedRsp) == 0);
                }

                free(rsp);
            }
        }

        AND_GIVEN("Multiple chunks are required to read the full response from "
                  "the Notecard") {
            // On the first call to _i2cChunkedReceive, a string of a's is
            // written to the output buffer, and the Notecard reports that there
            // are still ALLOC_CHUNK bytes to read.
            auto firstChunk = [](uint8_t *buf, uint32_t *size, bool, uint32_t,
            uint32_t *available) -> const char* {
                memset(buf, 'a', *available);
                *size = *available;
                *available = ALLOC_CHUNK;

                return NULL;
            };
            // On the second call, the remaining ALLOC_CHUNK bytes are written
            // out, which is a string of a's terminated with a newline.
            // available is set to 0, indicating there's nothing left to read.
            auto secondChunk = [](uint8_t *buf, uint32_t *size, bool, uint32_t,
            uint32_t *available) -> const char* {
                memset(buf, 'a', *available - 1);
                buf[*available - 1] = '\n';
                *size = *available;
                *available = 0;

                return NULL;
            };
            const char *(*recvFakeSequence[])(uint8_t *, uint32_t *, bool,
                                              uint32_t, uint32_t *) = {
                firstChunk,
                secondChunk
            };
            SET_CUSTOM_FAKE_SEQ(_i2cChunkedReceive, recvFakeSequence, 2);

            WHEN("_i2cNoteTransaction is called") {
                err = _i2cNoteTransaction(req, strlen(req), &rsp, timeoutMs);

                // Ensure that _i2cChunkedReceive was actually called more than
                // once. If it isn't, then something has gone awry and this test
                // is no longer testing what it intends to, or there's been a
                // regression/bug.
                REQUIRE(_i2cChunkedReceive_fake.call_count > 1);

                THEN("No error is returned") {
                    CHECK(err == NULL);
                }

                THEN("The response buffer contains exactly what was returned by"
                     " _i2cChunkedReceive") {
                    char expectedRsp[ALLOC_CHUNK * 2 + 1];
                    size_t expectedSize = sizeof(expectedRsp);
                    memset(expectedRsp, 'a', expectedSize - 2);
                    expectedRsp[expectedSize - 2] = '\n';
                    expectedRsp[expectedSize - 1] = '\0';

                    CHECK(strcmp(rsp, expectedRsp) == 0);
                }

                free(rsp);
            }

            AND_GIVEN("Allocating a larger response buffer fails") {
                NoteMalloc_fake.custom_fake = NULL;
                // The first allocation that sets up the initial response buffer
                // is ok.
                auto normalMalloc = [](size_t size) -> void * {
                    return malloc(size);
                };
                // The second one used to grow the response buffer fails.
                auto failMalloc = [](size_t size) -> void * {
                    return NULL;
                };
                void *(*mallocFakeSequence[])(size_t) = {
                    normalMalloc,
                    failMalloc
                };
                SET_CUSTOM_FAKE_SEQ(NoteMalloc, mallocFakeSequence, 2);

                WHEN("_i2cNoteTransaction is called") {
                    err = _i2cNoteTransaction(req, strlen(req), &rsp, timeoutMs);

                    THEN("An error is returned") {
                        CHECK(err != NULL);
                    }
                }
            }
        }
    }

    THEN("The request parameter is unchanged") {
        CHECK(strcmp(originalReq, req) == 0);
    }

    CHECK(NoteLockI2C_fake.call_count == NoteUnlockI2C_fake.call_count);

    RESET_FAKE(NoteMalloc);
    RESET_FAKE(NoteLockI2C);
    RESET_FAKE(NoteUnlockI2C);
    RESET_FAKE(_i2cChunkedTransmit);
    RESET_FAKE(_i2cNoteQueryLength);
    RESET_FAKE(_i2cChunkedReceive);
}

}


