# Agent Instructions

This repository uses living architecture documentation. Treat architecture docs like README docs: if a code change changes the shape, flow, boundaries, dependencies, public APIs, runtime model, or testing strategy, update the architecture docs in the same change.

Before non-trivial code changes:

1. Read `ARCHITECTURE.md`.
2. Read `docs/architecture/architecture.html` when a visual graph or traversal overview helps.
3. Read any other relevant files under `docs/architecture/`.
4. Read `source-research/research-index.md` and the relevant source-research artifacts described below when the change involves architecture, inter-module dependencies, public APIs, hooks, transports, build variants, runtime flow, or broad code generation.
5. Check `README.md` and tests relevant to the behavior being changed.

After every non-trivial code change, explicitly evaluate both maintenance surfaces:

1. Architecture docs: update `ARCHITECTURE.md` and `docs/architecture/` when the change affects system shape, flow, boundaries, dependencies, public APIs, runtime model, or testing strategy.
2. Source research: update `source-research/` when the change affects source-derived behavior, dependencies, public APIs, hooks, runtime flow, build variants, or architecture claims.

If either surface is not affected, say so explicitly in the final response or PR notes.

## Source Research Artifacts

This repository includes durable source-derived research under `source-research/`. Use these artifacts as required orientation and impact-analysis context for architecture questions, dependency analysis, public API changes, hook/transport changes, runtime-flow changes, build/test strategy changes, and broad code-generation or refactoring work.

Start with:

1. `source-research/research-index.md`
2. `source-research/overview.md`
3. `source-research/inventory/symbol-index.json`
4. `source-research/inventory/dependency-graph.json`
5. `source-research/inventory/claims-ledger.json`
6. Relevant node artifacts under `source-research/artifacts/`

Use `source-research` to identify relevant symbols, callers, callees, aliases/macros, module boundaries, public surfaces, finalized claims, and known build/configuration variants. Then read the implementation source before making or asserting behavior changes.

Source files remain authoritative. If source and `source-research` disagree, trust source and update or mark the affected research artifacts stale. If source-research prose and machine-readable JSON disagree, prefer the current JSON and repair stale prose. Use only finalized/source-backed claims as established facts; treat preliminary, stale, or inferred claims as leads to verify in source.

When code changes affect behavior, dependencies, public APIs, hooks, runtime flow, build variants, or architecture claims, update the relevant `source-research` artifacts in the same change. Prefer the `source-first-repo-research` SKILL tools when available:

- Run lifecycle and evidence refreshes after artifact updates:
  - `research_lifecycle.py analyze source-research --write`
  - `research_lifecycle.py evidence source-research --write`
  - `validate_research.py source-research`
- For finalized create-complete validation, run `validate_research.py source-research --final-response`.
- For C/C++ graph-impacting changes, rerun `cmake_ast_validate.py` against the relevant compile database, review unresolved edge candidates, and merge source-confirmed repairs with `graph_edge_upsert.py`.
- Do not edit dependency graph facts only in prose. Update canonical JSON artifacts and validate them.
- If the SKILL tools are unavailable, make the source-research update manually from source evidence and state the validation limitation in the final response or PR notes.

When you change architecture-relevant behavior:

- Update the closest relevant `ARCHITECTURE.md` or `docs/architecture/*.md` file.
- Keep `docs/architecture/architecture.html` synchronized with the Markdown architecture docs and the current source-research evidence.
- Add or update an ADR under `docs/architecture/decisions/` when the change records a durable design decision or tradeoff.
- Update `source-research/` when the same change affects source-derived behavior, dependencies, public APIs, hooks, runtime flow, build variants, or architecture claims.
- Keep docs factual and current. Remove stale statements instead of adding contradictory notes.
- Apply the two-surface maintenance check above after every non-trivial code change.

`note-c` is a portable C SDK. Keep platform-specific behavior behind hooks or in adapter libraries unless the public architecture intentionally changes.
