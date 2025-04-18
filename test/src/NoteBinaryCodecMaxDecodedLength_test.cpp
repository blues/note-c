/*!
 * @file NoteBinaryCodecMaxDecodedLength_test.cpp
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
FAKE_VALUE_FUNC(uint32_t, _cobsGuaranteedFit, uint32_t)

const uint32_t bufferSize = 10;

namespace
{

SCENARIO("NoteBinaryCodecMaxDecodedLength")
{
    GIVEN("Parameters are in order") {
        const uint32_t EXPECTED_RESULT = 79;
        _cobsGuaranteedFit_fake.return_val = EXPECTED_RESULT;
        const uint32_t result = NoteBinaryCodecMaxDecodedLength(bufferSize);

        THEN("_cobsGuaranteedFit is invoked") {
            CHECK(_cobsGuaranteedFit_fake.call_count > 0);
        }

        WHEN("_cobsGuaranteedFit is invoked") {
            THEN("The parameters are passed without modification") {
                CHECK(_cobsGuaranteedFit_fake.arg0_val == bufferSize);
            }

            THEN("The result is returned without modification") {
                CHECK(EXPECTED_RESULT == result);
            }
        }
    }

    RESET_FAKE(_cobsGuaranteedFit);
}

}


