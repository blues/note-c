/*!
 * @file NoteGetFnSerial_test.cpp
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

SCENARIO("NoteGetFnSerial")
{
    SECTION("Get serial functions with all NULL parameters") {
        NoteGetFnSerial(NULL, NULL, NULL, NULL);
    }

    SECTION("Get serial reset function only") {
        serialResetFn resetFunc = NULL;
        NoteGetFnSerial(&resetFunc, NULL, NULL, NULL);
        REQUIRE(resetFunc != NULL);
    }

    SECTION("Get serial transmit function only") {
        serialTransmitFn transmitFunc = NULL;
        NoteGetFnSerial(NULL, &transmitFunc, NULL, NULL);
        REQUIRE(transmitFunc != NULL);
    }

    SECTION("Get serial available function only") {
        serialAvailableFn availableFunc = NULL;
        NoteGetFnSerial(NULL, NULL, &availableFunc, NULL);
        REQUIRE(availableFunc != NULL);
    }

    SECTION("Get serial receive function only") {
        serialReceiveFn receiveFunc = NULL;
        NoteGetFnSerial(NULL, NULL, NULL, &receiveFunc);
        REQUIRE(receiveFunc != NULL);
    }

    SECTION("Get all serial functions") {
        serialResetFn resetFunc = NULL;
        serialTransmitFn transmitFunc = NULL;
        serialAvailableFn availableFunc = NULL;
        serialReceiveFn receiveFunc = NULL;
        NoteGetFnSerial(&resetFunc, &transmitFunc, &availableFunc, &receiveFunc);
        REQUIRE(resetFunc != NULL);
        REQUIRE(transmitFunc != NULL);
        REQUIRE(availableFunc != NULL);
        REQUIRE(receiveFunc != NULL);
    }
}

}
