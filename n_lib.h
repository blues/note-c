// Copyright 2018 Inca Roads LLC.  All rights reserved.
// Use of this source code is governed by licenses granted by the
// copyright holder including that found in the LICENSE file.

#pragma once

#include "notecard.h"

// C-callable functions
#ifdef __cplusplus
extern "C" {
#endif

// How long to wait for the card for any given transaction
#define NOTECARD_TRANSACTION_TIMEOUT_SEC     10

// Transactions
char *i2cNotecardTransaction(char *json, char **jsonResponse);
bool i2cNotecardReset(void);
char *serialNotecardTransaction(char *json, char **jsonResponse);
bool serialNotecardReset(void);

// Hooks
void NotecardFnLockNotecard(void);
void NotecardFnUnlockNotecard(void);
void NotecardFnSerialReset(void);
void NotecardFnSerialWriteLine(char *);
void NotecardFnSerialWrite(char *);
bool NotecardFnSerialAvailable(void);
char NotecardFnSerialRead(void);
void NotecardFnI2CReset(void);
char *NotecardFnI2CTransmit(uint16_t DevAddress, uint8_t* pBuffer, uint16_t Size);
char *NotecardFnI2CReceive(uint16_t DevAddress, uint8_t* pBuffer, uint16_t Size, uint32_t *avail);
bool NotecardFnNotecardReset(void);
char *NotecardFnTransaction(char *json, char **jsonResponse);

// Readability wrappers.  Anything starting with _ is simply calling the wrapper function
#define _LockNotecard NotecardFnLockNotecard
#define _UnlockNotecard NotecardFnUnlockNotecard
#define _SerialReset NotecardFnSerialReset
#define _SerialWriteLine NotecardFnSerialWriteLine
#define _SerialWrite NotecardFnSerialWrite
#define _SerialAvailable NotecardFnSerialAvailable
#define _SerialRead NotecardFnSerialRead
#define _I2CReset NotecardFnI2CReset
#define _I2CTransmit NotecardFnI2CTransmit
#define _I2CReceive NotecardFnI2CReceive
#define _NotecardReset NotecardFnNotecardReset
#define _Transaction NotecardFnTransaction
#define _Debug NotecardFnDebug
#define _Malloc NotecardFnMalloc
#define _Free NotecardFnFree
#define _GetMs NotecardFnGetMs
#define _DelayMs NotecardFnDelayMs
#define _LockI2C NotecardFnLockI2C
#define _UnlockI2C NotecardFnUnlockI2C
#define _I2CAddress NotecardFnI2CAddress
#define _I2CMax NotecardFnI2CMax

// End of C-callable functions
#ifdef __cplusplus
}
#endif
