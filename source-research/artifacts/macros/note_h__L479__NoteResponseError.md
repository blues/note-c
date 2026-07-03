# `NoteResponseError`

## Status

Finalized. Source-inspected response error predicate macro.

## Identity

- Kind: macro
- Definition: `note.h:479`
- Declaration/interface: public response helper macro
- Signature/API shape: `NoteResponseError(rsp)`
- Visibility: public note-c response helper
- Public/exported names: `NoteResponseError`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson_helpers.c:152:JIsNullString` finalized
- External/system dependencies: none
- Data/type/global dependencies: response `J` tree

## Behavior

Expands to `(!JIsNullString(rsp, "err"))`, treating a nonblank string `err` field as an error.

## Important Boundaries

- Preconditions and assumptions: macro argument may be evaluated by `JIsNullString`.
- Error/null/empty behavior: null response returns true because `JIsNullString(NULL, "err")` returns false.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: object lookup through `JIsNullString`.
- Portability/platform constraints: field matching follows JSON helper behavior.

## Direct Call Sites

Exact source search found broad helper use, including `NoteBinaryStoreReceive`, `NoteBinaryStoreTransmit`, and many higher-level helper APIs.

## Role In Callers

`NoteBinaryStoreReceive` rejects null or error responses from the setup transaction before receiving binary bytes.

## Tests Observed

`NoteResponseError_test.cpp`, `NoteTransaction_test.cpp`, and binary store tests exercise this macro.

## Graph Links

- Graph node id: `macro:note.h:479:NoteResponseError`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
