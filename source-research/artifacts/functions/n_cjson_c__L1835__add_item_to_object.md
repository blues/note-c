# `_add_item_to_object`

## Status

Finalized. Source-inspected JSON object-field attachment helper.

## Identity

- Kind: function
- Definition: `n_cjson.c:1835`
- Declaration/interface: file-local `NOTE_C_STATIC`
- Signature/API shape: `Jbool _add_item_to_object(J * const object, const char * const string, J * const item, const Jbool constant_key)`
- Visibility: internal to `n_cjson.c` unless `NOTE_C_TEST` exposes statics
- Public/exported names: none

## Dependency Status

- Project-local dependencies:
  - `function:n_cjson.c:1826:_cast_away_const` finalized
  - `function:n_cjson.c:146:_j_strdup` finalized
  - `macro:n_lib.h:242:_Free` finalized
  - `function:n_cjson.c:1783:_add_item_to_array` finalized
- External/system dependencies: none directly
- Data/type/global dependencies: mutates `item->string`, `item->type`, and the object's child list

## Behavior

Returns `false` if `object`, `string`, or `item` is `NULL`. For constant keys, it casts the key pointer with `_cast_away_const` and marks the item type with `JStringIsConst`. For normal keys, it duplicates the key with `_j_strdup`; allocation failure returns `false`. If the item already owns a non-constant key string, it frees that string through `_Free`. It then installs the new key and type flags on `item` and delegates insertion to `_add_item_to_array(object, item)`.

## Important Boundaries

- Preconditions and assumptions: caller supplies a valid object-like `J` parent and an item intended to be adopted by that parent.
- Error/null/empty behavior: null inputs or failed key duplication return `false` without freeing `item`.
- Ownership and lifetime: on success, `item` becomes part of the parent child list. For non-constant keys, the helper owns a duplicated key string. Existing non-constant `item->string` is released before replacement.
- Concurrency/global state: allocation/free uses SDK hook globals through `_j_strdup` and `_Free`.
- Portability/platform constraints: const-key paths are represented by `JStringIsConst` so `JDelete` will not free borrowed key strings.
- Performance shape: O(strlen(key) + number of existing children).

## Direct Call Sites

Exact source search found callers in `n_cjson.c`:

- Public item add/reference wrappers: `JAddItemToObject`, `JAddItemToObjectCS`, `JAddItemReferenceToObject`.
- Typed object add wrappers: `JAddTrueToObject`, `JAddFalseToObject`, `JAddBoolToObject`, `JAddNumberToObject`, `JAddIntToObject`, `JAddStringToObject`, `JAddRawToObject`, `JAddObjectToObject`, `JAddArrayToObject`.

## Role In Callers

In the active receive path, `JAddIntToObject` uses this helper to attach `"offset"` and `"length"` numeric children to the `card.binary.get` request object. The helper transfers the new number item into the request object on success.

## Tests Observed

None. No focused direct test was inspected for this helper.

## Graph Links

- Graph node id: `function:n_cjson.c:1835:_add_item_to_object`
- Structured graph: `inventory/dependency-graph.json`
- Verified outgoing edges to `_cast_away_const`, `_j_strdup`, `_Free`, and `_add_item_to_array`.

## Evidence Gaps

No unresolved source gaps for this node.
