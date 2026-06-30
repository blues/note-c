# `JMinify`

## Status

Finalized. Source-inspected public in-place JSON minifier.

## Identity

- Kind: function
- Definition: `n_cjson.c:2616`
- Declaration/interface: `n_cjson.h:297`
- Public/exported names: `JMinify`
- Macro aliases/wrappers: none found
- Signature/API shape: `void JMinify(char *json)`
- Visibility: public cJSON API function implemented in `n_cjson.c`
- Ownership/lifetime role: mutates the caller-provided JSON buffer in place

## Dependency Status

- Project-local dependencies:
- No project-local callees.
- External/system dependencies: none
- Data/type/global dependencies: reads and writes the caller-provided character buffer only
- Artifact coverage: source-inspected artifact

## Behavior

Returns immediately when `json` is `NULL`. Otherwise compacts the input buffer in place by skipping spaces, tabs, carriage returns, newlines, C++-style `//` comments through newline, and C-style `/* ... */` comments. String literals are copied through with escape handling so whitespace and comment-like sequences inside strings are preserved. All other bytes are copied forward, and the output is NUL-terminated.

## Important Boundaries

- Preconditions and assumptions: input must be a writable NUL-terminated buffer.
- Error/null/empty behavior: null input is a no-op; empty string remains empty.
- Ownership and lifetime: no allocation or ownership transfer.
- Concurrency/global state: no global state.
- Portability/platform constraints: byte-oriented ASCII whitespace/comment/string scanning.
- Performance shape: single pass over the buffer with in-place writes.
- Security or parsing constraints: does not validate JSON. If a `/*` comment or string literal is unterminated, the source pointer can advance past the first NUL terminator before the outer loop condition is checked again.

## Direct Call Sites

Exact source search found only the public declaration in `n_cjson.h:297` and the definition in `n_cjson.c:2616`. No production repository caller was found.

## Role In Callers

No repository caller role. Public cJSON-compatible utility for downstream callers that want to remove whitespace and comments from a mutable JSON buffer.

## Tests Observed

None. Focused test search found no `JMinify` or minify test references.

## Graph Links

- Graph node id: `function:n_cjson.c:2616:JMinify`
- Structured graph: `inventory/dependency-graph.json`
- Verified as a leaf with no project-local callees.

## Evidence Gaps

No unresolved source gaps for this node.
