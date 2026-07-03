# `_n_atoh`

## Status

Finalized. Source-inspected bounded hex string parser.

## Identity

- Kind: function
- Definition: `n_request.c:975`
- Declaration/interface: `n_lib.h:160`
- Signature/API shape: `uint64_t _n_atoh(char *p, int maxLen)`
- Visibility: internal helper declared in `n_lib.h`
- Public/exported names: `_n_atoh`

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: none
- Data/type/global dependencies: none

## Behavior

Parses up to `maxLen` characters from `p` as hexadecimal, accepting digits, lowercase A-F, uppercase A-F, and spaces. It multiplies the accumulator by 16 for every accepted character, adds numeric value for hex digits, treats spaces as zero nibbles, and stops at the first other character.

## Important Boundaries

- Preconditions and assumptions: `p` must be non-null and readable for up to `maxLen` bytes.
- Error/null/empty behavior: invalid first character or zero length returns 0.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: O(maxLen).
- Portability/platform constraints: ASCII hex assumptions.

## Direct Call Sites

Exact source search found `_crcError` as the production caller for response CRC sequence and checksum fields.

## Role In Callers

`_crcError` uses `_n_atoh` to decode the four-hex sequence number and eight-hex CRC value from the Notecard response tail.

## Tests Observed

`_crcError_test.cpp` exercises this indirectly through matching and mismatched CRC fields.

## Graph Links

- Graph node id: `function:n_request.c:975:_n_atoh`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
