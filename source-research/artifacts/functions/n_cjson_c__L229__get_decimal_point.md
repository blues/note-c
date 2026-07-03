# `_get_decimal_point`

## Status

Finalized. Source-inspected decimal separator helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:229`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `unsigned char _get_decimal_point(void)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: `localeconv` only when `ENABLE_LOCALES` is defined
- Data/type/global dependencies: locale state only in locale-enabled builds

## Behavior

Returns the current locale decimal separator when `ENABLE_LOCALES` is defined, using `localeconv()->decimal_point[0]`. Otherwise returns `'.'`.

## Important Boundaries

- Preconditions and assumptions: locale-enabled builds assume `localeconv()` returns a valid decimal point string.
- Error/null/empty behavior: no error channel.
- Ownership and lifetime: no allocation.
- Concurrency/global state: locale-enabled variant reads process locale state.
- Performance shape: O(1).

## Direct Call Sites

Exact source search found callers in `_parse_number` and `_print_number`.

## Role In Callers

Number parse/print helpers normalize between locale decimal separators and JSON's required `.` separator.

## Tests Observed

Focused JSON number handling tests exercise number printing through `_print_number`; no locale-specific test was inspected.

## Graph Links

- Graph node id: `function:n_cjson.c:229:_get_decimal_point`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
