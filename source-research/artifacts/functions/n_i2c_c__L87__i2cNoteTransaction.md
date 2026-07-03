# `_i2cNoteTransaction`

## Status

Finalized. Source-inspected I2C JSON transaction adapter.

## Identity

- Kind: function
- Definition: `n_i2c.c:87`
- Declaration/interface: `n_lib.h:124`
- Public/exported names: `_i2cNoteTransaction` inside the note-c internal transport surface
- Macro aliases/wrappers: no macro alias found; selected dynamically through `notecardTransaction`
- Signature/API shape: `const char *_i2cNoteTransaction(const char *request, size_t reqLen, char **response, uint32_t timeoutMs)`
- Visibility: internal adapter entry point with prototype in `n_lib.h`
- Ownership/lifetime role: sends an optional request over I2C and optionally returns a heap-allocated response buffer through `*response`

## Dependency Status

- Project-local dependencies:
  - `macro:n_lib.h:245:_LockI2C` finalized
  - `function:n_i2c.c:475:_i2cChunkedTransmit` finalized
  - `macro:note.h:990:NOTE_C_LOG_ERROR` finalized
  - `macro:n_lib.h:246:_UnlockI2C` finalized
  - `function:n_i2c.c:30:_delayIO` finalized
  - `function:n_i2c.c:48:_i2cNoteQueryLength` finalized
  - `macro:note.h:75:ERRSTR` finalized
  - `macro:note.h:78:ERRSTR` finalized
  - `macro:n_lib.h:241:_Malloc` finalized
  - `function:n_i2c.c:365:_i2cChunkedReceive` finalized
  - `macro:n_lib.h:242:_Free` finalized
  - `macro:note.h:76:NOTE_C_LOG_DEBUG` finalized
  - `macro:note.h:1014:NOTE_C_LOG_DEBUG` finalized
- External/system dependencies: `memcpy`
- Data/type/global dependencies: reads `ALLOC_CHUNK` and transaction timeout constants; delay behavior is affected by `cardTurboIO` inside `_delayIO`; hook-backed macros reach I2C lock, address, transmit/receive, malloc/free, time/delay, and logging hooks
- Artifact coverage: source-inspected artifact with verified direct call edges

## Behavior

Locks the I2C bus for the whole transaction. If `reqLen > 0`, transmits the request through `_i2cChunkedTransmit`; transmit errors are logged, the bus is unlocked, and the error is returned. If `response == NULL`, unlocks and returns success after any request transmit.

When a response is requested, performs the I2C pre-read delay, asks `_i2cNoteQueryLength` how many bytes are available, allocates an `ALLOC_CHUNK`-rounded buffer plus one null byte, and receives response chunks with `_i2cChunkedReceive` until `available` becomes zero. If later chunks are pending, it grows the buffer in `ALLOC_CHUNK` multiples, copies existing bytes, frees the old buffer, and continues. The response is null-terminated, assigned to `*response`, and must be freed by the caller using the configured free hook.

## Important Boundaries

- Preconditions and assumptions: nonzero `reqLen` requires `request` to address at least `reqLen` bytes acceptable to the I2C chunk transmitter; non-null `response` must point to writable `char *` storage.
- Error/null/empty behavior: zero-length requests skip transmit; null `response` skips all receive work; query, allocation, transmit, and receive failures return an error string and leave `*response` unchanged.
- Ownership and lifetime: successful response capture transfers the allocated response buffer to the caller; allocation/growth failure paths free any temporary buffer they own.
- Concurrency/global state: serializes the entire transaction with `_LockI2C`/`_UnlockI2C`.
- Portability/platform constraints: all bus-specific operations remain behind hook-backed I2C macros and chunk helpers.
- Performance shape: one request transmit, one response-length query, then one or more chunk receives; heap growth rounds pending data up to `ALLOC_CHUNK` to reduce fragmentation.
- Security or parsing constraints: returns raw null-terminated response bytes and does not parse or validate JSON.

## Direct Call Sites

Exact source search found the prototype in `n_lib.h:124`, the definition in `n_i2c.c:87`, assignment to `notecardTransaction` by `_noteSetActiveInterface` in `n_hooks.c:227`, and focused tests. No direct production call was found.

## Role In Callers

`_noteSetActiveInterface` installs this adapter into the active transaction function pointer for `NOTE_C_INTERFACE_I2C`; `_noteJSONTransaction` dispatches to it through `notecardTransaction`. Public request flows therefore reach this function indirectly after I2C has been selected.

## Tests Observed

Focused tests in `test/src/_i2cNoteTransaction_test.cpp` cover transmit errors, null response, query errors, initial allocation failure, receive errors, single and multiple chunks, growth allocation failure, zero-length and null requests, unchanged request bytes, and lock/unlock count balance. `_noteSetActiveInterface_test.cpp` covers registration of this function as the I2C transaction target. Gaps: real I2C hooks are not exercised, and tests do not cover every possible hook-side timing behavior.

## Graph Links

- Graph node id: `function:n_i2c.c:87:_i2cNoteTransaction`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing call edges to lock/unlock, delay/query, allocation/free, chunk transmit/receive, and logging/error helpers.
- Verified incoming dynamic edges from `_noteSetActiveInterface` (`assigns_function_pointer`) and `_noteJSONTransaction` (`dispatches_via_pointer`).

## Evidence Gaps

No unresolved source gaps for this node.
