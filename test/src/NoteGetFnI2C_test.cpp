/*!
 * @file NoteGetFnI2C_test.cpp
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

SCENARIO("NoteGetFnI2C")
{
    SECTION("Get I2C functions with all NULL parameters") {
        NoteGetFnI2C(NULL, NULL, NULL);
    }

    SECTION("Get I2C reset function only") {
        i2cResetFn resetFunc = NULL;
        NoteGetFnI2C(&resetFunc, NULL, NULL);
        REQUIRE(resetFunc != NULL);
    }

    SECTION("Get I2C transmit function only") {
        i2cTransmitFn transmitFunc = NULL;
        NoteGetFnI2C(NULL, &transmitFunc, NULL);
        REQUIRE(transmitFunc != NULL);
    }

    SECTION("Get I2C receive function only") {
        i2cReceiveFn receiveFunc = NULL;
        NoteGetFnI2C(NULL, NULL, &receiveFunc);
        REQUIRE(receiveFunc != NULL);
    }

    SECTION("Get all I2C functions") {
        i2cResetFn resetFunc = NULL;
        i2cTransmitFn transmitFunc = NULL;
        i2cReceiveFn receiveFunc = NULL;
        NoteGetFnI2C(&resetFunc, &transmitFunc, &receiveFunc);
        REQUIRE(resetFunc != NULL);
        REQUIRE(transmitFunc != NULL);
        REQUIRE(receiveFunc != NULL);
    }
}

}
