/*!
 * @file NoteTransactionHooks_test.cpp
 *
 * Written by the Blues Inc. team.
 *
 * Copyright (c) 2023 Blues Inc. MIT License. Use of this source code is
 * governed by licenses granted by the copyright holder including that found in
 * the
 * <a href="https://github.com/blues/note-c/blob/master/LICENSE">LICENSE</a>
 * file.
 *
 */

#include <catch2/catch_test_macros.hpp>

#include "n_lib.h"

namespace
{

bool txnStartCalled = false;
bool txnStopCalled = false;

bool MyTxnStart(uint32_t)
{
    txnStartCalled = true;
    return true;
}

void MyTxnStop()
{
    txnStopCalled = true;
    return;
}

SCENARIO("NoteTransactionHooks")
{
    SECTION("Hooks not set") {
        _noteTransactionStart(0);
        _noteTransactionStop();

        CHECK(!txnStartCalled);
        CHECK(!txnStopCalled);
    }

    SECTION("Hooks set") {
        NoteSetFnTransaction(MyTxnStart, MyTxnStop);
        _noteTransactionStart(0);
        _noteTransactionStop();

        CHECK(txnStartCalled);
        CHECK(txnStopCalled);
    }

    txnStartCalled = false;
    txnStopCalled = false;
}

}
