# `NoteLocationValid`

## Status

Finalized. Source-inspected non-suppressed location validity wrapper.

## Identity

- Kind: function
- Definition: `n_helpers.c:1139`
- Declaration/interface: `note.h:1765`
- Signature/API shape: `bool NoteLocationValid(char *errbuf, uint32_t errbuflen)`
- Visibility: public SDK API
- Public/exported names: `NoteLocationValid`
- Macro aliases/wrappers: none
- Ownership/lifetime role: writes optional caller error buffer through `NoteLocationValidST`

## Dependency Status

- Project-local dependencies:
  - `function:n_helpers.c:1156:NoteLocationValidST` finalized
- External/system dependencies: none
- Data/type/global dependencies: `locationTimer`, `locationValid`, `locationLastErr`

## Behavior

Forces a fresh validity evaluation by setting `locationTimer` to `0`, setting `locationValid` to false, clearing `locationLastErr`, and then returning `NoteLocationValidST(errbuf, errbuflen)`.

All response handling, suppression timer behavior after this reset, error-buffer writes, `"mode": "off"` handling, and response cleanup are owned by the finalized suppressed helper.

## Important Boundaries

- Preconditions and assumptions: same caller buffer assumptions as `NoteLocationValidST`.
- Error/null/empty behavior: clears cached error before delegating; return value is the suppressed helper result.
- Ownership and lifetime: no allocation or response ownership in this wrapper.
- Concurrency/global state: mutates static location cache/timer state without explicit locking.
- Performance shape: O(1) plus delegated location-valid check.
- Portability/platform constraints: Notecard I/O and timing behavior are delegated.

## Direct Call Sites

Exact source search found the public declaration, the implementation, and focused tests. No production library caller was found; this is a public API entry point.

## Role In Callers

Public entry point for callers that want an immediate location validity check rather than the time-suppressed cached variant.

## Tests Observed

`test/src/NoteLocationValid_test.cpp` covers no response, error propagation, `"mode": "off"` valid behavior, and the interaction between the fresh wrapper and subsequent cached `NoteLocationValidST` calls.

## Graph Links

- Graph node id: `function:n_helpers.c:1139:NoteLocationValid`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
