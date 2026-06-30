# `_notePingDrainInput`

## Status

Finalized. Source-inspected static ping input-drain helper, finalized transport/timing callees, direct `NotePing` caller, hook boundaries, and focused indirect tests.

## Identity

- Kind: function
- Definition: `n_request.c:786`
- Declaration/interface: none; static file-local helper
- Public/exported names: none
- Macro aliases/wrappers: none
- Signature/API shape: `static void _notePingDrainInput(void)`
- Visibility: file-local to `n_request.c`
- Ownership/lifetime role: no allocation or ownership transfer; discards already-buffered transport input

## Dependency Status

- Project-local dependencies:
  - `function:n_hooks.c:209:NoteGetActiveInterface` finalized
  - `macro:n_lib.h:243:_GetMs` finalized
  - `macro:n_lib.h:232:_SerialAvailable` finalized
  - `macro:n_lib.h:233:_SerialReceive` finalized
  - `macro:n_lib.h:244:_DelayMs` finalized
  - `macro:n_lib.h:245:_LockI2C` finalized
  - `macro:n_lib.h:236:_I2CReceive` finalized
  - `macro:n_lib.h:247:_I2CAddress` finalized
  - `macro:n_lib.h:246:_UnlockI2C` finalized
- External/system dependencies: none directly; serial/I2C behavior is platform-hook supplied through finalized wrappers
- Data/type/global dependencies: reads active interface state through `NoteGetActiveInterface`; timing, serial, I2C, address, and lock behavior cross hook/global wrapper boundaries
- Artifact coverage: source, direct caller, finalized callees, serial/I2C branches, lock/error boundaries, and focused indirect tests inspected

## Behavior

Reads the active interface. On `NOTE_C_INTERFACE_SERIAL`, it drains all currently available serial bytes by repeatedly calling `_SerialAvailable()` and discarding `_SerialReceive()` results. After each drain burst, it updates `lastByteMs`, then returns once the line has been quiet for 20 ms or once 100 ms has elapsed from the start; otherwise it delays 1 ms and repeats. On `NOTE_C_INTERFACE_I2C`, it stack-allocates a 32-byte scratch buffer, locks the I2C mutex, probes available bytes with `_I2CReceive(_I2CAddress(), scratch, 0, &available)`, and then reads chunks of up to 32 bytes until the hook reports no more available data. Any I2C receive error unlocks and returns. Other active-interface values do nothing.

## Important Boundaries

- Preconditions and assumptions: called by `NotePing` while the Notecard lock is already held and after transaction start succeeds.
- Error/null/empty behavior: no return value and no error propagation. Serial inactive/missing hooks are inherited from wrappers as no available bytes or NUL receives. I2C receive errors are swallowed after balanced unlock.
- Ownership and lifetime: no allocation; uses a stack scratch buffer for I2C drain.
- Concurrency/global state: serial branch is protected by caller's Notecard lock; I2C branch additionally uses the I2C lock wrappers. Reads active-interface and transport hook state.
- Portability/platform constraints: all transport access remains behind hook wrappers.
- Performance shape: serial branch is intended to stop after a 20 ms quiet window or 100 ms elapsed cap, but both timer checks occur after the inner `_SerialAvailable()` drain loop returns. If availability remains continuously true, the inner drain loop does not reach those checks. I2C branch drains in up-to-32-byte chunks while the hook updates `available`.
- Security or parsing constraints: discards raw bytes; no parsing.

## Direct Call Sites

Exact source search found one direct production caller:

- `n_request.c:897` `NotePing` calls this helper after `_TransactionStart(500)` succeeds and after `_LockNote()`, immediately before the one-shot `_Transaction` ping.

## Role In Callers

`NotePing` uses this helper to discard stale serial/I2C bytes before sending the `echo` ping. The call is deliberately inside the Notecard lock so another operation cannot refill the buffer between drain and transaction.

## Tests Observed

No test directly names `_notePingDrainInput`. `test/src/NotePing_test.cpp` covers it indirectly: the serial drain case sets three pending bytes, verifies `_noteSerialReceive` is called three times, and verifies the transaction observes zero remaining serial bytes. The I2C case switches the active interface to I2C, verifies `_noteI2CReceive` is called once, verifies I2C lock/unlock are each called once, verifies the receive saw the lock held, and verifies lock depth returns to zero.

## Test Coverage Notes

Focused tests do not cover serial quiet-window or max-time expiration, continuously true serial availability, I2C multi-chunk draining, or I2C receive-error paths.

## Graph Links

- Graph node id: `function:n_request.c:786:_notePingDrainInput`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
