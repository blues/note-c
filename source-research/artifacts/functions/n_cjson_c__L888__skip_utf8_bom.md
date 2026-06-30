# `_skip_utf8_bom`

## Status

Finalized. Source-inspected root parse UTF-8 BOM skipper.

## Identity

- Kind: function
- Definition: `n_cjson.c:888`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `parse_buffer *_skip_utf8_bom(parse_buffer * const buffer)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `macro:n_cjson.c:249:can_access_at_index` finalized
  - `macro:n_cjson.c:252:buffer_at_offset` finalized
- External/system dependencies: `strncmp`
- Data/type/global dependencies: `parse_buffer.content`, `parse_buffer.offset`

## Behavior

Returns NULL for a null buffer, null content, or any nonzero starting offset. At offset zero, checks for a UTF-8 BOM byte sequence and advances the offset by three bytes when present. Returns the same buffer pointer either way for valid offset-zero input.

## Important Boundaries

- Preconditions and assumptions: only intended at the start of root parsing.
- Error/null/empty behavior: invalid buffer/content/offset returns NULL.
- Ownership and lifetime: no allocation.
- Concurrency/global state: no global state.
- Performance shape: O(1).
- Portability/platform constraints: matches literal UTF-8 BOM bytes.

## Direct Call Sites

Exact source search found `JParseWithOpts` as the sole production caller.

## Role In Callers

`JParseWithOpts` chains this helper before whitespace skipping and root value parsing.

## Tests Observed

Parser-focused public tests exercise root parsing indirectly. No direct BOM-specific focused test was identified.

## Graph Links

- Graph node id: `function:n_cjson.c:888:_skip_utf8_bom`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
