// Copyright 2018 Inca Roads LLC.  All rights reserved.
// Use of this source code is governed by licenses granted by the
// copyright holder including that found in the LICENSE file.

#pragma once

// In case they're not yet defined
#include <stdbool.h>
#include <stdint.h>

// C-callable functions
#ifdef __cplusplus
extern "C" {
#endif

// cJSON wrappers
#include "n_cjson.h"

// Card callback functions
typedef void (*mutexFn) (void);
typedef void * (*mallocFn) (size_t size);
typedef void (*freeFn) (void *);
typedef void (*delayMsFn) (uint32_t ms);
typedef uint32_t (*getMsFn) (void);
typedef void (*debugOutputFn) (char *text);
typedef void (*serialResetFn) (void);
typedef void (*serialWriteLineFn) (char *text);
typedef void (*serialWriteFn) (char *text);
typedef bool (*serialAvailableFn) (void);
typedef char (*serialReadFn) (void);
typedef void (*i2cResetFn) (void);
typedef char * (*i2cTransmitFn) (uint16_t DevAddress, uint8_t* pBuffer, uint16_t Size);
typedef char * (*i2cReceiveFn) (uint16_t DevAddress, uint8_t* pBuffer, uint16_t Size, uint32_t *avail);

// External API
bool NotecardReset(void);
J *NotecardNewRequest(char *request);
J *NotecardTransaction(J *req);
void NotecardSetFnDebugOutput(debugOutputFn fn);
void NotecardSetFnMutex(mutexFn lockI2Cfn, mutexFn unlockI2Cfn, mutexFn lockNotecardfn, mutexFn unlockNotecardfn);
void NotecardSetFnMem(mallocFn mallocfn, freeFn freefn);
void NotecardSetFnTimer(delayMsFn delayfn, getMsFn millisfn);
void NotecardSetFnSerial(serialResetFn resetfn, serialWriteLineFn printlnfn, serialWriteFn writefn, serialAvailableFn availfn, serialReadFn readfn);
void NotecardSetFnI2C(uint32_t i2caddr, uint32_t i2cmax, i2cResetFn resetfn, i2cTransmitFn transmitfn, i2cReceiveFn receivefn);

// Calls to the functions set above
void NotecardFnDebug(const char *format, ...);
void *NotecardFnMalloc(size_t size);
void NotecardFnFree(void *);
uint32_t NotecardFnGetMs(void);
void NotecardFnDelayMs(uint32_t ms);
void NotecardFnLockI2C(void);
void NotecardFnUnlockI2C(void);
uint32_t NotecardFnI2CAddress(void);
uint32_t NotecardFnI2CMax(void);

// JSON helpers
void JInit(void);
void JCheck(void);
void *JMalloc(size_t size);
void JFree(void *p);
bool JIsPresent(J *rsp, char *field);
char *JGetString(J *rsp, char *field);
double JGetDouble(J *rsp, char *field);
int JGetInt(J *rsp, char *field);
bool JGetBool(J *rsp, char *field);
bool JIsNullString(J *rsp, char *field);
bool JIsExactString(J *rsp, char *field, char *teststr);
bool JContainsString(J *rsp, char *field, char *substr);

// Helper functions for apps that wish to limit their C library dependencies
#define FTOA_PRECISION	(10)
#define FTOA_MAX        ((2*FTOA_PRECISION)+1+1)
char * JFtoA(double f, char * buf, int precision);
double JAtoF(const char *string, char **endPtr);
int JB64EncodeLen(int len);
int JB64Encode(char * coded_dst, const char *plain_src,int len_plain_src);
int JB64DecodeLen(const char * coded_src);
int JB64Decode(char * plain_dst, const char *coded_src);
#ifdef __cplusplus
}
#endif

// End of C-callable functions
#ifdef __cplusplus
}
#endif
