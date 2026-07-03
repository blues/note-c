# `NoteGetServiceConfig`

## Status

Finalized. Source-inspected public immediate-refresh service-configuration wrapper, direct caller search, and focused tests.

## Identity

- Kind: function
- Definition: `n_helpers.c:1557`
- Declaration/interface: `note.h:1928`
- Public/exported names: `NoteGetServiceConfig`
- Macro aliases/wrappers: none
- Signature/API shape: `bool NoteGetServiceConfig(char *productBuf, int productBufLen, char *serviceBuf, int serviceBufLen, char *deviceBuf, int deviceBufLen, char *snBuf, int snBufLen)`
- Visibility: public note-c helper API
- Ownership/lifetime role: passes caller-provided output buffers through to `NoteGetServiceConfigST`

## Dependency Status

- Project-local dependencies:
  - `function:n_helpers.c:1582:NoteGetServiceConfigST` finalized
- External/system dependencies: none
- Data/type/global dependencies: writes static `serviceConfigTimer`
- Artifact coverage: source, declaration, direct caller search, and focused tests inspected

## Behavior

Sets static `serviceConfigTimer` to zero and returns `NoteGetServiceConfigST(...)` with the same output buffer pointers and lengths. The wrapper forces the suppressed variant to consider a refresh when product/device cache strings are already populated, but it does not clear any cached service-configuration strings.

## Important Boundaries

- Preconditions and assumptions: output-buffer preconditions are inherited from `NoteGetServiceConfigST`.
- Error/null/empty behavior: refresh, cache, output-copy, and error behavior are delegated to `NoteGetServiceConfigST`; this wrapper only resets the timer.
- Ownership and lifetime: no allocation in this wrapper.
- Concurrency/global state: mutates static `serviceConfigTimer` without local locking.
- Portability/platform constraints: transport and timing behavior remain behind the suppressed variant.
- Performance shape: one delegated refresh attempt or cache operation.
- Security or parsing constraints: no parsing in this wrapper.

## Direct Call Sites

No production callers were found by exact source search outside the function definition and public declaration.

## Role In Callers

No production caller role is currently present in source. This is the public non-suppressed service-configuration helper API.

## Tests Observed

`test/src/NoteGetServiceConfig_test.cpp` covers no response with empty cache, successful response caching/copying, and a subsequent no-response call that returns false while copying previously cached values.

## Test Coverage Notes

Focused tests do not directly assert `serviceConfigTimer` reset, suppressed cache-hit behavior, error responses carrying `"err"`, or the exact `"hub.get"` request string.

## Graph Links

- Graph node id: `function:n_helpers.c:1557:NoteGetServiceConfig`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
