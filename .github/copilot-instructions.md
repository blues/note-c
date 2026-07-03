# Copilot Instructions

Before making non-trivial code changes in this repository, read:

- `AGENTS.md`
- `ARCHITECTURE.md`
- `docs/architecture/architecture.html` when a visual graph or traversal overview helps
- Relevant files under `docs/architecture/`
- `source-research/research-index.md` and relevant source-research artifacts for architecture, dependency, public API, hook, transport, runtime-flow, build-variant, or broad code-generation work

Keep architecture documentation current with code changes. Update the closest relevant architecture file when a change affects boundaries, public APIs, hook signatures, dependencies, data/control flow, transport behavior, runtime assumptions, or build/test strategy. Keep the Markdown docs and `docs/architecture/architecture.html` synchronized when applicable. Update `source-research/` when the same change affects source-derived behavior, dependencies, public APIs, hooks, runtime flow, build variants, or architecture claims.

If a change is implementation-only and does not require architecture doc updates, mention that explicitly in PR notes.
