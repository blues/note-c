/*!
 * @file JReferenceAndFailure_test.cpp
 *
 * Covers the two areas the packed-layout work left thinnest: reference nodes,
 * and what happens when the allocator says no.
 *
 * REFERENCES. `_create_reference` builds a node by memcpy'ing a source node
 * into a fresh sizeof(J) allocation. Under NOTE_C_STORAGE_OPTIMIZATION a source
 * node may be LARGER than sizeof(J) and may carry pointers into its own
 * allocation, so the copy has to be corrected or the reference ends up claiming
 * storage it does not own. A real heap-buffer-overflow of exactly this shape
 * reached review, and it reproduced through nothing more exotic than
 * JAddObjectToObject followed by JAddItemReferenceToObject. Every referent
 * shape and every legal deletion order is therefore enumerated here rather than
 * spot-checked.
 *
 * ALLOCATION FAILURE. A parse, duplication or setter that fails partway is the
 * least-travelled path in the library and the easiest place for a half-built
 * node to escape. Each operation below is run once per allocation it makes,
 * failing that allocation and no other, and checked for leaks each time.
 *
 * Every assertion holds in both layouts.
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

#include <string>
#include <vector>

#include "j_layout_test_support.hpp"

using namespace jlayout;

namespace
{

// A referent shape, built fresh for each case.
//
// `keyed` is the property that made the original bug reachable: an item created
// through JAddXToObject carries a key, and under the storage optimization that
// key may live in trailing bytes of the item's own allocation, making the node
// bigger than sizeof(J).
struct Referent {
    const char *name;
    bool keyed;
    // Builds the referent inside `owner` (keyed) or standalone (unkeyed).
    J *(*build)(J *owner);
};

J *buildNumber(J *owner)
{
    return owner ? JAddNumberToObject(owner, "source-key", 42)
           : JCreateNumber(42);
}

J *buildObject(J *owner)
{
    return owner ? JAddObjectToObject(owner, "source-key") : JCreateObject();
}

J *buildArray(J *owner)
{
    J *a = owner ? JAddArrayToObject(owner, "source-key") : JCreateArray();
    if (a != NULL) {
        JAddItemToArray(a, JCreateNumber(1));
    }
    return a;
}

J *buildShortString(J *owner)
{
    // Short enough to pack into the node's inline region alongside its key.
    return owner ? JAddStringToObject(owner, "source-key", "abc")
           : JCreateString("abc");
}

J *buildLongString(J *owner)
{
    // Far too long to inline, so valuestring is a separate heap block. A
    // reference must borrow that pointer without ever freeing it.
    static const char *kLong =
        "a value long enough that it cannot possibly be packed into the node "
        "itself, forcing valuestring onto the heap where the reference will "
        "borrow rather than own it";
    return owner ? JAddStringToObject(owner, "source-key", kLong)
           : JCreateString(kLong);
}

J *buildBool(J *owner)
{
    return owner ? JAddBoolToObject(owner, "source-key", true) : JCreateBool(true);
}

const std::vector<Referent> &referents()
{
    static const std::vector<Referent> r = {
        { "number",       true,  buildNumber },
        { "object",       true,  buildObject },
        { "array",        true,  buildArray },
        { "short string", true,  buildShortString },
        { "long string",  true,  buildLongString },
        { "bool",         true,  buildBool },
        { "unkeyed number",       false, buildNumber },
        { "unkeyed short string", false, buildShortString },
        { "unkeyed long string",  false, buildLongString },
    };
    return r;
}

SCENARIO("Reference nodes describe their own allocation, not the referent's")
{
    for (size_t i = 0; i < referents().size(); ++i) {
        const Referent &r = referents()[i];

        GIVEN(std::string("a reference to a ") + r.name) {
            installTrackedAllocator();

            J *owner = NULL;
            J *referent = NULL;
            if (r.keyed) {
                owner = JCreateObject();
                REQUIRE(owner != NULL);
                referent = r.build(owner);
            } else {
                referent = r.build(NULL);
            }
            REQUIRE(referent != NULL);

            J *destination = JCreateObject();
            REQUIRE(destination != NULL);

            // The exact sequence that overflowed the reference's allocation:
            // the key is installed AFTER the struct copy, using whatever
            // trailing capacity the node claims to have.
            JAddItemReferenceToObject(destination, "x", referent);
            REQUIRE(destination->child != NULL);

            J *reference = JGetObjectItem(destination, "x");
            REQUIRE(reference != NULL);

            THEN("the reference's key is inside the reference, never the referent") {
                REQUIRE(JGetItemName(reference) != NULL);
                CHECK(strcmp(JGetItemName(reference), "x") == 0);
                CHECK(!insideNodeAllocation(referent, JGetItemName(reference)));
            }

            THEN("the reference claims no more storage than it allocated") {
                // The bug was objlen surviving the memcpy, so the reference
                // believed it was as large as its (larger) source.
                CHECK(nodeAllocSize(reference) <= nodeAllocSize(referent));
                CHECK(nodeAllocSize(reference) == sizeof(J));
            }

            THEN("the referent is untouched") {
                if (r.keyed) {
                    REQUIRE(JGetItemName(referent) != NULL);
                    CHECK(strcmp(JGetItemName(referent), "source-key") == 0);
                }
                if (JIsString(referent)) {
                    CHECK(strcmp(JStringValue(referent), JStringValue(reference)) == 0);
                }
            }

            // Deletion order is the whole point: a reference must not free
            // anything the referent owns, and the referent must remain
            // independently valid whichever side goes first.
            THEN("deleting the destination first leaves the referent intact") {
                JDelete(destination);
                if (r.keyed) {
                    CHECK(strcmp(JGetItemName(referent), "source-key") == 0);
                }
                if (JIsString(referent)) {
                    CHECK(strlen(JStringValue(referent)) > 0);
                }
                char *out = JPrintUnformatted(r.keyed ? owner : referent);
                CHECK(out != NULL);
                JFree(out);
                JDelete(r.keyed ? owner : referent);
                CHECK(stats().liveBlocks == 0);
            }

            THEN("deleting the referent first leaves the destination freeable") {
                JDelete(r.keyed ? owner : referent);
                JDelete(destination);
                CHECK(stats().liveBlocks == 0);
            }

            THEN("rendering the reference matches rendering the referent") {
                char *viaRef = JPrintUnformatted(reference);
                char *direct = JPrintUnformatted(referent);
                REQUIRE(viaRef != NULL);
                REQUIRE(direct != NULL);
                CHECK(std::string(viaRef) == std::string(direct));
                JFree(viaRef);
                JFree(direct);

                JDelete(destination);
                JDelete(r.keyed ? owner : referent);
                CHECK(stats().liveBlocks == 0);
            }
        }
    }
}

SCENARIO("Array references behave the same as object references")
{
    for (size_t i = 0; i < referents().size(); ++i) {
        const Referent &r = referents()[i];

        GIVEN(std::string("an array reference to a ") + r.name) {
            installTrackedAllocator();

            J *owner = NULL;
            J *referent = NULL;
            if (r.keyed) {
                owner = JCreateObject();
                REQUIRE(owner != NULL);
                referent = r.build(owner);
            } else {
                referent = r.build(NULL);
            }
            REQUIRE(referent != NULL);

            J *destination = JCreateArray();
            REQUIRE(destination != NULL);
            JAddItemReferenceToArray(destination, referent);
            REQUIRE(destination->child != NULL);

            J *reference = JGetArrayItem(destination, 0);
            REQUIRE(reference != NULL);

            THEN("an array reference carries no key") {
                CHECK(reference->string == NULL);
            }

            THEN("the reference claims only its own allocation") {
                CHECK(nodeAllocSize(reference) == sizeof(J));
            }

            THEN("either deletion order is clean") {
                JDelete(destination);
                JDelete(r.keyed ? owner : referent);
                CHECK(stats().liveBlocks == 0);
            }

            THEN("the reverse deletion order is clean too") {
                JDelete(r.keyed ? owner : referent);
                JDelete(destination);
                CHECK(stats().liveBlocks == 0);
            }
        }
    }
}

SCENARIO("A reference survives being re-keyed and duplicated")
{
    installTrackedAllocator();

    // Re-keying is where the overflow actually landed: _j_key_set writes into
    // the trailing capacity the node claims. Walk key lengths across the inline
    // boundary so a node that mis-reported its capacity is caught.
    GIVEN("a reference to a keyed short-string member") {
        J *owner = JCreateObject();
        REQUIRE(owner != NULL);
        J *referent = JAddStringToObject(owner, "source-key", "abc");
        REQUIRE(referent != NULL);

        for (size_t len = 1; len <= 40; ++len) {
            std::string key(len, 'k');
            J *destination = JCreateObject();
            REQUIRE(destination != NULL);
            JAddItemReferenceToObject(destination, key.c_str(), referent);
            REQUIRE(destination->child != NULL);

            J *reference = JGetObjectItem(destination, key.c_str());
            REQUIRE(reference != NULL);
            CHECK(strcmp(JGetItemName(reference), key.c_str()) == 0);
            // The referent's own key must not have been trampled by a write
            // that ran past the reference's allocation.
            CHECK(strcmp(JGetItemName(referent), "source-key") == 0);
            CHECK(strcmp(JStringValue(referent), "abc") == 0);

            JDelete(destination);
        }

        JDelete(owner);
        CHECK(stats().liveBlocks == 0);
    }

    GIVEN("a duplicated reference") {
        J *owner = JCreateObject();
        REQUIRE(owner != NULL);
        J *referent = JAddStringToObject(owner, "source-key", "abc");
        REQUIRE(referent != NULL);

        J *destination = JCreateObject();
        REQUIRE(destination != NULL);
        JAddItemReferenceToObject(destination, "x", referent);
        REQUIRE(destination->child != NULL);

        // The duplicate must own everything outright, so it has to outlive the
        // source it was copied from.
        J *copy = JDuplicate(destination, true);
        REQUIRE(copy != NULL);

        JDelete(destination);
        JDelete(owner);

        THEN("the duplicate is independent of both") {
            char *out = JPrintUnformatted(copy);
            REQUIRE(out != NULL);
            CHECK(std::string(out) == "{\"x\":\"abc\"}");
            JFree(out);
        }

        JDelete(copy);
        CHECK(stats().liveBlocks == 0);
    }
}

// ---------------------------------------------------------------------------
// Allocation failure
// ---------------------------------------------------------------------------

// Runs `op` once per allocation it makes, failing exactly one allocation each
// time, and requires that nothing leaks on any of those paths.
//
// The number of allocations is discovered by running the operation once with no
// injection, so this stays correct as the layout changes how many allocations
// an operation needs.
template <typename Op>
void forEachAllocationFailure(const char *what, Op op)
{
    installTrackedAllocator();
    op();
    const size_t total = stats().mallocCalls;
    REQUIRE(total > 0);
    REQUIRE(stats().liveBlocks == 0);

    for (size_t ordinal = 1; ordinal <= total; ++ordinal) {
        installTrackedAllocator();
        failAllocationAt(ordinal);
        op();
        stopFailingAllocations();

        INFO(what << ": failing allocation " << ordinal << " of " << total);
        CHECK(stats().liveBlocks == 0);
    }
    installTrackedAllocator();
}

SCENARIO("Allocation failure at any point leaks nothing")
{
    // Representative parses: keys and values on both sides of the inline
    // boundary, nesting, arrays, escapes, and numbers.
    const char *documents[] = {
        "{\"a\":\"\"}",
        "{\"a\":1}",
        "{\"key\":\"value\"}",
        "{\"a\":\"abc\",\"b\":\"defghijklmnopqrstuvwxyz0123456789\"}",
        "{\"outer\":{\"inner\":[1,2,3],\"s\":\"x\"}}",
        "[\"\\u00e9\",\"\\ud83d\\ude00\",\"tab\\there\"]",
        "{\"n\":-1.5e3,\"big\":9007199254740993,\"t\":true,\"z\":null}",
    };

    for (size_t d = 0; d < sizeof(documents) / sizeof(documents[0]); ++d) {
        const char *json = documents[d];

        GIVEN(std::string("parsing ") + json) {
            forEachAllocationFailure(json, [json]() {
                J *j = JParse(json);
                JDelete(j);
            });
        }

        GIVEN(std::string("parse, duplicate and print of ") + json) {
            forEachAllocationFailure(json, [json]() {
                J *j = JParse(json);
                if (j != NULL) {
                    J *copy = JDuplicate(j, true);
                    char *out = JPrintUnformatted(copy);
                    JFree(out);
                    JDelete(copy);
                }
                JDelete(j);
            });
        }
    }

    GIVEN("setter evacuation of packed content") {
        // Overwriting a packed string member with a number forces the key and
        // value out of the numeric region. That evacuation allocates, and the
        // library documents it as best-effort: the content may be dropped, but
        // the node must stay consistent and nothing may leak.
        forEachAllocationFailure("setter evacuation", []() {
            J *o = JCreateObject();
            if (o != NULL) {
                JAddStringToObject(o, "key", "packed value");
                J *item = JGetObjectItem(o, "key");
                if (item != NULL) {
                    JSetIntValue(item, 7);
                    JSetNumberValue(item, 1.5);
                }
                char *out = JPrintUnformatted(o);
                JFree(out);
            }
            JDelete(o);
        });
    }

    GIVEN("reference creation and re-keying") {
        forEachAllocationFailure("reference creation", []() {
            J *owner = JCreateObject();
            J *referent = (owner != NULL) ? JAddStringToObject(owner, "source-key", "abc") : NULL;
            J *destination = JCreateObject();
            if ((referent != NULL) && (destination != NULL)) {
                JAddItemReferenceToObject(destination, "x", referent);
            }
            JDelete(destination);
            JDelete(owner);
        });
    }

    GIVEN("adding members one at a time") {
        forEachAllocationFailure("object construction", []() {
            J *o = JCreateObject();
            if (o != NULL) {
                JAddStringToObject(o, "s", "short");
                JAddStringToObject(o, "long-key-past-the-inline-region", "a longer value");
                JAddNumberToObject(o, "n", 1.5);
                JAddBoolToObject(o, "b", true);
                J *arr = JAddArrayToObject(o, "a");
                if (arr != NULL) {
                    JAddItemToArray(arr, JCreateString("elem"));
                }
            }
            JDelete(o);
        });
    }
}

SCENARIO("A setter that cannot evacuate packed content refuses rather than corrupts")
{
    // Under NOTE_C_STORAGE_OPTIMIZATION a short member's key and value live in
    // the bytes valueint/valuenumber occupy, so a numeric setter has to move
    // them to the heap before writing. When that allocation fails there are
    // only two options, and only one of them is safe.
    //
    // An earlier revision cleared the pointers and carried on. That rendered
    // {"temp":"hot"} as {"":""} -- key renamed to empty, value blanked -- and
    // the result still PARSES, so nothing downstream can detect it. On a
    // Notecard host under memory pressure that means quietly shipping a
    // malformed field to Notehub. The historical layout cannot do this at all,
    // because its setters never touch the key.
    //
    // So the setter now abandons the write and preserves document content.
    // Losing the update is ordinary note-c behavior under memory pressure --
    // JAddItemToObject and its siblings return void and have always dropped an
    // item silently on allocation failure. Corrupting a document that then
    // still parses is what was new.
    //
    // This test is written to pass in both layouts: in the historical one there
    // is nothing to evacuate and the set simply succeeds, so the assertion is
    // that the KEY AND VALUE SURVIVE, not that the set was skipped.

    GIVEN("a packed short member and an allocator that refuses everything") {
        installTrackedAllocator();

        J *object = JCreateObject();
        REQUIRE(object != NULL);
        REQUIRE(JAddStringToObject(object, "temp", "hot") != NULL);
        J *item = JGetObjectItem(object, "temp");
        REQUIRE(item != NULL);

        WHEN("a number is set over it with no memory available") {
            // Fail from the very next allocation onward.
            failAllocationAt(1);
            JSetNumberValue(item, 1.5);
            stopFailingAllocations();

            THEN("the key and value are intact, not silently emptied") {
                REQUIRE(JGetItemName(item) != NULL);
                CHECK(strcmp(JGetItemName(item), "temp") == 0);

                char *out = JPrintUnformatted(object);
                REQUIRE(out != NULL);
                CHECK(std::string(out) == "{\"temp\":\"hot\"}");
                JFree(out);
            }
        }

        WHEN("an integer is set over it with no memory available") {
            failAllocationAt(1);
            JSetIntValue(item, 7);
            stopFailingAllocations();

            THEN("the key and value are intact") {
                REQUIRE(JGetItemName(item) != NULL);
                CHECK(strcmp(JGetItemName(item), "temp") == 0);

                char *out = JPrintUnformatted(object);
                REQUIRE(out != NULL);
                CHECK(std::string(out) == "{\"temp\":\"hot\"}");
                JFree(out);
            }
        }

        WHEN("memory is available") {
            THEN("the set applies normally and the key still survives") {
                JSetNumberValue(item, 1.5);
                REQUIRE(JGetItemName(item) != NULL);
                CHECK(strcmp(JGetItemName(item), "temp") == 0);
                CHECK(item->valuenumber == (JNUMBER)1.5);
            }
        }

        JDelete(object);
        CHECK(stats().liveBlocks == 0);
    }
}

SCENARIO("Malformed input is rejected without leaking")
{
    // Truncations and structural errors, including several that fail AFTER a
    // node has been linked into a parent's child list -- the case where a
    // partially built tree has to be unwound rather than simply discarded.
    const char *malformed[] = {
        "",                          "{",
        "}",                         "[",
        "]",                         "{\"a\"",
        "{\"a\":",                   "{\"a\":}",
        "{\"a\":1",                  "{\"a\":1,}",
        "{\"a\":1,",                 "{\"a\":1 \"b\":2}",
        "{\"a\" 1}",                 "{a:1}",
        "{'a':1}",                   "{\"a\":1}}",
        "[1,]",                      "[1 2]",
        "[1,,2]",                    "[",
        "{\"unterminated\":\"str",   "{\"unterminated key",
        "\"bare string",             "{\"a\":\"\\\"}",
        "{\"a\":\"\\q\"}",           "{\"a\":\"\\u\"}",
        "{\"a\":\"\\u00\"}",         "{\"a\":\"\\uZZZZ\"}",
        "{\"a\":\"\\ud83d\"}",       "{\"a\":\"\\ud83d\\u0041\"}",
        "{\"a\":\"\\udc00\"}",       "{\"a\":01}",
        "{\"a\":1.2.3}",             "{\"a\":+1}",
        "{\"a\":.5}",                "{\"a\":1e}",
        "{\"a\":tru}",               "{\"a\":nul}",
        "{\"a\":Infinity}",          "{\"good\":1,\"bad\":}",
        "{\"good\":1,\"nested\":{\"deeper\":[1,2,}}",
        "[{\"a\":1},{\"b\":}]",
    };

    installTrackedAllocator();

    for (size_t i = 0; i < sizeof(malformed) / sizeof(malformed[0]); ++i) {
        const char *json = malformed[i];

        WHEN(std::string("parsing malformed input: ") + json) {
            installTrackedAllocator();
            J *j = JParse(json);

            THEN("nothing is leaked, whether or not it parsed") {
                // A few of these are accepted by this parser (it is lenient in
                // places). Either answer is fine; what must hold is that the
                // heap comes back clean.
                JDelete(j);
                INFO("input: " << json);
                CHECK(stats().liveBlocks == 0);
            }
        }
    }

    // The same inputs, with every allocation failing in turn: an error path
    // reached while ANOTHER error path is unwinding.
    for (size_t i = 0; i < sizeof(malformed) / sizeof(malformed[0]); ++i) {
        const char *json = malformed[i];
        if (strlen(json) < 6) {
            continue;   // too short to allocate anything interesting
        }

        WHEN(std::string("parsing ") + json + " under allocation failure") {
            installTrackedAllocator();
            J *probe = JParse(json);
            const size_t total = stats().mallocCalls;
            JDelete(probe);

            for (size_t ordinal = 1; ordinal <= total; ++ordinal) {
                installTrackedAllocator();
                failAllocationAt(ordinal);
                J *j = JParse(json);
                stopFailingAllocations();
                JDelete(j);
                INFO("input: " << json << ", failing allocation " << ordinal);
                CHECK(stats().liveBlocks == 0);
            }
            installTrackedAllocator();
        }
    }

    NoteSetFn(malloc, free, NULL, NULL);
}

}
