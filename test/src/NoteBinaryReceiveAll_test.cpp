/*!
 * @file NoteBinaryReceiveAll_test.cpp
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
FAKE_VALUE_FUNC(const char *, NoteBinaryDataDecodedLength, uint32_t *)
FAKE_VALUE_FUNC(const char *, NoteBinaryReceive, uint8_t *, uint32_t, uint32_t, uint32_t)

uint8_t buffer[12];
const uint32_t bufLen = sizeof(buffer);
uint32_t dataLen = 0;

namespace
{

SCENARIO("NoteBinaryReceiveAll")
{
    RESET_FAKE(NoteBinaryDataDecodedLength);
    RESET_FAKE(NoteBinaryReceive);
    dataLen = 17;

    GIVEN("Bad parameters are supplied") {
        WHEN("buffer is NULL") {
            const char *err = NoteBinaryReceiveAll(NULL, bufLen, &dataLen);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
        WHEN("dataLen is NULL") {
            const char *err = NoteBinaryReceiveAll(buffer, bufLen, NULL);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("NoteBinaryDataDecodedLength() is invoked") {
        WHEN("An error is encountered") {
            const char *errMsg = "ERROR! Hacking too much time!";
            NoteBinaryDataDecodedLength_fake.return_val = errMsg;
            const char *err = NoteBinaryReceiveAll(buffer, bufLen, &dataLen);

            REQUIRE(NoteBinaryDataDecodedLength_fake.call_count > 0);
            THEN("NoteBinaryReceive() is not invoked") {
                CHECK(NoteBinaryReceive_fake.call_count == 0);
            }
            THEN("The dataLen is set to zero") {
                CHECK(dataLen == 0);
            }
            THEN("The error is returned") {
                CHECK(!strcmp(err,errMsg));
            }
        }
        WHEN("No error is encountered") {
            const uint32_t DECODED_LEN = 79;
            NoteBinaryDataDecodedLength_fake.custom_fake = [](uint32_t *len) -> const char * {
                *len = DECODED_LEN;
                return NULL;
            };
            const char *err = NoteBinaryReceiveAll(buffer, bufLen, &dataLen);

            REQUIRE(NoteBinaryDataDecodedLength_fake.call_count > 0);
            THEN("NoteBinaryReceive() is invoked") {
                CHECK(NoteBinaryReceive_fake.call_count > 0);
            }
            THEN("The decoded length is passed to NoteBinaryReceive()") {
                CHECK(NoteBinaryReceive_fake.arg3_history[0] == DECODED_LEN);
            }
        }
    }
    GIVEN("NoteBinaryReceive() is invoked") {
        const uint32_t DECODED_LEN = 79;
        NoteBinaryDataDecodedLength_fake.custom_fake = [](uint32_t *len) -> const char * {
            *len = DECODED_LEN;
            return NULL;
        };
        WHEN("An error is encountered") {
            const char *errMsg = "ERROR! Hacking too much time!";
            NoteBinaryReceive_fake.return_val = errMsg;
            const char *err = NoteBinaryReceiveAll(buffer, bufLen, &dataLen);

            REQUIRE(NoteBinaryReceive_fake.call_count > 0);
            THEN("The dataLen is set to zero") {
                CHECK(dataLen == 0);
            }
            THEN("The error is returned") {
                CHECK(!strcmp(err,errMsg));
            }
        }
        WHEN("No error is encountered") {
            const char *err = NoteBinaryReceiveAll(buffer, bufLen, &dataLen);

            REQUIRE(NoteBinaryReceive_fake.call_count > 0);
            THEN("The decoded length is returned") {
                CHECK(dataLen == DECODED_LEN);
            }
            THEN("The return value is NULL") {
                CHECK(err == NULL);
            }
        }
    }
}

}

#endif // NOTE_C_TEST
