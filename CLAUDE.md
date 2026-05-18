# Claude Instructions

This repository uses living architecture documentation. Treat these files as required context, the same way you would treat README files.

Before non-trivial code changes:

1. Read `AGENTS.md` for repository operating rules.
2. Read `ARCHITECTURE.md`.
3. Read relevant files under `docs/architecture/`.

When a change affects architecture, update the docs in the same change. Architecture-relevant changes include module boundaries, public APIs, hook signatures, dependencies, data/control flow, transport behavior, runtime assumptions, and build/test strategy.

If no architecture doc update is needed, say that explicitly in your final response or PR notes.
