# `JGetBool`

## Status

Finalized. Source-inspected JSON object boolean-field accessor, direct callers, and focused tests.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:137`
- Declaration/interface: `note.h:1278`
- Public/exported names: `JGetBool`
- Macro aliases/wrappers: none
- Signature/API shape: `bool JGetBool(J *json, const char *field)`
- Visibility: public note-c helper API
- Ownership/lifetime role: borrowed read of caller-owned object tree

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1732:JGetObjectItem` finalized
  - `function:n_cjson.c:2690:JIsBool` finalized
  - `function:n_cjson.c:2681:JIsTrue` finalized
- External/system dependencies: none
- Data/type/global dependencies: reads object child pointer returned by `JGetObjectItem`
- Artifact coverage: source, declaration, direct callers, and focused tests inspected

## Behavior

Returns `false` when `json` is `NULL`, when `field` is missing, or when the looked-up item is not considered boolean by `JIsBool`. For a boolean item, returns `JIsTrue(item)`, so a false item returns `false` and a true item returns `true`.

## Important Boundaries

- Preconditions and assumptions: `field` may be `NULL`; the finalized `JGetObjectItem` path returns `NULL` for a null key.
- Error/null/empty behavior: null object, missing field, null field, and non-bool field all return `false`.
- Ownership and lifetime: no allocation; returns a scalar derived from a borrowed object child.
- Concurrency/global state: no global state.
- Portability/platform constraints: none.
- Performance shape: O(number of object children * key length) for lookup, then O(1) type checks.
- Security or parsing constraints: depends on `JIsBool` accepting true/false bit presence before `JIsTrue` checks exact true base type.

## Direct Call Sites

Production callers found by exact source search:

- `n_helpers.c:1352` `NoteIsConnectedST` reads `connected` from `hub.status`.
- `n_helpers.c:1665` `NoteGetStatusST` reads `usb` from `card.status`.
- `n_helpers.c:1666` `NoteGetStatusST` reads `connected` before deriving signal presence.

## Role In Callers

`NoteIsConnectedST` and `NoteGetStatusST` use missing, non-bool, and false values the same way at this layer: all produce `false`. Higher-level request-building tests also use `JGetBool` to inspect generated boolean fields.

## Tests Observed

`test/src/JGetBool_test.cpp` covers null JSON, missing field, non-bool field, and true bool lookup. Additional caller-focused request tests assert generated bool fields via `JGetBool`, including `NoteAdd_test.cpp`, `NoteFactoryReset_test.cpp`, `NoteSetSyncMode_test.cpp`, and `NoteSetUploadMode_test.cpp`.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:137:JGetBool`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused `JGetBool_test.cpp` case covers an explicit false value, null field name, or malformed boolean type bitmask.
