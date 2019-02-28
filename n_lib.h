// Copyright 2018 Inca Roads LLC.  All rights reserved.
// Use of this source code is governed by licenses granted by the
// copyright holder including that found in the LICENSE file.

#pragma once

#include "Notecard.h"

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
char *NotecardFnI2CTransmit(uint16_t DevAddress, uint8_t* pBuffer, uint16_t Size, uint32_t TimeoutMs);
char *NotecardFnI2CReceive(uint16_t DevAddress, uint8_t* pBuffer, uint16_t Size, uint32_t TimeoutMs);
bool NotecardFnNotecardReset(void);
char *NotecardFnTransaction(char *json, char **jsonResponse);

// Wrappers
#define _malloc NotecardFnMalloc
#define _free NotecardFnFree
#define _delay NotecardFnDelayMs
#define _millis NotecardFnGetMs

// End of C-callable functions
#ifdef __cplusplus
}
#endif
