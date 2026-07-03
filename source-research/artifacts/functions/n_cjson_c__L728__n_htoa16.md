# `_n_htoa16`

## Status

Finalized. Source-inspected 16-bit uppercase hex formatter.

## Identity

- Kind: function
- Definition: `n_cjson.c:728`
- Declaration/interface: `n_lib.h:159`
- Signature/API shape: `void _n_htoa16(uint16_t n, unsigned char *p)`
- Visibility: internal utility with declaration in `n_lib.h`
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: writes caller-provided output buffer

## Behavior

Writes four uppercase hexadecimal digits for the high-to-low nibbles of `n`, then appends a NUL terminator. Nibbles 0-9 map to `'0'`-`'9'`; 10-15 map to `'A'`-`'F'`.

## Important Boundaries

- Preconditions and assumptions: caller provides at least five output bytes.
- Error/null/empty behavior: no null check or error channel.
- Ownership and lifetime: writes only to caller buffer.
- Concurrency/global state: no global state.
- Performance shape: fixed four-iteration loop.

## Direct Call Sites

Exact source search found callers in `_print_string_ptr` for `\uXXXX` escaping and `_crcAdd` for request CRC sequence number formatting.

## Role In Callers

String printing uses `_n_htoa16` to emit JSON unicode escapes for control bytes below 32 that do not have short escape forms.

## Tests Observed

Focused public printing tests exercise string serialization indirectly. No direct `_n_htoa16` test was inspected.

## Graph Links

- Graph node id: `function:n_cjson.c:728:_n_htoa16`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
