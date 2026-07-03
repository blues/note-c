# `NoteErrorClean`

## Status

Finalized. Source-inspected public in-place Notecard error tag cleaner.

## Identity

- Kind: function
- Definition: `n_request.c:945`
- Declaration/interface: `note.h:524`
- Public/exported names: `NoteErrorClean`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteErrorClean(char *errbuf)`
- Visibility: public note-c error helper implemented in `n_request.c`
- Ownership/lifetime role: mutates caller-owned error string storage in place

## Dependency Status

- Project-local dependencies: none
- External/system dependencies: `strlen`, `strchr`, `memmove`
- Data/type/global dependencies: none
- Artifact coverage: source-inspected artifact

## Behavior

Repeatedly finds the first `{...}` brace-delimited segment in `errbuf` and removes it with `memmove`. If the opening brace is preceded by a space, that preceding space is removed as part of the segment. The loop stops when no opening brace is found or when an opening brace has no following closing brace.

## Important Boundaries

- Preconditions and assumptions: `errbuf` must be a non-null, mutable, NUL-terminated string.
- Error/null/empty behavior: no return value; strings without a full brace pair are left unchanged from the first unmatched opening brace onward.
- Ownership and lifetime: no allocation; caller retains ownership of the same buffer.
- Concurrency/global state: no global state.
- Portability/platform constraints: uses standard C string/memory functions only.
- Performance shape: repeated scans and `memmove`; worst case is proportional to string length times number of removed tag pairs.
- Security or parsing constraints: brace matching is simple first-opening/first-closing removal, not nested-brace parsing.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:524`, the definition in `n_request.c:944`, and focused tests. No production repository caller, wrapper, or macro alias was found.

## Role In Callers

No production repository caller role. Public helper for downstream code that wants user-facing error text without Notecard `{type}` tags.

## Tests Observed

Focused tests in `test/src/NoteErrorClean_test.cpp` cover tag-only input, tags without a prefixed space separator, tags with a prefixed space, multiple brace pairs, and missing closing brace. Gaps: null input, nested braces, unmatched closing braces, and read-only string storage are not covered.

## Graph Links

- Graph node id: `function:n_request.c:945:NoteErrorClean`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges: none; this is a source-level leaf apart from external C library string/memory routines.

## Evidence Gaps

No unresolved source gaps for this node.
