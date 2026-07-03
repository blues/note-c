# `NoteUserAgent`

## Status

Finalized. Source-inspected weak user-agent object builder.

## Identity

- Kind: function
- Definition: `n_ua.c:95`
- Declaration/interface: weak function outside MSVC, normal function under MSVC
- Signature/API shape: `J *NoteUserAgent(void)`
- Visibility: public/overrideable helper when user-agent support is compiled
- Public/exported names: `NoteUserAgent`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:2450:JCreateObject` finalized
  - `function:n_cjson.c:2013:JAddStringToObject` finalized
  - `function:n_hooks.c:209:NoteGetActiveInterface` finalized
  - `function:n_cjson.c:1974:JAddNumberToObject` finalized
  - `function:n_ua.c:80:NoteUserAgentUpdate` finalized
- External/system dependencies: compiler/version macros
- Data/type/global dependencies: user-agent global strings/numeric CPU and OS fields

## Behavior

Creates a JSON object, adds agent and compiler strings, records the active request interface as `none`, `serial`, `i2c`, or `unknown`, conditionally adds nonzero CPU numeric fields and non-null CPU/OS strings, calls `NoteUserAgentUpdate`, and returns the object. Allocation failure for the root object returns NULL. Individual add helper failures are not checked.

## Important Boundaries

- Preconditions and assumptions: excluded in low-memory builds through surrounding user-agent compile guards.
- Error/null/empty behavior: root allocation failure returns NULL; optional fields are skipped when zero/null.
- Ownership and lifetime: caller owns the returned JSON object.
- Concurrency/global state: reads global user-agent configuration and active interface state.
- Performance shape: bounded by field count and JSON allocations.
- Portability/platform constraints: compiler string depends on preprocessor/compiler macros.

## Direct Call Sites

Exact source search found `_noteTransactionShouldLock` as the transaction caller when user-agent injection is enabled.

## Role In Callers

The transaction path builds this object for hub.set/product requests, but current source serializes the request before attaching the body, so this mutation is not reflected in the already-created transmit buffer.

## Tests Observed

`NoteUserAgent_test.cpp` covers field construction; `NoteTransaction_test.cpp` covers user-agent injection predicates.

## Graph Links

- Graph node id: `function:n_ua.c:95:NoteUserAgent`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
