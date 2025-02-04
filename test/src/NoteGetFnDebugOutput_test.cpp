/*!
 * @file NoteGetFnDebugOutput_test.cpp
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

void mockDebugOutput(const char *message)
{
    (void)message;
}

SCENARIO("NoteGetFnDebugOutput")
{
    SECTION("Get debug output function with NULL parameter") {
        NoteGetFnDebugOutput(NULL);
    }

    SECTION("Get debug output function with valid parameter") {
        debugOutputFn fn = NULL;
        NoteGetFnDebugOutput(&fn);
        REQUIRE(fn != NULL);
    }
}

}
