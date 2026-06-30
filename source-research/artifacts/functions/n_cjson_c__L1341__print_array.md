# `_print_array`

## Status

Finalized. Source-inspected JSON array printer in the recursive print-value SCC.

## Identity

- Kind: function
- Definition: `n_cjson.c:1341`
- Declaration/interface: file-local prototype at `n_cjson.c:865`
- Signature/API shape: `Jbool _print_array(const J * const item, printbuffer * const output_buffer)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:351:_ensure` finalized
  - `function:n_cjson.c:1179:_print_value` recursive SCC peer
  - `function:n_cjson.c:400:_update_offset` finalized
- External/system dependencies: none
- Data/type/global dependencies: `J.child`, `J.next`, `printbuffer.format`, `printbuffer.offset`, `printbuffer.depth`

## Behavior

Writes `[` into the printbuffer, increments offset and depth, then walks the array child linked list. Each child is emitted through `_print_value`, `_update_offset` advances past the emitted fragment, and comma separators are inserted between siblings with an added space in formatted mode. The helper finally reserves room for `]` plus a terminator, writes the closing bracket and NUL, decrements depth, and returns true.

## Important Boundaries

- Preconditions and assumptions: caller passes a non-null array `item`; the function reads `item->child` before checking `output_buffer`.
- Error/null/empty behavior: null `output_buffer`, child print failure, or `_ensure` failure returns false.
- Ownership and lifetime: no standalone allocation; writes into caller-owned printbuffer storage.
- Concurrency/global state: no global state.
- Performance shape: O(number of array children plus recursive child output).
- Portability/platform constraints: participates in the `_print_value`/`_print_array`/`_print_object` recursion; no print-side nesting limit is enforced here.

## Direct Call Sites

Exact source search found `_print_value` as the production caller for `JArray` nodes.

## Role In Callers

All public print entry points reach this helper through `_print_value` when serializing arrays, including recursive arrays nested inside objects or other arrays.

## Tests Observed

`JPrintUnformatted_test.cpp` exercises arrays and nested arrays through `JPrintUnformatted`. JSON number tests also exercise object printing that can contain numbers, but array coverage is mainly the focused print test.

## Graph Links

- Graph node id: `function:n_cjson.c:1341:_print_array`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused tests were inspected for allocation failure, formatted array output, deeply recursive arrays, or preallocated-buffer failure behavior.
