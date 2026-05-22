# Repository Architecture

Replace this template with the current architecture of the repository or subsystem. Keep it short enough that future agents will read it before making changes.

## Purpose

Describe what this code owns and what it deliberately does not own.

## Major Components

- `path/or/module`: responsibility.
- `path/or/module`: responsibility.

## Data and Control Flow

```text
caller
  |
  v
component
  |
  v
external system or dependency
```

## Public Contracts

List APIs, file formats, protocols, hooks, or behavior that downstream users depend on.

## Dependencies

List important runtime, build, and test dependencies. Call out vendored or embedded dependencies and how they are updated.

## Runtime Model

Describe how the software runs in development, CI, production, or on hardware.

## Testing Strategy

Describe which tests protect which architectural contracts, and how to run them.

## Update Triggers

Update this file when changes affect module boundaries, public APIs, data/control flow, runtime topology, build/test/release workflow, or important cross-repo dependencies.
