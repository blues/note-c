# `JParse`

## Status

Finalized. Source-inspected public JSON parser wrapper.

## Identity

- Kind: function
- Definition: `n_cjson.c:976`
- Declaration/interface: `n_cjson.h:187`
- Signature/API shape: `J *JParse(const char *value)`
- Visibility: public cJSON/note-c JSON API
- Public/exported names: `JParse`, macro alias `JConvertFromJSONString` in `n_cjson.h:311`

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:902:JParseWithOpts` finalized
- External/system dependencies: none
- Data/type/global dependencies: caller-provided JSON text

## Behavior

Thin wrapper that returns `JParseWithOpts(value, 0, 0)`, accepting trailing non-NUL content according to the underlying parser's default mode.

## Important Boundaries

- Preconditions and assumptions: `value` must be a NUL-terminated JSON text string.
- Error/null/empty behavior: parse failure returns NULL through `JParseWithOpts`.
- Ownership and lifetime: on success caller owns the returned `J` tree and must call `JDelete`.
- Concurrency/global state: underlying parser updates `global_error`.
- Performance shape: follows `JParseWithOpts`.
- Portability/platform constraints: follows parser nesting and allocator behavior.

## Direct Call Sites

Exact source search found production callers in `NoteRequestResponseJSON`, `_noteTransactionShouldLock`, and `NotePing`, plus many focused tests. `n_cjson.h:311` aliases `JConvertFromJSONString` to this function.

## Role In Callers

`_noteTransactionShouldLock` uses `JParse` to turn Notecard response JSON into the returned `J` response tree and to classify parse failures as I/O-style corrupt responses.

## Tests Observed

`JSON_number_handling_test.cpp`, `JPrintUnformatted_test.cpp`, `NoteTransaction_test.cpp`, `NotePing_test.cpp`, and `NoteRequestResponseJSON_test.cpp` exercise this parser wrapper.

## Graph Links

- Graph node id: `function:n_cjson.c:976:JParse`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
