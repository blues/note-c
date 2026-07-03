# Source-Derived Overview

Status: complete source-first create pass. Generated at 2026-06-29T22:42:19+00:00.

This repository is a portable C SDK centered on `note.h` as the public interface, `n_hooks.c` as the platform hook and active-interface dispatch layer, `n_request.c` as the JSON request transaction orchestrator, `n_i2c.c` and `n_serial.c` as transport implementations, `n_cjson.c`/`n_cjson_helpers.c` as the embedded JSON layer, `n_helpers.c` as high-level Notecard helper APIs, and codec/utility files for COBS, MD5, numeric conversion, and portability helpers.

The finalized research artifacts cover 585 source nodes: public macros and typedefs, internal helpers, transport functions, JSON core routines, high-level helpers, hook registration and dispatch, binary/COBS/MD5 behavior, compile-time configuration branches, and focused test evidence where available. Machine-readable source maps live under `inventory/`; per-node source artifacts live under `artifacts/`; the finalized claims are in `inventory/claims-ledger.json`.

Key source-backed conclusions:

- Platform behavior is behind hook typedefs and hook registry state in `n_hooks.c`; active serial/I2C dispatch is selected by `_noteSetActiveInterface`.
- Request object APIs flow through `NoteRequest*`, `NoteTransaction`, and `_noteTransactionShouldLock`; raw JSON string requests use the separate `NoteRequestResponseJSON` path.
- I2C and serial transports move newline-framed bytes; binary payload helpers own COBS framing and MD5 verification.
- JSON behavior is cJSON-derived, with `J` linked nodes, `parse_buffer`/`printbuffer` internals, custom numeric conversion, allocation through SDK hooks, and explicit ownership transfer rules for add/detach/delete APIs.
- Low-memory configuration disables user-agent and CRC request paths, changes ERRSTR/log behavior, and reduces allocation chunk size.
- Public header constants and typedefs are finalized, including I2C compatibility macro lattices, payload segment layout, template type codes, numeric bounds, log levels, and hook callback signatures.

Use `source-research/research-index.md` as the re-entry point, `inventory/dependency-graph.json` for graph queries, and individual artifacts for source locations and evidence details.
