# 0001: Keep Living Architecture Docs With Code Changes

## Status

Accepted

## Context

AI agents and humans both need fast, current context before making changes to `note-c`. README files explain usage, but they do not reliably capture internal boundaries, design tradeoffs, cross-repo adapter expectations, or compatibility-sensitive public contracts.

Because `note-c` is embedded by multiple Blues SDKs, architecture knowledge can drift unless it is maintained as part of normal code changes.

## Decision

Maintain architecture documentation as first-class repository content:

- Use root `ARCHITECTURE.md` as the repository-level architecture entrypoint.
- Use `docs/architecture/` for maintenance rules, ADRs, and templates.
- Instruct future agents through `AGENTS.md`, `CLAUDE.md`, `GEMINI.md`, Copilot instructions, and Cursor rules to read and update these docs.
- Require architecture doc updates when code changes affect boundaries, dependencies, APIs, transport/runtime behavior, or testing strategy.

## Consequences

Architecture docs become part of the normal definition of done for architecture-relevant changes. Small implementation-only changes do not require doc churn, but the author should explicitly say the docs were considered.
