# `_utf16_literal_to_utf8`

## Status

Finalized. Source-inspected UTF-16 JSON escape to UTF-8 converter.

## Identity

- Kind: function
- Definition: `n_cjson.c:504`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `unsigned char _utf16_literal_to_utf8(const unsigned char * const input_pointer, const unsigned char * const input_end, unsigned char **output_pointer)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:476:_parse_hex4` finalized
- External/system dependencies: none
- Data/type/global dependencies: UTF-16 surrogate ranges and UTF-8 byte encoding rules

## Behavior

Validates that the first `\uXXXX` sequence is present, parses it, rejects an isolated low surrogate, and handles high-surrogate pairs by requiring a following `\uXXXX` low surrogate. It computes the Unicode codepoint, chooses a one- to four-byte UTF-8 encoding, writes bytes through `*output_pointer`, advances the output pointer by the emitted length, and returns the consumed input sequence length (`6` or `12`). Invalid or truncated escapes return 0.

## Important Boundaries

- Preconditions and assumptions: caller points at the backslash of a JSON `\u` escape and supplies an output buffer large enough for the converted bytes.
- Error/null/empty behavior: invalid escapes return 0 without an error string.
- Ownership and lifetime: no allocation; writes into caller-owned output storage.
- Concurrency/global state: no global state.
- Performance shape: fixed bounded work.
- Portability/platform constraints: emits UTF-8 bytes manually and assumes JSON escape source bytes are ASCII.

## Direct Call Sites

Exact source search found `_parse_string` as the sole production caller.

## Role In Callers

`_parse_string` uses this helper for `\uXXXX` escape sequences while unescaping JSON string literals.

## Tests Observed

Public parser/print tests exercise string parsing indirectly. No direct unicode-specific test was identified in the focused source pass.

## Graph Links

- Graph node id: `function:n_cjson.c:504:_utf16_literal_to_utf8`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
