/*!
 * @file NotePrintf_test.cpp
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
#include <fff.h>

#include "n_lib.h"

DEFINE_FFF_GLOBALS
FAKE_VALUE_FUNC(bool, NotePrint, const char *)

namespace
{

char printBuf[32] = {0};

bool NotePrintSave(const char* text)
{
    strcpy(printBuf, text);
    return true;
}

SCENARIO("NotePrintf")
{
    // Arrange
    NotePrint_fake.custom_fake = NotePrintSave;
    NoteSetFnDefault(malloc, free, NULL, NULL);
    const char msg[] = "Hello world!";

    // Act
    NotePrintf("Hello %s!", "world");

    // Assert
    CHECK(strcmp(printBuf, msg) == 0);

    RESET_FAKE(NotePrint);
}

}


