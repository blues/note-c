# Codecs, Binary, And Portability Worker Report

## Scope

- Worker: `019f1417-dee0-7eb1-9f23-3be579bbc0de`
- Report status: merged as source-backed inspected evidence
- Source policy: source-only; focused test/build evidence only
- Primary source inspected: `n_cobs.c`, `n_b64.c`, `n_md5.c`, `n_str.c`, relevant declarations in `note.h` and `n_lib.h`
- Supporting source inspected: binary helpers in `n_helpers.c`, request CRC paths in `n_request.c`, chunked dispatch in `n_i2c.c`, `n_serial.c`, `n_hooks.c`, and CMake inclusion logic in `CMakeLists.txt`

## Evidence Summary

- COBS internals are private declarations in `n_lib.h`. Public binary codec APIs are declared in `note.h` and implemented in `n_helpers.c`.
- Binary COBS uses local `NOTE_C_BINARY_EOP = '\n'`; `_cobsEncode` XORs code/data bytes with EOP so encoded bytes do not contain newline, and binary store transmit appends newline separately.
- `_cobsDecode` supports in-place decode, inserts restored zero bytes between non-`0xFF` blocks, stops if the decoded code byte is zero, clamps truncated input to remaining bytes, and returns a decoded length only. It has no explicit malformed-input error channel.
- `_cobsEncodedLength` excludes EOP. `_cobsEncodedMaxLength` includes EOP. `_cobsGuaranteedFit` computes the inverse fit for a buffer that includes EOP and returns 0 for buffers `<= 2`.
- Base64 helpers are local: `JB64EncodeLen` returns encoded capacity including NUL; `JB64Encode` writes NUL and returns count including NUL; `JB64DecodeLen` returns decoded capacity plus NUL; `JB64Decode` writes NUL and returns decoded byte count.
- JSON binary helpers use base64. `JAddBinaryToObject` allocates, encodes, then `JCreateStringValue` adopts the allocated string; normal `JDelete` frees non-reference `valuestring`. `JGetBinaryFromObject` allocates decoded data, NUL-terminates for convenience, and returns ownership to caller for `JFree`.
- MD5 is self-contained. `NoteMD5Init`, `NoteMD5Update`, `NoteMD5Final`, and `NoteMD5Transform` implement block MD5; one-shot and string wrappers call those. Binary receive/transmit compare or send lowercase hex MD5 strings through `NoteMD5HashString`.
- Payload sleep helpers base64-encode `NotePayloadDesc` data for `card.attn` payloads and decode after wake. Segment helpers store raw binary chunks as 4-byte type plus native `uint32_t` length plus bytes.

## Verified Searches

- Exact production search for `_cobsDecode`, `_cobsEncode`, `_cobsEncodedLength`, `_cobsEncodedMaxLength`, `_cobsGuaranteedFit`, `NoteBinaryCodec*`, and `NoteBinaryStore*` found declarations in `note.h`/`n_lib.h`, COBS definitions in `n_cobs.c`, and production callers in `n_helpers.c`.
- Exact production search for `JB64EncodeLen`, `JB64Encode`, `JB64DecodeLen`, `JB64Decode`, `JAddBinaryToObject`, and `JGetBinaryFromObject` found definitions in `n_b64.c`/`n_cjson_helpers.c`, public declarations in `note.h`, and callers in `n_helpers.c` sleep payload paths.
- Exact production search for `NoteMD5*` and `n_htoa8` found declarations in `note.h`, definitions/internal calls in `n_md5.c`, and production callers at `n_helpers.c:469` and `n_helpers.c:615`.
- Exact production search for `_ChunkedReceive` and `_ChunkedTransmit` found binary store callers in `n_helpers.c`, dispatch in `n_hooks.c`, and transport implementations in `n_i2c.c`/`n_serial.c`.

## Boundaries

- Binary store receive validates buffer, locks the Notecard, requests `card.binary.get`, receives newline-framed bytes, errors if extra data remains, strips newline, decodes in place, requires decoded length match, NUL-marks `buffer[decLen]`, then verifies MD5.
- Binary store transmit mutates the caller buffer: computes MD5 first, shifts unencoded bytes to the buffer end, encodes at the front, appends newline, sends `card.binary.put`, then chunk-transmits. On most failures it decodes back to restore caller data; on success it leaves the buffer modified.
- General JSON request CRC is separate from COBS binary framing. CRC is disabled under `NOTE_C_LOW_MEM`, added only to requests, validated/stripped before JSON parse, and causes retry. `{bad-bin}` is not retried in the general request path; binary transmit has its own 3-attempt validation loop.
- `NoteBinaryCodecDecode` size checking uses `_cobsGuaranteedFit(encDataLen)`, which is not a strict decoded-output upper bound. In-place decode with `decBufSize == encDataLen` is safe by COBS shrink behavior, but artifacts should avoid claiming this wrapper fully validates arbitrary small output buffers.

## Portability

- `n_str.c` is included only when `NOT HAVE_STRLCPY OR NOT HAVE_STRLCAT`; CMake publishes `HAVE_STRLCPY` and `HAVE_STRLCAT` compile definitions to targets.
- `note.h` declares only missing `strlcpy` or `strlcat`; `n_str.c` defines both, weak on non-MSVC and non-weak for `_MSC_VER`.
- Partial platform availability matters: if exactly one of `strlcpy` or `strlcat` exists, CMake still compiles `n_str.c`, which defines both helpers.
- External/system dependencies in this slice are standard C memory/string routines, CMake `CheckSymbolExists`, and platform hooks for allocation/free, time, locks, I2C, and serial transport. No external crypto, base64, or COBS library is used.

## Recommended Claims

- Public `NoteBinaryCodec*` APIs wrap private COBS helpers with newline-as-EOP XOR policy.
- `card.binary.*` helpers own Notecard handshakes, length checks, MD5 status verification, newline transport framing, and reset/error behavior.
- `strlcpy` and `strlcat` are conditional bundled portability helpers, with weak definitions except on MSVC and CMake-defined public guards.
- COBS decode reports length only and does not distinguish malformed/truncated input from valid short decode; wrapper buffer validation is strongest for documented in-place or encoded-buffer-sized usage.
