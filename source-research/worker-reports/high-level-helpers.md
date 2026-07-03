# High-Level Helper APIs Worker Report

## Scope

- Worker: `019f1417-f92e-7322-b4f1-06db54f24019`
- Report status: merged as source-backed inspected evidence
- Source policy: source-only; no tests or source-research summaries
- Primary source inspected: `n_helpers.c:1-2592`, `n_ua.c:1-223`, `n_request.c`, `n_hooks.c`, `note.h`, `n_lib.h`, and selected JSON source used by helpers.

## Helper Families

- Request primitives: `NoteNewRequest` builds `{ "req": ... }`; `NoteNewCommand` builds `{ "cmd": ... }`; `NoteRequestResponse*` free the request; `NoteTransaction` does not. `NoteResponseError` and `NoteDeleteResponse` are macros over `JIsNullString` and `JDelete`.
- Product/service/upload/sync: `NoteSetProductID`, `NoteSetSerialNumber`, `NoteSetUploadMode`, and `NoteSetSyncMode` build `hub.set`; `NoteGetServiceConfig*` reads `hub.get` with a 4-hour cache for the `ST` form.
- Env: `NoteSetEnvDefault*` builds `env.default`; `NoteGetEnv*` builds `env.get`, initializes output buffer to the default first, then copies `"text"` if present. `NoteGetEnvInt` parses with `atoi`.
- Time/region: `NoteTimeST` polls `card.time` behind suppression and refresh timers; `NoteTime` and `NoteTimeValid` reset the suppression timer. `NoteTimeSet(0, ...)` returns to Notecard time; nonzero manual time disables automatic fetch.
- Location/status: wrappers for `card.location`, `card.location.mode`, `hub.status`, `card.status`, `card.version`, `card.voltage`, and `card.temp` mostly default outputs to empty or zero and return `false` on null/error response. `NoteGetLocation` defines success by presence of `lat` and `lon`, not only by `NoteResponseError`.
- Contact: `NoteGetContact` reads `card.contact`; `NoteSetContact` writes only non-null fields to `card.contact`.
- Note/template/route: `NoteTemplate` builds `note.template`; `NoteAdd` builds `note.add`; `NoteSend` is only `#define NoteSend NoteAdd`; `NoteSendToRoute` first converts a body via `note.event`, detaches response `"body"`, then sends `web.<method>`.
- Binary store: codec helpers wrap COBS encode/decode. Store helpers use `card.binary`, `card.binary.get`, `card.binary.put`, raw `_ChunkedReceive`/`_ChunkedTransmit`, MD5 status verification, and explicit Notecard locking.
- Payload/sleep: `NotePayloadSaveAndSleep` base64-encodes descriptor data then calls `NoteSleep`; `NoteSleep` sends `cmd:"card.attn"` with a string-reference payload; wake/retrieve uses `req:"card.attn"` with `start:true`.

## Caller And Callee Evidence

- Source-only `rg` over production C/header files found inspected helper symbols only in `note.h`, `n_helpers.c`, internal helper-to-helper calls, and one user-agent call in `n_request.c`.
- `NoteSend` exact search found only `note.h:2033` aliasing `NoteAdd` and `n_helpers.c:2083` defining `NoteAdd`; no production caller.
- `NoteUserAgent` exact search found only declaration/definition plus `n_request.c:481`.
- `JPrintUnformatted(req)` exact search found `n_request.c:437` before user-agent body insertion at `n_request.c:481-483`, with no later reserialization of that request.

## Ownership And Error Behavior

- `JAddItemToObject` consumes the child on success and deletes it on failure; helpers use this for `body` ownership.
- `JDetachItemFromObject` transfers ownership out of the parent.
- `JGetString` returns a borrowed pointer or empty string; `JGetObject` returns a borrowed child; `JGetNumber`, `JGetInt`, and `JGetBool` default to `0` or `false`.
- Most helpers ignore `JAdd*` return values, so allocation failure while adding fields can produce partial requests rather than immediate local failure.
- `NotePayloadSet` does not copy the buffer, but `NotePayloadFree` always frees `desc->data`; model the descriptor as owning the pointer after set/retrieve/add.
- `NotePayloadSaveAndSleep` dereferences `desc` before null checking, despite nearby comments describing payload as optional.
- Binary helpers return `const char *` error-or-NULL; most other helpers return `bool`.

## Conditionals

- `NOTE_C_LOW_MEM` is auto-defined when float/double exponent sizes match, can be enabled by CMake, and is also enabled by legacy `NOTE_LOWMEM`.
- Under low memory, `n_lib.h` defines `NOTE_DISABLE_USER_AGENT`; CMake omits `n_ua.c` from the target. Low memory also disables request CRC paths and malloc debug logging.
- `n_ua.c` weakly defines `NoteUserAgent` and `NoteUserAgentUpdate` outside MSVC and outside `NOTE_C_LOW_MEM`; setters retain caller-provided string pointers, not copies.
- Current source attempts user-agent insertion for `hub.set` with `product` and no `body`, but insertion happens after request serialization. Evidence does not support a claim that the UA body reaches the wire in this path.

## Recommended Claims

- Record `n_helpers.c` as the high-level request-wrapper layer over `NoteNewRequest`, `NoteRequest`, and `NoteRequestResponse`.
- Record request/response/body ownership exactly, including `NoteTransaction` not freeing requests and `NoteRequestResponse*` freeing them.
- Mark user-agent behavior carefully as attempted post-serialization insertion unless source changes or contrary source evidence is found.
- Record low-memory boundaries: UA disabled/omitted, CRC disabled, compact `ERRSTR`, and smaller allocation chunk.
- Record static cache/suppression behavior for time, location-validity, connectivity, service config, and status helpers.
- Record binary store as a mixed JSON-plus-raw-transfer subsystem with locking, COBS, MD5, offset alignment, and reset-on-integrity-error behavior.
