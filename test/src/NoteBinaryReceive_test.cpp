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
FAKE_VALUE_FUNC(const char *, NoteBinaryRequiredRxBuffer, size_t *)
FAKE_VALUE_FUNC(J *, NoteRequestResponse, J *)
FAKE_VALUE_FUNC(const char *, NoteChunkedReceive, uint8_t *, size_t *, bool,
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
size_t bufLen = sizeof(buf);
size_t dataLen = 0;

char rawMsg[] = "Hello Blues!";
size_t rawMsgLen = strlen(rawMsg);

namespace
{

SCENARIO("NoteBinaryReceive")
{
    RESET_FAKE(NoteNewRequest);
    RESET_FAKE(NoteBinaryRequiredRxBuffer);
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
    NoteBinaryRequiredRxBuffer_fake.custom_fake = [](size_t *size)
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

    GIVEN("NoteBinaryRequiredRxBuffer fails") {
        NoteBinaryRequiredRxBuffer_fake.custom_fake = [](size_t *size)
        -> const char * {
            *size = 0;

            return NULL;
        };

        WHEN("NoteBinaryReceive is called") {
            const char *err = NoteBinaryReceive(buf, bufLen, &dataLen);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("NoteBinaryRequiredRxBuffer indicates there's no binary data") {
        NoteBinaryRequiredRxBuffer_fake.custom_fake = NULL;
        NoteBinaryRequiredRxBuffer_fake.return_val = "some error";

        WHEN("NoteBinaryReceive is called") {
            const char *err = NoteBinaryReceive(buf, bufLen, &dataLen);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("The receive buffer isn't big enough") {
        NoteBinaryRequiredRxBuffer_fake.custom_fake = [](size_t *size)
        -> const char * {
            *size = bufLen + 1;

            return NULL;
        };

        WHEN("NoteBinaryReceive is called") {
            const char *err = NoteBinaryReceive(buf, bufLen, &dataLen);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("Allocating the card.binary.get request fails") {
        NoteNewRequest_fake.custom_fake = NULL;
        NoteNewRequest_fake.return_val = NULL;

        WHEN("NoteBinaryReceive is called") {
            const char *err = NoteBinaryReceive(buf, bufLen, &dataLen);

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
            const char *err = NoteBinaryReceive(buf, bufLen, &dataLen);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("NoteChunkedReceive returns an error") {
        NoteChunkedReceive_fake.return_val = "some error";

        WHEN("NoteBinaryReceive is called") {
            const char *err = NoteBinaryReceive(buf, bufLen, &dataLen);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("NoteChunkedReceive indicates there's unexpectedly more data "
          "available") {
        NoteChunkedReceive_fake.custom_fake = [](uint8_t *, size_t *, bool,
        size_t, uint32_t *available) -> const char* {
            *available = 1;

            return NULL;
        };

        WHEN("NoteBinaryReceive is called") {
            const char *err = NoteBinaryReceive(buf, bufLen, &dataLen);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("The binary payload is received") {
        NoteChunkedReceive_fake.custom_fake = [](uint8_t *buffer, size_t *size,
        bool, size_t, uint32_t *available) -> const char* {
            uint32_t encodedMaxLen = cobsEncodedMaxLength(rawMsgLen);
            uint32_t encodedLen = cobsEncode((uint8_t *)rawMsg, rawMsgLen, '\n',
                                             buffer);

            buffer[encodedLen] = '\n';
            *size = encodedLen + 1;
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
                const char *err = NoteBinaryReceive(buf, bufLen, &dataLen);

                THEN("An error is returned") {
                    CHECK(err != NULL);
                }
            }
        }

        AND_GIVEN("The computed MD5 matches the status field") {
            WHEN("NoteBinaryReceive is called") {
                const char *err = NoteBinaryReceive(buf, bufLen, &dataLen);

                THEN("No error is returned") {
                    CHECK(err == NULL);
                }

                THEN("The length of the payload is returned in the dataLen out"
                     " parameter") {
                    CHECK(dataLen == rawMsgLen);
                }

                THEN("The decoded payload is as expected, with no trailing "
                     "newline") {
                    CHECK(memcmp(buf, rawMsg, dataLen) == 0);
                }
            }
        }
    }

    // GIVEN("The initial card.binary request fails") {
    //     NoteRequestResponse_fake.return_val = NULL;

    //     WHEN("NoteBinaryReceive is called") {
    //         const char *err = NoteBinaryReceive(buf, size);

    //         THEN("An error is returned") {
    //             CHECK(err != NULL);
    //         }
    //     }
    // }

    // GIVEN("The response to the initial card.binary request has an error") {
    //     J *rsp = JCreateObject();
    //     JAddStringToObject(rsp, "err", "some error");
    //     NoteRequestResponse_fake.return_val = rsp;

    //     WHEN("NoteBinaryReceive is called") {
    //         const char *err = NoteBinaryReceive(buf, size);

    //         THEN("An error is returned") {
    //             CHECK(err != NULL);
    //         }
    //     }
    // }

    // GIVEN("The response to the initial card.binary request indicates there's no"
    //       "binary data to read") {
    //     J *rsp = JCreateObject();
    //     JAddIntToObject(rsp, "cobs", 0);
    //     NoteRequestResponse_fake.return_val = rsp;

    //     WHEN("NoteBinaryReceive is called") {
    //         const char *err = NoteBinaryReceive(buf, size);

    //         THEN("An error is returned") {
    //             CHECK(err != NULL);
    //         }
    //     }
    // }

    // GIVEN("The response to the initial card.binary request indicates there's"
    //       "more data to read than will fit in the provided buffer") {
    //     J *rsp = JCreateObject();
    //     JAddIntToObject(rsp, "cobs", size + 1);
    //     NoteRequestResponse_fake.return_val = rsp;

    //     WHEN("NoteBinaryReceive is called") {
    //         const char *err = NoteBinaryReceive(buf, size);

    //         THEN("An error is returned") {
    //             CHECK(err != NULL);
    //         }
    //     }
    // }

    THEN("The Notecard is locked and unlocked the same number of times") {
        CHECK(NoteLockNote_fake.call_count == NoteUnlockNote_fake.call_count);
    }
}

}

#endif // NOTE_C_TEST
