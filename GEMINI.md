# Gemini Instructions

This repository uses living architecture documentation. Treat these files as required context before non-trivial code changes:

1. `AGENTS.md`
2. `ARCHITECTURE.md`
3. Relevant files under `docs/architecture/`

When a change affects module boundaries, public APIs, hook signatures, dependencies, data/control flow, transport behavior, runtime assumptions, or build/test strategy, update the architecture docs in the same change.

If no architecture doc update is needed, say so explicitly in the final response or PR notes.
