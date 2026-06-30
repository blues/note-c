# `NoteNewCommand`

## Status

Finalized. Source-inspected public command JSON constructor, direct caller, constants, and focused tests.

## Identity

- Kind: function
- Definition: `n_request.c:182`
- Declaration/interface: `note.h:349`
- Public/exported names: `NoteNewCommand`
- Macro aliases/wrappers: none
- Signature/API shape: `J *NoteNewCommand(const char *request)`
- Visibility: public SDK function declared in `note.h`
- Ownership/lifetime role: returns a caller-owned JSON command object

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:2450:JCreateObject` finalized
  - `function:n_cjson.c:2013:JAddStringToObject` finalized
- External/system dependencies: none
- Data/type/global dependencies: reads global string constant `c_cmd`, declared in `n_lib.h` and defined as `"cmd"` in `n_const.c`
- Artifact coverage: source, declaration, constants, direct caller, and focused tests inspected

## Behavior

Creates a JSON object with `JCreateObject`. If object allocation succeeds, adds a string field named by `c_cmd` (`"cmd"`) with value `request` using `JAddStringToObject`. Returns the object pointer whether or not field attachment succeeds; returns `NULL` only when `JCreateObject` returns `NULL`.

## Important Boundaries

- Preconditions and assumptions: normal callers pass a non-null Notecard command name. A null `request` value causes `JAddStringToObject` to fail while this function still returns the empty object.
- Error/null/empty behavior: object allocation failure returns `NULL`; field-add failure is not separately reported or cleaned up.
- Ownership and lifetime: caller owns the returned object and must delete it or pass it to a request API that consumes ownership.
- Concurrency/global state: reads immutable global command-key string pointer.
- Portability/platform constraints: portable JSON construction; allocation behavior follows JSON allocator hooks.
- Performance shape: O(length of command string) on the successful field-add path.
- Security or parsing constraints: no validation of command name syntax.

## Direct Call Sites

Exact production caller found by source search:

- `n_helpers.c:1765` `NoteSleep` creates a `card.attn` command so the Notecard does not send a response while the host goes to sleep.

## Role In Callers

`NoteSleep` treats a null command object as failure and skips `NoteRequest`. It then attaches optional payload/mode fields to the command before sending it through the bool request wrapper.

## Tests Observed

`test/src/NoteNewCommand_test.cpp` covers `JCreateObject` failure returning `NULL` and `JCreateObject` success returning a non-null command object. `test/src/NoteSleep_test.cpp` fakes `NoteNewCommand` to cover caller behavior when command construction fails or succeeds.

## Test Coverage Notes

Focused constructor tests do not assert that the returned object contains a `"cmd"` field, do not cover null command names, and do not simulate `JAddStringToObject` failure after object allocation.

## Graph Links

- Graph node id: `function:n_request.c:182:NoteNewCommand`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
