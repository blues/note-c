/*!
 * @file NoteBinaryReceive_test.cpp
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
FAKE_VALUE_FUNC(J *, NoteNewRequest, const char *)
FAKE_VALUE_FUNC(const char *, NoteBinaryDataEncodedLength, uint32_t *)
FAKE_VALUE_FUNC(J *, NoteRequestResponse, J *)
FAKE_VALUE_FUNC(const char *, NoteChunkedReceive, uint8_t *, uint32_t *, bool,
                size_t, uint32_t *)
FAKE_VOID_FUNC(NoteLockNote)
FAKE_VOID_FUNC(NoteUnlockNote)

// Most of these variables have to be global because they're accessed in
// lambda functions used as fakes for various note-c functions. They can't be
// captured by the lambdas because the lambdas need to be convertible to plain
// old function pointers in order to be used by the fff mocking/fake framework.
// If a lambda captures anything, it can't be converted in this way, and you get
// a compiler error.
uint8_t buf[32];
uint32_t bufLen = sizeof(buf);
uint32_t decodedOffset = 0;
uint32_t decodedLen = 17;

char rawMsg[] = "Hello Blues!";
uint32_t rawMsgLen = strlen(rawMsg);

namespace
{

SCENARIO("NoteBinaryReceive")
{
    RESET_FAKE(NoteNewRequest);
    RESET_FAKE(NoteBinaryDataEncodedLength);
    RESET_FAKE(NoteRequestResponse);
    RESET_FAKE(NoteChunkedReceive);
    RESET_FAKE(NoteLockNote);
    RESET_FAKE(NoteUnlockNote);

    NoteSetFnDefault(malloc, free, NULL, NULL);

    // These fakes are the default. Tests below may override them to exercise
    // different scenarios.
    NoteNewRequest_fake.custom_fake = [](const char *req) -> J* {
        return JCreateObject();
    };
    NoteBinaryDataEncodedLength_fake.custom_fake = [](uint32_t *size)
    -> const char * {
        *size = bufLen;

        return NULL;
    };
    NoteRequestResponse_fake.custom_fake = [](J *req) -> J * {
        JDelete(req);
        J *rsp = JCreateObject();
        char hash[NOTE_MD5_HASH_STRING_SIZE] = {0};
        NoteMD5HashString((unsigned char *)rawMsg, rawMsgLen, hash,
                          NOTE_MD5_HASH_STRING_SIZE);
        JAddStringToObject(rsp, "status", hash);

        return rsp;
    };

    GIVEN("Bad parameters are supplied") {
        WHEN("buffer is NULL") {
            const char *err = NoteBinaryReceive(NULL, bufLen, decodedOffset, decodedLen);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
        WHEN("bufLen is too small") {
            const char *err = NoteBinaryReceive(buf, bufLen, decodedOffset, bufLen);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
        WHEN("decodedLen is zero") {
            const char *err = NoteBinaryReceive(buf, bufLen, decodedOffset, 0);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("Allocating the card.binary.get request fails") {
        NoteNewRequest_fake.custom_fake = NULL;
        NoteNewRequest_fake.return_val = NULL;

        WHEN("NoteBinaryReceive is called") {
            const char *err = NoteBinaryReceive(buf, bufLen, decodedOffset, decodedLen);

            REQUIRE(NoteNewRequest_fake.call_count > 0);
            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("The response to the card.binary.get request has an error") {
        NoteRequestResponse_fake.custom_fake = [](J *req) -> J * {
            JDelete(req);
            J *rsp = JCreateObject();
            JAddStringToObject(rsp, "err", "some error");

            return rsp;
        };

        WHEN("NoteBinaryReceive is called") {
            const char *err = NoteBinaryReceive(buf, bufLen, decodedOffset, decodedLen);

            REQUIRE(NoteRequestResponse_fake.call_count > 0);
            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("NoteChunkedReceive returns an error") {
        NoteChunkedReceive_fake.return_val = "some error";

        WHEN("NoteBinaryReceive is called") {
            const char *err = NoteBinaryReceive(buf, bufLen, decodedOffset, decodedLen);

            REQUIRE(NoteChunkedReceive_fake.call_count > 0);
            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("NoteChunkedReceive indicates there's unexpectedly more data "
          "available") {
        NoteChunkedReceive_fake.custom_fake = [](uint8_t *, uint32_t *, bool,
        size_t, uint32_t *available) -> const char* {
            *available = 1;

            return NULL;
        };

        WHEN("NoteBinaryReceive is called") {
            const char *err = NoteBinaryReceive(buf, bufLen, decodedOffset, decodedLen);

            REQUIRE(NoteChunkedReceive_fake.call_count > 0);
            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("The binary payload is received") {
        NoteChunkedReceive_fake.custom_fake = [](uint8_t *buffer, uint32_t *size,
        bool, size_t, uint32_t *available) -> const char* {
            uint32_t outLen = NoteBinaryCodecEncode((uint8_t *)rawMsg, rawMsgLen, buffer, *size);

            buffer[outLen] = '\n';
            *size = outLen + 1;
            *available = 0;

            return NULL;
        };

        AND_GIVEN("The computed MD5 hash doesn't match the status field") {
            NoteRequestResponse_fake.custom_fake = [](J *req) -> J * {
                JDelete(req);
                J *rsp = JCreateObject();
                JAddStringToObject(rsp, "status", "garbage");

                return rsp;
            };

            WHEN("NoteBinaryReceive is called") {
                const char *err = NoteBinaryReceive(buf, bufLen, decodedOffset, decodedLen);

                REQUIRE(NoteChunkedReceive_fake.call_count > 0);
                REQUIRE(NoteRequestResponse_fake.call_count > 0);
                THEN("An error is returned") {
                    CHECK(err != NULL);
                }
            }
        }

        AND_GIVEN("The computed MD5 matches the status field") {
            WHEN("NoteBinaryReceive is called") {
                uint32_t decodedLen = rawMsgLen;
                const char *err = NoteBinaryReceive(buf, bufLen, decodedOffset, decodedLen);

                REQUIRE(NoteChunkedReceive_fake.call_count > 0);
                THEN("No error is returned") {
                    CHECK(err == NULL);
                }

                THEN("The decoded payload is as expected, with no trailing "
                     "newline") {
                    CHECK(memcmp(buf, rawMsg, decodedLen) == 0);
                }
            }
        }
    }
    CHECK(NoteLockNote_fake.call_count == NoteUnlockNote_fake.call_count);
}

}

#endif // NOTE_C_TEST
