/*!
 * @file NoteGetFnMutex_test.cpp
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

SCENARIO("NoteGetFnMutex")
{
    SECTION("Get mutex functions with all NULL parameters") {
        NoteGetFnMutex(NULL, NULL, NULL, NULL);
    }

    SECTION("Get I2C lock function only") {
        mutexFn lockI2C = NULL;
        NoteGetFnMutex(&lockI2C, NULL, NULL, NULL);
        REQUIRE(lockI2C != NULL);
    }

    SECTION("Get I2C unlock function only") {
        mutexFn unlockI2C = NULL;
        NoteGetFnMutex(NULL, &unlockI2C, NULL, NULL);
        REQUIRE(unlockI2C != NULL);
    }

    SECTION("Get Note lock function only") {
        mutexFn lockNote = NULL;
        NoteGetFnMutex(NULL, NULL, &lockNote, NULL);
        REQUIRE(lockNote != NULL);
    }

    SECTION("Get Note unlock function only") {
        mutexFn unlockNote = NULL;
        NoteGetFnMutex(NULL, NULL, NULL, &unlockNote);
        REQUIRE(unlockNote != NULL);
    }

    SECTION("Get all mutex functions") {
        mutexFn lockI2C = NULL;
        mutexFn unlockI2C = NULL;
        mutexFn lockNote = NULL;
        mutexFn unlockNote = NULL;
        NoteGetFnMutex(&lockI2C, &unlockI2C, &lockNote, &unlockNote);
        REQUIRE(lockI2C != NULL);
        REQUIRE(unlockI2C != NULL);
        REQUIRE(lockNote != NULL);
        REQUIRE(unlockNote != NULL);
    }
}

}
