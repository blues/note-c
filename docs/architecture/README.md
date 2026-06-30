# Living Architecture Documentation

Architecture docs are maintained with the code. Future agents and humans should be able to read these files before changing `note-c` and quickly understand the current system shape, boundaries, and tradeoffs.

## Rules for Agents

Before non-trivial code changes:

1. Read the nearest applicable `AGENTS.md`, `CLAUDE.md`, or equivalent tool instruction file.
2. Read `ARCHITECTURE.md`.
3. Read relevant files under `docs/architecture/`.

After code changes:

- Update architecture docs when behavior, boundaries, dependencies, APIs, data flow, runtime topology, or testing strategy changes.
- Add an ADR for durable decisions that future maintainers should understand.
- Keep docs concise and factual. Prefer diagrams and lists over prose walls.
- Delete or rewrite stale architecture notes. Do not preserve obsolete statements just because they used to be true.
- If the change does not affect architecture, state that explicitly in the final response or PR notes.

## File Types

- `../../ARCHITECTURE.md`: repository-level architecture entrypoint.
- `architecture.html`: single-file graph traversal view for humans and agents. It should summarize architecture at component level and point back to source-backed evidence in `../../source-research/`.
- `../../source-research/`: structured source-derived symbol, dependency, claim, and build-variant evidence for agents and tooling.
- `../architecture.rst`: Sphinx documentation page that links to the published architecture map.
- `decisions/*.md`: architecture decision records.
- `templates/repo-architecture.md`: template for future architecture docs.
- `../../AGENTS.md`, `../../CLAUDE.md`, `../../GEMINI.md`, `../../.github/copilot-instructions.md`, and `../../.cursor/rules/architecture-docs.mdc`: AI entrypoints that point future tools back to these docs.

## What Belongs Here

Architecture docs should capture:

- Module responsibilities.
- Data/control flow between major components.
- Public contracts and compatibility constraints.
- Runtime and platform assumptions.
- Testing strategy for behavior that crosses module boundaries.
- Important tradeoffs, especially when there were plausible alternatives.

Keep `../../ARCHITECTURE.md`, `architecture.html`, and relevant `../../source-research/` artifacts synchronized when architecture changes. Treat `source-research/` as the structured source-backed evidence layer, not a replacement for reading implementation source. When graph facts or architecture claims change, update the nearest architecture doc and the canonical source-research JSON artifacts, then run the source-research validation tools described in `../../AGENTS.md`.

Architecture docs should not become:

- Full API reference.
- Changelog.
- Exhaustive file-by-file inventory.
- Raw meeting notes.
