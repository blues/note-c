# Agent Instructions

This repository uses living architecture documentation. Treat architecture docs like README docs: if a code change changes the shape, flow, boundaries, dependencies, public APIs, runtime model, or testing strategy, update the architecture docs in the same change.

Before non-trivial code changes:

1. Read `ARCHITECTURE.md`.
2. Read any relevant files under `docs/architecture/`.
3. Check `README.md` and tests relevant to the behavior being changed.

When you change architecture-relevant behavior:

- Update the closest relevant `ARCHITECTURE.md` or `docs/architecture/*.md` file.
- Add or update an ADR under `docs/architecture/decisions/` when the change records a durable design decision or tradeoff.
- Keep docs factual and current. Remove stale statements instead of adding contradictory notes.
- If no architecture doc update is needed, mention that explicitly in your final response or PR notes.

`note-c` is a portable C SDK. Keep platform-specific behavior behind hooks or in adapter libraries unless the public architecture intentionally changes.
