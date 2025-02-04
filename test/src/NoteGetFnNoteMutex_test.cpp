/*!
 * @file NoteGetFnNoteMutex_test.cpp
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

SCENARIO("NoteGetFnNoteMutex")
{
    SECTION("Get Note mutex functions with NULL parameters") {
        NoteGetFnNoteMutex(NULL, NULL);
    }

    SECTION("Get Note lock function only") {
        mutexFn lockNote = NULL;
        NoteGetFnNoteMutex(&lockNote, NULL);
        REQUIRE(lockNote != NULL);
    }

    SECTION("Get Note unlock function only") {
        mutexFn unlockNote = NULL;
        NoteGetFnNoteMutex(NULL, &unlockNote);
        REQUIRE(unlockNote != NULL);
    }

    SECTION("Get both Note mutex functions") {
        mutexFn lockNote = NULL;
        mutexFn unlockNote = NULL;
        NoteGetFnNoteMutex(&lockNote, &unlockNote);
        REQUIRE(lockNote != NULL);
        REQUIRE(unlockNote != NULL);
    }
}

}
