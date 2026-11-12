/*!
 * @file JPublicApiCompat_test.cpp
 *
 * Pins the observable behavior of the public `J` macros that were converted to
 * function calls when NOTE_C_STORAGE_OPTIMIZATION was introduced.
 *
 * Converting a macro to a call is a silent source and behavior change unless
 * every property callers could have depended on is reproduced: the result type,
 * the result value, how many times each operand is evaluated, and the treatment
 * of NULL. The expectations here were captured by running the same program
 * against a pristine `git archive HEAD` build of the historical implementation,
 * not derived from the current code -- an A/B between two builds of a rewrite
 * cannot detect a change that both builds share.
 *
 * Every assertion holds in BOTH layouts. The file is C++ on purpose: it is also
 * the compile-time guard on the signatures. In C, handing a `const char *` to a
 * `char *` is a warning; in C++ it is ill-formed, so a regression that
 * re-qualified JGetObjectItemName's result would fail this build outright.
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

#include "n_lib.h"

// The C half of this check, compiled as C with -Werror. See
// test/src/j_public_api_compat_c.c for why a C++ translation unit alone is not
// sufficient to pin these contracts.
extern "C" int jPublicApiCompatC(void);

namespace
{

// Counts how many times the value operand of a setter macro is evaluated.
int operandEvaluations = 0;

JNUMBER countedOperand(JNUMBER value)
{
    ++operandEvaluations;
    return value;
}

// Counts how many times the object operand is evaluated. The historical
// JSetIntValue macro named it three times; the current conditional-plus-call
// form names it exactly twice, once in the test and once in the call.
// JSetNumberValue already named it twice and is unchanged.
int objectEvaluations = 0;

J *countedObject(J *item)
{
    ++objectEvaluations;
    return item;
}

SCENARIO("Public API compatibility: JSetIntValue")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    // The historical macro was
    //     ((object) ? (object)->valueint = (object)->valuenumber = (number) : (number))
    // which parses as `valueint = (valuenumber = number)`. Both members
    // therefore saw the operand in its ORIGINAL arithmetic form, so a
    // fractional operand landed truncated in valueint and intact in
    // valuenumber. This is the property a JINTEGER parameter would destroy.
    SECTION("A fractional operand truncates into valueint and survives in valuenumber") {
        J *item = JCreateNumber(0);
        REQUIRE(item != NULL);

        JSetIntValue(item, 1.5);
        CHECK(item->valueint == 1);
        CHECK(item->valuenumber == (JNUMBER)1.5);

        // Truncation is toward zero, as a C conversion, not a floor.
        JSetIntValue(item, -2.5);
        CHECK(item->valueint == -2);
        CHECK(item->valuenumber == (JNUMBER)-2.5);

        JDelete(item);
    }

    SECTION("An integral operand lands in both members") {
        J *item = JCreateNumber(0);
        REQUIRE(item != NULL);

        JSetIntValue(item, 42);
        CHECK(item->valueint == 42);
        CHECK(item->valuenumber == (JNUMBER)42);

        JSetIntValue(item, -42);
        CHECK(item->valueint == -42);
        CHECK(item->valuenumber == (JNUMBER)-42);

        JDelete(item);
    }

    SECTION("An unsigned operand is not sign-mangled") {
        J *item = JCreateNumber(0);
        REQUIRE(item != NULL);

        // Exactly representable as both float and double, and past INT32_MAX,
        // so a stray 32-bit intermediate would show up here.
        JSetIntValue(item, 4000000000u);
        CHECK(item->valueint == 4000000000LL);

        JDelete(item);
    }

    SECTION("An out-of-range operand saturates rather than being undefined") {
        J *item = JCreateNumber(0);
        REQUIRE(item != NULL);

        // The historical macro's implicit JNUMBER-to-JINTEGER conversion was
        // undefined here. Saturating matches JSetNumberValue, which has always
        // done this, and is the one deliberate divergence: it only affects
        // inputs for which the old macro had no defined result.
        JSetIntValue(item, (JNUMBER)1e30);
        CHECK(item->valueint == JINTEGER_MAX);

        JSetIntValue(item, (JNUMBER)-1e30);
        CHECK(item->valueint == JINTEGER_MIN);

        JDelete(item);
    }

    SECTION("A NULL object is a no-op that returns the operand UNCONVERTED") {
        // The historical macro short-circuited on NULL and yielded `(number)`
        // itself -- not the helper's return type. So the NULL branch keeps the
        // operand's value AND its type: 1.5 stays 1.5, as a double.
        //
        // Calling the helper unconditionally would yield 1 as a JINTEGER, and
        // under NOTE_C_SINGLE_PRECISION would round the operand through a float
        // on the way out. Verified against a pristine `git archive HEAD` build.
        CHECK(JSetIntValue((J *)NULL, 42) == 42);
        CHECK(JSetIntValue((J *)NULL, 1.5) == 1.5);
        CHECK(JSetNumberValue((J *)NULL, 4.5) == 4.5);

        // A value that is not exactly representable as a float, so a stray
        // conversion through JNUMBER shows up here in a single-precision build.
        CHECK(JSetIntValue((J *)NULL, 16777217.0) == 16777217.0);
        CHECK(JSetNumberValue((J *)NULL, 16777217.0) == 16777217.0);
    }

    SECTION("The value operand is evaluated exactly once") {
        J *item = JCreateNumber(0);
        REQUIRE(item != NULL);

        operandEvaluations = 0;
        JSetIntValue(item, countedOperand(7.25));
        CHECK(operandEvaluations == 1);
        CHECK(item->valueint == 7);
        CHECK(item->valuenumber == (JNUMBER)7.25);

        // Including on the NULL path, where the old macro took the other
        // branch of the conditional but still evaluated `number` just once --
        // and yielded it unconverted, so 7.25 rather than 7.
        operandEvaluations = 0;
        CHECK(JSetIntValue((J *)NULL, countedOperand(7.25)) == 7.25);
        CHECK(operandEvaluations == 1);

        JDelete(item);
    }

    SECTION("A side effect in the object operand is not lost") {
        J *item = JCreateNumber(0);
        REQUIRE(item != NULL);

        objectEvaluations = 0;
        JSetIntValue(countedObject(item), 9);
        CHECK(objectEvaluations == 2);      // was 3 historically
        CHECK(item->valueint == 9);

        objectEvaluations = 0;
        JSetNumberValue(countedObject(item), 4.5);
        CHECK(objectEvaluations == 2);      // unchanged from historical
        CHECK(item->valuenumber == (JNUMBER)4.5);

        JDelete(item);
    }

    SECTION("Setting a numeric value over a string item is safe") {
        // Under NOTE_C_STORAGE_OPTIMIZATION the bytes of valueint and
        // valuenumber may currently hold this item's packed value and key, so
        // the setter has to evacuate them before writing. Nothing about that is
        // visible from here except that the key survives and nothing leaks.
        J *object = JCreateObject();
        REQUIRE(object != NULL);
        REQUIRE(JAddStringToObject(object, "key", "a string value") != NULL);

        J *item = JGetObjectItem(object, "key");
        REQUIRE(item != NULL);

        JSetIntValue(item, 123);
        CHECK(item->valueint == 123);
        CHECK(strcmp(JGetItemName(item), "key") == 0);

        JDelete(object);
    }
}

SCENARIO("Public API compatibility: JSetNumberValue")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("The value lands in both members") {
        J *item = JCreateNumber(0);
        REQUIRE(item != NULL);

        JSetNumberValue(item, 1.5);
        CHECK(item->valuenumber == (JNUMBER)1.5);
        CHECK(item->valueint == 1);

        JDelete(item);
    }

    SECTION("An out-of-range value saturates valueint") {
        J *item = JCreateNumber(0);
        REQUIRE(item != NULL);

        JSetNumberValue(item, (JNUMBER)1e30);
        CHECK(item->valueint == JINTEGER_MAX);
        CHECK(item->valuenumber == (JNUMBER)1e30);

        JSetNumberValue(item, (JNUMBER)-1e30);
        CHECK(item->valueint == JINTEGER_MIN);

        JDelete(item);
    }

    SECTION("A NULL object is a no-op that returns the operand") {
        CHECK(JSetNumberValue((J *)NULL, 4.5) == (JNUMBER)4.5);
    }

    SECTION("The value operand is evaluated exactly once") {
        J *item = JCreateNumber(0);
        REQUIRE(item != NULL);

        operandEvaluations = 0;
        JSetNumberValue(item, countedOperand(3.5));
        CHECK(operandEvaluations == 1);
        CHECK(item->valuenumber == (JNUMBER)3.5);

        JDelete(item);
    }

    SECTION("Setting a numeric value over a string item is safe") {
        J *object = JCreateObject();
        REQUIRE(object != NULL);
        REQUIRE(JAddStringToObject(object, "key", "a string value") != NULL);

        J *item = JGetObjectItem(object, "key");
        REQUIRE(item != NULL);

        JSetNumberValue(item, 1.25);
        CHECK(item->valuenumber == (JNUMBER)1.25);
        CHECK(strcmp(JGetItemName(item), "key") == 0);

        JDelete(object);
    }
}

SCENARIO("Public API compatibility: the same contracts hold when compiled as C")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    // Most of this check is the fact that j_public_api_compat_c.c COMPILED at
    // all: it binds JGetObjectItemName's result to a mutable char*, assigns
    // through it, and takes its address, each of which stops compiling if the
    // macro is re-qualified or turned into a function call. The runtime call
    // below covers the value contracts from a C translation unit as well, in
    // case C and C++ ever diverge in argument promotion or macro expansion.
    SECTION("The C translation unit agrees") {
        CHECK(jPublicApiCompatC() == 0);
    }
}

SCENARIO("Public API compatibility: numeric state on a string node")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    // JSetIntValue/JSetNumberValue have always written the numeric members
    // WITHOUT changing the node's base type, so a JString or JRaw node can
    // carry numeric state. It was observable through the accessors, through
    // direct field access, and through JDuplicate.
    //
    // An earlier revision guarded JIntValue/JNumberValue/JDuplicate on "is this
    // a string type", because under NOTE_C_STORAGE_OPTIMIZATION those bytes may
    // be lending themselves to an inline value or key. That was too blunt: it
    // made the state unreadable even in the DEFAULT layout, where nothing is
    // ever overlaid. The guard is now on whether the members actually hold
    // numbers right now, which is unconditionally true in the default layout.
    //
    // Reference values captured from a pristine `git archive HEAD` build:
    //   fields=123/123.5  accessors=123/123.5  copy=123/123.5

    GIVEN("a string node with a number written onto it") {
        J *item = JCreateString("abc");
        REQUIRE(item != NULL);
        JSetIntValue(item, 123.5);

        THEN("the fields hold it") {
            CHECK(item->valueint == 123);
            CHECK(item->valuenumber == (JNUMBER)123.5);
        }
        THEN("the accessors report it") {
            CHECK(JIntValue(item) == 123);
            CHECK(JNumberValue(item) == (JNUMBER)123.5);
        }
        THEN("the string value is untouched") {
            CHECK(strcmp(JStringValue(item), "abc") == 0);
        }
        THEN("a duplicate carries it") {
            J *copy = JDuplicate(item, false);
            REQUIRE(copy != NULL);
            CHECK(copy->valueint == 123);
            CHECK(copy->valuenumber == (JNUMBER)123.5);
            CHECK(strcmp(JStringValue(copy), "abc") == 0);
            JDelete(copy);
        }

        JDelete(item);
    }

    GIVEN("a raw node with a number written onto it") {
        J *item = JCreateRaw("{\"x\":1}");
        REQUIRE(item != NULL);
        JSetNumberValue(item, 7.25);

        THEN("the accessors report it and the raw text survives") {
            CHECK(JNumberValue(item) == (JNUMBER)7.25);
            CHECK(JIntValue(item) == 7);
            CHECK(strcmp(item->valuestring, "{\"x\":1}") == 0);
        }

        JDelete(item);
    }

    GIVEN("a string node that has NOT had a number written onto it") {
        // Here the numeric members may genuinely be holding packed characters,
        // so the accessors must report zero rather than character data.
        J *object = JCreateObject();
        REQUIRE(object != NULL);
        REQUIRE(JAddStringToObject(object, "k", "packed") != NULL);
        J *item = JGetObjectItem(object, "k");
        REQUIRE(item != NULL);

        THEN("the accessors report zero, never packed bytes") {
            CHECK(JIntValue(item) == 0);
            CHECK(JNumberValue(item) == 0.0);
        }
        THEN("re-keying after a setter keeps both the key and the number") {
            JSetIntValue(item, 55);
            CHECK(JIntValue(item) == 55);
            J *moved = JDetachItemFromObject(object, "k");
            REQUIRE(moved != NULL);
            J *dest = JCreateObject();
            REQUIRE(dest != NULL);
            JAddItemToObject(dest, "a-much-longer-replacement-key", moved);
            CHECK(strcmp(JGetItemName(moved), "a-much-longer-replacement-key") == 0);
            CHECK(JIntValue(moved) == 55);
            JDelete(dest);
        }

        JDelete(object);
    }
}

SCENARIO("Public API compatibility: numeric state survives copying, re-keying and referencing")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    // Whether a string node's numeric bytes hold NUMBERS or packed CHARACTERS
    // cannot be inferred from "is an inline pointer currently aimed at them",
    // and an earlier revision that inferred it was wrong in both directions:
    //
    //   FALSE POSITIVE. _create_reference() copies a node's bytes verbatim and
    //   then clears the inline flags, since a reference borrows rather than
    //   owns. The copied CHARACTERS stayed in the numeric region unreferenced,
    //   so a reference to JCreateString("abc") reported JIntValue() == 6513249
    //   -- the bytes of "abc" read as an integer.
    //
    //   DESTRUCTION. After a setter evacuated the region and wrote a number,
    //   the region looked free again, so _j_key_set() reclaimed it for a SHORT
    //   replacement key and overwrote the number. A long key does not reach
    //   this path, which is why the earlier re-key test missed it.
    //
    // JNumericLive records the fact instead. Reference values from a pristine
    // `git archive HEAD` build:
    //   duplicate accessor=55   rekey accessor=55   reference source=0/0 ref=0/0

    GIVEN("a packed string member with a number written onto it") {
        J *owner = JCreateObject();
        REQUIRE(owner != NULL);
        J *item = JAddStringToObject(owner, "k", "packed");
        REQUIRE(item != NULL);
        JSetIntValue(item, 55);
        REQUIRE(JIntValue(item) == 55);

        THEN("a duplicate keeps the number, the key and the string") {
            J *copy = JDuplicate(item, false);
            REQUIRE(copy != NULL);
            CHECK(JIntValue(copy) == 55);
            REQUIRE(JGetItemName(copy) != NULL);
            CHECK(strcmp(JGetItemName(copy), "k") == 0);
            CHECK(strcmp(JStringValue(copy), "packed") == 0);
            JDelete(copy);
        }

        THEN("a SHORT replacement key does not reclaim the number's bytes") {
            J *moved = JDetachItemFromObject(owner, "k");
            REQUIRE(moved != NULL);
            J *dest = JCreateObject();
            REQUIRE(dest != NULL);
            JAddItemToObject(dest, "x", moved);
            CHECK(JIntValue(moved) == 55);
            REQUIRE(JGetItemName(moved) != NULL);
            CHECK(strcmp(JGetItemName(moved), "x") == 0);
            CHECK(strcmp(JStringValue(moved), "packed") == 0);
            JDelete(dest);
        }

        THEN("replacement keys of every length across the inline boundary are safe") {
            for (size_t len = 1; len <= 24; ++len) {
                J *o2 = JCreateObject();
                REQUIRE(o2 != NULL);
                J *m2 = JAddStringToObject(o2, "k", "packed");
                REQUIRE(m2 != NULL);
                JSetIntValue(m2, 55);
                m2 = JDetachItemFromObject(o2, "k");
                REQUIRE(m2 != NULL);

                std::string key(len, 'q');
                J *d2 = JCreateObject();
                REQUIRE(d2 != NULL);
                JAddItemToObject(d2, key.c_str(), m2);

                INFO("replacement key length = " << len);
                CHECK(JIntValue(m2) == 55);
                CHECK(strcmp(JGetItemName(m2), key.c_str()) == 0);
                CHECK(strcmp(JStringValue(m2), "packed") == 0);

                JDelete(d2);
                JDelete(o2);
            }
        }

        JDelete(owner);
    }

    GIVEN("a reference to an untouched packed string") {
        J *source = JCreateString("abc");
        REQUIRE(source != NULL);
        J *references = JCreateArray();
        REQUIRE(references != NULL);
        JAddItemReferenceToArray(references, source);
        J *reference = JGetArrayItem(references, 0);
        REQUIRE(reference != NULL);

        THEN("neither the source nor the reference fabricates a number") {
            // The characters of "abc" occupy the numeric region of the source,
            // and _create_reference copied them into the reference.
            CHECK(JIntValue(source) == 0);
            CHECK(JNumberValue(source) == 0.0);
            CHECK(JIntValue(reference) == 0);
            CHECK(JNumberValue(reference) == 0.0);
        }
        THEN("the reference still renders as the string") {
            CHECK(strcmp(JStringValue(reference), "abc") == 0);
        }

        JDelete(references);
        JDelete(source);
    }

    GIVEN("a reference to a string node that DOES hold a number") {
        J *owner = JCreateObject();
        REQUIRE(owner != NULL);
        J *source = JAddStringToObject(owner, "k", "packed");
        REQUIRE(source != NULL);
        JSetIntValue(source, 55);

        J *references = JCreateArray();
        REQUIRE(references != NULL);
        JAddItemReferenceToArray(references, source);
        J *reference = JGetArrayItem(references, 0);
        REQUIRE(reference != NULL);

        THEN("the reference reports the same number as the source") {
            CHECK(JIntValue(source) == 55);
            CHECK(JIntValue(reference) == 55);
        }

        JDelete(references);
        JDelete(owner);
    }
}

SCENARIO("Public API compatibility: a key that aliases the one already held")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    // The documented "move a member, keep its name" idiom hands a node its own
    // key back. An interior pointer into that key is just as reachable, and
    // under NOTE_C_STORAGE_OPTIMIZATION the key may live in the node's own
    // allocation -- so the destination the new key is copied to can OVERLAP the
    // source. That was an ASan-confirmed memcpy-param-overlap; it is a memmove
    // now. Exercised across the inline boundary so both storage classes are hit.
    const char *base = "a-long-key-past-the-inline-region";

    SECTION("exact alias") {
        J *src = JCreateObject();
        REQUIRE(src != NULL);
        J *item = JAddNumberToObject(src, base, 1);
        REQUIRE(item != NULL);
        item = JDetachItemFromObject(src, base);
        REQUIRE(item != NULL);
        J *dst = JCreateObject();
        REQUIRE(dst != NULL);
        JAddItemToObject(dst, JGetItemName(item), item);
        CHECK(strcmp(JGetItemName(item), base) == 0);
        JDelete(dst);
        JDelete(src);
    }

    SECTION("prefix-shifted aliases across the inline boundary") {
        for (size_t skip = 1; skip <= 20; ++skip) {
            J *src = JCreateObject();
            REQUIRE(src != NULL);
            J *item = JAddNumberToObject(src, base, 1);
            REQUIRE(item != NULL);
            item = JDetachItemFromObject(src, base);
            REQUIRE(item != NULL);

            J *dst = JCreateObject();
            REQUIRE(dst != NULL);
            JAddItemToObject(dst, JGetItemName(item) + skip, item);

            INFO("skip = " << skip);
            CHECK(strcmp(JGetItemName(item), base + skip) == 0);
            CHECK(JIntValue(item) == 1);

            JDelete(dst);
            JDelete(src);
        }
    }

    SECTION("alias of a short inline key") {
        J *src = JCreateObject();
        REQUIRE(src != NULL);
        J *item = JAddStringToObject(src, "abcd", "v");
        REQUIRE(item != NULL);
        item = JDetachItemFromObject(src, "abcd");
        REQUIRE(item != NULL);
        J *dst = JCreateObject();
        REQUIRE(dst != NULL);
        JAddItemToObject(dst, JGetItemName(item) + 1, item);
        CHECK(strcmp(JGetItemName(item), "bcd") == 0);
        CHECK(strcmp(JStringValue(item), "v") == 0);
        JDelete(dst);
        JDelete(src);
    }
}

SCENARIO("Public API compatibility: JGetObjectItemName")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("An item with no key yields NULL, not the empty string") {
        // JGetItemName() substitutes "" for a missing key. This macro must not,
        // or every historical `if (JGetObjectItemName(x) != NULL)` silently
        // inverts.
        J *array = JCreateArray();
        REQUIRE(array != NULL);
        JAddItemToArray(array, JCreateNumber(1));
        REQUIRE(array->child != NULL);

        CHECK(JGetObjectItemName(array->child) == NULL);
        CHECK(JGetObjectItemName(array) == NULL);

        JDelete(array);
    }

    SECTION("An item with a key yields the key") {
        J *object = JCreateObject();
        REQUIRE(object != NULL);
        REQUIRE(JAddNumberToObject(object, "key", 1) != NULL);

        CHECK(strcmp(JGetObjectItemName(object->child), "key") == 0);

        JDelete(object);
    }

    SECTION("The result is a mutable char* lvalue") {
        J *object = JCreateObject();
        REQUIRE(object != NULL);
        REQUIRE(JAddNumberToObject(object, "key", 1) != NULL);

        J *item = object->child;

        // Binding to `char *` is the compile-time assertion; in C++ this line
        // does not compile at all if the result is re-qualified as const.
        char *name = JGetObjectItemName(item);
        REQUIRE(name != NULL);
        CHECK(strcmp(name, "key") == 0);

        // Taking its address proves it is still an lvalue -- the historical
        // macro expanded to a member access, so `JGetObjectItemName(x) = p`
        // was legal. Assigning through it here would orphan the old key, so
        // the address is taken and discarded rather than written.
        char **slot = &JGetObjectItemName(item);
        CHECK(*slot == name);

        // Writing THROUGH the pointer was also legal, and remains so in both
        // layouts even when the key is packed into the node's own allocation.
        name[0] = 'K';
        CHECK(strcmp(JGetItemName(item), "Key") == 0);

        JDelete(object);
    }
}

}
