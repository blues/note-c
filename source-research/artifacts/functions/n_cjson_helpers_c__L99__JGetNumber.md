# `JGetNumber`

## Status

Finalized. Source-inspected JSON object numeric field helper, dependency chain, direct production callers, and focused tests.

## Identity

- Kind: function
- Definition: `n_cjson_helpers.c:99`
- Declaration/interface: `note.h:1224`
- Public/exported names: `JGetNumber`
- Macro aliases/wrappers: none
- Signature/API shape: `JNUMBER JGetNumber(J *json, const char *field)`
- Visibility: public note-c JSON helper declared in `note.h`
- Ownership/lifetime role: borrowed read of a field in caller-owned JSON object

## Dependency Status

- Project-local dependencies:
- `function:n_cjson.c:1732:JGetObjectItem` finalized
- `function:n_cjson.c:2705:JIsNumber` finalized
- `function:n_cjson_helpers.c:91:JNumberValue` finalized in this slice
- External/system dependencies: none
- Data/type/global dependencies: reads JSON object child list and number value
- Artifact coverage: implementation, public declaration, dependencies, direct callers, and focused tests inspected

## Behavior

Returns `0.0` when `json` is null, when `field` is missing, or when the field exists but is not a JSON number. Otherwise, it returns `JNumberValue(item)` for the located field.

## Important Boundaries

- Preconditions and assumptions: `json` should be a JSON object for object-field lookup semantics; `field` is forwarded to `JGetObjectItem`, whose lower lookup returns null for null names.
- Error/null/empty behavior: null object, missing field, null field name, and non-number field all return `0.0`, making missing and zero-valued numeric fields indistinguishable from the return value alone.
- Ownership and lifetime: returns a numeric value only; it borrows the located child and does not allocate or transfer ownership.
- Concurrency/global state: no global state.
- Portability/platform constraints: numeric precision follows the configured `JNUMBER` type and parser behavior.
- Performance shape: O(number of object children * key length) for lookup, then O(1).
- Security or parsing constraints: type-checks with `JIsNumber` before reading `valuenumber`.

## Direct Call Sites

Exact source search found direct production callers:

- `n_helpers.c:1439` and `n_helpers.c:1442` `NoteGetLocation` reads `"lat"` and `"lon"` from a `card.location` response after presence checks.
- `n_helpers.c:2182` `NoteGetVoltage` reads response `"value"` from `card.voltage`.
- `n_helpers.c:2204` `NoteGetTemperature` reads response `"value"` from `card.temp`.

## Role In Callers

Provides the shared object-field number extraction layer for location coordinates and numeric card telemetry helpers.

## Tests Observed

Focused `test/src/JGetNumber_test.cpp` covers null object, missing field, non-number field, and valid number. `test/src/JSON_number_handling_test.cpp` covers parsed numeric boundaries and floating-point values through `JGetNumber`.

## Graph Links

- Graph node id: `function:n_cjson_helpers.c:99:JGetNumber`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `JGetObjectItem`, `JIsNumber`, and `JNumberValue`.

## Evidence Gaps

No unresolved source gaps for this node. Direct callers remain separate helper artifacts.
