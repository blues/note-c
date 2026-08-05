/*!
 * @file JLayout_test.cpp
 *
 * Validates the packed `J` layout (NOTE_C_STORAGE_OPTIMIZATION) against the
 * historical one, which is the default. Every assertion here holds in BOTH
 * builds: the layout difference must be invisible to every observable behavior.
 * See test/include/j_layout_test_support.hpp for how the A/B comparison is
 * wired, and docs/architecture/decisions/0002-j-node-storage-layout.md for
 * the specification.
 *
 * Written by the Blues Inc. team.
 *
 * Copyright (c) 2025 Blues Inc. MIT License. Use of this source code is
 * governed by licenses granted by the copyright holder including that found in
 * the
 * <a href="https://github.com/blues/note-c/blob/master/LICENSE">LICENSE</a>
 * file.
 *
 */

#include <catch2/catch_test_macros.hpp>

#include <cstddef>   // offsetof, for the layout-derived size expectations

#include "j_layout_test_support.hpp"

using namespace jlayout;

namespace
{

// Deep structural comparison that does not rely on printing, so a print bug
// cannot mask a tree bug. Compares type, key, value, and child/sibling shape.
bool sameTree(const J *a, const J *b)
{
    while (true) {
        if ((a == NULL) != (b == NULL)) {
            return false;
        }
        if (a == NULL) {
            return true;
        }
        if ((a->type & 0xFF) != (b->type & 0xFF)) {
            return false;
        }
        if ((a->string == NULL) != (b->string == NULL)) {
            return false;
        }
        if (a->string && strcmp(a->string, b->string) != 0) {
            return false;
        }
        if ((a->valuestring == NULL) != (b->valuestring == NULL)) {
            return false;
        }
        if (a->valuestring && strcmp(a->valuestring, b->valuestring) != 0) {
            return false;
        }
        if ((a->type & 0xFF) == JNumber) {
            if (a->valueint != b->valueint) {
                return false;
            }
            if (a->valuenumber != b->valuenumber) {
                return false;
            }
        }
        if (!sameTree(a->child, b->child)) {
            return false;
        }
        a = a->next;
        b = b->next;
    }
}

SCENARIO("J layout: every corpus document round-trips and its cost is recorded")
{
    installTrackedAllocator();

    uint64_t digest = fnv1aSeed();
    size_t totalAllocs = 0;
    size_t totalBytes = 0;
    size_t totalNodes = 0;

    for (const Doc &doc : corpus()) {
        for (const Mode &mode : modes()) {
            // Measure the cost of holding this document in memory: parse with a
            // clean counter, then read the live totals before tearing down.
            resetStats();
            const size_t liveBytesBefore = stats().liveBytes;
            const size_t liveBlocksBefore = stats().liveBlocks;

            J *parsed = JParse(doc.json);
            REQUIRE(parsed != NULL);

            const size_t heldBytes = stats().liveBytes - liveBytesBefore;
            const size_t heldBlocks = stats().liveBlocks - liveBlocksBefore;
            const size_t nodes = countNodes(parsed);
            std::vector<const void *> nodePtrs;
            collectNodes(parsed, nodePtrs);
            const ChunkTotals chunks = chunkTotals(nodePtrs);

            char *printed = (char *)_print(parsed, mode.format, mode.omitempty);
            REQUIRE(printed != NULL);

            // 0. The output must match what the SHIPPED implementation
            //    rendered. Everything else in this suite compares the rewrite
            //    against itself -- across layouts, or against a digest computed
            //    from its own output -- and so cannot see a regression that
            //    both layouts share. This row comes from a pristine
            //    `git archive` build recorded by
            //    scripts/generate_corpus_golden.sh.
            {
                const std::string *golden = goldenFor(doc.name, mode.name);
                INFO("golden lookup for " << doc.name << "/" << mode.name);
                REQUIRE(golden != NULL);   // a missing golden is a failure,
                // never a silently skipped check

                const GoldenException *ex = goldenExceptionFor(doc.name, mode.name);
                if (ex == NULL) {
                    CHECK(*golden == printed);
                } else {
                    INFO("deliberate deviation: " << ex->why);
                    // Still different, so reverting the fix fails here rather
                    // than silently re-matching the historical output.
                    CHECK(*golden != printed);
                    if (ex->historicalOutputIsInvalidJson) {
                        // The strongest form of the claim: what shipped could
                        // not be reparsed, and what we emit now can.
                        J *historical = JParse(golden->c_str());
                        CHECK(historical == NULL);
                        JDelete(historical);
                    }
                }
            }

            // 1. The output must be valid JSON.
            J *reparsed = JParse(printed);
            REQUIRE(reparsed != NULL);

            // 2. Printing the reparsed tree must reproduce the text exactly.
            //    This catches damage that would still parse.
            //
            //    Skipped only for documents whose numbers cannot survive a
            //    text->float->text round trip in a NOTE_C_SINGLE_PRECISION
            //    build. That is pre-existing numeric-conversion behavior,
            //    identical in both layouts.
            //
            //    formatted+omitempty IS checked here. It used not to be a fixed
            //    point, because _print_object emitted a member's indentation
            //    before deciding whether omitempty elided it; that is fixed.
            char *reprinted = (char *)_print(reparsed, mode.format, mode.omitempty);
            REQUIRE(reprinted != NULL);
            const bool lossy = numbersAreLossy(doc);
            if (!lossy) {
                CHECK(std::string(reprinted) == printed);
            }

            // 3. For the non-omitempty modes the reparsed tree must be
            //    structurally identical to the original. (omitempty renders a
            //    strict subset, so the trees legitimately differ there.)
            if (!mode.omitempty && !lossy) {
                CHECK(sameTree(parsed, reparsed));
            }

            // 4. JDuplicate must produce an independent, equal tree. This is the
            //    path where a shared inline key would dangle.
            J *dup = JDuplicate(parsed, true);
            REQUIRE(dup != NULL);
            CHECK(sameTree(parsed, dup));
            char *dupPrinted = (char *)_print(dup, mode.format, mode.omitempty);
            REQUIRE(dupPrinted != NULL);
            CHECK(std::string(dupPrinted) == printed);
            // Free the original FIRST, then use the duplicate: if the duplicate
            // had borrowed storage from the original, this reads freed memory
            // and the sanitizer build catches it.
            JDelete(parsed);
            parsed = NULL;
            CHECK(std::string(dupPrinted) == printed);
            char *dupAgain = (char *)_print(dup, mode.format, mode.omitempty);
            REQUIRE(dupAgain != NULL);
            CHECK(std::string(dupAgain) == printed);

            if (!lossy) {
                digest = fnv1a(digest, doc.name);
                digest = fnv1a(digest, mode.name);
                digest = fnv1a(digest, printed);
            }

            totalAllocs += heldBlocks;
            totalBytes += heldBytes;
            totalNodes += nodes;

            report().row(doc.name, mode.name, heldBlocks, heldBytes, nodes, chunks, printed);

            JFree(dupAgain);
            JFree(dupPrinted);
            JDelete(dup);
            JFree(reprinted);
            JDelete(reparsed);
            JFree(printed);
        }
    }

    report().summary("total_allocs", totalAllocs);
    report().summary("total_bytes", totalBytes);
    report().summary("total_nodes", totalNodes);
    report().summary("sizeof_J", sizeof(J));

    // Nothing may outlive the loop.
    THEN("No allocation leaks across the whole corpus") {
        CHECK(stats().liveBlocks == 0);
        CHECK(stats().liveBytes == 0);
    }

    // A single constant both builds must agree on. If this fires, the two
    // layouts have diverged in what they render and the cross-build diff will
    // say exactly where.
    THEN("The rendering digest matches the layout-independent expectation") {
        INFO("layout=" << layoutName() << " sizeof(J)=" << sizeof(J)
             << " digest=" << digest << "ULL");
        CHECK(digest == J_LAYOUT_EXPECTED_DIGEST);
    }

    NoteSetFn(malloc, free, NULL, NULL);
}

SCENARIO("J layout: ownership and inline storage invariants")
{
    installTrackedAllocator();

    GIVEN("A parsed object whose key and value are both short") {
        J *obj = JParse("{\"k\":\"v\"}");
        REQUIRE(obj != NULL);
        J *member = obj->child;
        REQUIRE(member != NULL);

        THEN("The key and value read back correctly regardless of where they live") {
            CHECK(std::string(member->string) == "k");
            CHECK(std::string(member->valuestring) == "v");
        }

        THEN("The packed build stores BOTH the key and the value in the node") {
            if (isPacked()) {
                // The parser measures the key, looks ahead to measure a string
                // value, and sizes one allocation for the node plus both. A
                // short string member therefore costs exactly one allocation.
                CHECK(insideNodeAllocation(member, member->string));
                CHECK(insideNodeAllocation(member, member->valuestring));
                CHECK((member->type & JStringIsConst) != 0);
                CHECK((member->type & JValueInline) != 0);
                // Value first, then key, both inside the numeric-member region.
                CHECK(member->valuestring < member->string);
            } else {
                CHECK_FALSE(insideNodeAllocation(member, member->string));
                CHECK_FALSE(insideNodeAllocation(member, member->valuestring));
                CHECK((member->type & JStringIsConst) == 0);
                CHECK((member->type & JValueInline) == 0);
            }
        }

        THEN("The whole member costs one allocation in the packed build") {
            // {"k":"v"} is a root object node plus one member node. Packed:
            // two allocations total. Unpacked: two nodes plus a key block plus
            // a value block.
            CHECK(stats().liveBlocks == (isPacked() ? 2u : 4u));
        }

        JDelete(obj);
        CHECK(stats().liveBlocks == 0);
    }

    GIVEN("A string built with JCreateString") {
        resetStats();
        J *item = JCreateString("packed into the node");
        REQUIRE(item != NULL);

        THEN("The packed build holds the value in the node's own allocation") {
            CHECK(std::string(item->valuestring) == "packed into the node");
            if (isPacked()) {
                CHECK(insideNodeAllocation(item, item->valuestring));
                CHECK((item->type & JValueInline) != 0);
                CHECK(stats().mallocCalls == 1);   // node+value in ONE block
            } else {
                CHECK_FALSE(insideNodeAllocation(item, item->valuestring));
                CHECK((item->type & JValueInline) == 0);
                CHECK(stats().mallocCalls == 2);   // node, then value
            }
        }

        JDelete(item);
        CHECK(stats().liveBlocks == 0);
    }

    GIVEN("A node whose number pair is written after it was given a key") {
        // The numeric pair IS the inline character buffer -- see the comment on
        // those members in n_cjson.h -- so writing a number to a node that is
        // currently holding a packed key or value has to evacuate that content
        // first. JSetNumberValue and JSetIntValue are function calls precisely
        // so the library gets the chance; the historical macros wrote the
        // members directly from the caller's translation unit and would have
        // destroyed the key. This pins the evacuation.
        J *root = JCreateObject();
        REQUIRE(root != NULL);
        J *sub = JCreateObject();
        REQUIRE(sub != NULL);
        JAddItemToObject(root, "key", sub);
        JSetNumberValue(sub, 72623859790382864.0);

        THEN("The key is intact") {
            char *out = JPrintUnformatted(root);
            REQUIRE(out != NULL);
            CHECK(std::string(out) == "{\"key\":{}}");
            JFree(out);
        }

        JDelete(root);
        CHECK(stats().liveBlocks == 0);
    }

    GIVEN("A member moved between objects while keeping its own name") {
        // Regression guard: the documented idiom passes the node's own key back
        // in, so a key setter that frees before copying is a use-after-free.
        J *a = JCreateObject();
        J *b = JCreateObject();
        J *c = JCreateObject();
        REQUIRE(a != NULL);
        REQUIRE(b != NULL);
        REQUIRE(c != NULL);
        JAddItemToObject(a, "member", JCreateObject());

        J *d1 = JDetachItemFromObject(a, "member");
        REQUIRE(d1 != NULL);
        JAddItemToObject(b, JGetItemName(d1), d1);
        J *d2 = JDetachItemFromObject(b, "member");
        REQUIRE(d2 != NULL);
        JAddItemToObject(c, JGetItemName(d2), d2);

        THEN("The name survives every move") {
            char *out = JPrintUnformatted(c);
            REQUIRE(out != NULL);
            CHECK(std::string(out) == "{\"member\":{}}");
            JFree(out);
        }

        JDelete(a);
        JDelete(b);
        JDelete(c);
        CHECK(stats().liveBlocks == 0);
    }

    GIVEN("A parsed object whose value exceeds the inline capacity") {
        J *obj = JParse("{\"k\":\"0123456789abcdefghijklmnop\"}");
        REQUIRE(obj != NULL);
        J *member = obj->child;
        REQUIRE(member != NULL);

        THEN("The value still reads back correctly") {
            CHECK(std::string(member->valuestring) == "0123456789abcdefghijklmnop");
        }

        JDelete(obj);
        CHECK(stats().liveBlocks == 0);
    }

    GIVEN("A number whose key would otherwise fit inline") {
        // This node is a number, so valueint and valuenumber are LIVE and the
        // inline region is not available: the key can only go in bytes past
        // the struct. This is the case that would corrupt a key if the inline
        // heuristic were too eager.
        J *obj = JParse("{\"count\":1234567890123456789}");
        REQUIRE(obj != NULL);
        J *member = obj->child;
        REQUIRE(member != NULL);

        THEN("Both the key and the full-precision integer survive") {
            CHECK(std::string(member->string) == "count");
            CHECK(member->valueint == 1234567890123456789LL);
            CHECK(JIntValue(member) == 1234567890123456789LL);
        }

        JDelete(obj);
        CHECK(stats().liveBlocks == 0);
    }

    GIVEN("A programmatically built object using the generic add") {
        J *root = JCreateObject();
        REQUIRE(root != NULL);
        J *body = JCreateObject();
        REQUIRE(body != NULL);

        // The idiom that forbids relocating the node on add: the caller keeps
        // using `body` afterwards.
        JAddItemToObject(root, "body", body);
        // An integer, so the expectation holds under NOTE_C_SINGLE_PRECISION too.
        JAddNumberToObject(body, "temp", 22);
        JAddStringToObject(body, "unit", "C");

        THEN("The retained pointer is still valid and the tree is correct") {
            char *out = JPrintUnformatted(root);
            REQUIRE(out != NULL);
            CHECK(std::string(out) == "{\"body\":{\"temp\":22,\"unit\":\"C\"}}");
            JFree(out);
        }

        JDelete(root);
        CHECK(stats().liveBlocks == 0);
    }

    GIVEN("An item whose key is replaced after creation") {
        J *root = JCreateObject();
        REQUIRE(root != NULL);
        JAddStringToObject(root, "short", "v");

        WHEN("The member is replaced with a much longer key") {
            J *repl = JCreateString("w");
            REQUIRE(repl != NULL);
            JReplaceItemInObject(root, "short", repl);

            THEN("The object reflects the replacement without leaking") {
                char *out = JPrintUnformatted(root);
                REQUIRE(out != NULL);
                CHECK(std::string(out) == "{\"short\":\"w\"}");
                JFree(out);
            }
        }

        JDelete(root);
        CHECK(stats().liveBlocks == 0);
    }

    GIVEN("A constant key added with JAddItemToObjectCS") {
        J *root = JCreateObject();
        REQUIRE(root != NULL);
        static const char kKey[] = "constant";
        JAddItemToObjectCS(root, kKey, JCreateNumber(1));

        THEN("The key is borrowed, not copied, and is not freed") {
            REQUIRE(root->child != NULL);
            CHECK(root->child->string == kKey);
            CHECK((root->child->type & JStringIsConst) != 0);
        }

        JDelete(root);
        CHECK(stats().liveBlocks == 0);
    }

    GIVEN("A string item created from a caller-owned heap buffer") {
        // JCreateStringValue ADOPTS the pointer -- JAddBinaryToObject relies on
        // JDelete freeing it -- so it must never be inlined or borrowed.
        char *owned = (char *)JMalloc(8);
        REQUIRE(owned != NULL);
        strcpy(owned, "adopted");
        J *item = JCreateStringValue(owned);
        REQUIRE(item != NULL);

        THEN("The node points at the caller's buffer and takes ownership") {
            CHECK(item->valuestring == owned);
            CHECK((item->type & JIsReference) == 0);
        }

        JDelete(item);
        CHECK(stats().liveBlocks == 0);
    }

    GIVEN("Detaching a member and re-adding it under a different key") {
        J *root = JCreateObject();
        REQUIRE(root != NULL);
        JAddStringToObject(root, "first", "value");
        J *detached = JDetachItemFromObject(root, "first");
        REQUIRE(detached != NULL);

        J *other = JCreateObject();
        REQUIRE(other != NULL);
        JAddItemToObject(other, "second", detached);

        THEN("The new key wins and the old inline key is harmlessly abandoned") {
            char *out = JPrintUnformatted(other);
            REQUIRE(out != NULL);
            CHECK(std::string(out) == "{\"second\":\"value\"}");
            JFree(out);
        }

        JDelete(root);
        JDelete(other);
        CHECK(stats().liveBlocks == 0);
    }

    NoteSetFn(malloc, free, NULL, NULL);
}

SCENARIO("Formatted omit-empty output carries no orphaned indentation")
{
    installTrackedAllocator();

    // Regression guard: _print_object used to emit a member's indentation
    // before deciding whether omitempty elided it, so every elided member left
    // whitespace behind and consumed buffer the caller had not budgeted for.
    struct Case {
        const char *json;
        const char *expected;
    };
    static const Case cases[] = {
        { "{\"a\":0}",                              "{\n}" },
        { "{\"a\":0,\"b\":1,\"c\":0}",              "{\n    \"b\": 1\n}" },
        { "{\"a\":false,\"b\":\"\",\"c\":\"x\"}",    "{\n    \"c\": \"x\"\n}" },
        { "{\"o\":{\"a\":0,\"b\":2},\"k\":1,\"d\":0}", "{\n    \"o\": {\n        \"b\": 2\n    },\n    \"k\": 1\n}" },
    };

    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
        J *j = JParse(cases[i].json);
        REQUIRE(j != NULL);

        WHEN(std::string("rendering ") + cases[i].json) {
            char *out = (char *)_print(j, true, true);
            REQUIRE(out != NULL);

            THEN("No whitespace is emitted for an elided member") {
                CHECK(std::string(out) == cases[i].expected);
            }
            THEN("The rendering is a fixed point under reparse and reprint") {
                J *re = JParse(out);
                REQUIRE(re != NULL);
                char *again = (char *)_print(re, true, true);
                REQUIRE(again != NULL);
                CHECK(std::string(again) == out);
                JFree(again);
                JDelete(re);
            }

            JFree(out);
        }

        JDelete(j);
    }

    CHECK(stats().liveBlocks == 0);
    NoteSetFn(malloc, free, NULL, NULL);
}

SCENARIO("Numeric setters are functions, not member-poking macros")
{
    installTrackedAllocator();

    GIVEN("An integer too large for a double") {
        J *n = JCreateNumber(0);
        REQUIRE(n != NULL);
        JINTEGER ret = JSetIntValue(n, 9007199254740993LL);

        THEN("It rounds through the double exactly as the historical macro did") {
            // The historical macro expanded to
            //   valueint = valuenumber = number
            // which parses as valueint = (valuenumber = number), so the operand
            // reached valueint only AFTER a trip through JNUMBER: 2^53+1 comes
            // back as 2^53.
            //
            // Assigning valueint straight from a JINTEGER parameter would keep
            // the extra bit, which is arguably better -- and is exactly why this
            // test exists. That change also truncates a FRACTIONAL operand at
            // the call site, silently turning JSetIntValue(n, 1.5) into
            // valuenumber == 1 where it had always been 1.5. The two behaviors
            // are the same one-line decision, so the historical rounding is
            // pinned here rather than improved on.
            //
            // Verified against a pristine `git archive HEAD` build, which
            // produces 9007199254740992 for all three of these.
            CHECK(ret == 9007199254740992LL);
            CHECK(n->valueint == 9007199254740992LL);
            CHECK(JIntValue(n) == 9007199254740992LL);
        }
        THEN("valuenumber is still kept in step") {
            CHECK(n->valuenumber == (JNUMBER)9007199254740993LL);
        }

        JDelete(n);
    }

    GIVEN("A fractional setter argument") {
        J *n = JCreateNumber(0);
        REQUIRE(n != NULL);
        JSetIntValue(n, 1.5);

        THEN("valueint truncates and valuenumber keeps the fraction") {
            // The other half of the same decision. See above.
            CHECK(n->valueint == 1);
            CHECK(n->valuenumber == (JNUMBER)1.5);
        }

        JDelete(n);
    }

    GIVEN("A setter argument with a side effect") {
        J *n = JCreateNumber(0);
        REQUIRE(n != NULL);
        int calls = 0;
        JSetIntValue(n, ++calls);

        THEN("It is evaluated exactly once") {
            CHECK(calls == 1);
        }

        JDelete(n);
    }

    GIVEN("A NULL item") {
        THEN("Both setters remain NULL-safe and return the value") {
            CHECK(JSetIntValue((J *)NULL, 7) == 7);
            CHECK(JSetNumberValue((J *)NULL, 7.5) == 7.5);
        }
    }

    GIVEN("An object member") {
        J *o = JCreateObject();
        REQUIRE(o != NULL);
        REQUIRE(JAddNumberToObject(o, "field", 1) != NULL);

        THEN("JGetObjectItemName reports the key without touching the struct") {
            CHECK(std::string(JGetObjectItemName(o->child)) == "field");
        }

        JDelete(o);
    }

    CHECK(stats().liveBlocks == 0);
    NoteSetFn(malloc, free, NULL, NULL);
}

SCENARIO("Re-keying an item with its own key is never a use-after-free")
{
    installTrackedAllocator();

    // Both add paths must tolerate `key` aliasing the node's current key. This
    // is the documented "move a member, keep its name" idiom, and it was a
    // use-after-free on master through JAddItemToObjectCS.
    struct Path {
        const char *name;
        bool constantKey;
    };
    static const Path paths[] = {
        { "JAddItemToObject", false },
        { "JAddItemToObjectCS", true },
    };

    for (size_t i = 0; i < sizeof(paths) / sizeof(paths[0]); ++i) {
        WHEN(std::string("moving a member via ") + paths[i].name) {
            J *a = JCreateObject();
            J *b = JCreateObject();
            J *c = JCreateObject();
            REQUIRE(a != NULL);
            REQUIRE(b != NULL);
            REQUIRE(c != NULL);
            REQUIRE(JAddStringToObject(a, "member", "v") != NULL);

            // Three hops, because the storage class of the key can differ on
            // each pass and only some passes take the aliasing branch.
            J *cur = JDetachItemFromObject(a, "member");
            REQUIRE(cur != NULL);
            for (int hop = 0; hop < 3; ++hop) {
                J *dst = (hop % 2) ? c : b;
                if (paths[i].constantKey) {
                    JAddItemToObjectCS(dst, JGetItemName(cur), cur);
                } else {
                    JAddItemToObject(dst, JGetItemName(cur), cur);
                }
                cur = JDetachItemFromObject(dst, "member");
                REQUIRE(cur != NULL);
            }

            THEN("The key survives every hop intact") {
                CHECK(std::string(JGetItemName(cur)) == "member");
                CHECK(std::string(cur->valuestring) == "v");
            }

            JDelete(cur);
            JDelete(a);
            JDelete(b);
            JDelete(c);
        }
    }

    CHECK(stats().liveBlocks == 0);
    NoteSetFn(malloc, free, NULL, NULL);
}

SCENARIO("The packing actually happens")
{
    installTrackedAllocator();

    // These assert the OPTIMIZATION, not just correctness. Everything else in
    // this file passes just as happily with packing disabled, so without these
    // the layout could silently stop packing and no test would notice.
    // Mutation-checked: each case below fails if the corresponding mechanism is
    // removed.

    struct Case {
        const char *what;
        const char *json;
        size_t packedAllocs;      // parse path
        size_t unpackedAllocs;
        // Each member's key and value length, INCLUDING the NUL. Every member
        // in a given case has the same shape. The expected node size is derived
        // from these rather than hardcoded, because the size of the inline
        // region is not the same on every ABI: JINTEGER is 8-byte aligned on
        // ARM and x86-64 but only 4-byte aligned on i386, so the same layout
        // yields a different sizeof(J) and a different amount of room before
        // the struct ends. Hardcoding "fits in sizeof(J)" passed everywhere the
        // region is 16 bytes and failed on 32-bit Linux, where it is 12.
        bool   memberIsString;
        size_t memberKeyLen;
        size_t memberValLen;
    };
    static const Case parsed[] = {
        // node+key+value in ONE allocation. Fails if the parser's value
        // lookahead is removed, or if _j_alloc_size stops using the inline
        // region as the string base.
        { "short string member",   "{\"a\":\"b\"}",            2, 4,  true,  2,  2 },
        { "empty string value",    "{\"a\":\"\"}",             2, 4,  true,  2,  1 },
        { "realistic member",      "{\"aqi_level\":\"good\"}",  2, 4,  true, 10,  5 },
        // node+key in one allocation; the number occupies the inline region.
        { "number member",         "{\"temperature\":22.5}",   2, 3,  false, 12,  0 },
        // three members, each self-contained.
        { "three string members",  "{\"a\":\"\",\"b\":\"\",\"c\":\"\"}", 4, 10, true,  2,  1 },
    };

    for (size_t i = 0; i < sizeof(parsed) / sizeof(parsed[0]); ++i) {
        WHEN(std::string("parsing ") + parsed[i].what) {
            resetStats();
            const size_t before = stats().liveBlocks;
            J *j = JParse(parsed[i].json);
            REQUIRE(j != NULL);
            const size_t used = stats().liveBlocks - before;

            THEN("It costs exactly the expected number of allocations") {
                CHECK(used == (isPacked() ? parsed[i].packedAllocs
                               : parsed[i].unpackedAllocs));
            }

            THEN("Each node is exactly the size its content requires") {
                // Counting allocations is not enough: a layout that packs into
                // an over-sized node makes the same number of allocations and
                // costs more memory. This pins the exact allocation formula.
                //
                // A string member's content starts at the inline region and the
                // node grows only if it overflows the struct; a number member
                // keeps its numeric fields, so its key always extends past the
                // struct by exactly its own length.
                if (isPacked()) {
                    const size_t inlineOff = offsetof(J, valueint);
                    size_t expected;
                    if (parsed[i].memberIsString) {
                        expected = inlineOff + parsed[i].memberValLen
                                   + parsed[i].memberKeyLen;
                        if (expected < sizeof(J)) {
                            expected = sizeof(J);
                        }
                    } else {
                        expected = sizeof(J) + parsed[i].memberKeyLen;
                    }
                    for (J *n = j->child; n != NULL; n = n->next) {
                        INFO("sizeof(J)=" << sizeof(J)
                             << " inline region=" << (sizeof(J) - inlineOff));
                        CHECK(nodeAllocSize(n) == expected);
                    }
                }
            }

            JDelete(j);
        }
    }

    // Construction path: every JAdd<T>ToObject sizes its node for the key.
    // Fails if _j_new_keyed stops passing the key length.
    WHEN("building members with the JAdd*ToObject helpers") {
        struct Built {
            const char *what;
            size_t packed;
            size_t unpacked;
        };
        resetStats();
        size_t before = stats().liveBlocks;
        J *o1 = JCreateObject();
        JAddNumberToObject(o1, "temperature", 22.5);
        size_t numberCost = stats().liveBlocks - before;

        before = stats().liveBlocks;
        J *o2 = JCreateObject();
        JAddBoolToObject(o2, "charging", true);
        size_t boolCost = stats().liveBlocks - before;

        before = stats().liveBlocks;
        J *o3 = JCreateObject();
        JAddStringToObject(o3, "aqi_level", "good");
        size_t stringCost = stats().liveBlocks - before;

        THEN("The key is carved out of the node rather than allocated") {
            // object node + member node, and nothing else, when packed.
            CHECK(numberCost == (isPacked() ? 2u : 3u));
            CHECK(boolCost   == (isPacked() ? 2u : 3u));
            CHECK(stringCost == (isPacked() ? 2u : 4u));
        }

        JDelete(o1);
        JDelete(o2);
        JDelete(o3);
    }

    // The numeric setters must evict inline content before writing the numeric
    // members. Fails if _j_unpack_numeric_region becomes a no-op. This uses a
    // STRING node, whose key and value both live in the numeric region -- the
    // earlier regression guard used an object node, whose key lives past the
    // struct and is therefore never at risk.
    WHEN("writing a number over a string node whose key and value are inline") {
        J *root = JCreateObject();
        REQUIRE(root != NULL);
        J *m = JAddStringToObject(root, "thekey", "thevalue");
        REQUIRE(m != NULL);
        if (isPacked()) {
            REQUIRE(insideNodeAllocation(m, m->string));
            REQUIRE(insideNodeAllocation(m, m->valuestring));
        }

        JSetNumberValue(m, 7.5);

        THEN("The key survives the write") {
            CHECK(std::string(JGetItemName(m)) == "thekey");
        }
        THEN("The value survives the write") {
            CHECK(std::string(m->valuestring) == "thevalue");
        }
        THEN("The object still renders correctly") {
            char *out = JPrintUnformatted(root);
            REQUIRE(out != NULL);
            CHECK(std::string(out) == "{\"thekey\":\"thevalue\"}");
            JFree(out);
        }

        JDelete(root);
    }

    // A reference node must not inherit the source's packing state. Fails if
    // _create_reference stops resetting objlen/JValueInline: the key offset is
    // then computed by subtracting pointers into two different allocations.
    WHEN("adding a reference to a string node that has an inline value") {
        for (int i = 0; i < 200; ++i) {
            J *src = JCreateString("a string value");
            REQUIRE(src != NULL);
            J *obj = JCreateObject();
            REQUIRE(obj != NULL);
            JAddItemReferenceToObject(obj, "akey", src);

            if (isPacked()) {
                REQUIRE(obj->child != NULL);
                // The reference describes its own allocation, not the source's.
                CHECK(nodeAllocSize(obj->child) == sizeof(J));
                CHECK((obj->child->type & JValueInline) == 0);
            }

            char *out = JPrintUnformatted(obj);
            REQUIRE(out != NULL);
            CHECK(std::string(out) == "{\"akey\":\"a string value\"}");
            JFree(out);
            JDelete(obj);
            JDelete(src);
        }
    }

    // The allocation cap must hold: a node that would exceed it falls back to
    // heap storage rather than truncating objlen. Fails if the cap is removed.
    WHEN("a value is far larger than the allocation cap") {
        std::string huge(60000, 'x');
        J *root = JCreateObject();
        REQUIRE(root != NULL);
        J *m = JAddStringToObject(root, "k", huge.c_str());
        REQUIRE(m != NULL);

        THEN("The value is intact and stored outside the node") {
            CHECK(std::string(m->valuestring) == huge);
            CHECK_FALSE(insideNodeAllocation(m, m->valuestring));
            if (isPacked()) {
                // The node must fall all the way back to a bare struct. Merely
                // checking objlen <= J_MAX_PACKED_ALLOC would pass for a node
                // that packed 60 KB inline, which is exactly what removing the
                // cap does.
                CHECK(nodeAllocSize(m) == sizeof(J));
            }
        }

        JDelete(root);
    }

    // The cap must also catch a node whose key and value are each individually
    // under the limit but whose SUM is over it. The operand guard alone does
    // not cover this, and an unbounded sum truncates when stored in objlen.
    WHEN("a key and a value are each under the cap but sum over it") {
        std::string bigKey(40000, 'k');
        std::string bigVal(40000, 'v');
        J *root = JCreateObject();
        REQUIRE(root != NULL);
        J *m = JAddStringToObject(root, bigKey.c_str(), bigVal.c_str());
        REQUIRE(m != NULL);

        THEN("The node falls back to a bare struct with both on the heap") {
            CHECK(std::string(m->valuestring) == bigVal);
            CHECK(std::string(JGetItemName(m)) == bigKey);
            if (isPacked()) {
                CHECK(nodeAllocSize(m) == sizeof(J));
            }
        }

        JDelete(root);
    }

    // Re-keying an item with its OWN key, where that key is on the heap rather
    // than inline. This is the shape the historical use-after-free needed: a
    // key short enough to be re-keyed but long enough not to fit in the node.
    // With an inline key the release is skipped and the hazard never arises,
    // which is why the earlier scenario could not reach it.
    WHEN("re-keying an item whose key is heap-allocated") {
        std::string longKey(64, 'k');
        J *a = JCreateObject();
        J *b = JCreateObject();
        REQUIRE(a != NULL);
        REQUIRE(b != NULL);
        // The GENERIC add, not JAddStringToObject: the latter sizes the node
        // for its key, so even a 64-char key would land inline and the heap
        // path would never be exercised. Here the node already exists at
        // exactly sizeof(J), leaving no room for a key this long.
        JAddItemToObject(a, longKey.c_str(), JCreateString("v"));

        J *m = JDetachItemFromObject(a, longKey.c_str());
        REQUIRE(m != NULL);
        if (isPacked()) {
            REQUIRE_FALSE(insideNodeAllocation(m, m->string));   // heap key
        }

        // Hand the node its own key back, through both add paths.
        JAddItemToObject(b, JGetItemName(m), m);
        J *m2 = JDetachItemFromObject(b, longKey.c_str());
        REQUIRE(m2 != NULL);
        JAddItemToObjectCS(b, JGetItemName(m2), m2);

        THEN("The key is intact") {
            CHECK(std::string(JGetItemName(m2)) == longKey);
            char *out = JPrintUnformatted(b);
            REQUIRE(out != NULL);
            CHECK(std::string(out) == "{\"" + longKey + "\":\"v\"}");
            JFree(out);
        }

        JDelete(a);
        JDelete(b);
    }

    CHECK(stats().liveBlocks == 0);
    NoteSetFn(malloc, free, NULL, NULL);
}

SCENARIO("J layout: key and value length sweep across the inline boundary")
{
    installTrackedAllocator();

    // Walk every key length and every value length from 0 to well past the
    // 16-byte inline capacity, in both the parsed and the programmatic path.
    for (size_t keyLen = 1; keyLen <= 24; ++keyLen) {
        for (size_t valLen = 0; valLen <= 24; ++valLen) {
            std::string key(keyLen, 'k');
            std::string val(valLen, 'v');
            std::string json = "{\"" + key + "\":\"" + val + "\"}";

            J *parsed = JParse(json.c_str());
            REQUIRE(parsed != NULL);
            J *member = parsed->child;
            REQUIRE(member != NULL);
            CHECK(std::string(member->string) == key);
            CHECK(std::string(member->valuestring) == val);

            char *out = JPrintUnformatted(parsed);
            REQUIRE(out != NULL);
            CHECK(std::string(out) == json);
            JFree(out);

            // Same shape built programmatically, which takes a different
            // storage path for the key.
            J *built = JCreateObject();
            REQUIRE(built != NULL);
            REQUIRE(JAddStringToObject(built, key.c_str(), val.c_str()) != NULL);
            char *builtOut = JPrintUnformatted(built);
            REQUIRE(builtOut != NULL);
            CHECK(std::string(builtOut) == json);
            JFree(builtOut);

            // And via the generic add of a pre-built item, which is the path
            // that cannot pack the key at construction time.
            J *generic = JCreateObject();
            REQUIRE(generic != NULL);
            JAddItemToObject(generic, key.c_str(), JCreateString(val.c_str()));
            char *genericOut = JPrintUnformatted(generic);
            REQUIRE(genericOut != NULL);
            CHECK(std::string(genericOut) == json);
            JFree(genericOut);

            JDelete(parsed);
            JDelete(built);
            JDelete(generic);
        }
    }

    THEN("The sweep leaks nothing") {
        CHECK(stats().liveBlocks == 0);
        CHECK(stats().liveBytes == 0);
    }

    NoteSetFn(malloc, free, NULL, NULL);
}

}
