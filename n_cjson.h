/*!
 * @file n_cjson.h
 *
 * Written by Ray Ozzie and Blues Inc. team.
 *
 * Portions Copyright (c) 2019 Blues Inc. MIT License. Use of this source code is
 * governed by licenses granted by the copyright holder including that found in
 * the
 * <a href="https://github.com/blues/note-c/blob/master/LICENSE">LICENSE</a>
 * file.
 *
 * MODIFIED for use in notecard primarily by altering default memory allocator
 * and by renaming the functions so that they won't conflict with a developer's
 * own decision to incorporate the actual production cJSON into their own app.
 * In no way shall this interfere with a production cJSON.
 *
 * Renaming was done as follows:
 * CJSON_ -> N_CJSON_
 * cJSON_ -> J
 * cJSON -> J
 *
 * Portions Copyright (c) 2009-2017 Dave Gamble and cJSON contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software i
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef J_h
#define J_h

#ifdef __cplusplus
extern "C"
{
#endif

/* project version */
#define N_CJSON_VERSION_MAJOR 1
#define N_CJSON_VERSION_MINOR 7
#define N_CJSON_VERSION_PATCH 7

#include <stddef.h>
#include <stdint.h>   /* uint16_t, for the packed J layout */

/* J Types: */
#define JInvalid (0)
#define JFalse  (1 << 0)
#define JTrue   (1 << 1)
#define JNULL   (1 << 2)
#define JNumber (1 << 3)
#define JString (1 << 4)
#define JArray  (1 << 5)
#define JObject (1 << 6)
#define JRaw    (1 << 7) /* raw json */

#define JIsReference 256
/* Set when valuestring points into this node's own allocation, so JDelete must
 * not free it separately. Distinct from JIsReference, which also governs whether
 * JDelete recurses into child. */
#define JValueInline 1024

/* Largest total allocation a single packed node may have.
 *
 * A node records its own allocation size in `objlen`, a uint16_t, so the packing
 * logic can tell how much inline room is left after a mutation. A node whose key
 * and value would push it past this bound keeps both on the heap and is
 * allocated at exactly sizeof(J), which bounds objlen by construction. A string
 * that large needs its own allocation regardless. */
#define J_MAX_PACKED_ALLOC 50000
#define JStringIsConst 512

/* Set when `string` (the key) points into this node's own allocation, as
 * opposed to a caller-owned buffer. Always accompanied by JStringIsConst, which
 * keeps its historical "do not free the key" meaning; this says WHERE the key
 * lives, which JStringIsConst alone cannot distinguish.
 *
 * The distinction is not cosmetic. Deciding "is this key inside my allocation?"
 * by comparing `item->string` against the node's address is only meaningful
 * when both pointers are already known to address the same object -- relational
 * comparison of pointers into different objects is undefined in C, and a
 * caller-owned key is very often a string literal in a different segment
 * entirely. Recording the answer at the moment the key is placed, when it is
 * known for free, removes the question.
 *
 * Only ever set under NOTE_C_STORAGE_OPTIMIZATION; the historical layout never
 * stores a key inside the node. Defined unconditionally so the shared code that
 * masks flags does not need a conditional. */
#define JKeyInline 2048

/* Set when valueint/valuenumber on a JString or JRaw node genuinely hold
 * numbers, because a setter put them there.
 *
 * A string node's numeric bytes are normally lending themselves to inline
 * content, but `JSetIntValue`/`JSetNumberValue` have always been able to write
 * numbers onto a string node without changing its type. Both states exist and
 * the base type cannot tell them apart, so the fact is recorded here.
 *
 * Inferring it instead -- "no inline pointer currently occupies the region,
 * therefore it holds numbers" -- is unsound in both directions, and an earlier
 * revision was wrong in both:
 *
 *   FALSE POSITIVE. _create_reference() copies a source node's bytes verbatim
 *   and then clears the inline ownership flags, because the reference borrows
 *   rather than owns. The copied CHARACTERS remain in the numeric region with
 *   no flag pointing at them, so a reference to JCreateString("abc") reported
 *   JIntValue() == 6513249 -- the bytes of "abc" read as an integer.
 *
 *   DESTRUCTION. After a setter evacuates the region and writes a number, the
 *   region looks unoccupied again, so _j_key_set() would reclaim it for a short
 *   key and silently overwrite the number that was just stored.
 *
 * With the fact recorded, the region is off limits to inline placement while
 * the flag is set, and the accessors read it only while it is set.
 *
 * Only ever set under NOTE_C_STORAGE_OPTIMIZATION. In the historical layout the
 * members are never overlaid, so nothing needs recording -- and setting a new
 * bit there would change the value a consumer reads from `item->type`. */
#define JNumericLive 4096

/* NOTE_C_STORAGE_OPTIMIZATION selects the memory-optimized `J` storage model.
 *
 * By default `J` has the historical layout: a 48-byte node on a 32-bit target,
 * whose key and string value always live in separate heap allocations. Defining
 * NOTE_C_STORAGE_OPTIMIZATION, or setting the CMake option of the same name,
 * selects a 40-byte node that carves an object member's key and short string
 * value out of the node's own allocation, so a short string member costs one
 * allocation instead of three.
 *
 * Both produce byte-identical JSON. On a representative corpus the optimization
 * removes roughly 38% of the allocations and 26% of the heap a parsed document
 * holds at double precision, and 38%/16% at single precision, where a float
 * JNUMBER already removes the historical layout's tail padding.
 *
 * IMPORTANT: this changes sizeof(J) AND the offset and width of nearly every
 * member. Every translation unit that sees `J` must be compiled with the same
 * setting -- the note-c sources and the consumer's sources alike -- exactly as
 * NOTE_C_SINGLE_PRECISION must be. The CMake option propagates it PUBLIC for
 * that reason. See docs/architecture/decisions/0002-j-node-storage-layout.md
 * for the full contract, including the fields whose observable content
 * changes. */

/*!
 @brief The core JSON object type used by note-c.

 When using note-c, treat this struct as opaque. You should never have to work
 directly with its members.

 A note on two long-standing member names, which are NOT being renamed because
 `J` is a public type and downstream code reads these fields directly:

 - `string` is the object member KEY, never a value. It is NULL for array
   elements and for a root item. `JGetItemName()` is the accessor.
 - `valuestring` is the value, for `JString` and `JRaw` items.
 - The flag guarding `string` is spelled `JStringIsConst`, but what it actually
   means is "the key is not separately allocated, so do not free it". Read it as
   JKeyIsConst.
 */
#ifndef NOTE_C_STORAGE_OPTIMIZATION

/* The historical layout, and the default. Every member keeps the offset, width
 * and meaning it has had since note-c 2.x, and a key or string value always
 * occupies its own heap allocation. */
typedef struct J {
    /* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
    struct J *next;
    struct J *prev;
    /* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */
    struct J *child;

    /* The type of the item, as above. */
    int type;

    /* The item's string, if type==JString  and type == JRaw */
    char *valuestring;
    /* writing to valueint is DEPRECATED, use JSetNumberValue instead */
    JINTEGER valueint;
    /* The item's number, if type==JNumber */
    JNUMBER valuenumber;
    /* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
    char *string;
} J;

#else /* NOTE_C_STORAGE_OPTIMIZATION */

typedef struct J {
    /* MEMBER ORDER IS LOAD-BEARING. JINTEGER and JNUMBER require 8-byte
     * alignment even where pointers are 4 bytes, so placing them last lets the
     * six 4-byte members pack into offsets 0..23 and the struct end exactly on
     * 40. Any other order costs 8 bytes of padding.
     *
     * INLINE STORAGE. valueint and valuenumber are meaningless for a JString or
     * JRaw item, so for those items the bytes they occupy hold the value string
     * and then the key instead, and the allocation is extended past the struct
     * only as far as the content needs:
     *
     *   string item : alloc = MAX(sizeof(J), offsetof(J, valueint)
     *                                        + strlen(value)+1 + strlen(key)+1)
     *                 laid out [value NUL][key NUL] from offsetof(J, valueint)
     *   other item  : alloc = sizeof(J) + strlen(key)+1
     *                 laid out [key NUL] from sizeof(J)
     *
     * So a short string member -- node, key and value together -- is ONE
     * allocation. Inline storage is an optimization, never a mode: any content
     * may live inline or in its own block, JValueInline and JStringIsConst say
     * which, and a setter that no longer fits or that needs the numeric members
     * back moves the affected content out to the heap first. The address of the
     * J itself never changes. */

    /* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
    struct J *next;
    struct J *prev;
    /* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */
    struct J *child;

    /* The item's string, if type==JString and type == JRaw. May point into this
     * node's own allocation (JValueInline), at a caller-owned buffer
     * (JIsReference), or at a separate heap block (neither flag). */
    char *valuestring;
    /* The item's name string, if this item is the child of, or is in the list of subitems of an object.
     * May point into this node's own allocation or at a caller-owned buffer
     * (JStringIsConst either way), or at a separate heap block. */
    char *string;

    /* The type of the item, as above. Narrowed from int to uint16_t: the base
     * types occupy 8 bits and the three flags reach 1024, so 16 bits is ample. */
    uint16_t type;
    /* Total bytes allocated for this node, including any inline storage past
     * the struct. Bounded by J_MAX_PACKED_ALLOC so it always fits. */
    uint16_t objlen;

    /* ===================================================================
     * THE NUMERIC PAIR IS ALSO THE INLINE CHARACTER BUFFER. READ THIS.
     * ===================================================================
     *
     * For a JString or JRaw item these two members are meaningless, so the
     * bytes they occupy are used to hold the item's value string and then its
     * key. Conceptually the declaration is this union:
     *
     *     union {
     *         char kvbuf[sizeof(JINTEGER) + sizeof(JNUMBER)];
     *         struct {
     *             JINTEGER valueint;
     *             JNUMBER  valuenumber;
     *         };
     *     };
     *
     * It is NOT written that way, deliberately. An anonymous struct inside a
     * union is standard only from C11; in C89, C99 and every C++ standard it is
     * a compiler extension, and under -pedantic-errors it is a hard error
     * rather than a warning. note-c pins no C standard, compiles with
     * -Wpedantic, and is parsed by C++ consumers such as note-arduino, so the
     * union form would make the header unbuildable on toolchains this library
     * is expected to work on. The members are therefore declared plainly and
     * the overlay is expressed in code instead.
     *
     * The overlay is addressed as (char *)item + offsetof(J, valueint), never
     * as sizeof(J) minus a constant, so nothing depends on these members
     * happening to be last. n_cjson.c carries a compile-time assertion that
     * they are contiguous and do run to the end of the struct; a reorder that
     * broke either property fails the build rather than corrupting nodes.
     *
     * Byte layout of a node on a 32-bit target (sizeof(J) == 40):
     *
     *     offset  0   4    8      12           16      20    22    24    32  40
     *            +----+----+------+------------+-------+-----+-----+-----+----+
     *            |next|prev|child |valuestring |string |type |objln|valueint |
     *            |    |    |      |            |       |     |     |valuenum |
     *            +----+----+------+------------+-------+-----+-----+-----+----+
     *     JString item:                                       [value\0][key\0]
     *                                                         ^ offsetof(valueint)
     *                                                         ...extending past 40
     *                                                         if the two need more
     *     other item:  valueint/valuenumber live here          [key\0] from 40 on
     *
     * The two are never used at once: kvbuf bytes are only written when the
     * item is a string type, and valueint/valuenumber are only read when it is
     * not. _j_is_string_type() gates that internally; JIntValue() and
     * JNumberValue() enforce it for callers. Accessing the bytes through a char
     * pointer is explicitly permitted by the aliasing rules of both C and C++.
     */
    JINTEGER valueint;
    JNUMBER valuenumber;
} J;

#endif /* NOTE_C_STORAGE_OPTIMIZATION */

typedef struct JHooks {
    void *(*malloc_fn)(size_t sz);
    void (*free_fn)(void *ptr);
} JHooks;

typedef int Jbool;

#if !defined(__WINDOWS__) && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
#define __WINDOWS__
#endif
#ifdef __WINDOWS__

/* When compiling for windows, we specify a specific calling convention to avoid issues where we are being called from a project with a different default calling convention.  For windows you have 2 define options:

N_CJSON_HIDE_SYMBOLS - Define this in the case where you don't want to ever dllexport symbols
N_CJSON_EXPORT_SYMBOLS - Define this on library build when you want to dllexport symbols (default)
N_CJSON_IMPORT_SYMBOLS - Define this if you want to dllimport symbol

For *nix builds that support visibility attribute, you can define similar behavior by

setting default visibility to hidden by adding
-fvisibility=hidden (for gcc)
or
-xldscope=hidden (for sun cc)
to CFLAGS

then using the N_CJSON_API_VISIBILITY flag to "export" the same symbols the way N_CJSON_EXPORT_SYMBOLS does

*/

/* export symbols by default, this is necessary for copy pasting the C and header file */
#if !defined(N_CJSON_HIDE_SYMBOLS) && !defined(N_CJSON_IMPORT_SYMBOLS) && !defined(N_CJSON_EXPORT_SYMBOLS)
#define N_CJSON_EXPORT_SYMBOLS
#endif

#if defined(N_CJSON_HIDE_SYMBOLS)
#define N_CJSON_PUBLIC(type)   type __stdcall
#elif defined(N_CJSON_EXPORT_SYMBOLS)
#define N_CJSON_PUBLIC(type)   __declspec(dllexport) type __stdcall
#elif defined(N_CJSON_IMPORT_SYMBOLS)
#define N_CJSON_PUBLIC(type)   __declspec(dllimport) type __stdcall
#endif
#else /* !WIN32 */
#if (defined(__GNUC__) || defined(__SUNPRO_CC) || defined (__SUNPRO_C)) && defined(N_CJSON_API_VISIBILITY)
#define N_CJSON_PUBLIC(type)   __attribute__((visibility("default"))) type
#else
#define N_CJSON_PUBLIC(type) type
#endif
#endif

/*!
  @brief The maximum nesting level for JSON objects before a parsing error.

  Default value: `100`

  For example, if you have a JSON object that contains multiple, nested
  objects like this

      {
          "x":
          {
              "x:"
              {
                  .
                  .
                  .
              }
          }
      }

  And the nesting level exceeds `N_CJSON_NESTING_LIMIT`, then calling `JParse` on
  a `J *` representing this object will return an error (NULL).

  This exists to prevent the cJSON parser from causing a stack overflow. The
  user may override this macro at build time (e.g.
  -DN_CJSON_NESTING_LIMIT=200) to increase or reduce the limit.
 */
#ifndef N_CJSON_NESTING_LIMIT
#define N_CJSON_NESTING_LIMIT 100
#endif

/* returns the version of J as a string */
N_CJSON_PUBLIC(const char*) JVersion(void);

/* Supply malloc, realloc and free functions to J */
N_CJSON_PUBLIC(void) JInitHooks(JHooks* hooks);

/* Memory Management: the caller is always responsible to free the results from all variants of JParse (with JDelete) and JPrint (with stdlib free, JHooks.free_fn, or JFree as appropriate). The exception is JPrintPreallocated, where the caller has full responsibility of the buffer. */
/* Supply a block of JSON, and this returns a J object you can interrogate. */
N_CJSON_PUBLIC(J *) JParse(const char *value);
/* ParseWithOpts allows you to require (and check) that the JSON is null terminated, and to retrieve the pointer to the final byte parsed. */
/* If you supply a ptr in return_parse_end and parsing fails, then return_parse_end will contain a pointer to the error so will match JGetErrorPtr(). */
N_CJSON_PUBLIC(J *) JParseWithOpts(const char *value, const char **return_parse_end, Jbool require_null_terminated);

/* Render a J entity to text for transfer/storage. */
N_CJSON_PUBLIC(char *) JPrint(const J *item);
/* Render a J entity to text for transfer/storage without any formatting. */
N_CJSON_PUBLIC(char *) JPrintUnformatted(const J *item);
/* Render a J entity to text for transfer/storage without any formatting and without fields that are false, 0, or "". */
N_CJSON_PUBLIC(char *) JPrintUnformattedOmitEmpty(const J *item);
/* Render a J entity to text using a buffered strategy. prebuffer is a guess at the final size. guessing well reduces reallocation. fmt=0 gives unformatted, =1 gives formatted */
N_CJSON_PUBLIC(char *) JPrintBuffered(const J *item, int prebuffer, Jbool fmt);
/* Render a J entity to text using a buffer already allocated in memory with given length. Returns 1 on success and 0 on failure. */
/* NOTE: J is not always 100% accurate in estimating how much memory it will use, so to be safe allocate 5 bytes more than you actually need */
N_CJSON_PUBLIC(Jbool) JPrintPreallocated(J *item, char *buffer, const int length, const Jbool format);
N_CJSON_PUBLIC(Jbool) JPrintPreallocatedOmitEmpty(J *item, char *buffer, const int length, const Jbool format);
/* Delete a J entity and all subentities. */
N_CJSON_PUBLIC(void) JDelete(J *c);

/* Returns the number of items in an array (or object). */
N_CJSON_PUBLIC(int) JGetArraySize(const J *array);
#define	JGetObjectItems JGetArraySize
/* Retrieve item number "index" from array "array". Returns NULL if unsuccessful. */
N_CJSON_PUBLIC(J *) JGetArrayItem(const J *array, int index);
/* Get item "string" from object. Case insensitive. */
N_CJSON_PUBLIC(J *) JGetObjectItem(const J * const object, const char * const string);
N_CJSON_PUBLIC(J *) JGetObjectItemCaseSensitive(const J * const object, const char * const string);
N_CJSON_PUBLIC(Jbool) JHasObjectItem(const J *object, const char *string);
/* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when JParse() returns 0. 0 when JParse() succeeds. */
N_CJSON_PUBLIC(const char *) JGetErrorPtr(void);

/* Check if the item is a string and return its valuestring */
N_CJSON_PUBLIC(char *) JGetStringValue(J *item);

/* These functions check the type of an item */
N_CJSON_PUBLIC(Jbool) JIsInvalid(const J * const item);
N_CJSON_PUBLIC(Jbool) JIsFalse(const J * const item);
N_CJSON_PUBLIC(Jbool) JIsTrue(const J * const item);
N_CJSON_PUBLIC(Jbool) JIsBool(const J * const item);
N_CJSON_PUBLIC(Jbool) JIsNull(const J * const item);
N_CJSON_PUBLIC(Jbool) JIsNumber(const J * const item);
N_CJSON_PUBLIC(Jbool) JIsString(const J * const item);
N_CJSON_PUBLIC(Jbool) JIsArray(const J * const item);
N_CJSON_PUBLIC(Jbool) JIsObject(const J * const item);
N_CJSON_PUBLIC(Jbool) JIsRaw(const J * const item);

/* These calls create a J item of the appropriate type. */
N_CJSON_PUBLIC(J *) JCreateTrue(void);
N_CJSON_PUBLIC(J *) JCreateFalse(void);
N_CJSON_PUBLIC(J *) JCreateBool(Jbool boolean);
N_CJSON_PUBLIC(J *) JCreateNumber(JNUMBER num);
N_CJSON_PUBLIC(J *) JCreateInteger(JINTEGER integer);
N_CJSON_PUBLIC(J *) JCreateString(const char *string);
/* raw json */
N_CJSON_PUBLIC(J *) JCreateRaw(const char *raw);
N_CJSON_PUBLIC(J *) JCreateArray(void);
N_CJSON_PUBLIC(J *) JCreateObject(void);

/* Create a string where valuestring references a string so
 * it will not be freed by JDelete */
N_CJSON_PUBLIC(J *) JCreateStringValue(const char *string);
N_CJSON_PUBLIC(J *) JCreateStringReference(const char *string);
/* Create an object/arrray that only references it's elements so
 * they will not be freed by JDelete */
N_CJSON_PUBLIC(J *) JCreateObjectReference(const J *child);
N_CJSON_PUBLIC(J *) JCreateArrayReference(const J *child);

/* These utilities create an Array of count items. */
N_CJSON_PUBLIC(J *) JCreateIntArray(const long int *numbers, int count);
N_CJSON_PUBLIC(J *) JCreateNumberArray(const JNUMBER *numbers, int count);
N_CJSON_PUBLIC(J *) JCreateStringArray(const char **strings, int count);

/* Append item to the specified array/object. */
N_CJSON_PUBLIC(void) JAddItemToArray(J *array, J *item);
N_CJSON_PUBLIC(void) JAddItemToObject(J *object, const char *string, J *item);
/* Use this when string is definitely const (i.e. a literal, or as good as), and will definitely survive the J object.
 * WARNING: When this function was used, make sure to always check that (item->type & JStringIsConst) is zero before
 * writing to `item->string` */
N_CJSON_PUBLIC(void) JAddItemToObjectCS(J *object, const char *string, J *item);
/* Append reference to item to the specified array/object. Use this when you want to add an existing J to a new J, but don't want to corrupt your existing J. */
N_CJSON_PUBLIC(void) JAddItemReferenceToArray(J *array, J *item);
N_CJSON_PUBLIC(void) JAddItemReferenceToObject(J *object, const char *string, J *item);

/* Remove/Detatch items from Arrays/Objects. */
N_CJSON_PUBLIC(J *) JDetachItemViaPointer(J *parent, J * const item);
N_CJSON_PUBLIC(J *) JDetachItemFromArray(J *array, int which);
N_CJSON_PUBLIC(void) JDeleteItemFromArray(J *array, int which);
N_CJSON_PUBLIC(J *) JDetachItemFromObject(J *object, const char *string);
N_CJSON_PUBLIC(J *) JDetachItemFromObjectCaseSensitive(J *object, const char *string);
N_CJSON_PUBLIC(void) JDeleteItemFromObject(J *object, const char *string);
N_CJSON_PUBLIC(void) JDeleteItemFromObjectCaseSensitive(J *object, const char *string);

/* Update array items. */
N_CJSON_PUBLIC(void) JInsertItemInArray(J *array, int which, J *newitem); /* Shifts pre-existing items to the right. */
N_CJSON_PUBLIC(Jbool) JReplaceItemViaPointer(J * const parent, J * const item, J * replacement);
N_CJSON_PUBLIC(void) JReplaceItemInArray(J *array, int which, J *newitem);
N_CJSON_PUBLIC(void) JReplaceItemInObject(J *object,const char *string,J *newitem);
N_CJSON_PUBLIC(void) JReplaceItemInObjectCaseSensitive(J *object,const char *string,J *newitem);

/* Duplicate a J item */
N_CJSON_PUBLIC(J *) JDuplicate(const J *item, Jbool recurse);
/* Duplicate will create a new, identical J item to the one you pass, in new memory that will
need to be released. With recurse!=0, it will duplicate any children connected to the item.
The item->next and ->prev pointers are always zero on return from Duplicate. */
/* Recursively compare two J items for equality. If either a or b is NULL or invalid, they will be considered unequal.
 * case_sensitive determines if object keys are treated case sensitive (1) or case insensitive (0) */
N_CJSON_PUBLIC(Jbool) JCompare(const J * const a, const J * const b, const Jbool case_sensitive);


N_CJSON_PUBLIC(void) JMinify(char *json);

/* Helper functions for creating and adding items to an object at the same time.
 * They return the added item or NULL on failure. */
N_CJSON_PUBLIC(J*) JAddTrueToObject(J * const object, const char * const name);
N_CJSON_PUBLIC(J*) JAddFalseToObject(J * const object, const char * const name);
N_CJSON_PUBLIC(J*) JAddBoolToObject(J * const object, const char * const name, const Jbool boolean);
N_CJSON_PUBLIC(J*) JAddNumberToObject(J * const object, const char * const name, const JNUMBER number);
N_CJSON_PUBLIC(J*) JAddIntToObject(J * const object, const char * const name, const JINTEGER integer);
N_CJSON_PUBLIC(J*) JAddStringToObject(J * const object, const char * const name, const char * const string);
N_CJSON_PUBLIC(J*) JAddRawToObject(J * const object, const char * const name, const char * const raw);
N_CJSON_PUBLIC(J*) JAddObjectToObject(J * const object, const char * const name);
N_CJSON_PUBLIC(J*) JAddArrayToObject(J * const object, const char * const name);
#define JConvertToJSONString JPrintUnformatted
#define JConvertFromJSONString JParse

/* Assign an item's numeric value.
 *
 * Both macro names are retained and both keep their historical behavior: the
 * value is written to valueint AND valuenumber, `number` is evaluated exactly
 * once, and a NULL object is a no-op that returns the operand. The macros now
 * expand to a call rather than to an assignment reaching into the struct.
 *
 * The outer conditional is retained deliberately, even though both helpers
 * handle a NULL object themselves. It is what preserves the NULL branch's
 * VALUE and TYPE: historically that branch was the unconverted `(number)`
 * operand, so `JSetIntValue(NULL, 1.5)` yielded 1.5 as a double. Calling a
 * helper unconditionally would yield 1 as a JINTEGER -- and under
 * NOTE_C_SINGLE_PRECISION would round the operand through a float on the way
 * out. Letting the conditional pick the common type reproduces both exactly.
 *
 * JSetNumberValue was already a call. JSetIntValue was
 *
 *     ((object) ? (object)->valueint = (object)->valuenumber = (number) : (number))
 *
 * which wrote two members directly from the caller's translation unit. Under
 * NOTE_C_STORAGE_OPTIMIZATION those bytes may hold a packed key or string
 * value, so writing them from outside the library would corrupt the node with
 * no way for the library to intervene. Routing through a function lets the
 * library evacuate that content first.
 *
 * `object` is evaluated TWICE: once by the conditional, once by the call. The
 * historical JSetIntValue evaluated it three times, so that is a change from
 * three to two; the historical JSetNumberValue already evaluated it twice, so
 * it is unchanged. `number` was never repeated and still is not.
 *
 * JSetIntHelper takes JNUMBER on purpose. The old macro parsed as
 * `valueint = (valuenumber = number)`, so both members saw the operand in its
 * original arithmetic form: JSetIntValue(item, 1.5) left valueint == 1 and
 * valuenumber == 1.5. A JINTEGER parameter would truncate at the call site and
 * store 1 in both.
 *
 * Both propagate to the sibling member, because valueint and valuenumber must
 * stay in step: valueint carries integers that valuenumber cannot represent
 * exactly (the parser converts the original text with JAtoI, so values beyond
 * 2^53 are exact in valueint and lossy in valuenumber), and _print_number
 * compares the two to decide integer-vs-float rendering. */
N_CJSON_PUBLIC(JNUMBER) JSetNumberHelper(J *object, JNUMBER number);
N_CJSON_PUBLIC(JINTEGER) JSetIntHelper(J *object, JNUMBER number);
#define JSetIntValue(object, number) ((object) ? JSetIntHelper((object), (JNUMBER)(number)) : (number))
#define JSetNumberValue(object, number) ((object) ? JSetNumberHelper((object), (JNUMBER)(number)) : (number))

/* Macro for iterating over an array or object. These read child/next rather
 * than writing a member, so unlike the setters above they carry no aliasing or
 * storage-layout hazard, and a for-loop cannot be expressed as a function. */
#define JArrayForEach(element, array) for(element = (array != NULL) ? (array)->child : NULL; element != NULL; element = element->next)
// Iterate over the fields of an object
#define JObjectForEach(element, array) JArrayForEach(element, array)

/* malloc/free objects using the malloc/free functions that have been set with JInitHooks */
N_CJSON_PUBLIC(void *) JMalloc(size_t size);
N_CJSON_PUBLIC(void) JFree(void *object);

#ifdef __cplusplus
}
#endif

#endif
