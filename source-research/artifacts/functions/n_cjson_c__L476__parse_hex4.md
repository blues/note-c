# `_parse_hex4`

## Status

Finalized. Source-inspected four-hex-digit parser.

## Identity

- Kind: function
- Definition: `n_cjson.c:476`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `unsigned long _parse_hex4(const unsigned char * const input)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: none

## Behavior

Parses exactly four hexadecimal characters from `input`, accepting digits and uppercase or lowercase A-F. It shifts the accumulated value between nibbles and returns 0 on the first invalid character.

## Important Boundaries

- Preconditions and assumptions: caller supplies at least four readable bytes.
- Error/null/empty behavior: invalid input returns 0, which is also the valid value for `0000`.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: fixed four-iteration loop.
- Portability/platform constraints: ASCII hex character assumptions.

## Direct Call Sites

Exact source search found `_utf16_literal_to_utf8` as the sole production caller.

## Role In Callers

UTF-16 escape conversion uses this helper to parse each `\uXXXX` code unit.

## Tests Observed

String parsing tests exercise this indirectly through `JParse` when escaped unicode is present.

## Graph Links

- Graph node id: `function:n_cjson.c:476:_parse_hex4`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
