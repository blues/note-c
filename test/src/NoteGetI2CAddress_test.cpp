/*!
 * @file NoteGetI2CAddress_test.cpp
 *
 * Written by the Blues Inc. team.
 *
 * Copyright (c) 2024 Blues Inc. MIT License. Use of this source code is
 * governed by licenses granted by the copyright holder including that found in
 * the
 * <a href="https://github.com/blues/note-c/blob/master/LICENSE">LICENSE</a>
 * file.
 *
 */

#include <catch2/catch_test_macros.hpp>
#include "fff.h"

#include "n_lib.h"

DEFINE_FFF_GLOBALS

namespace
{

SCENARIO("NoteGetI2CAddress")
{
    SECTION("Get I2C address with NULL parameter") {
        NoteGetI2CAddress(NULL);
    }

    SECTION("Get I2C address with valid parameter") {
        uint32_t i2cAddress = 0;
        NoteGetI2CAddress(&i2cAddress);
        REQUIRE(i2cAddress != 0);
    }
}

}
