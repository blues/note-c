# JSON Core Worker Report

## Scope

- Worker: `019f1417-c7a1-7a30-8fb0-a2d49cf83c83`
- Report status: merged as source-backed inspected evidence
- Source policy: source-only; no tests, generated summaries, or artifacts
- Primary source inspected: `n_cjson.h:43-326`, `n_cjson.c:91-2851`, `n_cjson_helpers.c:19-467`, `n_atof.c:70-293`, `n_ftoa.c:52-510`, `note.h`, allocator/build support in `n_lib.h`, `n_hooks.c`, and `CMakeLists.txt`

## Key Symbols

- `J`: linked JSON node with `next`, `prev`, `child`, type flags, `valuestring`, `valueint`, `valuenumber`, and object-key `string`.
- `_jNew_Item`, `JDelete`, `JMalloc`, `JFree`: allocate/free via `_Malloc`/`_Free`, which map to `NoteMalloc`/`NoteFree`; `NoteSetFn` and `NoteSetFnDefault` install allocator hooks.
- `JInitHooks`: declared in `n_cjson.h:183`, but exact production search found no definition or use.
- `JParseWithOpts`, `JParse`, `JGetErrorPtr`: parse root value, optional null-termination enforcement, global parse-error pointer.
- `JPrint`, `JPrintUnformatted`, `JPrintUnformattedOmitEmpty`, `JPrintBuffered`, `JPrintPreallocated*`: render trees to allocator-owned or caller-owned buffers.
- `JAddItemToArray/Object`, `JAdd*ToObject`, `JDetach*`, `JDeleteItem*`, `JReplace*`: tree ownership/mutation APIs.
- `JGetString`, `JGetNumber`, `JGetInt`, `JGetBool`, `JGetObject`, `JGetArray`, `JGetItemType`, `JBaseItemType`: typed helper layer.
- `JAtoN`, `JNtoA`, `JItoA`, `JAtoI`: custom numeric conversion helpers.

## Verified Searches

- `JAtoN` is called by `_parse_number` in `n_cjson.c`, helper string classification in `n_cjson_helpers.c`, and env helpers in `n_helpers.c`. `JNtoA` and `JItoA` are used by `_print_number` in `n_cjson.c`.
- `JParse` and `JPrintUnformatted` production callers are in `n_request.c` for request/response JSON parsing and request serialization.
- `JAddItemToObject` and `JAdd*ToObject` have high fanout in `n_helpers.c`, `n_request.c`, and `n_ua.c`; direct ownership-transfer comments appear near `n_helpers.c:2094`.
- Low-level `JGetObjectItem` is mostly wrapped by `n_cjson_helpers.c`; production code generally uses typed helpers.
- `JDetachItemFromObject` is used in `n_helpers.c:2146`; `JDelete` is used widely in `n_helpers.c` and `n_request.c`.
- `JInitHooks` exact search found the header declaration only.

## Ownership And Lifetime

- `JParse*` returns a tree owned by the caller; free with `JDelete`.
- `JPrint*` allocating variants return `_Malloc` memory; free with `JFree` or `NoteFree`. `JPrintPreallocated*` writes into caller-owned buffers.
- `JAddItemToArray/Object` transfers ownership to the parent on success and deletes the item on invalid input/failure.
- `JDetach*` unlinks and returns a node with `next` and `prev` cleared; caller owns it. Delete variants detach then `JDelete`.
- `JDelete` recursively deletes children unless `JIsReference`; frees `valuestring` unless reference; frees object key `string` unless `JStringIsConst`.
- `JCreateString` copies input. `JCreateStringReference` does not own/free `valuestring`. `JCreateStringValue` stores the pointer without copying and without `JIsReference`, so `JDelete` frees it; `JAddBinaryToObject` uses this to transfer an `_Malloc` buffer into the tree.

## Errors And Bounds

- `JParse` does not require trailing null termination. `JParseWithOpts(..., require_null_terminated=true)` rejects trailing garbage.
- `N_CJSON_NESTING_LIMIT` defaults to 100; arrays/objects fail beyond it.
- String parse rejects invalid escapes, malformed UTF-16 surrogate pairs, and unexpected end; failures clean partial child lists.
- Number parse uses a 64-byte temporary token, `JAtoN`, then saturates `valueint` to `JINTEGER_MIN/MAX`; very long numeric tokens can leave unconsumed input unless the caller enforces full consumption.
- `JAtoN` keeps at most 18 mantissa digits and caps decimal exponent processing at `MAX_EXPONENT` 511.
- `JNtoA` default precision is `JNTOA_PRECISION` 16 with `JNTOA_MAX` 44; internal precision is clamped by `uintmax_t` width. Core JSON printing emits `null` for NaN/Inf.
- Helper field getters return defaults on missing/wrong type: empty string, `0`, `false`, or `NULL`. Some item-level helpers do not type-check (`JStringValue`, `JNumberValue`, `JIntValue`).

## Recommended Claims

- Record JSON as a cJSON-derived linked-node core exposed through `n_cjson.h` and included by `note.h`.
- Record allocator reality as `NoteMalloc`/`NoteFree` hook path; mark `JInitHooks`/`JHooks` as declared but not implemented in production source.
- Record ownership transfer rules for add/detach/delete/replace and the `JCreateStringValue` implementation nuance.
- Record parse/print boundaries: prefix parsing by default, optional full-consumption mode, nesting limit, global parse-error state, custom number conversion limits.
- Record helper layer as borrowed-access/default-value wrappers over core `J` APIs, with binary helpers allocating/freeing through the JSON allocator.
