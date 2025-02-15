/*!
 * @file NoteSetFnI2C_test.cpp
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
#include "fff.h"

#include "n_lib.h"

DEFINE_FFF_GLOBALS
FAKE_VALUE_FUNC(bool, _i2cNoteReset)
FAKE_VALUE_FUNC(const char *, _i2cNoteTransaction, const char *, size_t, char **, uint32_t)

namespace
{

uint8_t i2cResetCalled = 0;
uint8_t i2cTransmitCalled = 0;
uint8_t i2cReceiveCalled = 0;

bool I2CReset(uint16_t)
{
    ++i2cResetCalled;
    return true;
}

const char *I2CTransmit(uint16_t, uint8_t*, uint16_t)
{
    ++i2cTransmitCalled;
    return NULL;
}

const char *I2CReceive(uint16_t, uint8_t*, uint16_t, uint32_t*)
{
    ++i2cReceiveCalled;
    return NULL;
}


SCENARIO("NoteSetFnI2C")
{
    char req[] = "{ \"req\": \"note.add\" }\n";
    char *resp = NULL;
    _i2cNoteReset_fake.return_val = true;
    _i2cNoteTransaction_fake.return_val = NULL;
    const uint16_t i2cAddr = 0xBEEF;
    const uint32_t i2cMax = 16;

    NoteSetFnI2C(i2cAddr, i2cMax, I2CReset, I2CTransmit, I2CReceive);

    CHECK(_noteI2CReset(i2cAddr));
    CHECK(i2cResetCalled == 1);

    CHECK(_noteI2CTransmit(i2cAddr, (uint8_t *)req, strlen(req)) == NULL);
    CHECK(i2cTransmitCalled == 1);

    CHECK(_noteI2CReceive(i2cAddr, NULL, 0, NULL) == NULL);
    CHECK(i2cReceiveCalled == 1);

    CHECK(strcmp(_noteActiveInterface(), "i2c") == 0);

    CHECK(_noteHardReset());
    CHECK(_i2cNoteReset_fake.call_count == 1);

    CHECK(_noteJSONTransaction(req, strlen(req), &resp, CARD_INTER_TRANSACTION_TIMEOUT_SEC) == NULL);
    CHECK(_i2cNoteTransaction_fake.call_count == 1);

    CHECK(NoteI2CAddress() == i2cAddr);
    CHECK(NoteI2CMax() == i2cMax);

    // Make sure we aren't able to set max higher than NOTE_I2C_MAX_MAX.
    NoteSetFnI2C(i2cAddr, NOTE_I2C_MAX_MAX + 1, I2CReset, I2CTransmit,
                 I2CReceive);
    CHECK(NoteI2CMax() == NOTE_I2C_MAX_MAX);

    // Unset the address, max, and callbacks.
    NoteSetFnI2C(i2cAddr, 0, NULL, NULL, NULL);
    // Make sure setting the address this way works, too.
    NoteSetI2CAddress(0);
    NoteSetFnDisabled();

    CHECK(_noteI2CReset(i2cAddr));
    CHECK(i2cResetCalled == 1);

    CHECK(_noteI2CTransmit(i2cAddr, (uint8_t *)req, strlen(req)) != NULL);
    CHECK(i2cTransmitCalled == 1);

    CHECK(_noteI2CReceive(i2cAddr, NULL, 0, NULL) != NULL);
    CHECK(i2cReceiveCalled == 1);

    CHECK(strcmp(_noteActiveInterface(), "unknown") == 0);

    CHECK(_noteHardReset());
    CHECK(_i2cNoteReset_fake.call_count == 1);

    CHECK(_noteJSONTransaction(req, strlen(req), &resp, CARD_INTER_TRANSACTION_TIMEOUT_SEC) != NULL);
    CHECK(_i2cNoteTransaction_fake.call_count == 1);

    CHECK(NoteI2CAddress() == NOTE_I2C_ADDR_DEFAULT);
    CHECK(NoteI2CMax() == NOTE_I2C_MAX_DEFAULT);

    RESET_FAKE(_i2cNoteReset);
    RESET_FAKE(_i2cNoteTransaction);
}

}


