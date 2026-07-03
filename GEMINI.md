# Gemini Instructions

`AGENTS.md` is the canonical repository instruction file. Read it first and follow its architecture-documentation and `source-research/` requirements.

Before non-trivial code changes, required context includes:

1. `AGENTS.md`
2. `ARCHITECTURE.md`
3. `docs/architecture/architecture.html` when a visual graph or traversal overview helps
4. Relevant files under `docs/architecture/`
5. `source-research/research-index.md` and the relevant source-research artifacts named in `AGENTS.md` when the task involves architecture, dependencies, public APIs, hooks, transports, runtime flow, build variants, or broad code generation

When a change affects module boundaries, public APIs, hook signatures, dependencies, data/control flow, transport behavior, runtime assumptions, or build/test strategy, update the architecture docs in the same change, including the Markdown docs, `docs/architecture/architecture.html`, and relevant files under `docs/architecture/` when applicable.

When a change affects source-derived behavior, dependencies, public APIs, hooks, runtime flow, build variants, or architecture claims, update `source-research/` and validate it with the `source-first-repo-research` SKILL tools when available.

After every non-trivial code change, explicitly evaluate both maintenance surfaces: architecture docs and `source-research/`.

If no architecture doc or source-research update is needed, say so explicitly in the final response or PR notes.
