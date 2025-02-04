/*!
 * @file NoteGetFnTransaction_test.cpp
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

SCENARIO("NoteGetFnTransaction")
{
    SECTION("Get transaction functions with NULL parameters") {
        NoteGetFnTransaction(NULL, NULL);
    }

    SECTION("Get transaction start function only") {
        txnStartFn startFn = NULL;
        NoteGetFnTransaction(&startFn, NULL);
        REQUIRE(startFn != NULL);
    }

    SECTION("Get transaction stop function only") {
        txnStopFn stopFn = NULL;
        NoteGetFnTransaction(NULL, &stopFn);
        REQUIRE(stopFn != NULL);
    }

    SECTION("Get both transaction functions") {
        txnStartFn startFn = NULL;
        txnStopFn stopFn = NULL;
        NoteGetFnTransaction(&startFn, &stopFn);
        REQUIRE(startFn != NULL);
        REQUIRE(stopFn != NULL);
    }
}

}
