// Copyright 2018 Inca Roads LLC.  All rights reserved.
// Use of this source code is governed by licenses granted by the
// copyright holder including that found in the LICENSE file.

#include <stdarg.h>
#include <stdlib.h>
#include "n_lib.h"

// Which I/O port to use
#define interfaceNone       0
#define interfaceSerial     1
#define interfaceI2C        2

// Externalized Hooks
debugOutputFn hookDebugOutput = NULL;
mutexFn hookLockI2C = NULL;
mutexFn hookUnlockI2C = NULL;
mutexFn hookLockNotecard = NULL;
mutexFn hookUnlockNotecard = NULL;
mallocFn hookMalloc = NULL;
freeFn hookFree = NULL;
delayMsFn hookDelayMs = NULL;
getMsFn hookGetMs = NULL;
uint32_t hookActiveInterface = interfaceNone;

serialResetFn hookSerialReset = NULL;
serialWriteLineFn hookSerialWriteLine = NULL;
serialWriteFn hookSerialWrite = NULL;
serialAvailableFn hookSerialAvailable = NULL;
serialReadFn hookSerialRead = NULL;

uint32_t i2cAddress = 0;
uint32_t i2cMax = 0;
i2cResetFn hookI2CReset = NULL;
i2cTransmitFn hookI2CTransmit = NULL;
i2cReceiveFn hookI2CReceive = NULL;

// Internal hooks
typedef bool (*nNotecardResetFn) (void);
typedef char * (*nTransactionFn) (char *, char **);
static nNotecardResetFn notecardReset = NULL;
static nTransactionFn notecardTransaction = NULL;

// Set the debug output hook
void NotecardSetFnDebugOutput(debugOutputFn fn) {
    hookDebugOutput = fn;
}
void NotecardSetFnMutex(mutexFn lockI2Cfn, mutexFn unlockI2Cfn, mutexFn lockNotecardfn, mutexFn unlockNotecardfn) {
    hookLockI2C = lockI2Cfn;
    hookUnlockI2C = unlockI2Cfn;
    hookLockNotecard = lockNotecardfn;
    hookUnlockNotecard = unlockNotecardfn;
}
void NotecardSetFnMem(mallocFn mallocfn, freeFn freefn) {
    hookMalloc = mallocfn;
    hookFree = freefn;
}
void NotecardSetFnTimer(delayMsFn delayfn, getMsFn millisfn) {
    hookDelayMs = delayfn;
    hookGetMs = millisfn;
}
void NotecardSetFnSerial(serialResetFn resetfn, serialWriteLineFn printlnfn, serialWriteFn writefn, serialAvailableFn availfn, serialReadFn readfn) {
    hookActiveInterface = interfaceSerial;

    hookSerialReset = resetfn;
    hookSerialWriteLine = printlnfn;
    hookSerialWrite = writefn;
    hookSerialAvailable = availfn;
    hookSerialRead = readfn;

    notecardReset = serialNotecardReset;
    notecardTransaction = serialNotecardTransaction;
}

void NotecardSetFnI2C(uint32_t i2caddress, uint32_t i2cmax, i2cResetFn resetfn, i2cTransmitFn transmitfn, i2cReceiveFn receivefn) {
    i2cAddress = i2caddress;
    i2cMax = i2cmax;

    hookActiveInterface = interfaceI2C;

    hookI2CReset = resetfn;
    hookI2CTransmit = transmitfn;
    hookI2CReceive = receivefn;

    notecardReset = i2cNotecardReset;
    notecardTransaction = i2cNotecardTransaction;
}


// Runtime hook wrappers
void NotecardFnDebug(const char *format, ...) {
    if (hookDebugOutput != NULL) {
        char line[256];
        va_list args;
        va_start(args, format);
        vsnprintf(line, sizeof(line), format, args);
        va_end(args);
        hookDebugOutput(line);
    }
}
uint32_t NotecardFnGetMs() {
    if (hookGetMs == NULL)
        return 0;
    return hookGetMs();
}
void NotecardFnDelayMs(uint32_t ms) {
    if (hookDelayMs != NULL)
        hookDelayMs(ms);
}
void *NotecardFnMalloc(size_t size) {
    if (hookMalloc == NULL)
        return NULL;
    return hookMalloc(size);
}
void NotecardFnFree(void *p) {
    if (hookFree != NULL)
        hookFree(p);
}
void NotecardFnLockI2C() {
    if (hookLockI2C != NULL)
        hookLockI2C();
}
void NotecardFnUnlockI2C() {
    if (hookUnlockI2C != NULL)
        hookUnlockI2C();
}
void NotecardFnLockNotecard() {
    if (hookLockNotecard != NULL)
        hookLockNotecard();
}
void NotecardFnUnlockNotecard() {
    if (hookUnlockNotecard != NULL)
        hookUnlockNotecard();
}
void NotecardFnSerialReset() {
    if (hookActiveInterface == interfaceSerial && hookSerialReset != NULL)
        hookSerialReset();
}
void NotecardFnSerialWriteLine(char *text) {
    if (hookActiveInterface == interfaceSerial && hookSerialWriteLine != NULL)
        hookSerialWriteLine(text);
}
void NotecardFnSerialWrite(char *text) {
    if (hookActiveInterface == interfaceSerial && hookSerialWrite != NULL)
        hookSerialWrite(text);
}
bool NotecardFnSerialAvailable() {
    if (hookActiveInterface == interfaceSerial && hookSerialAvailable != NULL)
        return hookSerialAvailable();
    return false;
}
char NotecardFnSerialRead() {
    if (hookActiveInterface == interfaceSerial && hookSerialRead != NULL)
        return hookRead();
    return 0;
}
void NotecardFnI2CReset() {
    if (hookActiveInterface == interfaceI2C && hookI2CReset != NULL)
        hookI2CReset();
}
char *NotecardFnI2CTransmit(uint16_t DevAddress, uint8_t* pBuffer, uint16_t Size, uint32_t TimeoutMs) {
    if (hookActiveInterface == interfaceI2C && hookI2CTransmit != NULL)
        return hookI2CTransmit(DevAddress, pBuffer, Size, TimeoutMs);
    return "i2c not active";
}
char *NotecardFnI2CReceive(uint16_t DevAddress, uint8_t* pBuffer, uint16_t Size, uint32_t TimeoutMs) {
    if (hookActiveInterface == interfaceI2C && hookI2CReceive != NULL)
        return hookI2CReceive(DevAddress, pBuffer, Size, TimeoutMs);
    return "i2c not active";
}
uint32_t NotecardFnI2CAddress() {
    if (i2cAddress == 0)
        return 0x17;
    return i2cAddress;
}
uint32_t NotecardFnI2CMax() {
    // Many Arduino libraries (such as ESP32) have a limit less than 32, so if the max isn't specified
    // we must assume the worst and segment the I2C messages into very tiny chunks.
    if (i2cMax == 0)
        return 30;
    // Notecard design specs
    if (i2cMax > 127)
        i2cMax = 127;
    return i2cMax;
}

bool NotecardFnNotecardReset() {
    if (notecardReset == NULL)
        return "notecard not initialized";
    return notecardReset();
}
char *NotecardFnTransaction(char *json, char **jsonResponse) {
    if (notecardTransaction == NULL)
        return "notecard not initialized";
    return notecardTransaction(json, jsonResponse);
}
