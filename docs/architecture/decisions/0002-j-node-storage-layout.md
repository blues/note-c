# 0002: Offer a Memory-Optimized `J` Storage Layout as an Opt-In Compile-Time Variant

## Status

Proposed

## Context

`J` is note-c's JSON node and the SDK's dominant consumer of heap. On a 32-bit target the node is 48 bytes, and an object member such as `"key":"value"` costs three separate heap allocations: the node, the key, and the value string. Once per-chunk allocator overhead is included, a parsed document occupies roughly 8–9× the size of its JSON text.

That ratio is a real constraint on Notecard hosts with tens of kilobytes of RAM. Measurements across two production applications showed the same pattern at scale: one held 592 allocations and 17.6 KB for its working set, another 2,898 allocations and 91.6 KB.

Two properties of the existing node make a substantial reduction available:

- `JINTEGER` and `JNUMBER` require 8-byte alignment even where pointers are 4 bytes, so the historical member order wastes 8 bytes per node to padding.
- A `JString` or `JRaw` node never uses `valueint` or `valuenumber`, leaving 16 bytes per string node inert.

The obstacle is not feasibility but compatibility. `J` is a public type whose members downstream code reads directly, across several adapter repositories (`note-arduino`, `note-zephyr`, `note-espidf`, POSIX integrations). Every consumer is rebuilt from source, so the risk is not binary mismatch but source-visible behavior change: a recompiled consumer that reads `valueint` on a string node, or relies on the size or offsets of `J`, sees something different.

## Decision

Implement the optimized representation, and gate it behind `NOTE_C_STORAGE_OPTIMIZATION`, **off by default**.

The optimized layout reorders members to eliminate padding (48 → 40 bytes) and reuses the bytes of `valueint`/`valuenumber` in a string node to hold the value and then the key inline, extending the node's allocation only as far as the content requires. An added `objlen` member records the node's allocation size; content that would exceed `J_MAX_PACKED_ALLOC` falls back to heap storage, which keeps `objlen` representable. Ownership gains two flags, `JValueInline` and `JKeyInline`, that record where a pointer points rather than inferring it later by comparing pointers — a comparison that is undefined in C when the operands address different objects.

The default stays historical for this release cycle because of a planned external beta of that layout, and because the optimized layout is source-visibly different: it changes `sizeof(J)`, moves nearly every member offset, narrows `type` from `int` to `uint16_t`, adds a member, and — most consequentially — makes `valueint` and `valuenumber` hold character data on string nodes where they historically held zero. Any recompiled consumer that touches those fields directly can observe the difference.

Promoting the optimization to the default is scoped as a separate decision, deferred by the maintainer until after a planned external beta test of the historical layout. That beta needs to exercise the representation already in the field, so the optimization stays opt-in for this release cycle regardless of its technical readiness. The later decision rests on the source-visible field behavior described above, whatever the beta surfaces, and validation against the adapter repositories.

## Consequences

Applications that are memory-constrained can opt in and recover roughly 38% of allocations and 26% of heap, at the cost of rebuilding every consumer with the flag set. Applications that are not constrained are unaffected and unexposed.

Two layouts must both be maintained and tested. CI covers `{default, optimized} × {double, single precision}`, plus an A/B job that proves both render byte-identical JSON. The layout-conditional code is deliberately confined to a small set of storage helpers in `n_cjson.c` so the parser, printer and public API remain layout-agnostic; that containment is a maintenance requirement, not an incidental property.

Under memory pressure the optimized setters abandon a write rather than overwrite packed key and value bytes in place, since the latter produced a silently altered document that still parsed. Losing the update instead is consistent with the surrounding API: `JAddItemToObject`, `JAddItemToObjectCS` and `JAddItemToArray` all return `void` and have always dropped an item silently when an allocation fails, leaving the caller's pointer dangling into freed storage. Making allocation failure detectable is a question about that whole API surface, not about this layout.

Because both layouts share one rewritten parser and printer, a cross-layout comparison cannot detect a regression common to both. Rendering is therefore also compared against goldens generated from a pristine build of the base revision (`scripts/generate_corpus_golden.sh`). That check found two pre-existing printer defects that the A/B could not see, including one that emitted invalid JSON for strings containing more than one escaped control character.

Every translation unit that sees `J` must be built with the same setting. CMake's `PUBLIC` propagation enforces this; other build systems must set the option for both the note-c sources and the consuming sources. This is a single-build consistency requirement, since note-c is always compiled from source as part of the consuming solution.

Two public macros, `JSetIntValue` and `JSetNumberValue`, become function calls, because as macros they wrote node bytes directly from the caller's translation unit — bytes the optimized layout may be using for packed content. Their observable behavior is preserved and pinned against a pristine build: the NULL branch still yields the unconverted operand, the value operand is still evaluated exactly once, and a fractional operand still truncates into `valueint` while surviving in `valuenumber`. Two divergences are deliberate — out-of-range conversion saturates instead of being undefined, and `JSetIntValue` evaluates a side-effecting `object` expression twice rather than three times.
