# Hook And Adapter Layer Worker Report

## Scope

- Worker: `019f1417-8364-73c3-b948-4b10f429319a`
- Report status: merged as source-backed inspected evidence
- Source policy: source-only; focused test evidence only
- Primary files inspected: `note.h`, `n_lib.h`, `n_hooks.c`
- Additional exact-call-site files inspected: `n_request.c`, `n_helpers.c`, `n_ua.c`, `n_printf.c`, `test/integration/link_test.c`

## Evidence Summary

- `note.h` defines the public hook ABI: interface constants, hook typedefs, hook registration/getter APIs, and runtime wrappers such as `NoteMalloc`, `NoteFree`, `NoteGetMs`, `NoteDelayMs`, `NoteLockI2C`, `NoteUnlockI2C`, `NoteI2CAddress`, and `NoteI2CMax`.
- `n_lib.h` declares internal hook wrappers and macro aliases. Important aliases include `_LockNote -> _noteLockNote`, `_TransactionStart -> _noteTransactionStart`, `_Transaction -> _noteJSONTransaction`, `_Reset -> _noteHardReset`, `_Malloc -> NoteMalloc`, `_GetMs -> NoteGetMs`, and `_LockI2C -> NoteLockI2C`.
- `n_hooks.c` owns the hook registry through global function pointers and active-interface dispatch state.
- `_noteSetActiveInterface` selects transport implementation pointers: serial maps to `_serialNoteReset`, `_serialNoteTransaction`, `_serialChunkedReceive`, `_serialChunkedTransmit`; I2C maps to `_i2cNoteReset`, `_i2cNoteTransaction`, `_i2cNoteChunkedReceive`, `_i2cNoteChunkedTransmit`; invalid/NONE clears the active dispatch pointers.
- Request paths call internal aliases around transaction startup, Notecard locking, transport dispatch, reset, unlock, and transaction stop.
- Binary helper paths call chunked receive/transmit aliases while holding `_LockNote`, then call `_noteTransactionShouldLock(..., false)` to avoid re-locking.
- Registration setters store raw function pointers. Default setters only fill NULL slots; non-default setters overwrite existing values and may set NULL.
- Runtime wrappers are null-tolerant: missing malloc returns NULL; free/delay/locks/debug no-op; missing millis returns 0; transaction start defaults true; transaction stop no-op; serial reset and I2C reset default true; serial available defaults false; serial receive returns `'\0'`; inactive I2C transmit/receive and JSON/chunked dispatch return error strings.
- Hook targets are caller-owned raw function pointers. Getters copy current raw pointers into non-NULL out parameters. Heartbeat is the only hook with a context pointer and is compiled only under `NOTE_C_HEARTBEAT_CALLBACK`.
- Concurrency is global-state based. Many setters/getters take `_LockNote`, but mutex setters/getters, debug getter, I2C address/MTU setters/getters, and `NoteGetActiveInterface` do not all take that lock. Locking itself is a no-op until mutex hooks have been registered.

## Source-Truth Gaps

- `n_lib.h` declares `_noteActiveInterface(void)` at line 140, but exact source search found no implementation or caller.
- `NoteSetFnDisabled` only sets the active interface to NONE through `_noteSetActiveInterface`; it does not clear registered serial, I2C, mutex, debug, or system hook pointers.
- Dynamic hook targets registered through `NoteSetFn*` are external adapter functions and cannot be resolved inside this repository.
