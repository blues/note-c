# `_print_object`

## Status

Finalized. Source-inspected JSON object printer in the recursive print-value SCC.

## Identity

- Kind: function
- Definition: `n_cjson.c:1523`
- Declaration/interface: file-local prototype at `n_cjson.c:867`
- Signature/API shape: `Jbool _print_object(const J * const item, printbuffer * const output_buffer)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:351:_ensure` finalized
  - `function:n_cjson_helpers.c:387:JGetItemType` finalized
  - `function:n_cjson.c:744:_print_string_ptr` finalized
  - `function:n_cjson.c:400:_update_offset` finalized
  - `function:n_cjson.c:1179:_print_value` recursive SCC peer
  - `function:n_cjson.c:1507:_last_non_omitted_object` finalized
- External/system dependencies: none
- Data/type/global dependencies: `J.child`, `J.next`, `J.string`, `printbuffer.format`, `printbuffer.omitempty`, `printbuffer.offset`, `printbuffer.depth`, `PRINT_TAB_CHARS`

## Behavior

Writes the opening object delimiter, optional formatted newline, and then walks the object child linked list. In formatted mode it emits indentation before each member. In omit-empty mode it classifies each current field with `JGetItemType` and skips false booleans, zero numbers, and blank strings. Printed fields serialize the key with `_print_string_ptr`, write `:` plus optional spacing, recursively emit the value with `_print_value`, update the offset, and use `_last_non_omitted_object` to decide whether a comma is needed before an optional newline. After the loop, it emits optional closing indentation, writes `}` and a NUL terminator, decrements depth, and returns true.

## Important Boundaries

- Preconditions and assumptions: caller passes a non-null object `item`; the function reads `item->child` before checking `output_buffer`.
- Error/null/empty behavior: null `output_buffer`, key/value print failure, or `_ensure` failure returns false.
- Ownership and lifetime: no standalone allocation; writes into caller-owned printbuffer storage.
- Concurrency/global state: no global state.
- Performance shape: O(number of object members plus recursive child output); omit-empty look-ahead can add sibling scans.
- Portability/platform constraints: default source sets `PRINT_TAB_CHARS` to 4. The dormant `PRINT_TAB_CHARS == 0` formatted branch references `output_buffer_offset`, which is not found elsewhere by source search.

## Direct Call Sites

Exact source search found `_print_value` as the production caller for `JObject` nodes.

## Role In Callers

All public print entry points reach this helper through `_print_value` when serializing objects, including the request serialization path through `JPrintUnformatted`.

## Tests Observed

`JPrintUnformatted_test.cpp` exercises nested object printing through `JPrintUnformatted`. `JSON_number_handling_test.cpp` uses object serialization for numeric fields. `JGetType_test.cpp` covers the detailed type classifications used by omit-empty decisions, but no focused omit-empty print test was found.

## Graph Links

- Graph node id: `function:n_cjson.c:1523:_print_object`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No focused tests were inspected for formatted object output, omit-empty output, allocation failure, or the dormant `PRINT_TAB_CHARS == 0` path.
