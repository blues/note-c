/*!
 * @file NoteGetFnDefault_test.cpp
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

SCENARIO("NoteGetFnDefault")
{
    SECTION("Get default functions with all NULL parameters") {
        NoteGetFnDefault(NULL, NULL, NULL, NULL, NULL);
    }

    SECTION("Get malloc function only") {
        mallocFn mallocFunc = NULL;
        NoteGetFnDefault(&mallocFunc, NULL, NULL, NULL, NULL);
        REQUIRE(mallocFunc != NULL);
    }

    SECTION("Get free function only") {
        freeFn freeFunc = NULL;
        NoteGetFnDefault(NULL, &freeFunc, NULL, NULL, NULL);
        REQUIRE(freeFunc != NULL);
    }

    SECTION("Get delay milliseconds function only") {
        delayMsFn delayMs = NULL;
        NoteGetFnDefault(NULL, NULL, &delayMs, NULL, NULL);
        REQUIRE(delayMs != NULL);
    }

    SECTION("Get delay microseconds function only") {
        delayUsFn delayUs = NULL;
        NoteGetFnDefault(NULL, NULL, NULL, &delayUs, NULL);
        REQUIRE(delayUs != NULL);
    }

    SECTION("Get milliseconds timer function only") {
        getMsFn getMs = NULL;
        NoteGetFnDefault(NULL, NULL, NULL, NULL, &getMs);
        REQUIRE(getMs != NULL);
    }

    SECTION("Get all default functions") {
        mallocFn mallocFunc = NULL;
        freeFn freeFunc = NULL;
        delayMsFn delayMs = NULL;
        delayUsFn delayUs = NULL;
        getMsFn getMs = NULL;
        NoteGetFnDefault(&mallocFunc, &freeFunc, &delayMs, &delayUs, &getMs);
        REQUIRE(mallocFunc != NULL);
        REQUIRE(freeFunc != NULL);
        REQUIRE(delayMs != NULL);
        REQUIRE(delayUs != NULL);
        REQUIRE(getMs != NULL);
    }
}

}
