# `NoteTurboIO`

## Status

Finalized. Source-inspected deprecated public no-op helper.

## Identity

- Kind: function
- Definition: `n_helpers.c:2587`
- Declaration/interface: `note.h:1782` with `NOTE_C_DEPRECATED`
- Public/exported names: `NoteTurboIO`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteTurboIO(bool enable)`
- Visibility: public note-c helper implemented in `n_helpers.c`
- Ownership/lifetime role: no ownership or memory effects

## Dependency Status

- Project-local dependencies:
  - `macro:note.h:998:NOTE_C_LOG_WARN` finalized
- External/system dependencies: configured debug output hook reached through the logging macro when logging is active
- Data/type/global dependencies: warning log level and debug hook state through `NOTE_C_LOG_WARN`; nearby historical `cardTurboIO` global is not written by this function
- Artifact coverage: source-inspected artifact

## Behavior

Ignores the `enable` argument and emits a warning message: `"NoteTurboIO is deprecated and has no effect."` It does not toggle `cardTurboIO` or any transport behavior.

## Important Boundaries

- Preconditions and assumptions: no pointer inputs. `enable` is explicitly cast to void.
- Error/null/empty behavior: no return value or failure channel. If debug output is disabled or no hook is installed, the warning is dropped through the finalized logging path.
- Ownership and lifetime: no allocations, frees, or ownership transfers.
- Concurrency/global state: does not mutate `cardTurboIO`; logging uses global debug hook/log-level state.
- Portability/platform constraints: public declaration is compiler-deprecation-annotated through `NOTE_C_DEPRECATED`; runtime behavior is otherwise portable.
- Performance shape: one warning log attempt.
- Security or parsing constraints: no parsing.

## Direct Call Sites

Exact source search found only the public declaration in `note.h:1782`, the definition in `n_helpers.c:2587`, and the warning string. No production repository caller was found. Broader search found the historical `cardTurboIO` global and transport reads, but this function does not write that global.

## Role In Callers

No production repository caller role. Deprecated compatibility API that now warns and has no effect.

## Tests Observed

No focused tests referencing `NoteTurboIO`, `TurboIO`, or `cardTurboIO` behavior were found.

## Graph Links

- Graph node id: `function:n_helpers.c:2587:NoteTurboIO`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edge to `NOTE_C_LOG_WARN`.

## Evidence Gaps

No unresolved source gaps for this node.
