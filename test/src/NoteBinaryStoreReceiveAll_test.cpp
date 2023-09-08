/*!
 * @file NoteBinaryStoreReceiveAll_test.cpp
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
FAKE_VALUE_FUNC(const char *, NoteBinaryStoreDecodedLength, uint32_t *)
FAKE_VALUE_FUNC(const char *, NoteBinaryStoreReceive, uint8_t *, uint32_t, uint32_t, uint32_t)

uint8_t buffer[12];
const uint32_t bufLen = sizeof(buffer);
uint32_t dataLen = 0;

namespace
{

SCENARIO("NoteBinaryStoreReceiveAll")
{
    RESET_FAKE(NoteBinaryStoreDecodedLength);
    RESET_FAKE(NoteBinaryStoreReceive);
    dataLen = 17;

    GIVEN("Bad parameters are supplied") {
        WHEN("buffer is NULL") {
            const char *err = NoteBinaryStoreReceiveAll(NULL, bufLen, &dataLen);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
        WHEN("dataLen is NULL") {
            const char *err = NoteBinaryStoreReceiveAll(buffer, bufLen, NULL);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("NoteBinaryStoreDecodedLength() is invoked") {
        WHEN("An error is encountered") {
            const char *errMsg = "ERROR! Hacking too much time!";
            NoteBinaryStoreDecodedLength_fake.return_val = errMsg;
            const char *err = NoteBinaryStoreReceiveAll(buffer, bufLen, &dataLen);

            REQUIRE(NoteBinaryStoreDecodedLength_fake.call_count > 0);
            THEN("NoteBinaryStoreReceive() is not invoked") {
                CHECK(NoteBinaryStoreReceive_fake.call_count == 0);
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
            NoteBinaryStoreDecodedLength_fake.custom_fake = [](uint32_t *len) -> const char * {
                *len = DECODED_LEN;
                return NULL;
            };
            const char *err = NoteBinaryStoreReceiveAll(buffer, bufLen, &dataLen);

            REQUIRE(NoteBinaryStoreDecodedLength_fake.call_count > 0);
            THEN("NoteBinaryStoreReceive() is invoked") {
                CHECK(NoteBinaryStoreReceive_fake.call_count > 0);
            }
            THEN("The decoded length is passed to NoteBinaryStoreReceive()") {
                CHECK(NoteBinaryStoreReceive_fake.arg3_history[0] == DECODED_LEN);
            }
        }
    }
    GIVEN("NoteBinaryStoreReceive() is invoked") {
        const uint32_t DECODED_LEN = 79;
        NoteBinaryStoreDecodedLength_fake.custom_fake = [](uint32_t *len) -> const char * {
            *len = DECODED_LEN;
            return NULL;
        };
        WHEN("An error is encountered") {
            const char *errMsg = "ERROR! Hacking too much time!";
            NoteBinaryStoreReceive_fake.return_val = errMsg;
            const char *err = NoteBinaryStoreReceiveAll(buffer, bufLen, &dataLen);

            REQUIRE(NoteBinaryStoreReceive_fake.call_count > 0);
            THEN("The dataLen is set to zero") {
                CHECK(dataLen == 0);
            }
            THEN("The error is returned") {
                CHECK(!strcmp(err,errMsg));
            }
        }
        WHEN("No error is encountered") {
            const char *err = NoteBinaryStoreReceiveAll(buffer, bufLen, &dataLen);

            REQUIRE(NoteBinaryStoreReceive_fake.call_count > 0);
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
