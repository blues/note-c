# Gemini Instructions

This repository uses living architecture documentation. Treat these files as required context before non-trivial code changes:

1. `AGENTS.md`
2. `ARCHITECTURE.md`
3. `docs/architecture/architecture.json`
4. `docs/architecture/architecture.html` when a visual overview helps
5. Relevant files under `docs/architecture/`

When a change affects module boundaries, public APIs, hook signatures, dependencies, data/control flow, transport behavior, runtime assumptions, or build/test strategy, update the architecture docs in the same change, including the Markdown, JSON, and HTML architecture maps when applicable.

If no architecture doc update is needed, say so explicitly in the final response or PR notes.
