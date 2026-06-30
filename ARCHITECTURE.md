# note-c Architecture

This file is the architecture entrypoint for `note-c`. Keep it current when code changes alter system boundaries, public contracts, transport behavior, build/test strategy, or cross-repo expectations.

Companion architecture artifacts:

- `docs/architecture/architecture.html`: human-readable graph traversal view.
- `source-research/`: source-derived structured evidence for symbols, dependencies, claims, and build variants.

## How To Use This File

`ARCHITECTURE.md` is the curated architecture entrypoint. It describes the intended system shape, durable boundaries, public contracts, runtime model, portability constraints, and testing strategy that maintainers should preserve when changing `note-c`.

For source-backed implementation evidence, use `source-research/`:

- `source-research/research-index.md`: re-entry point and validation status.
- `source-research/overview.md`: source-derived system summary.
- `source-research/inventory/dependency-graph.json`: caller/callee and dependency graph.
- `source-research/inventory/symbol-index.json`: symbol and artifact lookup.
- `source-research/inventory/claims-ledger.json`: finalized source-backed claims.

Use this file to understand what the architecture is meant to be. Use `source-research/` to verify how the current source implements it and to scope impact before code changes. Source remains authoritative: if implementation, `source-research/`, and architecture docs disagree, inspect the source, then update the stale architecture or research artifact in the same change.

## Purpose

`note-c` is the portable C SDK for communicating with a Blues Notecard over serial or I2C. It owns the core request/response model, bundled JSON helpers, protocol framing, transport orchestration, and hook-based platform abstraction.

`note-c` should remain platform-neutral. Platform-specific I/O, memory, timing, mutex, and logging behavior belongs behind hooks or in adapter libraries such as `note-arduino`, `note-posix`, `note-zephyr`, or `note-espidf`.

## Major Components

- `note.h`: public SDK API, version constants, hook typedefs, request helpers, and compatibility surface.
- `n_lib.h`: internal declarations shared across implementation files.
- `n_request.c`: request/response lifecycle, transaction orchestration, retry/timeout behavior, and request ownership boundaries.
- `n_serial.c`: serial transport implementation and chunked newline-framed serial transmit/receive behavior.
- `n_i2c.c`: I2C transport implementation and chunked newline-framed I2C transmit/receive behavior.
- `n_hooks.c`: global function-pointer hook registry, active-interface dispatch, and invocation of platform hooks for memory, time, mutexes, debug output, and transports.
- `n_cjson.c`, `n_cjson.h`, `n_cjson_helpers.c`: bundled JSON representation and helper APIs.
- `n_helpers.c`, `n_str.c`, `n_printf.c`, `n_atof.c`, `n_ftoa.c`, `n_b64.c`, `n_cobs.c`, `n_md5.c`, `n_const.c`, `n_ua.c`: portability helpers, encoding, formatting, constants, and utility behavior.
- `test/`: unit tests and mocks for protecting SDK behavior without requiring real hardware.
- `scripts/`: local automation for checks, documentation, and release support.

## Data and Control Flow

```text
application or adapter library
        |
        v
public API in note.h
        |
        v
request/response and JSON helpers
        |
        v
transport selection and chunking
        |
        v
registered serial or I2C hook
        |
        v
platform driver / hardware bus
        |
        v
Notecard
```

Applications normally build requests as `J` objects, send them through `NoteRequest`, `NoteRequestResponse`, retrying variants, or higher-level helpers, then release returned responses through the JSON/delete APIs. The consuming request wrappers delete the input request object after transaction; lower-level `NoteTransaction` paths leave request ownership with the caller. `NoteRequestResponseJSON` is a separate raw newline-delimited JSON string path with caller-owned request and response strings.

Transport and platform behavior is supplied through hooks so the same core code can run on microcontrollers, embedded Linux, tests, and other C/C++ environments. Serial and I2C transports move raw newline-framed bytes through hook dispatch. Binary payload helpers, not the transport implementations, own COBS framing and MD5 verification.

## Public Contracts

The main compatibility contracts are:

- Public functions, typedefs, constants, and macros in `note.h`.
- JSON object behavior exposed through `J` and helper functions.
- Request/response ownership semantics for public transaction APIs.
- Hook signatures for serial, I2C, memory, mutex, time, and debug output.
- Notecard request/response semantics and timeout/retry behavior.
- Build configuration behavior for low-memory, single-precision, user-agent, CRC, and portability-helper variants.
- Version constants and release expectations documented in `README.md`.

Breaking changes to these contracts require deliberate versioning, migration notes, and architecture documentation updates.

## Dependencies

`note-c` is intentionally self-contained and portable. It vendors the JSON implementation and avoids mandatory platform runtime dependencies. Adapter repositories may embed or wrap this repository, including `note-arduino`, `note-zephyr`, `note-espidf`, and POSIX-focused integrations.

Build configuration is part of the portability model. CMake detects platform `strlcpy`/`strlcat` support and only includes bundled `n_str.c` helpers when needed. Low-memory builds disable user-agent support and request CRC paths, omit `n_ua.c`, use compact error/log constants, and reduce allocation chunk size.

## Runtime Model

At runtime, host code initializes the relevant hooks, constructs Notecard requests, and calls `note-c` APIs. `note-c` serializes requests, sends bytes through the selected hook-backed transport, parses responses, and returns JSON objects or status to the caller.

Hook state is global to the SDK instance. Hook registration stores caller-owned function pointers, and `_noteSetActiveInterface` selects the active serial or I2C dispatch table. Mutex hooks are optional: many hook setters/getters and transport paths use the internal lock macros when available, but not every hook accessor is lock-protected.

Unit tests use mocks to validate behavior without hardware. Hardware validation may be performed through adapter libraries or Notestation-backed workflows when bus-level or device-level behavior matters.

## Testing Strategy

Use the existing unit test suite for core request, JSON, hook, and transport behavior. Add or update tests when a change affects public APIs, retry/timeout behavior, memory ownership, transport chunking, or edge cases that can regress across adapter repositories.

Before heavy Docker-based test runs in the OpenClaw workspace, follow the resource policy in the workspace `AGENTS.md` and run the resource check first.

## Update Triggers

Update this file or `docs/architecture/` when a change affects:

- Public APIs or hook signatures.
- Transport framing, chunking, timeout, retry, or request lifecycle behavior.
- Memory ownership or allocation strategy.
- JSON representation or helper semantics.
- Cross-repo expectations for adapter libraries.
- Build, test, CI, release, or versioning strategy.

When updating architecture, keep this Markdown overview, `docs/architecture/architecture.html`, and relevant `source-research/` artifacts consistent with the source.
