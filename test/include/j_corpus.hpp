/*!
 * @file j_corpus.hpp
 *
 * The document corpus and render modes, factored out of
 * j_layout_test_support.hpp so they can be compiled against a PRISTINE
 * historical build of the library.
 *
 * That is the whole reason this file is separate. An A/B between two builds of
 * the same rewritten parser and printer cannot detect a regression that both
 * builds share, so scripts/generate_corpus_golden.sh compiles this corpus
 * against `git archive HEAD` and records what the historical implementation
 * renders. The result is checked in as test/data/, and the layout suite
 * compares against it. Nothing here may depend on Catch2 or on anything newer
 * than the public note-c API, or that generator stops building.
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

#pragma once

#include <string>
#include <vector>

#include "n_lib.h"

namespace jlayout
{

// ---------------------------------------------------------------------------
// Corpus
// ---------------------------------------------------------------------------

struct Doc {
    const char *name;
    const char *json;
    // True when the document contains a number that does not survive a
    // text->float->text round trip under NOTE_C_SINGLE_PRECISION. This is a
    // pre-existing property of note-c's numeric conversion, identical in both
    // J layouts, so those documents are exempted from the idempotence,
    // structural-equality and digest checks in single-precision builds only.
    bool lossyUnderSinglePrecision;
};

// True when this build cannot round-trip the document's numbers.
inline bool numbersAreLossy(const Doc &d)
{
#ifdef NOTE_C_SINGLE_PRECISION
    return d.lossyUnderSinglePrecision;
#else
    (void)d;
    return false;
#endif
}

// Dozens of documents spanning every _print_value arm, both sides of the inline
// capacity boundary, key/value escape handling, deep and wide shapes, and the
// falsy shapes that drive omitempty.
inline const std::vector<Doc> &corpus()
{
    static const std::vector<Doc> docs = {
        // --- scalars at the root ---
        { "null", "null", false },
        { "true", "true", false },
        { "false", "false", false },
        { "zero", "0", false },
        { "negative-int", "-12345", false },
        { "big-int-exact", "9007199254740993", true },
        { "huge-int", "1234567890123456789", true },
        { "fraction", "3.25", true },
        { "negative-fraction", "-0.125", true },
        { "exp-small", "1.5e-7", true },
        { "string-short", "\"hi\"", false },
        { "string-empty", "\"\"", false },

        // --- inline capacity boundary (J_INLINE_CAP is 16 including NUL) ---
        { "val-len-14", "\"abcdefghijklmn\"", false },
        { "val-len-15", "\"abcdefghijklmno\"", false },
        { "val-len-16", "\"abcdefghijklmnop\"", false },
        { "val-len-17", "\"abcdefghijklmnopq\"", false },
        { "val-len-31", "\"abcdefghijklmnopqrstuvwxyz01234\"", false },
        { "key-len-14", "{\"abcdefghijklmn\":1}", false },
        { "key-len-15", "{\"abcdefghijklmno\":1}", false },
        { "key-len-16", "{\"abcdefghijklmnop\":1}", false },
        { "key-len-17", "{\"abcdefghijklmnopq\":1}", false },
        { "key-and-val-boundary", "{\"abcdefghijklmno\":\"abcdefghijklmno\"}", false },

        // --- escapes, in values and in keys ---
        { "escapes-short", "\"q\\\"w\\\\e\\nr\\tt\\b\\f\\r\"", false },
        { "escapes-control", "\"\\u0001\\u0002\\u0003\"", false },
        { "escapes-mixed", "\"a\\u0001b\\u0002c\"", false },
        { "escaped-key", "{\"a\\u0001b\":\"v\"}", false },
        { "escaped-key-quote", "{\"q\\\"k\":\"w\"}", false },
        { "utf8-value", "\"\\u00e9\\u20ac\"", false },

        // --- containers ---
        { "empty-object", "{}", false },
        { "empty-array", "[]", false },
        { "flat-array", "[1,2,3]", false },
        { "mixed-array", "[1,\"two\",false,3.25,null,{},[]]", true },
        { "simple-object", "{\"a\":\"b\"}", false },
        { "nested-object", "{\"a\":{\"b\":{\"c\":1}}}", false },
        { "object-of-arrays", "{\"a\":[1,[2,[3]]],\"b\":[]}", false },
        { "array-of-objects", "[{\"a\":1},{\"b\":2}]", false },

        // --- omitempty-relevant shapes ---
        { "all-falsy", "{\"a\":0,\"b\":false,\"c\":\"\"}", false },
        { "falsy-around", "{\"a\":0,\"b\":1,\"c\":0}", false },
        { "falsy-array", "[0,false,\"\",1]", false },
        { "nested-falsy", "{\"outer\":{\"a\":0,\"b\":2,\"c\":false},\"keep\":1,\"drop\":0}", false },

        // --- realistic Notecard traffic ---
        {
            "note-add", "{\"req\":\"note.add\",\"file\":\"sensors.qo\",\"sync\":true,"
            "\"body\":{\"temp\":22.5,\"humid\":48.2,\"volts\":3.31,\"count\":17}}",
            true
        },
        {
            "env-vars", "{\"body\":{\"a\":\"1\",\"b\":\"2\",\"c\":\"3\",\"d\":\"4\","
            "\"e\":\"5\",\"f\":\"6\",\"g\":\"7\",\"h\":\"8\"}}",
            false
        },
        {
            "note-changes", "{\"notes\":{\"a1\":{\"body\":{\"t\":1}},"
            "\"a2\":{\"body\":{\"t\":2}},\"a3\":{\"body\":{\"t\":3}}}}",
            false
        },
        {
            "card-status", "{\"status\":\"{normal}\",\"usb\":true,\"storage\":8,"
            "\"time\":1598479763,\"connected\":true}",
            true
        },
        {
            "web-post", "{\"req\":\"web.post\",\"route\":\"weather\",\"name\":\"/current\","
            "\"body\":{\"lat\":42.36,\"lon\":-71.06},\"content\":\"application/json\"}",
            true
        },
        {
            "long-string-value", "{\"body\":{\"msg\":\"this string is definitely longer "
            "than sixteen characters and forces the overflow path\"}}",
            false
        },
        {
            "many-members", "{\"k00\":0,\"k01\":1,\"k02\":2,\"k03\":3,\"k04\":4,\"k05\":5,"
            "\"k06\":6,\"k07\":7,\"k08\":8,\"k09\":9,\"k10\":10,\"k11\":11}",
            false
        },
        { "deep-nesting", "[[[[[[[[[[1]]]]]]]]]]", false },
        { "number-array-20", "{\"data\":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20]}", false },
        { "string-array", "[\"alpha\",\"beta\",\"gamma\",\"delta\",\"epsilon\"]", false },
    };
    return docs;
}

struct Mode {
    const char *name;
    Jbool format;
    Jbool omitempty;
};

inline const std::vector<Mode> &modes()
{
    static const std::vector<Mode> m = {
        { "compact", false, false },
        { "compact-omitempty", false, true },
        { "formatted", true, false },
        { "formatted-omitempty", true, true },
    };
    return m;
}

}
