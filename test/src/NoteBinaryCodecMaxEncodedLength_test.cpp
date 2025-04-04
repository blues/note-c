/*!
 * @file NoteBinaryCodecMaxEncodedLength_test.cpp
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
FAKE_VALUE_FUNC(uint32_t, _cobsEncodedMaxLength, uint32_t)

const uint32_t unencodedLen = 10;

namespace
{

SCENARIO("NoteBinaryCodecMaxEncodedLength")
{
    GIVEN("Parameters are in order") {
        const uint32_t EXPECTED_RESULT = 79;
        _cobsEncodedMaxLength_fake.return_val = EXPECTED_RESULT;
        const uint32_t result = NoteBinaryCodecMaxEncodedLength(unencodedLen);

        THEN("_cobsEncodedMaxLength is invoked") {
            CHECK(_cobsEncodedMaxLength_fake.call_count > 0);
        }

        WHEN("_cobsEncodedMaxLength is invoked") {
            THEN("The parameters are passed without modification") {
                CHECK(_cobsEncodedMaxLength_fake.arg0_val == unencodedLen);
            }

            THEN("The result is returned without modification") {
                CHECK(EXPECTED_RESULT == result);
            }
        }
    }

    RESET_FAKE(_cobsEncodedMaxLength);
}

}


