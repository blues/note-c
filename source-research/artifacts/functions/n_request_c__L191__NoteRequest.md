# `NoteRequest`

## Status

Finalized. Source-inspected public boolean request wrapper.

## Identity

- Kind: function
- Definition: `n_request.c:191`
- Declaration/interface: `note.h:442`
- Signature/API shape: `bool NoteRequest(J *req)`
- Visibility: public SDK API
- Public/exported names: `NoteRequest`
- Macro aliases/wrappers: none
- Ownership/lifetime role: consumes and deletes the request through `NoteRequestResponse`; deletes the response before returning

## Dependency Status

- Project-local dependencies:
  - `function:n_request.c:220:NoteRequestResponse` finalized
  - `function:n_cjson_helpers.c:152:JIsNullString` finalized
  - `function:n_cjson.c:209:JDelete` finalized
- External/system dependencies: none
- Data/type/global dependencies: response field constant `c_err`

## Behavior

Calls `NoteRequestResponse(req)`. If the response is `NULL`, returns false. Otherwise it computes success as `JIsNullString(rsp, c_err)`, deletes the response with `JDelete(rsp)`, and returns the boolean result.

Because `NoteRequestResponse` consumes a non-null request, this function always frees the request object for normal public use. Because this wrapper deletes the response before returning, callers cannot inspect response fields. For command requests, the underlying transaction path returns an empty object after successful send, so this wrapper reports true when no `"err"` field is present.

## Important Boundaries

- Preconditions and assumptions: caller passes either NULL or an owning request/command `J *`.
- Error/null/empty behavior: null request maps to false through `NoteRequestResponse`; null response maps to false; non-empty `"err"` maps to false; absent or empty `"err"` maps to true.
- Ownership and lifetime: consumes request ownership and response ownership; no returned object.
- Concurrency/global state: delegates transaction locking, transport, reset, and retry behavior to the request/transaction stack.
- Performance shape: one request/response transaction plus response deletion.
- Portability/platform constraints: all platform behavior stays behind the transaction and hook layers.

## Direct Call Sites

Exact source search found production callers in `n_helpers.c` bool-style helpers, including auxiliary serial flow control, printing, environment defaults, location mode/set/clear, sleep, factory reset, product/serial/upload/sync configuration, template/add/route helpers, and contact setting. The public header documents the command caveat for command-style requests.

## Role In Callers

Provides the standard fire-and-report-success API for helpers that only need to know whether a request or command completed without a response error. Callers build a request object, add fields, and transfer ownership to this function.

## Tests Observed

`test/src/NoteRequest_test.cpp` covers null request, null transaction response, response with `"err"`, and successful response. Numerous helper tests fake `NoteRequest` to verify caller-specific request construction and bool propagation.

## Graph Links

- Graph node id: `function:n_request.c:191:NoteRequest`
- Structured graph: `inventory/dependency-graph.json`

## Evidence Gaps

No unresolved source gaps for this node.
