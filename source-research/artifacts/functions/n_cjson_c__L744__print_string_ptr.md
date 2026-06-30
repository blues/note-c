# `_print_string_ptr`

## Status

Finalized. Source-inspected JSON string escaping helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:744`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `Jbool _print_string_ptr(const unsigned char * const input, printbuffer * const output_buffer)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:351:_ensure` finalized
  - `function:n_cjson.c:728:_n_htoa16` finalized
- External/system dependencies: `memcpy`
- Data/type/global dependencies: mutates printbuffer contents

## Behavior

Prints a JSON string literal into the printbuffer. Null input is rendered as `""`. Non-null input is scanned to count characters needing escaping. If no escaping is needed, it writes a quoted copy. Otherwise it writes a quoted escaped string, using short escapes for quote, backslash, backspace, form feed, newline, carriage return, and tab; other control bytes below 32 are emitted as `\uXXXX` through `_n_htoa16`.

## Important Boundaries

- Preconditions and assumptions: `output_buffer` must be valid and backed by `_ensure`-managed storage.
- Error/null/empty behavior: null output buffer or allocation/growth failure returns false; null input prints `""`.
- Ownership and lifetime: no allocation outside `_ensure` buffer management.
- Concurrency/global state: no global state.
- Performance shape: O(input length), with a first scan and then copy/escape pass.

## Direct Call Sites

Exact source search found callers in `_print_string`, `_print_object` for object keys, and public/preallocated print paths through value/object printers.

## Role In Callers

String and object printers use this helper to produce JSON-escaped key and value string fragments.

## Tests Observed

Focused `JPrintUnformatted` tests exercise nested object/string output indirectly. No direct escape-table test was inspected in this slice.

## Graph Links

- Graph node id: `function:n_cjson.c:744:_print_string_ptr`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_ensure` and `_n_htoa16`.

## Evidence Gaps

No unresolved source gaps for this node.
