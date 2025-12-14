/*!
 * @file JObjectf_test.cpp
 *
 * Written by the Blues Inc. team.
 *
 * Copyright (c) 2024 Blues Inc. MIT License. Use of this source code is
 * governed by licenses granted by the copyright holder including that found in
 * the
 * <a href="https://github.com/blues/note-c/blob/master/LICENSE">LICENSE</a>
 * file.
 *
 */

#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <cmath>

#include "n_lib.h"

namespace
{

// ==========================================================================
// NULL AND EMPTY INPUT HANDLING
// ==========================================================================

SCENARIO("JObjectf: NULL and empty input")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("NULL format returns empty object") {
        J *obj = JObjectf(NULL);
        REQUIRE(obj != NULL);
        CHECK(obj->child == NULL);
        JDelete(obj);
    }

    SECTION("Empty format string returns empty object") {
        J *obj = JObjectf("");
        REQUIRE(obj != NULL);
        CHECK(obj->child == NULL);
        JDelete(obj);
    }

    SECTION("Whitespace-only format returns empty object") {
        J *obj = JObjectf("   \t\n\r  ");
        REQUIRE(obj != NULL);
        CHECK(obj->child == NULL);
        JDelete(obj);
    }
}

// ==========================================================================
// VALUE FORMAT SPECIFIER: %s (STRING)
// ==========================================================================

SCENARIO("JObjectf: %s string format specifier")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("Valid string") {
        J *obj = JObjectf("name:%s", "Alice");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "name"), "Alice") == 0);
        JDelete(obj);
    }

    SECTION("Empty string") {
        J *obj = JObjectf("name:%s", "");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "name"), "") == 0);
        JDelete(obj);
    }

    SECTION("String with spaces") {
        J *obj = JObjectf("msg:%s", "hello world");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "msg"), "hello world") == 0);
        JDelete(obj);
    }

    SECTION("NULL string argument skips field") {
        J *obj = JObjectf("before:%d name:%s after:%d", (JINTEGER)1, (const char *)NULL, (JINTEGER)2);
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "before") == 1);
        CHECK(!JIsPresent(obj, "name"));
        CHECK(JGetInt(obj, "after") == 2);
        JDelete(obj);
    }

    SECTION("Multiple strings") {
        J *obj = JObjectf("first:%s last:%s", "John", "Doe");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "first"), "John") == 0);
        CHECK(strcmp(JGetString(obj, "last"), "Doe") == 0);
        JDelete(obj);
    }
}

// ==========================================================================
// VALUE FORMAT SPECIFIER: %d (INTEGER)
// ==========================================================================

SCENARIO("JObjectf: %d integer format specifier")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("Positive integer") {
        J *obj = JObjectf("count:%d", (JINTEGER)42);
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "count") == 42);
        JDelete(obj);
    }

    SECTION("Negative integer") {
        J *obj = JObjectf("offset:%d", (JINTEGER)-100);
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "offset") == -100);
        JDelete(obj);
    }

    SECTION("Zero") {
        J *obj = JObjectf("zero:%d", (JINTEGER)0);
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "zero") == 0);
        JDelete(obj);
    }

    SECTION("Large positive integer") {
        J *obj = JObjectf("big:%d", (JINTEGER)1000000000LL);
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "big") == 1000000000LL);
        JDelete(obj);
    }

    SECTION("Large negative integer") {
        J *obj = JObjectf("neg:%d", (JINTEGER)-1000000000LL);
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "neg") == -1000000000LL);
        JDelete(obj);
    }
}

// ==========================================================================
// VALUE FORMAT SPECIFIER: %f (FLOATING POINT)
// ==========================================================================

SCENARIO("JObjectf: %f floating point format specifier")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("Positive float") {
        J *obj = JObjectf("temp:%f", (JNUMBER)98.6);
        REQUIRE(obj != NULL);
        CHECK(std::abs(JGetNumber(obj, "temp") - 98.6) < 0.0001);
        JDelete(obj);
    }

    SECTION("Negative float") {
        J *obj = JObjectf("temp:%f", (JNUMBER)-40.5);
        REQUIRE(obj != NULL);
        CHECK(std::abs(JGetNumber(obj, "temp") - (-40.5)) < 0.0001);
        JDelete(obj);
    }

    SECTION("Zero float") {
        J *obj = JObjectf("zero:%f", (JNUMBER)0.0);
        REQUIRE(obj != NULL);
        CHECK(JGetNumber(obj, "zero") == 0.0);
        JDelete(obj);
    }

    SECTION("Small decimal") {
        J *obj = JObjectf("small:%f", (JNUMBER)0.001);
        REQUIRE(obj != NULL);
        CHECK(std::abs(JGetNumber(obj, "small") - 0.001) < 0.00001);
        JDelete(obj);
    }

    SECTION("Pi approximation") {
        J *obj = JObjectf("pi:%f", (JNUMBER)3.14159265359);
        REQUIRE(obj != NULL);
        CHECK(std::abs(JGetNumber(obj, "pi") - 3.14159265359) < 0.0000001);
        JDelete(obj);
    }
}

// ==========================================================================
// VALUE FORMAT SPECIFIER: %b (BOOLEAN)
// ==========================================================================

SCENARIO("JObjectf: %b boolean format specifier")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("Boolean true (1)") {
        J *obj = JObjectf("active:%b", 1);
        REQUIRE(obj != NULL);
        CHECK(JGetBool(obj, "active") == true);
        JDelete(obj);
    }

    SECTION("Boolean false (0)") {
        J *obj = JObjectf("active:%b", 0);
        REQUIRE(obj != NULL);
        CHECK(JGetBool(obj, "active") == false);
        JDelete(obj);
    }

    SECTION("Non-zero treated as true") {
        J *obj = JObjectf("active:%b", 42);
        REQUIRE(obj != NULL);
        CHECK(JGetBool(obj, "active") == true);
        JDelete(obj);
    }

    SECTION("Negative non-zero treated as true") {
        J *obj = JObjectf("active:%b", -1);
        REQUIRE(obj != NULL);
        CHECK(JGetBool(obj, "active") == true);
        JDelete(obj);
    }
}

// ==========================================================================
// VALUE FORMAT SPECIFIER: %o and %a (OBJECT/ARRAY)
// ==========================================================================

SCENARIO("JObjectf: %o object format specifier")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("Valid object is moved") {
        J *inner = JCreateObject();
        REQUIRE(inner != NULL);
        JAddStringToObject(inner, "key", "value");

        J *obj = JObjectf("data:%o", inner);
        REQUIRE(obj != NULL);

        J *data = JGetObject(obj, "data");
        REQUIRE(data != NULL);
        CHECK(strcmp(JGetString(data, "key"), "value") == 0);

        // inner is now owned by obj, only delete obj
        JDelete(obj);
    }

    SECTION("NULL object skips field") {
        J *obj = JObjectf("before:%d data:%o after:%d", (JINTEGER)1, (J *)NULL, (JINTEGER)2);
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "before") == 1);
        CHECK(!JIsPresent(obj, "data"));
        CHECK(JGetInt(obj, "after") == 2);
        JDelete(obj);
    }

    SECTION("Array via %o") {
        J *arr = JCreateArray();
        REQUIRE(arr != NULL);
        JAddItemToArray(arr, JCreateNumber(1));
        JAddItemToArray(arr, JCreateNumber(2));
        JAddItemToArray(arr, JCreateNumber(3));

        J *obj = JObjectf("items:%o", arr);
        REQUIRE(obj != NULL);

        J *items = JGetArray(obj, "items");
        REQUIRE(items != NULL);
        CHECK(JGetArraySize(items) == 3);

        JDelete(obj);
    }

    SECTION("Nested objects") {
        J *level2 = JObjectf("c:3");
        J *level1 = JObjectf("b:2 nested:%o", level2);
        J *obj = JObjectf("a:1 inner:%o", level1);

        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);

        J *inner = JGetObject(obj, "inner");
        REQUIRE(inner != NULL);
        CHECK(JGetInt(inner, "b") == 2);

        J *nested = JGetObject(inner, "nested");
        REQUIRE(nested != NULL);
        CHECK(JGetInt(nested, "c") == 3);

        JDelete(obj);
    }
}

SCENARIO("JObjectf: %a array format specifier (synonym for %o)")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("Array via %a") {
        J *arr = JCreateArray();
        REQUIRE(arr != NULL);
        JAddItemToArray(arr, JCreateString("one"));
        JAddItemToArray(arr, JCreateString("two"));

        J *obj = JObjectf("values:%a", arr);
        REQUIRE(obj != NULL);

        J *values = JGetArray(obj, "values");
        REQUIRE(values != NULL);
        CHECK(JGetArraySize(values) == 2);

        JDelete(obj);
    }

    SECTION("NULL via %a skips field") {
        J *obj = JObjectf("arr:%a", (J *)NULL);
        REQUIRE(obj != NULL);
        CHECK(!JIsPresent(obj, "arr"));
        JDelete(obj);
    }
}

// ==========================================================================
// LITERAL BOOLEAN VALUES
// ==========================================================================

SCENARIO("JObjectf: literal boolean values")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("true literal") {
        J *obj = JObjectf("flag:true");
        REQUIRE(obj != NULL);
        J *item = JGetObjectItem(obj, "flag");
        REQUIRE(item != NULL);
        CHECK(JIsBool(item));
        CHECK(JIsTrue(item));
        JDelete(obj);
    }

    SECTION("false literal") {
        J *obj = JObjectf("flag:false");
        REQUIRE(obj != NULL);
        J *item = JGetObjectItem(obj, "flag");
        REQUIRE(item != NULL);
        CHECK(JIsBool(item));
        CHECK(JIsFalse(item));
        JDelete(obj);
    }

    SECTION("true followed by other chars is string") {
        J *obj = JObjectf("word:trueish");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "word"), "trueish") == 0);
        JDelete(obj);
    }

    SECTION("false followed by other chars is string") {
        J *obj = JObjectf("word:falsehood");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "word"), "falsehood") == 0);
        JDelete(obj);
    }

    SECTION("TRUE (uppercase) is unquoted string, not boolean") {
        J *obj = JObjectf("word:TRUE");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "word"), "TRUE") == 0);
        JDelete(obj);
    }

    SECTION("FALSE (uppercase) is unquoted string, not boolean") {
        J *obj = JObjectf("word:FALSE");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "word"), "FALSE") == 0);
        JDelete(obj);
    }

    SECTION("Multiple booleans") {
        J *obj = JObjectf("a:true b:false c:true");
        REQUIRE(obj != NULL);
        CHECK(JGetBool(obj, "a") == true);
        CHECK(JGetBool(obj, "b") == false);
        CHECK(JGetBool(obj, "c") == true);
        JDelete(obj);
    }
}

// ==========================================================================
// LITERAL INTEGER VALUES
// ==========================================================================

SCENARIO("JObjectf: literal integer values")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("Zero") {
        J *obj = JObjectf("n:0");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "n") == 0);
        JDelete(obj);
    }

    SECTION("Single digit") {
        J *obj = JObjectf("n:7");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "n") == 7);
        JDelete(obj);
    }

    SECTION("Multi-digit") {
        J *obj = JObjectf("n:12345");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "n") == 12345);
        JDelete(obj);
    }

    SECTION("Negative integer") {
        J *obj = JObjectf("n:-42");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "n") == -42);
        JDelete(obj);
    }

    SECTION("Negative multi-digit") {
        J *obj = JObjectf("n:-98765");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "n") == -98765);
        JDelete(obj);
    }

    SECTION("Multiple integers") {
        J *obj = JObjectf("a:1 b:2 c:3");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(JGetInt(obj, "b") == 2);
        CHECK(JGetInt(obj, "c") == 3);
        JDelete(obj);
    }
}

// ==========================================================================
// LITERAL FLOAT VALUES
// ==========================================================================

SCENARIO("JObjectf: literal float values")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("Simple decimal") {
        J *obj = JObjectf("n:3.14");
        REQUIRE(obj != NULL);
        CHECK(std::abs(JGetNumber(obj, "n") - 3.14) < 0.001);
        JDelete(obj);
    }

    SECTION("Zero point something") {
        J *obj = JObjectf("n:0.5");
        REQUIRE(obj != NULL);
        CHECK(std::abs(JGetNumber(obj, "n") - 0.5) < 0.001);
        JDelete(obj);
    }

    SECTION("Negative float") {
        J *obj = JObjectf("n:-2.718");
        REQUIRE(obj != NULL);
        CHECK(std::abs(JGetNumber(obj, "n") - (-2.718)) < 0.001);
        JDelete(obj);
    }

    SECTION("Zero point zero") {
        J *obj = JObjectf("n:0.0");
        REQUIRE(obj != NULL);
        CHECK(JGetNumber(obj, "n") == 0.0);
        JDelete(obj);
    }

    SECTION("Integer with decimal is float") {
        J *obj = JObjectf("n:42.0");
        REQUIRE(obj != NULL);
        CHECK(JGetNumber(obj, "n") == 42.0);
        JDelete(obj);
    }

    SECTION("Multiple floats") {
        J *obj = JObjectf("lat:40.7128 lon:-74.0060");
        REQUIRE(obj != NULL);
        CHECK(std::abs(JGetNumber(obj, "lat") - 40.7128) < 0.0001);
        CHECK(std::abs(JGetNumber(obj, "lon") - (-74.0060)) < 0.0001);
        JDelete(obj);
    }
}

// ==========================================================================
// SINGLE-QUOTED STRING LITERALS
// ==========================================================================

SCENARIO("JObjectf: single-quoted string literals")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("Simple string") {
        J *obj = JObjectf("msg:'hello'");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "msg"), "hello") == 0);
        JDelete(obj);
    }

    SECTION("String with spaces") {
        J *obj = JObjectf("msg:'hello world'");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "msg"), "hello world") == 0);
        JDelete(obj);
    }

    SECTION("Empty string") {
        J *obj = JObjectf("msg:''");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "msg"), "") == 0);
        JDelete(obj);
    }

    SECTION("Escaped single quote") {
        J *obj = JObjectf("msg:'it\\'s ok'");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "msg"), "it's ok") == 0);
        JDelete(obj);
    }

    SECTION("Escaped backslash") {
        J *obj = JObjectf("path:'c:\\\\dir'");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "path"), "c:\\dir") == 0);
        JDelete(obj);
    }

    SECTION("Multiple escaped quotes") {
        J *obj = JObjectf("msg:'\\'a\\' and \\'b\\''");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "msg"), "'a' and 'b'") == 0);
        JDelete(obj);
    }

    SECTION("Contains double quote (no escape needed)") {
        J *obj = JObjectf("msg:'say \"hi\"'");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "msg"), "say \"hi\"") == 0);
        JDelete(obj);
    }
}

// ==========================================================================
// DOUBLE-QUOTED STRING LITERALS
// ==========================================================================

SCENARIO("JObjectf: double-quoted string literals")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("Simple string") {
        J *obj = JObjectf("msg:\"hello\"");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "msg"), "hello") == 0);
        JDelete(obj);
    }

    SECTION("String with spaces") {
        J *obj = JObjectf("msg:\"hello world\"");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "msg"), "hello world") == 0);
        JDelete(obj);
    }

    SECTION("Empty string") {
        J *obj = JObjectf("msg:\"\"");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "msg"), "") == 0);
        JDelete(obj);
    }

    SECTION("Escaped double quote") {
        J *obj = JObjectf("msg:\"say \\\"hi\\\"\"");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "msg"), "say \"hi\"") == 0);
        JDelete(obj);
    }

    SECTION("Escaped backslash") {
        J *obj = JObjectf("path:\"c:\\\\dir\"");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "path"), "c:\\dir") == 0);
        JDelete(obj);
    }

    SECTION("Contains single quote (no escape needed)") {
        J *obj = JObjectf("msg:\"it's ok\"");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "msg"), "it's ok") == 0);
        JDelete(obj);
    }
}

// ==========================================================================
// UNQUOTED STRING VALUES
// ==========================================================================

SCENARIO("JObjectf: unquoted string values")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("Simple word") {
        J *obj = JObjectf("status:ok");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "status"), "ok") == 0);
        JDelete(obj);
    }

    SECTION("Longer word") {
        J *obj = JObjectf("mode:continuous");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "mode"), "continuous") == 0);
        JDelete(obj);
    }

    SECTION("With dot (domain-style)") {
        J *obj = JObjectf("product:com.blues.app");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "product"), "com.blues.app") == 0);
        JDelete(obj);
    }

    SECTION("With dot (file extension)") {
        J *obj = JObjectf("file:data.qo");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "file"), "data.qo") == 0);
        JDelete(obj);
    }

    SECTION("API endpoint style") {
        J *obj = JObjectf("req:hub.set");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "req"), "hub.set") == 0);
        JDelete(obj);
    }

    SECTION("With underscore") {
        J *obj = JObjectf("key:my_value");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "key"), "my_value") == 0);
        JDelete(obj);
    }

    SECTION("With trailing digits") {
        J *obj = JObjectf("name:sensor123");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "name"), "sensor123") == 0);
        JDelete(obj);
    }

    SECTION("Mixed dots and underscores") {
        J *obj = JObjectf("id:device_123.sensor_a");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "id"), "device_123.sensor_a") == 0);
        JDelete(obj);
    }

    SECTION("Multiple unquoted strings") {
        J *obj = JObjectf("a:one b:two c:three");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "a"), "one") == 0);
        CHECK(strcmp(JGetString(obj, "b"), "two") == 0);
        CHECK(strcmp(JGetString(obj, "c"), "three") == 0);
        JDelete(obj);
    }
}

// ==========================================================================
// FIELD NAME PARSING
// ==========================================================================

SCENARIO("JObjectf: field name parsing")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("Simple literal name") {
        J *obj = JObjectf("name:value");
        REQUIRE(obj != NULL);
        CHECK(JIsPresent(obj, "name"));
        JDelete(obj);
    }

    SECTION("Name with underscore") {
        J *obj = JObjectf("my_field:value");
        REQUIRE(obj != NULL);
        CHECK(JIsPresent(obj, "my_field"));
        JDelete(obj);
    }

    SECTION("Name starting with underscore") {
        J *obj = JObjectf("_private:%d _internal:%s", (JINTEGER)42, "data");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "_private") == 42);
        CHECK(strcmp(JGetString(obj, "_internal"), "data") == 0);
        JDelete(obj);
    }

    SECTION("Name with multiple underscores") {
        J *obj = JObjectf("__dunder__:true _a_b_c_:%d", (JINTEGER)99);
        REQUIRE(obj != NULL);
        CHECK(JGetBool(obj, "__dunder__") == true);
        CHECK(JGetInt(obj, "_a_b_c_") == 99);
        JDelete(obj);
    }

    SECTION("Name with digits") {
        J *obj = JObjectf("field1:value");
        REQUIRE(obj != NULL);
        CHECK(JIsPresent(obj, "field1"));
        JDelete(obj);
    }

    SECTION("Name starting with uppercase") {
        J *obj = JObjectf("MyField:value");
        REQUIRE(obj != NULL);
        CHECK(JIsPresent(obj, "MyField"));
        JDelete(obj);
    }

    SECTION("%s field name from argument") {
        J *obj = JObjectf("%s:%d", "dynamic", (JINTEGER)42);
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "dynamic") == 42);
        JDelete(obj);
    }

    SECTION("Multiple %s field names") {
        J *obj = JObjectf("%s:%d %s:%d", "x", (JINTEGER)10, "y", (JINTEGER)20);
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "x") == 10);
        CHECK(JGetInt(obj, "y") == 20);
        JDelete(obj);
    }

    SECTION("Mixed literal and %s field names") {
        J *obj = JObjectf("literal:%d %s:%d", (JINTEGER)1, "dynamic", (JINTEGER)2);
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "literal") == 1);
        CHECK(JGetInt(obj, "dynamic") == 2);
        JDelete(obj);
    }
}

// ==========================================================================
// SEPARATOR AND WHITESPACE HANDLING
// ==========================================================================

SCENARIO("JObjectf: separator and whitespace handling")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("Space separator") {
        J *obj = JObjectf("a:1 b:2");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(JGetInt(obj, "b") == 2);
        JDelete(obj);
    }

    SECTION("Tab separator") {
        J *obj = JObjectf("a:1\tb:2");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(JGetInt(obj, "b") == 2);
        JDelete(obj);
    }

    SECTION("Newline separator") {
        J *obj = JObjectf("a:1\nb:2");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(JGetInt(obj, "b") == 2);
        JDelete(obj);
    }

    SECTION("Carriage return separator") {
        J *obj = JObjectf("a:1\rb:2");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(JGetInt(obj, "b") == 2);
        JDelete(obj);
    }

    SECTION("Comma separator") {
        J *obj = JObjectf("a:1,b:2");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(JGetInt(obj, "b") == 2);
        JDelete(obj);
    }

    SECTION("Comma with space (JSON-like)") {
        J *obj = JObjectf("a:1, b:2, c:3");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(JGetInt(obj, "b") == 2);
        CHECK(JGetInt(obj, "c") == 3);
        JDelete(obj);
    }

    SECTION("Multiple spaces") {
        J *obj = JObjectf("a:1    b:2");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(JGetInt(obj, "b") == 2);
        JDelete(obj);
    }

    SECTION("Mixed separators") {
        J *obj = JObjectf("a:1, \t\n b:2");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(JGetInt(obj, "b") == 2);
        JDelete(obj);
    }

    SECTION("Leading whitespace") {
        J *obj = JObjectf("   a:1");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        JDelete(obj);
    }

    SECTION("Trailing whitespace") {
        J *obj = JObjectf("a:1   ");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        JDelete(obj);
    }
}

// ==========================================================================
// ERROR CASES AND EDGE CASES
// ==========================================================================

SCENARIO("JObjectf: error and edge cases")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("Missing colon stops parsing, returns partial") {
        J *obj = JObjectf("a:1 badfield b:2");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(!JIsPresent(obj, "badfield"));
        CHECK(!JIsPresent(obj, "b"));
        JDelete(obj);
    }

    SECTION("Missing value stops parsing") {
        J *obj = JObjectf("a:1 b:");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(!JIsPresent(obj, "b"));
        JDelete(obj);
    }

    SECTION("Unknown format specifier stops parsing") {
        J *obj = JObjectf("a:1 b:%x", 42);
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(!JIsPresent(obj, "b"));
        JDelete(obj);
    }

    SECTION("Duplicate field names - later overwrites") {
        J *obj = JObjectf("x:1 x:2 x:3");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "x") == 3);
        JDelete(obj);
    }

    SECTION("Single field") {
        J *obj = JObjectf("single:42");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "single") == 42);
        JDelete(obj);
    }

    SECTION("Many fields") {
        J *obj = JObjectf("a:1 b:2 c:3 d:4 e:5 f:6 g:7 h:8 i:9 j:10");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(JGetInt(obj, "e") == 5);
        CHECK(JGetInt(obj, "j") == 10);
        JDelete(obj);
    }

    SECTION("Unterminated single-quoted string stops parsing") {
        J *obj = JObjectf("a:1 b:'unterminated");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(!JIsPresent(obj, "b"));
        JDelete(obj);
    }

    SECTION("Unterminated double-quoted string stops parsing") {
        J *obj = JObjectf("a:1 b:\"unterminated");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(!JIsPresent(obj, "b"));
        JDelete(obj);
    }

    SECTION("Just minus sign as value stops parsing") {
        J *obj = JObjectf("a:1 b:-");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(!JIsPresent(obj, "b"));
        JDelete(obj);
    }

    SECTION("Minus followed by non-digit is unquoted string") {
        // Actually, looking at the parser, a leading minus only works with digits
        // So "b:-x" should fail to parse as number and fall through
        J *obj = JObjectf("a:1 b:-x");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        // -x is not a valid unquoted string (doesn't start with letter)
        CHECK(!JIsPresent(obj, "b"));
        JDelete(obj);
    }

    SECTION("Lone decimal point as value stops parsing") {
        J *obj = JObjectf("a:1 b:.");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(!JIsPresent(obj, "b"));
        JDelete(obj);
    }

    SECTION("Number with trailing dot parsed as float") {
        J *obj = JObjectf("n:42.");
        REQUIRE(obj != NULL);
        // 42. is a valid float literal
        CHECK(JGetNumber(obj, "n") == 42.0);
        JDelete(obj);
    }

    SECTION("Backslash at end of quoted string") {
        // Backslash at end with no following char - unterminated
        J *obj = JObjectf("a:1 b:'test\\");
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        CHECK(!JIsPresent(obj, "b"));
        JDelete(obj);
    }

    SECTION("Double backslash in quoted string") {
        J *obj = JObjectf("msg:'path\\\\to\\\\file'");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "msg"), "path\\to\\file") == 0);
        JDelete(obj);
    }

    SECTION("NULL %s field name stops parsing") {
        J *obj = JObjectf("a:1 %s:2", (const char *)NULL);
        REQUIRE(obj != NULL);
        CHECK(JGetInt(obj, "a") == 1);
        // Parsing stopped after NULL field name
        JDelete(obj);
    }
}

// ==========================================================================
// COMPLEX REAL-WORLD EXAMPLES
// ==========================================================================

SCENARIO("JObjectf: complex real-world examples")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("Notecard hub.set request") {
        J *obj = JObjectf("req:hub.set product:com.blues.app mode:continuous outbound:60");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "req"), "hub.set") == 0);
        CHECK(strcmp(JGetString(obj, "product"), "com.blues.app") == 0);
        CHECK(strcmp(JGetString(obj, "mode"), "continuous") == 0);
        CHECK(JGetInt(obj, "outbound") == 60);
        JDelete(obj);
    }

    SECTION("Notecard note.add request") {
        J *body = JObjectf("temp:%f humidity:%f", (JNUMBER)72.5, (JNUMBER)45.0);
        REQUIRE(body != NULL);

        J *req = JObjectf("req:note.add file:sensors.qo body:%o", body);
        REQUIRE(req != NULL);

        CHECK(strcmp(JGetString(req, "req"), "note.add") == 0);
        CHECK(strcmp(JGetString(req, "file"), "sensors.qo") == 0);

        J *bodyObj = JGetObject(req, "body");
        REQUIRE(bodyObj != NULL);
        CHECK(std::abs(JGetNumber(bodyObj, "temp") - 72.5) < 0.01);
        CHECK(std::abs(JGetNumber(bodyObj, "humidity") - 45.0) < 0.01);

        JDelete(req);
    }

    SECTION("Notecard command") {
        J *obj = JObjectf("cmd:card.led mode:breathe");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "cmd"), "card.led") == 0);
        CHECK(strcmp(JGetString(obj, "mode"), "breathe") == 0);
        JDelete(obj);
    }

    SECTION("Mixed value types") {
        J *obj = JObjectf("name:%s age:%d active:%b score:%f status:ok",
                          "Alice", (JINTEGER)30, 1, (JNUMBER)95.5);
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "name"), "Alice") == 0);
        CHECK(JGetInt(obj, "age") == 30);
        CHECK(JGetBool(obj, "active") == true);
        CHECK(std::abs(JGetNumber(obj, "score") - 95.5) < 0.01);
        CHECK(strcmp(JGetString(obj, "status"), "ok") == 0);
        JDelete(obj);
    }

    SECTION("Using comma separators for readability") {
        J *obj = JObjectf("req:hub.set, product:com.blues.test, mode:periodic, outbound:120");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "req"), "hub.set") == 0);
        CHECK(strcmp(JGetString(obj, "product"), "com.blues.test") == 0);
        CHECK(strcmp(JGetString(obj, "mode"), "periodic") == 0);
        CHECK(JGetInt(obj, "outbound") == 120);
        JDelete(obj);
    }

    SECTION("Multi-line C string concatenation style") {
        J *obj = JObjectf(
            "req:note.add "
            "file:data.qo "
            "sync:%b",
            1);
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "req"), "note.add") == 0);
        CHECK(strcmp(JGetString(obj, "file"), "data.qo") == 0);
        CHECK(JGetBool(obj, "sync") == true);
        JDelete(obj);
    }
}

// ==========================================================================
// JAddf MACRO TESTS
// ==========================================================================

SCENARIO("JAddf: add fields to existing object")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("Add to empty object") {
        J *obj = JCreateObject();
        REQUIRE(obj != NULL);

        JAddf(obj, "a:1 b:2");

        CHECK(JGetInt(obj, "a") == 1);
        CHECK(JGetInt(obj, "b") == 2);
        JDelete(obj);
    }

    SECTION("Add to object with existing fields") {
        J *obj = JObjectf("existing:42");
        REQUIRE(obj != NULL);

        JAddf(obj, "new:100");

        CHECK(JGetInt(obj, "existing") == 42);
        CHECK(JGetInt(obj, "new") == 100);
        JDelete(obj);
    }

    SECTION("Overwrite existing field") {
        J *obj = JObjectf("value:old");
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "value"), "old") == 0);

        JAddf(obj, "value:new");

        CHECK(strcmp(JGetString(obj, "value"), "new") == 0);
        JDelete(obj);
    }

    SECTION("Add with format specifiers") {
        J *obj = JObjectf("base:1");
        REQUIRE(obj != NULL);

        JAddf(obj, "name:%s count:%d", "test", (JINTEGER)99);

        CHECK(JGetInt(obj, "base") == 1);
        CHECK(strcmp(JGetString(obj, "name"), "test") == 0);
        CHECK(JGetInt(obj, "count") == 99);
        JDelete(obj);
    }

    SECTION("Multiple JAddf calls") {
        J *obj = JCreateObject();
        REQUIRE(obj != NULL);

        JAddf(obj, "a:1");
        JAddf(obj, "b:2");
        JAddf(obj, "c:3");

        CHECK(JGetInt(obj, "a") == 1);
        CHECK(JGetInt(obj, "b") == 2);
        CHECK(JGetInt(obj, "c") == 3);
        JDelete(obj);
    }

    SECTION("JAddf with NULL target does not crash") {
        // This should not crash - JMerge handles NULL target
        JAddf(NULL, "a:1");
        // If we get here without crashing, test passes
        CHECK(true);
    }
}

// ==========================================================================
// JMerge EDGE CASES
// ==========================================================================

SCENARIO("JMerge: NULL handling")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("NULL source does nothing") {
        J *target = JCreateObject();
        REQUIRE(target != NULL);
        JAddStringToObject(target, "existing", "value");

        // This should do nothing (source is NULL)
        JMerge(target, NULL);

        // Target should be unchanged
        CHECK(strcmp(JGetString(target, "existing"), "value") == 0);
        JDelete(target);
    }

    SECTION("NULL target deletes source to prevent memory leak") {
        J *source = JCreateObject();
        REQUIRE(source != NULL);
        JAddStringToObject(source, "key", "value");

        // This should delete source to prevent leak
        JMerge(NULL, source);

        // If we get here without leak detector complaining, test passes
        // (source was deleted inside JMerge)
        CHECK(true);
    }

    SECTION("Both NULL does nothing") {
        // Should not crash
        JMerge(NULL, NULL);
        CHECK(true);
    }

    SECTION("Empty source object") {
        J *target = JCreateObject();
        REQUIRE(target != NULL);
        JAddStringToObject(target, "existing", "value");

        J *source = JCreateObject();
        REQUIRE(source != NULL);
        // source has no children

        JMerge(target, source);

        // Target should be unchanged
        CHECK(strcmp(JGetString(target, "existing"), "value") == 0);
        JDelete(target);
    }

    SECTION("Multiple fields merged") {
        J *target = JCreateObject();
        REQUIRE(target != NULL);
        JAddStringToObject(target, "a", "1");

        J *source = JCreateObject();
        REQUIRE(source != NULL);
        JAddStringToObject(source, "b", "2");
        JAddStringToObject(source, "c", "3");

        JMerge(target, source);

        CHECK(strcmp(JGetString(target, "a"), "1") == 0);
        CHECK(strcmp(JGetString(target, "b"), "2") == 0);
        CHECK(strcmp(JGetString(target, "c"), "3") == 0);
        JDelete(target);
    }

    SECTION("Overwrites existing field") {
        J *target = JCreateObject();
        REQUIRE(target != NULL);
        JAddStringToObject(target, "key", "old");

        J *source = JCreateObject();
        REQUIRE(source != NULL);
        JAddStringToObject(source, "key", "new");

        JMerge(target, source);

        CHECK(strcmp(JGetString(target, "key"), "new") == 0);
        JDelete(target);
    }
}

// ==========================================================================
// JObjectfv TESTS
// ==========================================================================

// Helper function to test JObjectfv
static J* testJObjectfvWrapper(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    J *result = JObjectfv(format, args);
    va_end(args);
    return result;
}

SCENARIO("JObjectfv: va_list version")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("Basic usage via wrapper") {
        J *obj = testJObjectfvWrapper("name:%s age:%d", "Bob", (JINTEGER)25);
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "name"), "Bob") == 0);
        CHECK(JGetInt(obj, "age") == 25);
        JDelete(obj);
    }

    SECTION("NULL format") {
        J *obj = testJObjectfvWrapper(NULL);
        REQUIRE(obj != NULL);
        CHECK(obj->child == NULL);
        JDelete(obj);
    }

    SECTION("Mixed types via wrapper") {
        J *obj = testJObjectfvWrapper("s:%s d:%d f:%f b:%b",
                                       "str", (JINTEGER)42, (JNUMBER)3.14, 1);
        REQUIRE(obj != NULL);
        CHECK(strcmp(JGetString(obj, "s"), "str") == 0);
        CHECK(JGetInt(obj, "d") == 42);
        CHECK(std::abs(JGetNumber(obj, "f") - 3.14) < 0.001);
        CHECK(JGetBool(obj, "b") == true);
        JDelete(obj);
    }
}

// ==========================================================================
// VARARG FLOAT/DOUBLE PROMOTION
// ==========================================================================

SCENARIO("JObjectf: vararg float promotion to double")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    // When floats are passed through varargs (...), they are promoted to double.
    // The implementation must use va_arg with double, not float/JNUMBER.

    SECTION("Double literal passed directly") {
        // 3.14159 is a double literal, tests double extraction
        J *obj = JObjectf("val:%f", 3.14159);
        REQUIRE(obj != NULL);
        CHECK(std::abs(JGetNumber(obj, "val") - 3.14159) < 0.00001);
        JDelete(obj);
    }

    SECTION("Float cast to double") {
        float f = 2.5f;
        J *obj = JObjectf("val:%f", (double)f);
        REQUIRE(obj != NULL);
        CHECK(std::abs(JGetNumber(obj, "val") - 2.5) < 0.0001);
        JDelete(obj);
    }

    SECTION("Multiple double values") {
        J *obj = JObjectf("a:%f b:%f c:%f", 1.1, 2.2, 3.3);
        REQUIRE(obj != NULL);
        CHECK(std::abs(JGetNumber(obj, "a") - 1.1) < 0.0001);
        CHECK(std::abs(JGetNumber(obj, "b") - 2.2) < 0.0001);
        CHECK(std::abs(JGetNumber(obj, "c") - 3.3) < 0.0001);
        JDelete(obj);
    }

    SECTION("Large double value") {
        J *obj = JObjectf("big:%f", 1.0e30);
        REQUIRE(obj != NULL);
        CHECK(JGetNumber(obj, "big") > 9.9e29);
        JDelete(obj);
    }

    SECTION("Small double value") {
        J *obj = JObjectf("small:%f", 1.0e-30);
        REQUIRE(obj != NULL);
        CHECK(JGetNumber(obj, "small") < 1.1e-30);
        CHECK(JGetNumber(obj, "small") > 0.0);
        JDelete(obj);
    }

    SECTION("JObjectfv with double values") {
        J *obj = testJObjectfvWrapper("x:%f y:%f", 100.5, 200.75);
        REQUIRE(obj != NULL);
        CHECK(std::abs(JGetNumber(obj, "x") - 100.5) < 0.001);
        CHECK(std::abs(JGetNumber(obj, "y") - 200.75) < 0.001);
        JDelete(obj);
    }
}

}
