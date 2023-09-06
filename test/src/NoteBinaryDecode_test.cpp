/*!
 * @file NoteBinaryDecode_test.cpp
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
FAKE_VALUE_FUNC(uint32_t, cobsDecode, uint8_t *, uint32_t, uint8_t, uint8_t *)

uint8_t inBuf[12];
uint32_t inLen;
uint8_t outBuf[10];
uint32_t outLen;

namespace
{

SCENARIO("NoteBinaryDecode")
{
    RESET_FAKE(cobsDecode);
    uint32_t inLen = sizeof(inBuf);
    uint32_t outLen = sizeof(outBuf);

    GIVEN("Bad parameters are supplied") {
        WHEN("inBuf is NULL") {
            const char *err = NoteBinaryDecode(NULL, inLen, outBuf, &outLen);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
        WHEN("outBuf is NULL") {
            const char *err = NoteBinaryDecode(inBuf, inLen, NULL, &outLen);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
        WHEN("outLen is NULL") {
            const char *err = NoteBinaryDecode(inBuf, inLen, outBuf, NULL);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
        WHEN("outLen is less than the size required for the worst-case decoding") {
            uint32_t badOutLen = (cobsGuaranteedFit(inLen) - 1);
            const char *err = NoteBinaryDecode(inBuf, inLen, outBuf, &badOutLen);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("Parameters are in order") {
        const uint32_t EXPECTED_RESULT = 79;
        cobsDecode_fake.return_val = EXPECTED_RESULT;
        const char *err = NoteBinaryDecode(inBuf, inLen, outBuf, &outLen);

        THEN("cobsDecode is invoked") {
            CHECK(cobsDecode_fake.call_count > 0);
        }

        WHEN("cobsDecode is invoked") {
            THEN("The parameters are passed without modification") {
                CHECK(cobsDecode_fake.arg0_history[0] == inBuf);
                CHECK(cobsDecode_fake.arg1_history[0] == inLen);
                CHECK(cobsDecode_fake.arg3_history[0] == outBuf);
            }

            THEN("The result is returned without modification") {
                CHECK(EXPECTED_RESULT == outLen);
            }
        }
    }
}

}

#endif // NOTE_C_TEST
