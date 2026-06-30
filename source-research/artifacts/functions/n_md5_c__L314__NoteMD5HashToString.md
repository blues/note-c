# `NoteMD5HashToString`

## Status

Finalized. Source-inspected public MD5 digest-to-hex helper.

## Identity

- Kind: function
- Definition: `n_md5.c:314`
- Declaration/interface: `note.h:1583`
- Public/exported names: `NoteMD5HashToString`
- Macro aliases/wrappers: none found
- Signature/API shape: `void NoteMD5HashToString(unsigned char *hash, char *strbuf, unsigned long buflen)`
- Visibility: public note-c helper implemented in `n_md5.c`
- Ownership/lifetime role: reads caller-provided 16-byte digest and writes a caller-provided string buffer

## Dependency Status

- Project-local dependencies:
  - `function:n_md5.c:57:n_htoa8` finalized
  - `function:n_str.c:31:strlcpy` finalized
- External/system dependencies: platform `strlcpy` may be used when available; repository provides fallback when needed
- Data/type/global dependencies: constants `NOTE_MD5_HASH_SIZE` and `NOTE_MD5_HASH_STRING_SIZE`
- Artifact coverage: source-inspected artifact

## Behavior

Initializes a stack `hashstr` buffer, loops over exactly `NOTE_MD5_HASH_SIZE` digest bytes, converts each byte to two lowercase hex characters with `n_htoa8`, then copies the generated hex string into `strbuf` using `strlcpy(strbuf, hashstr, buflen)`.

## Important Boundaries

- Preconditions and assumptions: `hash` must point to at least 16 readable bytes; `strbuf` must be valid for `buflen` bytes.
- Error/null/empty behavior: no null checks and no error return. `strlcpy` truncation is silent because the return value is ignored.
- Ownership and lifetime: caller owns both input digest and output buffer.
- Concurrency/global state: no global state.
- Portability/platform constraints: fallback/system `strlcpy` selection is build-dependent.
- Performance shape: fixed 16-byte conversion plus bounded string copy.
- Security or parsing constraints: callers need at least `NOTE_MD5_HASH_STRING_SIZE` bytes for a full hex string plus terminator.

## Direct Call Sites

Production-only exact search found only the public declaration in `note.h:1583` and the definition in `n_md5.c:314`. HITL support calls it from `test/hitl/card.binary/lib/notecard_binary/NotecardBinary.h`. No production repository caller, wrapper, or fake was found.

## Role In Callers

No production repository caller role. Public helper for downstream callers that already have a binary MD5 digest and need its hex string representation.

## Tests Observed

No focused unit test for this helper was found. HITL support uses it for expected MD5 string comparison/emission, but not as standalone conversion coverage. Gaps include truncation, null handling, and direct expected-string conversion tests.

## Graph Links

- Graph node id: `function:n_md5.c:314:NoteMD5HashToString`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `n_htoa8` and `strlcpy`.

## Evidence Gaps

No unresolved source gaps for this node.
