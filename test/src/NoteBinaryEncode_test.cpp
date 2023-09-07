/*!
 * @file NoteBinaryEncode_test.cpp
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

#include <string.h>

#include <catch2/catch_test_macros.hpp>
#include "fff.h"

#include "n_lib.h"

DEFINE_FFF_GLOBALS
FAKE_VALUE_FUNC(uint32_t, cobsEncode, uint8_t *, uint32_t, uint8_t, uint8_t *)

uint8_t inBuf[10] = "Hi there!";
uint32_t inLen;
uint8_t outBuf[12];
uint32_t outLen;

namespace
{

SCENARIO("NoteBinaryEncode")
{
    RESET_FAKE(cobsEncode);
    uint32_t inLen = strlen((const char *)inBuf);
    uint32_t outLen = sizeof(outBuf);

    GIVEN("Bad parameters are supplied") {
        WHEN("inBuf is NULL") {
            const char *err = NoteBinaryEncode(NULL, inLen, outBuf, &outLen);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
        WHEN("outBuf is NULL") {
            const char *err = NoteBinaryEncode(inBuf, inLen, NULL, &outLen);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
        WHEN("outLen is NULL") {
            const char *err = NoteBinaryEncode(inBuf, inLen, outBuf, NULL);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
        WHEN("outLen is less than the size required for in place encoding") {
            uint32_t badOutLen = (cobsEncodedLength(inBuf, inLen) - 1);
            const char *err = NoteBinaryEncode(inBuf, inLen, outBuf, &badOutLen);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("Parameters are in order") {
        const uint32_t EXPECTED_RESULT = 79;
        cobsEncode_fake.return_val = EXPECTED_RESULT;
        const char *err = NoteBinaryEncode(inBuf, inLen, outBuf, &outLen);

        THEN("cobsEncode is invoked") {
            CHECK(cobsEncode_fake.call_count > 0);
        }

        WHEN("cobsEncode is invoked") {
            THEN("The parameters are passed without modification") {
                CHECK(cobsEncode_fake.arg0_history[0] == inBuf);
                CHECK(cobsEncode_fake.arg1_history[0] == inLen);
                CHECK(cobsEncode_fake.arg3_history[0] == outBuf);
            }

            THEN("The result is returned without modification") {
                CHECK(EXPECTED_RESULT == outLen);
            }
        }
    }
}

}

#endif // NOTE_C_TEST
