/*!
 * @file NoteGetFnI2CMutex_test.cpp
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

SCENARIO("NoteGetFnI2CMutex")
{
    SECTION("Get I2C mutex functions with NULL parameters") {
        NoteGetFnI2CMutex(NULL, NULL);
    }

    SECTION("Get I2C lock function only") {
        mutexFn lockI2C = NULL;
        NoteGetFnI2CMutex(&lockI2C, NULL);
        REQUIRE(lockI2C != NULL);
    }

    SECTION("Get I2C unlock function only") {
        mutexFn unlockI2C = NULL;
        NoteGetFnI2CMutex(NULL, &unlockI2C);
        REQUIRE(unlockI2C != NULL);
    }

    SECTION("Get both I2C mutex functions") {
        mutexFn lockI2C = NULL;
        mutexFn unlockI2C = NULL;
        NoteGetFnI2CMutex(&lockI2C, &unlockI2C);
        REQUIRE(lockI2C != NULL);
        REQUIRE(unlockI2C != NULL);
    }
}

}
