# `_print_string`

## Status

Finalized. Source-inspected JSON string item print wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:856`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `Jbool _print_string(const J * const item, printbuffer * const p)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:744:_print_string_ptr` finalized
- External/system dependencies: none
- Data/type/global dependencies: reads `item->valuestring`

## Behavior

Delegates to `_print_string_ptr((unsigned char*)item->valuestring, p)` and returns that result.

## Important Boundaries

- Preconditions and assumptions: caller passes a non-null string `J` item; this wrapper does not null-check `item`.
- Error/null/empty behavior: null `valuestring` is handled by `_print_string_ptr` as an empty JSON string.
- Ownership and lifetime: reads borrowed `valuestring`, writes to caller printbuffer.
- Concurrency/global state: no global state.
- Performance shape: O(length of valuestring).

## Direct Call Sites

Exact source search found `_print_value` as the production caller for `JString` values.

## Role In Callers

`_print_value` dispatches string nodes here, and this wrapper centralizes string-value escaping through `_print_string_ptr`.

## Tests Observed

Focused public printing tests exercise this indirectly through `JPrintUnformatted`.

## Graph Links

- Graph node id: `function:n_cjson.c:856:_print_string`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `_print_string_ptr`.

## Evidence Gaps

No unresolved source gaps for this node.
