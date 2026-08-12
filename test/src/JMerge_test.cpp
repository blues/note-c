/*!
 * @file JMerge_test.cpp
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

#include "n_lib.h"

namespace
{

SCENARIO("JMerge")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("NULL target") {
        J *source = JCreateObject();
        REQUIRE(source != NULL);
        JAddStringToObject(source, "key", "value");

        JMerge(NULL, source);
        // Should not crash, source is deleted
    }

    SECTION("NULL source") {
        J *target = JCreateObject();
        REQUIRE(target != NULL);
        JAddStringToObject(target, "key", "value");

        JMerge(target, NULL);
        // Should not crash, target unchanged
        CHECK(JIsPresent(target, "key"));
        CHECK(strcmp(JGetString(target, "key"), "value") == 0);

        JDelete(target);
    }

    SECTION("Both NULL") {
        JMerge(NULL, NULL);
        // Should not crash
    }

    SECTION("Merge into empty target") {
        J *target = JCreateObject();
        REQUIRE(target != NULL);

        J *source = JCreateObject();
        REQUIRE(source != NULL);
        JAddStringToObject(source, "name", "test");
        JAddNumberToObject(source, "count", 42);
        JAddBoolToObject(source, "active", true);

        JMerge(target, source);
        // source is now deleted, don't use it

        CHECK(JIsPresent(target, "name"));
        CHECK(strcmp(JGetString(target, "name"), "test") == 0);
        CHECK(JGetNumber(target, "count") == 42);
        CHECK(JGetBool(target, "active") == true);

        JDelete(target);
    }

    SECTION("Merge from empty source") {
        J *target = JCreateObject();
        REQUIRE(target != NULL);
        JAddStringToObject(target, "existing", "value");

        J *source = JCreateObject();
        REQUIRE(source != NULL);

        JMerge(target, source);
        // source is now deleted

        CHECK(JIsPresent(target, "existing"));
        CHECK(strcmp(JGetString(target, "existing"), "value") == 0);

        JDelete(target);
    }

    SECTION("Merge with no overlapping fields") {
        J *target = JCreateObject();
        REQUIRE(target != NULL);
        JAddStringToObject(target, "targetKey", "targetValue");

        J *source = JCreateObject();
        REQUIRE(source != NULL);
        JAddStringToObject(source, "sourceKey", "sourceValue");

        JMerge(target, source);

        CHECK(JIsPresent(target, "targetKey"));
        CHECK(strcmp(JGetString(target, "targetKey"), "targetValue") == 0);
        CHECK(JIsPresent(target, "sourceKey"));
        CHECK(strcmp(JGetString(target, "sourceKey"), "sourceValue") == 0);

        JDelete(target);
    }

    SECTION("Merge overwrites existing fields") {
        J *target = JCreateObject();
        REQUIRE(target != NULL);
        JAddStringToObject(target, "shared", "oldValue");
        JAddNumberToObject(target, "count", 10);

        J *source = JCreateObject();
        REQUIRE(source != NULL);
        JAddStringToObject(source, "shared", "newValue");
        JAddNumberToObject(source, "count", 99);

        JMerge(target, source);

        CHECK(strcmp(JGetString(target, "shared"), "newValue") == 0);
        CHECK(JGetNumber(target, "count") == 99);

        JDelete(target);
    }

    SECTION("Merge with nested objects") {
        J *target = JCreateObject();
        REQUIRE(target != NULL);
        JAddStringToObject(target, "top", "level");

        J *source = JCreateObject();
        REQUIRE(source != NULL);
        J *nested = JAddObjectToObject(source, "nested");
        REQUIRE(nested != NULL);
        JAddStringToObject(nested, "inner", "value");

        JMerge(target, source);

        CHECK(JIsPresent(target, "top"));
        J *mergedNested = JGetObject(target, "nested");
        CHECK(mergedNested != NULL);
        CHECK(strcmp(JGetString(mergedNested, "inner"), "value") == 0);

        JDelete(target);
    }

    SECTION("Merge with arrays") {
        J *target = JCreateObject();
        REQUIRE(target != NULL);

        J *source = JCreateObject();
        REQUIRE(source != NULL);
        J *arr = JAddArrayToObject(source, "items");
        REQUIRE(arr != NULL);
        JAddItemToArray(arr, JCreateNumber(1));
        JAddItemToArray(arr, JCreateNumber(2));
        JAddItemToArray(arr, JCreateNumber(3));

        JMerge(target, source);

        J *mergedArr = JGetArray(target, "items");
        CHECK(mergedArr != NULL);
        CHECK(JGetArraySize(mergedArr) == 3);

        JDelete(target);
    }

    SECTION("Merge multiple fields") {
        J *target = JCreateObject();
        REQUIRE(target != NULL);
        JAddStringToObject(target, "keep", "this");

        J *source = JCreateObject();
        REQUIRE(source != NULL);
        JAddStringToObject(source, "field1", "value1");
        JAddStringToObject(source, "field2", "value2");
        JAddStringToObject(source, "field3", "value3");
        JAddNumberToObject(source, "num", 123);
        JAddBoolToObject(source, "flag", false);

        JMerge(target, source);

        CHECK(strcmp(JGetString(target, "keep"), "this") == 0);
        CHECK(strcmp(JGetString(target, "field1"), "value1") == 0);
        CHECK(strcmp(JGetString(target, "field2"), "value2") == 0);
        CHECK(strcmp(JGetString(target, "field3"), "value3") == 0);
        CHECK(JGetNumber(target, "num") == 123);
        CHECK(JGetBool(target, "flag") == false);

        JDelete(target);
    }
}

}
