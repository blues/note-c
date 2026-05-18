# Copilot Instructions

Before making non-trivial code changes in this repository, read:

- `AGENTS.md`
- `ARCHITECTURE.md`
- `docs/architecture/architecture.json`
- `docs/architecture/architecture.html` when a visual overview helps
- Relevant files under `docs/architecture/`

Keep architecture documentation current with code changes. Update the closest relevant architecture file when a change affects boundaries, public APIs, hook signatures, dependencies, data/control flow, transport behavior, runtime assumptions, or build/test strategy. Keep the Markdown, JSON, and HTML architecture maps synchronized when applicable.

If a change is implementation-only and does not require architecture doc updates, mention that explicitly in PR notes.
