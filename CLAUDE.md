# Claude Instructions

`AGENTS.md` is the canonical repository instruction file. Read it first and follow its architecture-documentation and `source-research/` requirements.

Before non-trivial code changes:

1. Read `AGENTS.md` for repository operating rules.
2. Read `ARCHITECTURE.md`.
3. Read `docs/architecture/architecture.html` when a visual graph or traversal overview helps.
4. Read relevant files under `docs/architecture/`.
5. Read `source-research/research-index.md` and the relevant source-research artifacts named in `AGENTS.md` when the task involves architecture, dependencies, public APIs, hooks, transports, runtime flow, build variants, or broad code generation.

When a change affects architecture, update the docs in the same change, including `ARCHITECTURE.md`, `docs/architecture/architecture.html`, and relevant files under `docs/architecture/` when applicable. Architecture-relevant changes include module boundaries, public APIs, hook signatures, dependencies, data/control flow, transport behavior, runtime assumptions, and build/test strategy.

When a change affects source-derived behavior, dependencies, public APIs, hooks, runtime flow, build variants, or architecture claims, update `source-research/` and validate it with the `source-first-repo-research` SKILL tools when available.

After every non-trivial code change, explicitly evaluate both maintenance surfaces: architecture docs and `source-research/`.

If no architecture doc or source-research update is needed, say that explicitly in your final response or PR notes.
