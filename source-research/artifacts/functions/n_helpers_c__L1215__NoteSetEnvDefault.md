# `NoteSetEnvDefault`

## Status

Finalized. Source-inspected environment default string helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1215`
- Declaration/interface: `note.h:1820`
- Signature/API shape: `bool NoteSetEnvDefault(const char *variable, char *buf)`
- Visibility: public SDK API
- Public/exported names: `NoteSetEnvDefault`
- Macro aliases/wrappers: none
- Ownership/lifetime role: builds a request object and transfers it to `NoteRequest`

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:173:NoteNewRequest` finalized
  - `function:n_cjson.c:2013:JAddStringToObject` finalized
  - `function:n_request.c:191:NoteRequest` finalized
- External/system dependencies: none
- Data/type/global dependencies: JSON request type `J`

## Behavior

Creates an `env.default` request with `NoteNewRequest`. If request allocation fails, returns false. Otherwise it adds `"name": variable` and `"text": buf` string fields, calls `NoteRequest(req)`, and returns that boolean result.

The helper does not inspect the return values from `JAddStringToObject`; field-add failures are only reflected if the eventual request fails through the request path.

## Important Boundaries

- Preconditions and assumptions: source assumes `variable` and `buf` are valid string pointers for JSON string creation; there are no local null guards.
- Error/null/empty behavior: null request allocation returns false; `NoteRequest` false propagates false.
- Ownership and lifetime: on successful request allocation, request ownership transfers to `NoteRequest`.
- Concurrency/global state: no local global state; transaction state is delegated.
- Performance shape: one small JSON request allocation plus one boolean request transaction.
- Portability/platform constraints: transport behavior remains behind `NoteRequest`.

## Direct Call Sites

Exact source search found public declaration, this implementation, and wrapper callers `NoteSetEnvDefaultInt` and `NoteSetEnvDefaultNumber`, which format numeric values into a local buffer before calling this string helper. Focused tests call it directly and numeric wrapper tests fake it.

## Role In Callers

Provides the shared string implementation for setting default environment variables. Integer and numeric variants delegate to it after converting values to string form.

## Tests Observed

`test/src/NoteSetEnvDefault_test.cpp` covers request allocation failure, `NoteRequest` failure, and `NoteRequest` success. `NoteSetEnvDefaultInt` and `NoteSetEnvDefaultNumber` tests fake this helper to verify formatting/delegation.

## Graph Links

- Graph node id: `function:n_helpers.c:1215:NoteSetEnvDefault`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
