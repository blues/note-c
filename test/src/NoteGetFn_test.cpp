/*!
 * @file NoteGetFn_test.cpp
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

SCENARIO("NoteGetFn")
{
    SECTION("Get platform functions with all NULL parameters") {
        NoteGetFn(NULL, NULL, NULL, NULL, NULL);
    }

    SECTION("Get malloc function only") {
        mallocFn mallocFunc = NULL;
        NoteGetFn(&mallocFunc, NULL, NULL, NULL, NULL);
        REQUIRE(mallocFunc != NULL);
    }

    SECTION("Get free function only") {
        freeFn freeFunc = NULL;
        NoteGetFn(NULL, &freeFunc, NULL, NULL, NULL);
        REQUIRE(freeFunc != NULL);
    }

    SECTION("Get delay milliseconds function only") {
        delayMsFn delayMs = NULL;
        NoteGetFn(NULL, NULL, &delayMs, NULL, NULL);
        REQUIRE(delayMs != NULL);
    }

    SECTION("Get delay microseconds function only") {
        delayUsFn delayUs = NULL;
        NoteGetFn(NULL, NULL, NULL, &delayUs, NULL);
        REQUIRE(delayUs != NULL);
    }

    SECTION("Get milliseconds timer function only") {
        getMsFn getMs = NULL;
        NoteGetFn(NULL, NULL, NULL, NULL, &getMs);
        REQUIRE(getMs != NULL);
    }

    SECTION("Get all platform functions") {
        mallocFn mallocFunc = NULL;
        freeFn freeFunc = NULL;
        delayMsFn delayMs = NULL;
        delayUsFn delayUs = NULL;
        getMsFn getMs = NULL;
        NoteGetFn(&mallocFunc, &freeFunc, &delayMs, &delayUs, &getMs);
        REQUIRE(mallocFunc != NULL);
        REQUIRE(freeFunc != NULL);
        REQUIRE(delayMs != NULL);
        REQUIRE(delayUs != NULL);
        REQUIRE(getMs != NULL);
    }
}

}
