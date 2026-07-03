# Source Research Index

## Scope

- Operation: create
- Research state: complete
- Internal unit: slice
- Execution mode: authorized-worker-background
- Documentation policy: forbidden
- Test policy: focused source evidence only
- Source root: `/Users/zak/Development/GitHub/blues/note-c`
- Artifact directory: `/Users/zak/Development/GitHub/blues/note-c/source-research`

## Inventory

- Source/build/interface files: 28
- Excluded test/source-evidence files: 208 via explicit exclusions; focused test evidence was inspected only for production/API symbols.
- Lifecycle status: 585 finalized / 585 tracked nodes; 0 preliminary.
- Node kinds: 341 functions, 205 macros, 5 structs, 34 typedefs.
- Test evidence: 431 focused, 154 none.
- Caller evidence: 405 verified, 180 none-found.
- Confidence: 585 high.
- Graph: 585 nodes, 1342 edges.
- Graph substrate: `inventory/graph-substrate.json`.

## Machine-Readable Artifacts

- `research-manifest.json`
- `inventory/source-inventory.json`
- `inventory/graph-substrate.json`
- `inventory/symbol-index.json`
- `inventory/dependency-graph.json`
- `inventory/source-node.schema.json`
- `inventory/claims-ledger.json`
- `inventory/progress-summary.json`
- `inventory/artifact-status.json`
- `inventory/evidence-validation.json`

## Worker Authorization

- User authorized as many bounded sub-agents/workers as needed to complete create research.
- Workers inspected assigned source slices and returned source-location evidence reports.
- The conductor merged evidence into canonical artifacts, graph metadata, lifecycle state, and claims.

## Lifecycle Frontier

- Research state is `complete`.
- `frontier/work-queue.json` has 0 ready nodes and 0 remaining preliminary nodes.
- Current active-slice file: `frontier/active-slice.json`.
- Active-slice status: complete.
- Normal final-response validation passed with `validate_research.py source-research --final-response`.

## Finalized Artifacts

- Full finalized-node list: `inventory/artifact-status.json`.
- Source-derived overview: `overview.md`.
- Completed source coverage includes public `note.h` macros/typedefs, hook registry and active-interface dispatch, request orchestration, I2C/serial transports, cJSON core and helpers, high-level helpers, binary/COBS/MD5 utilities, compile-time configuration branches, and portability helpers.

## Claims

- Claims ledger: `inventory/claims-ledger.json`.
- Counts: 29 finalized, 29 total.
- Claims are source-backed and reference implementation locations, worker evidence reports, artifact status, and evidence validation outputs.

## Validation

- Last lifecycle evidence pass had 0 violations and 0 warnings.
- Last normal validation passed: `validate_research.py source-research --require-slices --min-finalized-nodes 585`.
- Final response gate passed: `validate_research.py source-research --final-response`.

## Evidence Notes

- The initial create pass ran before CMake was available, so Apple `clang` syntax checks from the initial substrate were recorded in `inventory/graph-substrate.json`; the later CMake/Clang AST validation section below records the compile-database update.
- Focused tests were inspected as source evidence but were not executed during this research pass.
- The initial source-research create pass changed only `source-research/` artifacts; later architecture-doc alignment should keep this index and the architecture docs consistent.

<!-- cmake-validation:start -->
## CMake / Clang AST Validation

- Updated: 2026-07-02T23:45:52+00:00
- CMake: installed with Homebrew; `cmake version 4.3.4`.
- Default production compile database: `inventory/compile_commands.default.json` with 15 translation units, `HAVE_STRLCPY`, `HAVE_STRLCAT`, `n_str.c` excluded, `n_ua.c` included.
- Low-memory/single-precision compile database: `inventory/compile_commands.lowmem-single.json` with 14 translation units, `HAVE_STRLCPY`, `HAVE_STRLCAT`, `NOTE_C_LOW_MEM`, `NOTE_C_SINGLE_PRECISION`, `n_str.c` and `n_ua.c` excluded.
- Both CMake builds passed with the same numeric-conversion warnings previously inferred by manual clang checks.
- Clang AST validation report: `inventory/cmake-ast-validation.json`.
- Default AST revalidation found 335 compiled function definitions and 768 project-local direct call edges, with 0 AST definitions missing from the graph and 0 unresolved direct-call edge candidates. Four graph functions remain feature-disabled in the default variant: heartbeat callback functions and `NOTE_C_SHOW_MALLOC` pointer formatting.
- Low-memory/single-precision AST revalidation found 325 compiled function definitions and 750 project-local direct call edges, with 0 AST definitions missing from the graph and 0 unresolved direct-call edge candidates. The additional graph functions not seen by that variant are expected compile-time exclusions for CRC helpers and related conversion support.
- The earlier CMake/Clang validation added 38 high-confidence unverified `calls_static` edges to `inventory/dependency-graph.json`; the 2026-07-02 revalidation added 0 edges.
- Dependency graph summary after revalidation: 585 nodes, 1342 edges.
- Final validation: `validate_research.py source-research` and `validate_research.py source-research --final-response` passed after the 2026-07-02 update.
<!-- cmake-validation:end -->
