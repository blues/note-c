/*!
 * @file j_public_api_compat_c.c
 *
 * The C half of the public-macro compatibility check.
 *
 * note-c is a C library, but every test in this suite is a C++ translation
 * unit, so until this file existed the public `J` macros were never once
 * compiled as C. That matters for the macros converted to function calls
 * alongside NOTE_C_STORAGE_OPTIMIZATION: C and C++ disagree about exactly the
 * properties those conversions put at risk.
 *
 *   - Binding a `const char *` to a `char *` is a hard error in C++ but only a
 *     warning in C, so the C++ test alone would catch a re-qualified
 *     JGetObjectItemName only on compilers configured to treat that warning as
 *     an error. This file is compiled with -Werror to close that.
 *   - Assigning through, or taking the address of, a macro result requires it
 *     to be an lvalue. A macro that became a function call stops being one.
 *     That is a hard error in both languages, and is checked here without
 *     relying on any warning setting at all.
 *
 * Everything below is either a compile-time construct or a cheap runtime
 * self-check invoked from JPublicApiCompat_test.cpp. The compile-time parts are
 * the point: if they stop compiling, the build fails, which is the assertion.
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

#include <stdlib.h>
#include <string.h>

#include "note.h"

/* Failure bits returned by jPublicApiCompatC(). Zero means every check held. */
#define JCOMPAT_C_OK                    0
#define JCOMPAT_C_KEYLESS_NOT_NULL      (1 << 0)
#define JCOMPAT_C_KEY_WRONG             (1 << 1)
#define JCOMPAT_C_SETINT_FRACTION_LOST  (1 << 2)
#define JCOMPAT_C_SETNUMBER_WRONG       (1 << 3)
#define JCOMPAT_C_NULL_NOT_PASSTHROUGH  (1 << 4)
#define JCOMPAT_C_ALLOC_FAILED          (1 << 5)

/* COMPILE-TIME CHECK 1: the result of JGetObjectItemName is a mutable `char *`.
 *
 * Written as a function so the conversion is checked at every call, and so a
 * `const char *` result is a -Wdiscarded-qualifiers error under the -Werror
 * this file is compiled with. */
static char *jCompatBindsToMutableCharPointer(J *item)
{
    char *name = JGetObjectItemName(item);
    return name;
}

/* COMPILE-TIME CHECK 2: it is still an LVALUE.
 *
 * The historical macro expanded to a member access, so `JGetObjectItemName(x)`
 * could be assigned to and have its address taken. Neither compiles if the
 * macro becomes a function call, in C or C++, regardless of warning flags.
 *
 * The assignment is a self-assignment so nothing is corrupted and no key is
 * orphaned; only its well-formedness is being tested. */
static char **jCompatResultIsAnLvalue(J *item)
{
    JGetObjectItemName(item) = JGetObjectItemName(item);
    return &JGetObjectItemName(item);
}

/* Runtime half: the observable contracts, checked from C rather than C++ in
 * case the two ever diverge in argument promotion or macro expansion. */
int jPublicApiCompatC(void)
{
    int result = JCOMPAT_C_OK;
    J *object = NULL;
    J *array = NULL;
    J *item = NULL;
    char **slot = NULL;

    /* A keyless item yields NULL, not "". */
    array = JCreateArray();
    if (array == NULL) {
        return JCOMPAT_C_ALLOC_FAILED;
    }
    JAddItemToArray(array, JCreateNumber(1));
    if (array->child == NULL) {
        JDelete(array);
        return JCOMPAT_C_ALLOC_FAILED;
    }
    if (JGetObjectItemName(array->child) != NULL) {
        result |= JCOMPAT_C_KEYLESS_NOT_NULL;
    }
    JDelete(array);

    object = JCreateObject();
    if (object == NULL) {
        return result | JCOMPAT_C_ALLOC_FAILED;
    }
    if (JAddNumberToObject(object, "key", 1) == NULL) {
        JDelete(object);
        return result | JCOMPAT_C_ALLOC_FAILED;
    }
    item = object->child;

    if ((JGetObjectItemName(item) == NULL)
            || (strcmp(JGetObjectItemName(item), "key") != 0)) {
        result |= JCOMPAT_C_KEY_WRONG;
    }

    /* Exercise the two compile-time checks so they cannot be optimized away as
     * unreferenced statics, and so the lvalue self-assignment actually runs. */
    if (jCompatBindsToMutableCharPointer(item) == NULL) {
        result |= JCOMPAT_C_KEY_WRONG;
    }
    slot = jCompatResultIsAnLvalue(item);
    if ((slot == NULL) || (*slot != JGetObjectItemName(item))) {
        result |= JCOMPAT_C_KEY_WRONG;
    }

    /* The historical macro parsed as valueint = (valuenumber = number), so a
     * fractional operand truncated into valueint and survived in valuenumber.
     * A JINTEGER parameter on the helper would store 1 in both. */
    JSetIntValue(item, 1.5);
    if ((item->valueint != 1) || (item->valuenumber != (JNUMBER)1.5)) {
        result |= JCOMPAT_C_SETINT_FRACTION_LOST;
    }

    JSetNumberValue(item, 2.5);
    if ((item->valuenumber != (JNUMBER)2.5) || (item->valueint != 2)) {
        result |= JCOMPAT_C_SETNUMBER_WRONG;
    }

    JDelete(object);

    /* A NULL object is a no-op that yields the operand. */
    if (JSetIntValue((J *)NULL, 42) != 42) {
        result |= JCOMPAT_C_NULL_NOT_PASSTHROUGH;
    }
    if (JSetNumberValue((J *)NULL, 4.5) != (JNUMBER)4.5) {
        result |= JCOMPAT_C_NULL_NOT_PASSTHROUGH;
    }

    return result;
}
