# `NoteMD5HashString`

## Status

Finalized. Source-inspected MD5 hex-string wrapper used by binary-store helpers.

## Identity

- Kind: function
- Definition: `n_md5.c:301`
- Declaration/interface: `note.h:1575`
- Signature/API shape: `void NoteMD5HashString(unsigned char *data, unsigned long len, char *strbuf, unsigned long buflen)`
- Visibility: public header declaration, implemented in `n_md5.c`
- Public/exported names: `NoteMD5HashString`
- Macro aliases/wrappers: none

## Dependency Status

- Project-local dependencies:
  - `function:n_md5.c:292:NoteMD5Hash` finalized
  - `function:n_md5.c:57:n_htoa8` finalized
  - `function:n_str.c:31:strlcpy` finalized as repository fallback/platform boundary
- External/system dependencies: none directly
- Data/type/global dependencies: stack `hash[16]` and `hashstr[48]`; writes caller-provided string buffer
- Artifact coverage: implementation, declaration, callee chain, direct callers, focused binary-store test evidence, and graph node inspected

## Behavior

Computes the binary MD5 digest for `data`/`len` with `NoteMD5Hash`, then converts each of the 16 digest bytes into two lowercase hex characters using `n_htoa8(&hashstr[i * 2])`. The conversion helper writes an intermediate NUL after each byte pair, but the next loop iteration overwrites that NUL with the next hex byte pair. The final pair leaves a NUL at index 32, and the explicit assignment at index 33 adds an extra NUL. The wrapper copies the generated hex string to `strbuf` with `strlcpy(strbuf, hashstr, buflen)`.

## Important Boundaries

- Preconditions and assumptions: caller must pass a writable output string buffer; when `len > 0`, `data` must be readable; lower MD5/hash helpers do not null-check inputs.
- Error/null/empty behavior: no error return. If `buflen` is too small, `strlcpy` truncates while preserving NUL termination when `buflen > 0`.
- Ownership and lifetime: borrows input data, writes caller output buffer, uses only stack scratch buffers, allocates nothing.
- Concurrency/global state: no global state.
- Portability/platform constraints: uses repository `strlcpy` fallback only on platforms that need it; otherwise platform `strlcpy` supplies the copy operation.
- Performance shape: O(len) for hashing plus fixed 16-byte hex conversion.

## Direct Call Sites

Exact source search `rg -n "\bNoteMD5Hash\b|\bNoteMD5HashString\b|\bNoteMD5HashToString\b" -g '!source-research/**' -g '!docs/**'` found:

- `note.h:1575` declaration.
- `n_md5.c:301` definition.
- `n_helpers.c:469` called by `NoteBinaryStoreReceive` to compare the decoded payload with the Notecard `"status"` hash.
- `n_helpers.c:615` called by `NoteBinaryStoreTransmit` to send a hash status with `"card.binary.put"`.

## Role In Callers

`NoteBinaryStoreReceive` uses the string to validate received binary payload integrity against the response `"status"` field. `NoteBinaryStoreTransmit` uses it as the `"status"` field when initializing a binary put transaction.

## Tests Observed

Focused binary-store tests were inspected:

- `test/src/NoteBinaryStoreReceive_test.cpp:66`-`test/src/NoteBinaryStoreReceive_test.cpp:69` uses `NoteMD5HashString` to build the fake `"status"` response.
- `test/src/NoteBinaryStoreReceive_test.cpp:177`-`test/src/NoteBinaryStoreReceive_test.cpp:190` asserts an error when the computed MD5 hash does not match `"status"`.
- `test/src/NoteBinaryStoreReceive_test.cpp:214`-`test/src/NoteBinaryStoreReceive_test.cpp:222` asserts success when the computed MD5 matches.

## Graph Links

- Graph node id: `function:n_md5.c:301:NoteMD5HashString`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `NoteMD5Hash`, `n_htoa8`, and `strlcpy`.
- Verified incoming edges from `NoteBinaryStoreReceive` and `NoteBinaryStoreTransmit`.

## Evidence Gaps

No unresolved source gaps for this node.
