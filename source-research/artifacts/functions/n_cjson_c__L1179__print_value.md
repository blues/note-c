# `_print_value`

## Status

Finalized. Source-inspected JSON value printer and dispatcher.

## Identity

- Kind: function
- Definition: `n_cjson.c:1179`
- Declaration/interface: file-local prototype at `n_cjson.c:863`
- Signature/API shape: `Jbool _print_value(const J * const item, printbuffer * const output_buffer)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:351:_ensure` finalized
  - `function:n_str.c:31:strlcpy` finalized
  - `function:n_cjson.c:412:_print_number` finalized
  - `function:n_cjson.c:856:_print_string` finalized
  - `function:n_cjson.c:1341:_print_array` recursive SCC peer
  - `function:n_cjson.c:1523:_print_object` recursive SCC peer
- External/system dependencies: `strlen`, `memcpy`
- Data/type/global dependencies: `J.type`, `J.valuestring`, JSON type constants, constant JSON literal strings

## Behavior

Rejects null item or output buffer, then dispatches on `item->type & 0xFF`. Null, false, and true values reserve enough space through `_ensure` and copy the fixed JSON literal through `strlcpy`. Number and string nodes delegate to `_print_number` and `_print_string`. Raw nodes require non-null `valuestring`, reserve `strlen(valuestring) + 1`, and copy the raw bytes including the terminator. Array and object nodes delegate to `_print_array` and `_print_object`. Unknown base types return false.

## Important Boundaries

- Preconditions and assumptions: caller owns a valid printbuffer and updates offsets at call boundaries where needed.
- Error/null/empty behavior: null inputs, raw nodes with null `valuestring`, unknown types, or failed callee/ensure operations return false.
- Ownership and lifetime: no standalone allocation; writes into caller-provided printbuffer storage.
- Concurrency/global state: no global state.
- Performance shape: O(value size plus recursive child output).
- Portability/platform constraints: forms a recursive SCC with `_print_array` and `_print_object`; print-side recursion is tracked by `printbuffer.depth` but no source-level nesting limit is enforced by these print helpers.

## Direct Call Sites

Exact source search found direct callers in `_print`, `JPrintBuffered`, `_printPreallocated`, `_print_array`, and `_print_object`.

## Role In Callers

This is the central JSON serialization dispatcher behind all public print APIs. `JPrintUnformatted` reaches it through `_print`; array/object printing re-enters it for each child.

## Tests Observed

`JPrintUnformatted_test.cpp` exercises strings, numbers, booleans, null, arrays, and nested objects through the public unformatted print API. `JSON_number_handling_test.cpp` gives focused coverage of number serialization through this dispatcher.

## Graph Links

- Graph node id: `function:n_cjson.c:1179:_print_value`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused tests were inspected for raw node printing, allocation failure, formatted output, omit-empty output, or preallocated-buffer failure behavior.
