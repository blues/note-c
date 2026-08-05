/*!
 * @file n_cjson.c
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

/* disable warnings about old C89 functions in MSVC */
#if !defined(_CRT_SECURE_NO_DEPRECATE) && defined(_MSC_VER)
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifdef __GNUC__
#pragma GCC visibility push(default)
#endif
#if defined(_MSC_VER)
#pragma warning (push)
/* disable warning about single line comments in system headers */
#pragma warning (disable : 4001)
#endif

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

// For Note, disable dependencies
#undef ENABLE_LOCALES

#include "n_lib.h"

#ifdef ENABLE_LOCALES
#include <locale.h>
#endif

#if defined(_MSC_VER)
#pragma warning (pop)
#endif
#ifdef __GNUC__
#pragma GCC visibility pop
#endif

#define PRINT_TAB_CHARS     4

typedef struct {
    const unsigned char *json;
    size_t position;
} error;
static error global_error = { NULL, 0 };

// Forwards
NOTE_C_STATIC J *_jNew_Item(void);

N_CJSON_PUBLIC(const char *) JGetErrorPtr(void)
{
    return (const char*) (global_error.json + global_error.position);
}

N_CJSON_PUBLIC(char *) JGetStringValue(J *item)
{
    if (!JIsString(item)) {
        return NULL;
    }

    return item->valuestring;
}

/* This is a safeguard to prevent copy-pasters from using incompatible C and header files */
#if (N_CJSON_VERSION_MAJOR != 1) || (N_CJSON_VERSION_MINOR != 7) || (N_CJSON_VERSION_PATCH != 7)
#error J.h and J.c have different versions. Make sure that both have the same.
#endif

N_CJSON_PUBLIC(const char*) JVersion(void)
{
    return NOTE_C_STRINGIZE(N_CJSON_VERSION_MAJOR) "." NOTE_C_STRINGIZE(N_CJSON_VERSION_MINOR) "." NOTE_C_STRINGIZE(N_CJSON_VERSION_PATCH);
}

NOTE_C_STATIC char _j_tolower(char c)
{
    if (c < 'A' || c > 'Z') {
        return c;
    }

    // 32 is the distance between any ASCII uppercase letter and its lowercase
    // counterpart.
    return c + 32;
}

/* Case insensitive string comparison, doesn't consider two NULL pointers equal though */
NOTE_C_STATIC int _case_insensitive_strcmp(const unsigned char *string1, const unsigned char *string2)
{
    if ((string1 == NULL) || (string2 == NULL)) {
        return 1;
    }

    if (string1 == string2) {
        return 0;
    }

    for(; _j_tolower(*string1) == _j_tolower(*string2); (void)string1++, string2++) {
        if (*string1 == '\0') {
            return 0;
        }
    }

    return _j_tolower(*string1) - _j_tolower(*string2);
}

NOTE_C_STATIC unsigned char* _j_strdup(const unsigned char* string)
{
    size_t length = 0;
    unsigned char *copy = NULL;

    if (string == NULL) {
        return NULL;
    }

    length = strlen((const char*)string) + sizeof("");
    copy = (unsigned char*)_Malloc(length);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, string, length);

    return copy;
}

/*!
 @brief Dynamically allocate a block of memory of the given size.

 This is simply a wrapper around the memory allocation function provided by the
 user via `NoteSetFn`.

 @param size The number of bytes to allocate.

 @returns A pointer to the first byte of the allocated memory or NULL on error.
 */
N_CJSON_PUBLIC(void *) JMalloc(size_t size)
{
    return _Malloc(size);
}

/*!
 @brief Free a block of dynamically allocated memory.

 This is simply a wrapper around the memory free function provided by the user
 via `NoteSetFn`.

 @param p A pointer to the block of memory to free.
 */
N_CJSON_PUBLIC(void) JFree(void *p)
{
    _Free(p);
}

NOTE_C_STATIC void *_cast_away_const(const void *string);
NOTE_C_STATIC J *_j_new_keyed(int type, const char *name, const char *strval);

/* ---------------------------------------------------------------------------
 * Node storage
 *
 * Every mutation of the two owned pointers -- item->valuestring and
 * item->string -- goes through the helpers below, so the rules about where
 * content lives and who frees it exist in exactly one place.
 *
 * Ownership is expressed by four type flags:
 *
 *   JIsReference    -> valuestring is borrowed from the caller (and, for the
 *                      reference constructors, so is child). Do not free.
 *   JValueInline    -> valuestring points into this node's own allocation.
 *                      Do not free; it goes away with the node.
 *   JStringIsConst  -> string (the key) is not separately allocated, so do not
 *                      free it. Historical flag, historical meaning.
 *   JKeyInline      -> string (the key) points into this node's own allocation,
 *                      as opposed to a caller-owned buffer. Implies
 *                      JStringIsConst, never set without it.
 *
 * The last two look redundant and are not. JStringIsConst answers "may I free
 * this?", which is all the free path needs. JKeyInline answers "is this inside
 * me?", which the packing code needs -- and which cannot be recovered later by
 * comparing the key pointer against the node address, because comparing
 * pointers into two different objects is undefined in C long before it could
 * yield an answer. The one flag that both cases share would force exactly that
 * comparison, so the two are kept apart and set together at the single point
 * where a key is placed.
 *
 * PACKED LAYOUT. The trailing sizeof(JINTEGER)+sizeof(JNUMBER) bytes of the
 * struct are the numeric members, which a JString/JRaw item never uses, so for
 * those items that region doubles as inline character storage and the
 * allocation is extended past the struct only as far as the content needs:
 *
 *     string item : [ ...struct... | value NUL | key NUL ]
 *                   inline region starts at J_INLINE_OFF (= sizeof(J) - 16)
 *     other item  : [ ...struct... ][ key NUL ]
 *                   key starts at sizeof(J); the numeric members are in use
 *
 * Inline storage is an optimization, never a mode. A setter that no longer
 * fits, or that needs the numeric members back, moves the affected content out
 * to the heap first (_j_unpack_numeric_region below). The J address is stable
 * across all of it; only pointers and flags change.
 * ------------------------------------------------------------------------- */

#ifdef NOTE_C_STORAGE_OPTIMIZATION

/* True for the two item types whose value is a character string. Only these
 * lend their numeric bytes out, so this is only referenced by the packing
 * helpers below. */
NOTE_C_STATIC bool _j_is_string_type(const J * const item)
{
    int t = item->type & 0xFF;
    return (t == JString) || (t == JRaw);
}

/* The node's inline character storage: the bytes that valueint and valuenumber
 * occupy. See the long comment on those members in n_cjson.h -- conceptually
 * they are a union with a char buffer, spelled out in code because the union
 * form is not portable to the compilers this library targets.
 *
 * Both bounds are DERIVED with offsetof rather than assumed. Writing
 * `sizeof(J) - (sizeof(JINTEGER) + sizeof(JNUMBER))` would silently be wrong
 * the moment the struct gained a member after valuenumber, or trailing padding,
 * and the mistake would be an aliasing bug rather than a compile error. */
#define J_INLINE_OFF  offsetof(J, valueint)
#define J_INLINE_SIZE (sizeof(J) - J_INLINE_OFF)

/* Compile-time guard on the property the inline region depends on: valueint and
 * valuenumber are contiguous and are the LAST members, so the bytes from
 * offsetof(J, valueint) to the end of the allocation belong to nothing else.
 *
 * The second clause allows for trailing alignment padding, which is bounded by
 * the struct's alignment (JINTEGER's) -- with NOTE_C_SINGLE_PRECISION, JNUMBER
 * is a float and the struct carries 4 bytes of padding -- while still failing
 * if a member were added after valuenumber and grew the struct. It cannot catch a member that fits entirely inside existing
 * padding, so: DO NOT DECLARE ANYTHING AFTER valuenumber.
 *
 * (A negative-width array rather than _Static_assert, so this does not impose
 * C11 on a consumer compiling at an older default.) */
typedef char _j_numeric_pair_is_contiguous[
    (offsetof(J, valuenumber) == offsetof(J, valueint) + sizeof(JINTEGER)) ? 1 : -1];
typedef char _j_numeric_pair_is_last[
    (sizeof(J) - offsetof(J, valuenumber) - sizeof(JNUMBER) < sizeof(JINTEGER)) ? 1 : -1];

/* Byte offset of `p` within this node's allocation.
 *
 * ONLY call this when a flag already establishes that `p` points into this
 * node -- JValueInline for the value, JKeyInline for the key. Pointer
 * subtraction, like relational comparison, is defined only for two pointers
 * into the same object; the flags are what make that true here. Nothing in this
 * file infers ownership BY comparing pointers, because the pointer being tested
 * would routinely be a string literal or an unrelated heap block, and the
 * comparison itself would be undefined before it could produce an answer. */
NOTE_C_STATIC size_t _j_inline_offset_of(const J * const item, const char *p)
{
    return (size_t)(p - (const char *)item);
}

/* True if `p` lies within the struct proper, i.e. it would be destroyed by a
 * write to the numeric members. Same precondition as above. */
NOTE_C_STATIC bool _j_within_struct(const J * const item, const char *p)
{
    return (p != NULL) && (_j_inline_offset_of(item, p) < sizeof(J));
}

/* Byte offset at which this node's key would be stored inline. Whether there is
 * actually room there is _j_key_inline_avail()'s question, not this one's.
 *
 * For a string item the key follows the value inside the inline region. For any
 * other item the numeric members are live, so the key can only go past the
 * struct. */
NOTE_C_STATIC size_t _j_key_inline_offset(const J * const item)
{
    if (!_j_is_string_type(item)) {
        return sizeof(J);
    }
    /* A setter has stored numbers here. Reclaiming the region for a short key
     * would silently destroy them, so the key can only go past the struct. */
    if (item->type & JNumericLive) {
        return sizeof(J);
    }
    if ((item->type & JValueInline) && (item->valuestring != NULL)) {
        return _j_inline_offset_of(item, item->valuestring) + strlen(item->valuestring) + 1;
    }
    /* No inline value, so the whole inline region is available. */
    return J_INLINE_OFF;
}

/* Bytes available for an inline key at that offset. */
NOTE_C_STATIC size_t _j_key_inline_avail(const J * const item)
{
    size_t off = _j_key_inline_offset(item);
    return (item->objlen > off) ? ((size_t)item->objlen - off) : 0;
}

/* The allocation size a node needs to hold this content inline.
 *
 * Returns sizeof(J) -- i.e. "store it all on the heap" -- when the content
 * would push the node past J_MAX_PACKED_ALLOC, which is what keeps objlen
 * representable. */
NOTE_C_STATIC size_t _j_alloc_size(Jbool isString, size_t vlen, size_t klen)
{
    size_t need;

    /* Reject the operands before adding them. Summing first and testing the
     * result afterwards would be wrong: on a 32-bit target a caller-supplied
     * string near SIZE_MAX makes the sum wrap to a small value that then passes
     * the bound check, and the node would be allocated far too small. */
    if ((vlen > J_MAX_PACKED_ALLOC) || (klen > J_MAX_PACKED_ALLOC)) {
        return sizeof(J);
    }

    if (isString) {
        need = J_INLINE_OFF + vlen + klen;
    } else {
        need = sizeof(J) + klen;
    }
    if (need < sizeof(J)) {
        need = sizeof(J);
    }
    if (need > J_MAX_PACKED_ALLOC) {
        return sizeof(J);
    }

    return need;
}

/* Move any content that lives within the struct proper out to the heap, so the
 * numeric members can be written without destroying it.
 *
 * Returns false if that could not be done, having changed nothing that the
 * caller must undo. The caller MUST then abandon the write: the content is
 * still sitting in the bytes the numeric members occupy, and overwriting it
 * would destroy it.
 *
 * An earlier revision treated this as best-effort and cleared the pointer when
 * the copy could not be allocated, on the theory that losing a key beats
 * dangling into storage about to be overwritten. Both are true and both are
 * wrong. Under memory pressure `JSetNumberValue(item, 1.5)` on a packed member
 * silently rendered `{"temp":"hot"}` as `{"":""}` -- the key renamed to empty,
 * the value blanked, and the result still parses, so nothing downstream can
 * detect it. For a Notecard host that means quietly shipping a malformed field
 * to Notehub. The historical layout cannot do this at all: its setters never
 * touch the key.
 *
 * Refusing the write preserves document content, which is the only outcome here
 * that cannot corrupt a document. Note the precise guarantee: content is
 * preserved and the node stays consistent, NOT that it is byte-for-byte
 * untouched -- a value copy that succeeded before a failing key copy has
 * already moved to the heap.
 *
 * Losing the update itself is ordinary note-c behavior under memory pressure:
 * JAddItemToObject and its siblings return void and have always dropped an
 * item silently when an allocation fails. Corrupting a document that then
 * still parses is what was new, and what this prevents. */
NOTE_C_STATIC bool _j_unpack_numeric_region(J * const item)
{
    if ((item->type & JValueInline) && _j_within_struct(item, item->valuestring)) {
        char *copy = (char *)_j_strdup((const unsigned char *)item->valuestring);
        if (copy == NULL) {
            return false;
        }
        item->valuestring = copy;
        item->type &= ~JValueInline;
    }
    /* JKeyInline, not JStringIsConst: a caller-owned key must be left exactly
     * where it is, and is not ours to copy or free. Only a key inside this
     * node's allocation is at risk from the numeric write, and only that case
     * makes the offset test below meaningful. */
    if ((item->type & JKeyInline) && _j_within_struct(item, item->string)) {
        char *copy = (char *)_j_strdup((const unsigned char *)item->string);
        if (copy == NULL) {
            /* The value may already have moved to the heap above. That is a
             * perfectly good state -- heap value, inline key -- and nothing has
             * been lost, so there is nothing to roll back. */
            return false;
        }
        item->string = copy;
        item->type &= ~(JKeyInline | JStringIsConst);
    }

    return true;
}

#endif /* NOTE_C_STORAGE_OPTIMIZATION */

/* True when valueint/valuenumber hold the numbers they appear to hold.
 *
 * This is the exact inverse of the work _j_unpack_numeric_region() would have
 * to do, which is what makes it correct by construction: if nothing needs
 * moving out of the numeric region, nothing is living there.
 *
 * The question is NOT answerable from the node's base type, and an earlier
 * revision that assumed it was introduced a real regression. `JSetIntValue` has
 * always been able to write numeric state onto a JString node without changing
 * its type, and that state was observable through JIntValue, JNumberValue,
 * direct field access and JDuplicate. Guarding those on "is it a string type"
 * made a string node's numbers unreadable even in the default layout, where the
 * members are never overlaid and the historical behavior was never at risk.
 *
 * In the default layout this is unconditionally true, so the historical
 * behavior is restored exactly. */
bool _jNumericIsLive(const J * const item)
{
    if (item == NULL) {
        return false;
    }
#ifdef NOTE_C_STORAGE_OPTIMIZATION
    /* Only string types ever lend their numeric bytes out; for every other type
     * the members are unconditionally numeric. */
    if (_j_is_string_type(item)) {
        return (item->type & JNumericLive) != 0;
    }
#endif
    return true;
}

/* Internal constructor.
 *
 * `alloc` is the total size to allocate, from _j_alloc_size(). The unpacked
 * build ignores it and always allocates sizeof(J), which lets every caller
 * request packing unconditionally. */
NOTE_C_STATIC J *_j_new_item_sized(size_t alloc)
{
#ifndef NOTE_C_STORAGE_OPTIMIZATION
    (void)alloc;
    J *node = (J *)_Malloc(sizeof(J));
#else
    J *node = (J *)_Malloc(alloc);
#endif
    if (node) {
        memset(node, '\0', sizeof(J));
#ifdef NOTE_C_STORAGE_OPTIMIZATION
        node->objlen = (uint16_t)alloc;
#endif
    }

    return node;
}

NOTE_C_STATIC J *_jNew_Item(void)
{
    return _j_new_item_sized(sizeof(J));
}

/* Assign the item's base type without disturbing the ownership flags.
 *
 * The parse functions each assign item->type wholesale once they know what they
 * parsed; a wholesale assignment would clear the flags describing where the
 * key and value already live, and make JDelete() free an interior pointer. */
NOTE_C_STATIC void _j_set_type(J * const item, int type)
{
    item->type = (uint16_t)(type | (item->type & (JIsReference | JStringIsConst | JValueInline | JKeyInline | JNumericLive)));
}

/* Release whatever the node separately owns, per the flags. */
NOTE_C_STATIC void _j_release_strings(J * const item)
{
    if (!(item->type & (JIsReference | JValueInline)) && (item->valuestring != NULL)) {
        _Free(item->valuestring);
    }
    if (!(item->type & JStringIsConst) && (item->string != NULL)) {
        _Free(item->string);
    }
}

/* Total allocation a constructor should request for an item that will hold a
 * value string of `vlen` bytes (including NUL) and a key of `klen` bytes
 * (including NUL, or 0 for none). */
NOTE_C_STATIC size_t _j_size_for(Jbool isString, size_t vlen, size_t klen)
{
#ifndef NOTE_C_STORAGE_OPTIMIZATION
    (void)isString;
    (void)vlen;
    (void)klen;
    return sizeof(J);
#else
    return _j_alloc_size(isString, vlen, klen);
#endif
}

/* Install a value string of `len` bytes including the NUL, preferring this
 * node's inline region. Returns the writable buffer, or NULL on failure. The
 * caller fills it, NUL included. */
NOTE_C_STATIC char *_j_valuestring_reserve(J * const item, size_t len, Jbool allowInline)
{
#ifdef NOTE_C_STORAGE_OPTIMIZATION
    /* Refuse the inline region if the key is already parked in it: the value
     * always starts at J_INLINE_OFF, so writing it would run over a key stored
     * behind it. Unreachable through today's call paths -- the parser reserves
     * the value slot before placing the key -- but the check costs nothing and
     * makes the helper safe to call in any order. */
    if (allowInline
            && _j_is_string_type(item)
            && !(item->type & JNumericLive)
            && ((size_t)item->objlen >= J_INLINE_OFF + len)
            && !((item->type & JKeyInline)
                 && (_j_inline_offset_of(item, item->string) < J_INLINE_OFF + len))) {
        /* Releasing an old heap value first, since we are about to stop
         * pointing at it. */
        if (!(item->type & (JIsReference | JValueInline)) && (item->valuestring != NULL)) {
            _Free(item->valuestring);
        }
        item->valuestring = (char *)item + J_INLINE_OFF;
        item->type |= JValueInline;
        item->type &= ~JIsReference;
        return item->valuestring;
    }
#else
    (void)allowInline;
#endif
    {
        char *heap = (char *)_Malloc(len);
        if (heap == NULL) {
            return NULL;
        }
        if (!(item->type & (JIsReference | JValueInline)) && (item->valuestring != NULL)) {
            _Free(item->valuestring);
        }
        item->valuestring = heap;
        item->type &= ~(JValueInline | JIsReference);
        return heap;
    }
}

/* Copy `s` in as the value string. */
NOTE_C_STATIC Jbool _j_valuestring_set(J * const item, const char *s, Jbool allowInline)
{
    size_t len;
    char *dest;

    if (s == NULL) {
        return false;
    }
    len = strlen(s) + 1;
    dest = _j_valuestring_reserve(item, len, allowInline);
    if (dest == NULL) {
        return false;
    }
    memcpy(dest, s, len);

    return true;
}

/* Install `key` as the node's member key.
 *
 * `constant` means the caller guarantees the key outlives the node, so it is
 * borrowed rather than copied (JAddItemToObjectCS). Otherwise the key is stored
 * in this node's remaining inline room when it fits, and on the heap when it
 * does not.
 *
 * ALIASING: `key` may be the node's own current key -- the documented
 * "move a member, keep its name" idiom does exactly that:
 *
 *     J *m = JDetachItemFromObject(src, name);
 *     JAddItemToObject(dst, JGetItemName(m), m);
 *
 * so the old key is never released before the replacement is in hand. */
NOTE_C_STATIC Jbool _j_key_set(J * const item, const char *key, Jbool constant)
{
    char *oldKey;
    int oldFlags;

    if (key == NULL) {
        return false;
    }

    /* The caller handed back the key we already hold. */
    if (key == item->string) {
        return true;
    }

    /* NOTE: `key` may still ALIAS storage this node owns without being equal to
     * it. The documented "move a member, keep its name" idiom makes that easy to
     * reach with an interior pointer:
     *
     *     J *m = JDetachItemFromObject(src, name);
     *     JAddItemToObject(dst, JGetItemName(m) + 1, m);
     *
     * Under NOTE_C_STORAGE_OPTIMIZATION the key may live in the node's own
     * allocation, so the destination computed below can overlap `key`. Hence
     * memmove rather than memcpy on the inline path; the heap path reads `key`
     * in full via _j_strdup before anything is released, so it is already safe. */

    oldKey = item->string;
    oldFlags = item->type;

    if (constant) {
        /* Borrowed from the caller: not ours to free, and NOT inside the node. */
        item->string = (char *)_cast_away_const(key);
        item->type |= JStringIsConst;
        item->type &= ~JKeyInline;
    } else {
        char *dest = NULL;

#ifdef NOTE_C_STORAGE_OPTIMIZATION
        size_t klen = strlen(key) + 1;
        if (klen <= _j_key_inline_avail(item)) {
            dest = (char *)item + _j_key_inline_offset(item);
            /* memmove, not memcpy: see the aliasing note above. Overlapping
             * memcpy is undefined, and this case is reachable from public API. */
            memmove(dest, key, klen);
            item->string = dest;
            item->type |= (JStringIsConst | JKeyInline);
        }
#endif
        if (dest == NULL) {
            char *copy = (char *)_j_strdup((const unsigned char *)key);
            if (copy == NULL) {
                return false;
            }
            item->string = copy;
            item->type &= ~(JStringIsConst | JKeyInline);
        }
    }

    /* Release the old key only now, and only if it is not the storage we just
     * wrote into. */
    if (!(oldFlags & JStringIsConst) && (oldKey != NULL) && (oldKey != item->string)) {
        _Free(oldKey);
    }

    return true;
}

/* Reserve inline storage for a key of `len` bytes including the NUL, for a
 * constructor that sized the node for it. Returns the writable buffer.
 *
 * The unpacked build hands back a heap block instead, so the parser's code path
 * is identical in both builds. Returns NULL on allocation failure. */
NOTE_C_STATIC char *_j_key_storage(J * const item, size_t len, size_t inlineOffset)
{
#ifdef NOTE_C_STORAGE_OPTIMIZATION
    /* `inlineOffset` is where the caller has decided the key belongs, computed
     * from the lengths it RESERVED rather than from current contents. That
     * distinction matters: the parser reserves the value slot before the value
     * has been decoded, so at this moment the value is still a zero-length
     * placeholder and deriving the key's offset from strlen(valuestring) would
     * park the key on top of the bytes the value is about to occupy. */
    if ((inlineOffset != 0) && ((size_t)item->objlen >= inlineOffset + len)) {
        item->string = (char *)item + inlineOffset;
        item->type |= (JStringIsConst | JKeyInline);
        return item->string;
    }
#else
    (void)inlineOffset;
#endif
    {
        char *heap = (char *)_Malloc(len);
        if (heap == NULL) {
            return NULL;
        }
        item->string = heap;
        item->type &= ~(JStringIsConst | JKeyInline);
        return heap;
    }
}

/*!
 @brief Free a `J` object.

 @param item A pointer to the object.
 */
N_CJSON_PUBLIC(void) JDelete(J *item)
{
    J *next = NULL;
    while (item != NULL) {
        next = item->next;
        if (!(item->type & JIsReference) && (item->child != NULL)) {
            JDelete(item->child);
        }
        _j_release_strings(item);
        _Free(item);
        item = next;
    }
}

/* get the decimal point character of the current locale */
NOTE_C_STATIC unsigned char _get_decimal_point(void)
{
#ifdef ENABLE_LOCALES
    struct lconv *lconv = localeconv();
    return (unsigned char) lconv->decimal_point[0];
#else
    return '.';
#endif
}

typedef struct {
    const unsigned char *content;
    size_t length;
    size_t offset;
    size_t depth; /* How deeply nested (in arrays/objects) is the input at the current offset. */
} parse_buffer;

/* check if the given size is left to read in a given parse buffer (starting with 1) */
#define can_read(buffer, size) ((buffer != NULL) && (((buffer)->offset + size) <= (buffer)->length))
/* check if the buffer can be accessed at the given index (starting with 0) */
#define can_access_at_index(buffer, index) ((buffer != NULL) && (((buffer)->offset + index) < (buffer)->length))
#define cannot_access_at_index(buffer, index) (!can_access_at_index(buffer, index))
/* get a pointer to the buffer at the position */
#define buffer_at_offset(buffer) ((buffer)->content + (buffer)->offset)

/* Parse the input text to generate a number, and populate the result into item. */
NOTE_C_STATIC Jbool _parse_number(J * const item, parse_buffer * const input_buffer)
{
    JNUMBER number = 0;
    unsigned char *after_end = NULL;
    unsigned char number_c_string[64];
    unsigned char decimal_point = _get_decimal_point();
    size_t i = 0;

    if ((input_buffer == NULL) || (input_buffer->content == NULL)) {
        return false;
    }

    /* copy the number into a temporary buffer and replace '.' with the decimal point
     * of the current locale (for strtod)
     * This also takes care of '\0' not necessarily being available for marking the end of the input */
    for (i = 0; (i < (sizeof(number_c_string) - 1)) && can_access_at_index(input_buffer, i); i++) {
        switch (buffer_at_offset(input_buffer)[i]) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '+':
        case '-':
        case 'e':
        case 'E':
            number_c_string[i] = buffer_at_offset(input_buffer)[i];
            break;

        case '.':
            number_c_string[i] = decimal_point;
            break;

        default:
            goto loop_end;
        }
    }
loop_end:
    number_c_string[i] = '\0';

    number = JAtoN((const char*)number_c_string, (char**)&after_end);
    if (number_c_string == after_end) {
        return false; /* parse_error */
    }
    item->valuenumber = number;

    // Saturate valueint in the case of overflow.
    // Cast integer limits to JNUMBER: INT64_MAX is not exactly representable as
    // double, and newer AppleClang rejects the implicit conversion under -Werror.
    if (number >= (JNUMBER)JINTEGER_MAX) {
        item->valueint = JINTEGER_MAX;
    } else if (number <= (JNUMBER)JINTEGER_MIN) {
        item->valueint = JINTEGER_MIN;
    } else {
        item->valueint = JAtoI((const char*)number_c_string);
    }

    _j_set_type(item, JNumber);

    input_buffer->offset += (size_t)(after_end - number_c_string);
    return true;
}

/* Convert a JNUMBER to a JINTEGER, saturating rather than invoking the
 * undefined behavior a plain out-of-range float-to-integer cast would.
 *
 * The limits are cast to JNUMBER before comparison because JINTEGER_MAX is not
 * exactly representable as a double, and newer AppleClang rejects the implicit
 * conversion under -Werror. */
NOTE_C_STATIC JINTEGER _j_saturate_to_integer(JNUMBER number)
{
    if (number >= (JNUMBER)JINTEGER_MAX) {
        return JINTEGER_MAX;
    }
    if (number <= (JNUMBER)JINTEGER_MIN) {
        return JINTEGER_MIN;
    }
    return (JINTEGER)number;
}

/* don't ask me, but the original JSetNumberValue returns an integer or JNUMBER */
N_CJSON_PUBLIC(JNUMBER) JSetNumberHelper(J *object, JNUMBER number)
{
    if (object == NULL) {
        return number;
    }

#ifdef NOTE_C_STORAGE_OPTIMIZATION
    /* The numeric members double as inline character storage, so anything
     * living there has to move to the heap before they are written. This is why
     * JSetNumberValue is a function and not a macro: a macro expanding to
     * `(object)->valuenumber = ...` in the caller's translation unit could not
     * do this, and would silently destroy a packed key or value.
     *
     * If the evacuation cannot allocate, refuse the write. Proceeding would
     * overwrite the key and value in place, which parses cleanly afterwards and
     * so cannot be detected downstream; preserving the existing content cannot
     * corrupt a document. Same shape as the NULL-object path above: the operand
     * comes back and the numeric members are not written. */
    if (!_j_unpack_numeric_region(object)) {
        return number;
    }
    /* Record that these bytes now hold numbers. Without it the region looks
     * unoccupied and _j_key_set() would reclaim it for a short key, silently
     * overwriting what is about to be written; and a reference copied from this
     * node could not tell numbers from packed characters. Only meaningful for a
     * string type -- for anything else the members are always numeric -- but it
     * is cheaper to set unconditionally than to branch. */
    object->type |= JNumericLive;
#endif

    // Saturate valueint in the case of overflow.
    object->valueint = _j_saturate_to_integer(number);

    return object->valuenumber = number;
}

/* Assign an integer value, propagating it to valuenumber so the two stay in
 * step. Backs the JSetIntValue macro.
 *
 * The parameter is JNUMBER, not JINTEGER, and that is load-bearing. The
 * historical macro was
 *
 *     ((object) ? (object)->valueint = (object)->valuenumber = (number) : (number))
 *
 * which parses as `valueint = (valuenumber = number)`, so BOTH members received
 * the operand in its original arithmetic form. JSetIntValue(item, 1.5) left
 * valueint == 1 and valuenumber == 1.5. Declaring this parameter JINTEGER would
 * truncate at the call site and store 1 in both, silently changing a public
 * contract; taking JNUMBER reproduces the old result exactly.
 *
 * The operand is also evaluated exactly once, as it was before. The old macro
 * repeated `object` three times but never repeated `number`, so a value
 * expression with side effects behaves as it always has.
 *
 * One divergence is intentional. The macro's implicit JNUMBER-to-JINTEGER
 * conversion is undefined when the value is out of JINTEGER's range; this
 * saturates instead, as JSetNumberHelper() already did. Every input for which
 * the macro was defined yields the same result. */
N_CJSON_PUBLIC(JINTEGER) JSetIntHelper(J *object, JNUMBER number)
{
    if (object == NULL) {
        return _j_saturate_to_integer(number);
    }

#ifdef NOTE_C_STORAGE_OPTIMIZATION
    /* See JSetNumberHelper(): free the numeric members of any inline content,
     * and refuse the write rather than overwrite it if that is not possible. */
    if (!_j_unpack_numeric_region(object)) {
        return _j_saturate_to_integer(number);
    }
    object->type |= JNumericLive;   /* see JSetNumberHelper() */
#endif

    object->valuenumber = number;

    return object->valueint = _j_saturate_to_integer(number);
}

typedef struct {
    unsigned char *buffer;
    size_t length;
    size_t offset;
    size_t depth; /* current nesting depth (for formatted printing) */
    Jbool noalloc;
    Jbool format; /* is this print a formatted print */
    Jbool omitempty;
} printbuffer;

/* realloc printbuffer if necessary to have at least "needed" bytes more */
NOTE_C_STATIC unsigned char* _ensure(printbuffer * const p, size_t needed)
{
    unsigned char *newbuffer = NULL;
    size_t newsize = 0;

    if ((p == NULL) || (p->buffer == NULL)) {
        return NULL;
    }

    if ((p->length > 0) && (p->offset >= p->length)) {
        /* make sure that offset is valid */
        return NULL;
    }

    needed += p->offset + 1;
    if (needed > INT_MAX) {
        /* sizes bigger than INT_MAX are currently not supported */
        return NULL;
    }

    if (needed <= p->length) {
        return p->buffer + p->offset;
    }

    if (p->noalloc) {
        return NULL;
    }

    /* otherwise reallocate manually */
    newsize = (ALLOC_CHUNK * ((needed / ALLOC_CHUNK) + ((needed % ALLOC_CHUNK) > 0)));  // chunked, linear calculation for new buffer to reduce memory waste
    newbuffer = (unsigned char*)_Malloc(newsize);
    if (!newbuffer) {
        _Free(p->buffer);
        p->length = 0;
        p->buffer = NULL;
        return NULL;
    }
    if (newbuffer) {
        memcpy(newbuffer, p->buffer, p->offset + 1);
    }
    _Free(p->buffer);

    p->length = newsize;
    p->buffer = newbuffer;

    return newbuffer + p->offset;
}

/* calculate the new length of the string in a printbuffer and update the offset */
NOTE_C_STATIC void _update_offset(printbuffer * const buffer)
{
    const unsigned char *buffer_pointer = NULL;
    if ((buffer == NULL) || (buffer->buffer == NULL)) {
        return;
    }
    buffer_pointer = buffer->buffer + buffer->offset;

    buffer->offset += strlen((const char*)buffer_pointer);
}

/* Render the number nicely from the given item into a string. */
NOTE_C_STATIC Jbool _print_number(const J * const item, printbuffer * const output_buffer)
{
    if (item == NULL) {
        return false;
    }

    unsigned char *output_pointer = NULL;
    JNUMBER vnum = item->valuenumber;
    JINTEGER vint = item->valueint;
    int length = 0;
    size_t i = 0;
    unsigned char number_buffer[JNTOA_MAX]; /* temporary buffer to print the number into */
    unsigned char decimal_point = _get_decimal_point();

    if (output_buffer == NULL) {
        return false;
    }

    /* This checks for NaN and Infinity */
    if ((vnum * 0) != 0) {
        char *nbuf = (char *) number_buffer;
        strlcpy(nbuf, "null", JNTOA_MAX);
        length = strlen(nbuf);
    } else {
        char *nbuf = (char *) number_buffer;

        if (vnum != (JNUMBER)vint) {
            JNtoA(vnum, nbuf, -1);
        } else {
            JItoA(vint, nbuf);
        }

        length = strlen(nbuf);
    }

    /* conversion failed or buffer overrun occured */
    if ((length < 0) || (length > (int)(sizeof(number_buffer) - 1))) {
        return false;
    }

    /* reserve appropriate space in the output */
    output_pointer = _ensure(output_buffer, (size_t)length + sizeof(""));
    if (output_pointer == NULL) {
        return false;
    }

    /* copy the printed number to the output and replace locale
     * dependent decimal point with '.' */
    for (i = 0; i < ((size_t)length); i++) {
        if (number_buffer[i] == decimal_point) {
            output_pointer[i] = '.';
            continue;
        }

        output_pointer[i] = number_buffer[i];
    }
    output_pointer[i] = '\0';

    output_buffer->offset += (size_t)length;

    return true;
}

/* parse 4 digit hexadecimal number */
NOTE_C_STATIC unsigned long _parse_hex4(const unsigned char * const input)
{
    unsigned long int h = 0;
    size_t i = 0;

    for (i = 0; i < 4; i++) {
        /* parse digit */
        if ((input[i] >= '0') && (input[i] <= '9')) {
            h += (unsigned int) input[i] - '0';
        } else if ((input[i] >= 'A') && (input[i] <= 'F')) {
            h += (unsigned int) 10 + input[i] - 'A';
        } else if ((input[i] >= 'a') && (input[i] <= 'f')) {
            h += (unsigned int) 10 + input[i] - 'a';
        } else { /* invalid */
            return 0;
        }

        if (i < 3) {
            /* shift left to make place for the next nibble */
            h = h << 4;
        }
    }

    return h;
}

/* converts a UTF-16 literal to UTF-8
 * A literal can be one or two sequences of the form \uXXXX */
NOTE_C_STATIC unsigned char _utf16_literal_to_utf8(const unsigned char * const input_pointer, const unsigned char * const input_end, unsigned char **output_pointer)
{
    long unsigned int codepoint = 0;
    unsigned long int first_code = 0;
    const unsigned char *first_sequence = input_pointer;
    unsigned char utf8_length = 0;
    unsigned char utf8_position = 0;
    unsigned char sequence_length = 0;
    unsigned char first_byte_mark = 0;

    if ((input_end - first_sequence) < 6) {
        /* input ends unexpectedly */
        goto fail;
    }

    /* get the first utf16 sequence */
    first_code = _parse_hex4(first_sequence + 2);

    /* check that the code is valid */
    if (((first_code >= 0xDC00) && (first_code <= 0xDFFF))) {
        goto fail;
    }

    /* UTF16 surrogate pair */
    if ((first_code >= 0xD800) && (first_code <= 0xDBFF)) {
        const unsigned char *second_sequence = first_sequence + 6;
        unsigned int second_code = 0;
        sequence_length = 12; /* \uXXXX\uXXXX */

        if ((input_end - second_sequence) < 6) {
            /* input ends unexpectedly */
            goto fail;
        }

        if ((second_sequence[0] != '\\') || (second_sequence[1] != 'u')) {
            /* missing second half of the surrogate pair */
            goto fail;
        }

        /* get the second utf16 sequence */
        second_code = _parse_hex4(second_sequence + 2);
        /* check that the code is valid */
        if ((second_code < 0xDC00) || (second_code > 0xDFFF)) {
            /* invalid second half of the surrogate pair */
            goto fail;
        }


        /* calculate the unicode codepoint from the surrogate pair */
        codepoint = 0x10000 + (((first_code & 0x3FF) << 10) | (second_code & 0x3FF));
    } else {
        sequence_length = 6; /* \uXXXX */
        codepoint = first_code;
    }

    /* encode as UTF-8
     * takes at maximum 4 bytes to encode:
     * 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */
    if (codepoint < 0x80) {
        /* normal ascii, encoding 0xxxxxxx */
        utf8_length = 1;
    } else if (codepoint < 0x800) {
        /* two bytes, encoding 110xxxxx 10xxxxxx */
        utf8_length = 2;
        first_byte_mark = 0xC0; /* 11000000 */
    } else if (codepoint < 0x10000) {
        /* three bytes, encoding 1110xxxx 10xxxxxx 10xxxxxx */
        utf8_length = 3;
        first_byte_mark = 0xE0; /* 11100000 */
    } else if (codepoint <= 0x10FFFF) {
        /* four bytes, encoding 1110xxxx 10xxxxxx 10xxxxxx 10xxxxxx */
        utf8_length = 4;
        first_byte_mark = 0xF0; /* 11110000 */
    } else {
        /* invalid unicode codepoint */
        goto fail;
    }

    /* encode as utf8 */
    for (utf8_position = (unsigned char)(utf8_length - 1); utf8_position > 0; utf8_position--) {
        /* 10xxxxxx */
        (*output_pointer)[utf8_position] = (unsigned char)((codepoint | 0x80) & 0xBF);
        codepoint >>= 6;
    }
    /* encode first byte */
    if (utf8_length > 1) {
        (*output_pointer)[0] = (unsigned char)((codepoint | first_byte_mark) & 0xFF);
    } else {
        (*output_pointer)[0] = (unsigned char)(codepoint & 0x7F);
    }

    *output_pointer += utf8_length;

    return sequence_length;

fail:
    return 0;
}

/* Parse the input text into an unescaped cinput, and populate item. */
/* Measure the JSON string token at the current offset without consuming it.
 *
 * On success *maxLen receives an upper bound on the decoded length INCLUDING the
 * terminating NUL (escapes only ever shrink), and *tokenEnd points at the
 * closing quote. Split out of _parse_string() so that _parse_object() can size a
 * node and its inline key in a single allocation before the node exists. */
NOTE_C_STATIC Jbool _j_parse_string_measure(parse_buffer * const input_buffer,
        size_t *maxLen,
        const unsigned char **tokenEnd)
{
    const unsigned char *input_end = NULL;
    size_t skipped_bytes = 0;

    if ((input_buffer == NULL) || (input_buffer->content == NULL)) {
        return false;
    }
    if (cannot_access_at_index(input_buffer, 0) || (buffer_at_offset(input_buffer)[0] != '\"')) {
        return false;
    }

    input_end = buffer_at_offset(input_buffer) + 1;
    while (((size_t)(input_end - input_buffer->content) < input_buffer->length) && (*input_end != '\"')) {
        /* is escape sequence */
        if (input_end[0] == '\\') {
            if ((size_t)(input_end + 1 - input_buffer->content) >= input_buffer->length) {
                /* prevent buffer overflow when last input character is a backslash */
                return false;
            }
            skipped_bytes++;
            input_end++;
        }
        input_end++;
    }
    if (((size_t)(input_end - input_buffer->content) >= input_buffer->length) || (*input_end != '\"')) {
        return false; /* string ended unexpectedly */
    }

    /* The span runs from the opening quote to the closing quote, so the decoded
     * content is at most (span - 1 - escapes) bytes; adding one for the NUL
     * gives (span - escapes). Counting the opening quote as content here would
     * over-reserve by a byte, which is harmless for correctness but pushes a
     * 15-character string just past the inline capacity. */
    *maxLen = (size_t)(input_end - buffer_at_offset(input_buffer)) - skipped_bytes;
    *tokenEnd = input_end;

    return true;
}

/* Decode the measured token into `output`, which must have room for the
 * measured length. Writes the terminating NUL and advances the input past the
 * closing quote. */
NOTE_C_STATIC Jbool _j_parse_string_decode(parse_buffer * const input_buffer,
        const unsigned char * const input_end,
        unsigned char * const output)
{
    const unsigned char *input_pointer = buffer_at_offset(input_buffer) + 1;
    unsigned char *output_pointer = output;

    /* loop through the string literal */
    while (input_pointer < input_end) {
        if (*input_pointer != '\\') {
            *output_pointer++ = *input_pointer++;
        }
        /* escape sequence */
        else {
            unsigned char sequence_length = 2;
            if ((input_end - input_pointer) < 1) {
                goto fail;
            }

            switch (input_pointer[1]) {
            case 'b':
                *output_pointer++ = '\b';
                break;
            case 'f':
                *output_pointer++ = '\f';
                break;
            case 'n':
                *output_pointer++ = '\n';
                break;
            case 'r':
                *output_pointer++ = '\r';
                break;
            case 't':
                *output_pointer++ = '\t';
                break;
            case '\"':
            case '\\':
            case '/':
                *output_pointer++ = input_pointer[1];
                break;

            /* UTF-16 literal */
            case 'u':
                sequence_length = _utf16_literal_to_utf8(input_pointer, input_end, &output_pointer);
                if (sequence_length == 0) {
                    /* failed to convert UTF16-literal to UTF-8 */
                    goto fail;
                }
                break;

            default:
                goto fail;
            }
            input_pointer += sequence_length;
        }
    }

    /* zero terminate the output */
    *output_pointer = '\0';

    input_buffer->offset = (size_t) (input_end - input_buffer->content);
    input_buffer->offset++;

    return true;

fail:
    if (input_pointer != NULL) {
        input_buffer->offset = (size_t)(input_pointer - input_buffer->content);
    }

    return false;
}

/* Parse the string at the current offset into item->valuestring. */
NOTE_C_STATIC Jbool _parse_string(J * const item, parse_buffer * const input_buffer)
{
    const unsigned char *input_end = NULL;
    size_t maxLen = 0;
    char *output = NULL;

    if (!_j_parse_string_measure(input_buffer, &maxLen, &input_end)) {
        return false;
    }

    /* Set the type before reserving, so the inline-storage heuristics see the
     * node's final type. */
    _j_set_type(item, JString);

    output = _j_valuestring_reserve(item, maxLen, true);
    if (output == NULL) {
        return false;
    }

    if (!_j_parse_string_decode(input_buffer, input_end, (unsigned char *)output)) {
        /* Release whatever the reserve just installed, so the caller's cleanup
         * does not see a half-built value. Uses the same ownership predicate as
         * _j_release_strings(), so an interior pointer is never freed. */
        if (!(item->type & (JIsReference | JValueInline))) {
            _Free(item->valuestring);
        }
        item->valuestring = NULL;
        item->type &= ~JValueInline;
        return false;
    }

    return true;
}

/* Convert a 16-bit number to 4 hex digits, null-terminating it */
void _n_htoa16(uint16_t n, unsigned char *p)
{
    int i;
    for (i=0; i<4; i++) {
        uint16_t nibble = (n >> 12) & 0xff;
        n = n << 4;
        if (nibble >= 10) {
            *p++ = 'A' + (nibble-10);
        } else {
            *p++ = '0' + nibble;
        }
    }
    *p = '\0';
}

/* Render the cstring provided to an escaped version that can be printed. */
NOTE_C_STATIC Jbool _print_string_ptr(const unsigned char * const input, printbuffer * const output_buffer)
{
    const unsigned char *input_pointer = NULL;
    unsigned char *output = NULL;
    unsigned char *output_pointer = NULL;
    size_t output_length = 0;
    /* numbers of additional characters needed for escaping */
    size_t escape_characters = 0;

    if (output_buffer == NULL) {
        return false;
    }

    /* empty string */
    if (input == NULL) {
        output = _ensure(output_buffer, 2);  // sizeof("\"\"")
        if (output == NULL) {
            return false;
        }
        output[0] = '"';
        output[1] = '"';
        output[2] = '\0';

        return true;
    }

    /* set "flag" to 1 if something needs to be escaped */
    for (input_pointer = input; *input_pointer; input_pointer++) {
        switch (*input_pointer) {
        case '\"':
        case '\\':
        case '\b':
        case '\f':
        case '\n':
        case '\r':
        case '\t':
            /* one character escape sequence */
            escape_characters++;
            break;
        default:
            if (*input_pointer < 32) {
                /* UTF-16 escape sequence uXXXX */
                escape_characters += 5;
            }
            break;
        }
    }
    output_length = (size_t)(input_pointer - input) + escape_characters;

    output = _ensure(output_buffer, output_length + 2);  // sizeof("\"\"")
    if (output == NULL) {
        return false;
    }

    /* no characters have to be escaped */
    if (escape_characters == 0) {
        output[0] = '\"';
        memcpy(output + 1, input, output_length);
        output[output_length + 1] = '\"';
        output[output_length + 2] = '\0';

        return true;
    }

    output[0] = '\"';
    output_pointer = output + 1;
    /* copy the string */
    for (input_pointer = input; *input_pointer != '\0'; (void)input_pointer++, output_pointer++) {
        if ((*input_pointer > 31) && (*input_pointer != '\"') && (*input_pointer != '\\')) {
            /* normal character, copy */
            *output_pointer = *input_pointer;
        } else {
            /* character needs to be escaped */
            *output_pointer++ = '\\';
            switch (*input_pointer) {
            case '\\':
                *output_pointer = '\\';
                break;
            case '\"':
                *output_pointer = '\"';
                break;
            case '\b':
                *output_pointer = 'b';
                break;
            case '\f':
                *output_pointer = 'f';
                break;
            case '\n':
                *output_pointer = 'n';
                break;
            case '\r':
                *output_pointer = 'r';
                break;
            case '\t':
                *output_pointer = 't';
                break;
            default:
                /* escape and print as unicode codepoint */
                *output_pointer++ = 'u';
                _n_htoa16(*input_pointer, output_pointer);
                /* _n_htoa16() writes 4 hex digits plus a NUL without advancing
                 * output_pointer, and the enclosing for loop supplies a further
                 * increment, so advancing by 3 here makes a \uXXXX escape
                 * consume exactly the 6 bytes that escape_characters budgets
                 * for it above. This was += 4, which over-advanced by one per
                 * escape: it overran the output reservation and left a stray
                 * NUL that truncated the document. */
                output_pointer += 3;
                break;
            }
        }
    }
    output[output_length + 1] = '\"';
    output[output_length + 2] = '\0';

    return true;
}

/* Invoke _print_string_ptr (which is useful) on an item. */
NOTE_C_STATIC Jbool _print_string(const J * const item, printbuffer * const p)
{
    return _print_string_ptr((unsigned char*)item->valuestring, p);
}

/* Predeclare these prototypes. */
NOTE_C_STATIC Jbool _parse_value(J * const item, parse_buffer * const input_buffer);
NOTE_C_STATIC Jbool _print_value(const J * const item, printbuffer * const output_buffer);
NOTE_C_STATIC Jbool _parse_array(J * const item, parse_buffer * const input_buffer);
NOTE_C_STATIC Jbool _print_array(const J * const item, printbuffer * const output_buffer);
NOTE_C_STATIC Jbool _parse_object(J * const item, parse_buffer * const input_buffer);
NOTE_C_STATIC Jbool _print_object(const J * const item, printbuffer * const output_buffer);

/* Utility to jump whitespace and cr/lf */
NOTE_C_STATIC parse_buffer *_buffer_skip_whitespace(parse_buffer * const buffer)
{
    if ((buffer == NULL) || (buffer->content == NULL)) {
        return NULL;
    }

    while (can_access_at_index(buffer, 0) && (buffer_at_offset(buffer)[0] <= 32)) {
        buffer->offset++;
    }

    if (buffer->offset == buffer->length) {
        buffer->offset--;
    }

    return buffer;
}

/* skip the UTF-8 BOM (byte order mark) if it is at the beginning of a buffer */
NOTE_C_STATIC parse_buffer *_skip_utf8_bom(parse_buffer * const buffer)
{
    if ((buffer == NULL) || (buffer->content == NULL) || (buffer->offset != 0)) {
        return NULL;
    }

    if (can_access_at_index(buffer, 4) && (strncmp((const char*)buffer_at_offset(buffer), "\xEF\xBB\xBF", 3) == 0)) {
        buffer->offset += 3;
    }

    return buffer;
}

/* Parse an object - create a new root, and populate. */
N_CJSON_PUBLIC(J *) JParseWithOpts(const char *value, const char **return_parse_end, Jbool require_null_terminated)
{
    parse_buffer buffer = { 0, 0, 0, 0 };
    J *item = NULL;

    /* reset error position */
    global_error.json = NULL;
    global_error.position = 0;

    if (value == NULL) {
        goto fail;
    }

    buffer.content = (const unsigned char*)value;
    buffer.length = strlen((const char*)value) + 1;   // Trailing '\0'
    buffer.offset = 0;

    item = _jNew_Item();
    if (item == NULL) { /* memory fail */
        goto fail;
    }

    if (!_parse_value(item, _buffer_skip_whitespace(_skip_utf8_bom(&buffer)))) {
        /* parse failure. ep is set. */
        goto fail;
    }

    /* if we require null-terminated JSON without appended garbage, skip and then check for a null terminator */
    if (require_null_terminated) {
        _buffer_skip_whitespace(&buffer);
        if ((buffer.offset >= buffer.length) || buffer_at_offset(&buffer)[0] != '\0') {
            goto fail;
        }
    }
    if (return_parse_end) {
        *return_parse_end = (const char*)buffer_at_offset(&buffer);
    }

    return item;

fail:
    if (item != NULL) {
        JDelete(item);
    }

    if (value != NULL) {
        error local_error;
        local_error.json = (const unsigned char*)value;
        local_error.position = 0;

        if (buffer.offset < buffer.length) {
            local_error.position = buffer.offset;
        } else if (buffer.length > 0) {
            local_error.position = buffer.length - 1;
        }

        if (return_parse_end != NULL) {
            *return_parse_end = (const char*)local_error.json + local_error.position;
        }

        global_error = local_error;
    }

    return NULL;
}

/*!
 @brief Parse the passed in C-string as JSON and return a `J` object
        representing it.

 @param value The JSON object as a C-string.

 @returns A `J` object or NULL on error (e.g. the string was invalid JSON).
 */
N_CJSON_PUBLIC(J *) JParse(const char *value)
{
    return JParseWithOpts(value, 0, 0);
}

#define cjson_min(a, b) ((a < b) ? a : b)

NOTE_C_STATIC unsigned char *_print(const J * const item, Jbool format, Jbool omitempty)
{
    static const size_t default_buffer_size = 128;
    printbuffer buffer[1];
    unsigned char *printed = NULL;

    memset(buffer, 0, sizeof(buffer));

    /* create buffer */
    buffer->buffer = (unsigned char*) _Malloc(default_buffer_size);
    buffer->length = default_buffer_size;
    buffer->format = format;
    buffer->omitempty = omitempty;
    if (buffer->buffer == NULL) {
        goto fail;
    }

    /* print the value */
    if (!_print_value(item, buffer)) {
        goto fail;
    }
    _update_offset(buffer);

    /* copy the JSON over to a new buffer */
    printed = (unsigned char*) _Malloc(buffer->offset + 1);
    if (printed == NULL) {
        goto fail;
    }
    memcpy(printed, buffer->buffer, cjson_min(buffer->length, buffer->offset + 1));
    printed[buffer->offset] = '\0'; /* just to be sure */

    /* free the buffer */
    _Free(buffer->buffer);

    return printed;

fail:
    if (buffer->buffer != NULL) {
        _Free(buffer->buffer);
    }

    if (printed != NULL) {
        _Free(printed);
    }

    return NULL;
}

/* Render a J item/entity/structure to text. */
N_CJSON_PUBLIC(char *) JPrint(const J *item)
{
    if (item == NULL) {
        return NULL;
    }
    return (char*)_print(item, true, false);
}

/*!
 @brief Get the unformatted string representation of a `J` object.

 The string returned by this function is dynamically allocated and MUST be freed
 by the caller with `JFree`. Unformatted means that the minimum JSON string
 is produced, without any additional whitespace.

 @param item The JSON object to get the unformatted string representation of.

 @returns The string or NULL on error.
 */
N_CJSON_PUBLIC(char *) JPrintUnformatted(const J *item)
{
    if (item == NULL) {
        return NULL;
    }
    return (char*)_print(item, false, false);
}

N_CJSON_PUBLIC(char *) JPrintUnformattedOmitEmpty(const J *item)
{
    if (item == NULL) {
        return NULL;
    }
    return (char*)_print(item, false, true);
}

N_CJSON_PUBLIC(char *) JPrintBuffered(const J *item, int prebuffer, Jbool fmt)
{
    printbuffer p = { 0, 0, 0, 0, 0, 0, 0 };

    if (item == NULL) {
        return NULL;
    }

    if (prebuffer < 0) {
        return NULL;
    }

    p.buffer = (unsigned char*)_Malloc((size_t)prebuffer);
    if (!p.buffer) {
        return NULL;
    }

    p.length = (size_t)prebuffer;
    p.offset = 0;
    p.noalloc = false;
    p.format = fmt;

    if (!_print_value(item, &p)) {
        _Free(p.buffer);
        return NULL;
    }

    return (char*)p.buffer;
}

NOTE_C_STATIC Jbool _printPreallocated(J *item, char *buf, const int len, const Jbool fmt, const Jbool omit)
{
    printbuffer p = { 0, 0, 0, 0, 0, 0, 0 };

    if (item == NULL) {
        return false;
    }
    if ((len < 0) || (buf == NULL)) {
        return false;
    }

    p.buffer = (unsigned char*)buf;
    p.length = (size_t)len;
    p.offset = 0;
    p.noalloc = true;
    p.format = fmt;
    p.omitempty = omit;

    return _print_value(item, &p);
}

N_CJSON_PUBLIC(Jbool) JPrintPreallocatedOmitEmpty(J *item, char *buf, const int len, const Jbool fmt)
{
    return _printPreallocated(item, buf, len, fmt, true);
}

N_CJSON_PUBLIC(Jbool) JPrintPreallocated(J *item, char *buf, const int len, const Jbool fmt)
{
    return _printPreallocated(item, buf, len, fmt, false);
}

/* Parser core - when encountering text, process appropriately. */
NOTE_C_STATIC Jbool _parse_value(J * const item, parse_buffer * const input_buffer)
{
    if (item == NULL) {
        return false;
    }
    if ((input_buffer == NULL) || (input_buffer->content == NULL)) {
        return false; /* no input */
    }

    /* parse the different types of values */
    /* null */
    if (can_read(input_buffer, 4) && (strncmp((const char*)buffer_at_offset(input_buffer), c_null, c_null_len) == 0)) {
        _j_set_type(item, JNULL);
        input_buffer->offset += 4;
        return true;
    }
    /* false */
    if (can_read(input_buffer, 5) && (strncmp((const char*)buffer_at_offset(input_buffer), c_false, c_false_len) == 0)) {
        _j_set_type(item, JFalse);
        input_buffer->offset += 5;
        return true;
    }
    /* true */
    if (can_read(input_buffer, 4) && (strncmp((const char*)buffer_at_offset(input_buffer), c_true, c_true_len) == 0)) {
        _j_set_type(item, JTrue);
        item->valueint = 1;
        input_buffer->offset += 4;
        return true;
    }
    /* string */
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == '\"')) {
        return _parse_string(item, input_buffer);
    }
    /* number */
    if (can_access_at_index(input_buffer, 0) && ((buffer_at_offset(input_buffer)[0] == '-') || ((buffer_at_offset(input_buffer)[0] >= '0') && (buffer_at_offset(input_buffer)[0] <= '9')))) {
        return _parse_number(item, input_buffer);
    }
    /* array */
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == '[')) {
        return _parse_array(item, input_buffer);
    }
    /* object */
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == '{')) {
        return _parse_object(item, input_buffer);
    }

    return false;
}

/* Render a value to text. */
NOTE_C_STATIC Jbool _print_value(const J * const item, printbuffer * const output_buffer)
{
    unsigned char *output = NULL;

    if ((item == NULL) || (output_buffer == NULL)) {
        return false;
    }

    switch ((item->type) & 0xFF) {
    case JNULL:
        output = _ensure(output_buffer, c_null_len+1);
        if (output == NULL) {
            return false;
        }
        strlcpy((char*)output, c_null, c_null_len+1);
        return true;

    case JFalse:
        output = _ensure(output_buffer, c_false_len+1);
        if (output == NULL) {
            return false;
        }
        strlcpy((char*)output, c_false, c_false_len+1);
        return true;

    case JTrue:
        output = _ensure(output_buffer, c_true_len+1);
        if (output == NULL) {
            return false;
        }
        strlcpy((char*)output, c_true, c_true_len+1);
        return true;

    case JNumber:
        return _print_number(item, output_buffer);

    case JRaw: {
        size_t raw_length = 0;
        if (item->valuestring == NULL) {
            return false;
        }

        raw_length = strlen(item->valuestring) + 1;   // Trailing '\0';
        output = _ensure(output_buffer, raw_length);
        if (output == NULL) {
            return false;
        }
        memcpy(output, item->valuestring, raw_length);
        return true;
    }

    case JString:
        return _print_string(item, output_buffer);

    case JArray:
        return _print_array(item, output_buffer);

    case JObject:
        return _print_object(item, output_buffer);

    default:
        return false;
    }
}

/* Build an array from input text. */
NOTE_C_STATIC Jbool _parse_array(J * const item, parse_buffer * const input_buffer)
{
    // This is a static function that is only called internally, and we are
    // guaranteed that item is not NULL. `cppcheck` is not able to infer this
    // from the code, because we are using a macro, NOTE_C_STATIC, to remove
    // the static keyword in the public header during testing.

    J *head = NULL; /* head of the linked list */
    J *current_item = NULL;

    // cppcheck-suppress nullPointerRedundantCheck
    if (input_buffer->depth >= N_CJSON_NESTING_LIMIT) {
        return false; /* to deeply nested */
    }
    // cppcheck-suppress nullPointerRedundantCheck
    input_buffer->depth++;

    // cppcheck-suppress nullPointerRedundantCheck
    if (buffer_at_offset(input_buffer)[0] != '[') {
        /* not an array */
        goto fail;
    }

    // _buffer_skip_whitespace() will verify the input_buffer is not NULL at
    // the new offset.
    // cppcheck-suppress nullPointerRedundantCheck
    input_buffer->offset++;
    _buffer_skip_whitespace(input_buffer);
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == ']')) {
        /* empty array */
        goto success;
    }

    /* check if we skipped to the end of the buffer */
    if (cannot_access_at_index(input_buffer, 0)) {
        // _buffer_skip_whitespace() has moved us one beyond the end, therefore
        // we need to move back one, and we know we can access it.
        // cppcheck-suppress nullPointerRedundantCheck
        input_buffer->offset--;
        goto fail;
    }

    /* step back to character in front of the first element */
    input_buffer->offset--;

    /* loop through the comma separated array elements */
    do {
        /* allocate next item */
        J *new_item = _jNew_Item();
        if (new_item == NULL) {
            goto fail; /* allocation failure */
        }

        /* attach next item to list */
        if (head == NULL) {
            /* start the linked list */
            current_item = head = new_item;
        } else {
            /* add to the end and advance */
            current_item->next = new_item;
            new_item->prev = current_item;
            current_item = new_item;
        }

        /* parse next value */
        input_buffer->offset++;
        _buffer_skip_whitespace(input_buffer);
        if (!_parse_value(current_item, input_buffer)) {
            goto fail; /* failed to parse value */
        }
        _buffer_skip_whitespace(input_buffer);
    } while (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == ','));

    if (cannot_access_at_index(input_buffer, 0) || buffer_at_offset(input_buffer)[0] != ']') {
        goto fail; /* expected end of array */
    }

success:
    input_buffer->depth--;

    _j_set_type(item, JArray);
    item->child = head;

    input_buffer->offset++;

    return true;

fail:
    if (head != NULL) {
        JDelete(head);
    }

    return false;
}

/* Render an array to text */
NOTE_C_STATIC Jbool _print_array(const J * const item, printbuffer * const output_buffer)
{
    unsigned char *output_pointer = NULL;
    size_t length = 0;
    J *current_element = item->child;

    if (output_buffer == NULL) {
        return false;
    }

    /* Compose the output array. */
    /* opening square bracket */
    output_pointer = _ensure(output_buffer, 1);
    if (output_pointer == NULL) {
        return false;
    }

    *output_pointer = '[';
    output_buffer->offset++;
    output_buffer->depth++;

    while (current_element != NULL) {
        if (!_print_value(current_element, output_buffer)) {
            return false;
        }
        _update_offset(output_buffer);
        if (current_element->next) {
            length = (size_t) (output_buffer->format ? 2 : 1);
            output_pointer = _ensure(output_buffer, length + 1);
            if (output_pointer == NULL) {
                return false;
            }
            *output_pointer++ = ',';
            if(output_buffer->format) {
                *output_pointer++ = ' ';
            }
            *output_pointer = '\0';
            output_buffer->offset += length;
        }
        current_element = current_element->next;
    }

    output_pointer = _ensure(output_buffer, 2);
    if (output_pointer == NULL) {
        return false;
    }
    *output_pointer++ = ']';
    *output_pointer = '\0';
    output_buffer->depth--;

    return true;
}

/* Build an object from the text. */
NOTE_C_STATIC Jbool _parse_object(J * const item, parse_buffer * const input_buffer)
{
    // This is a static function that is only called internally, and we are
    // guaranteed that item is not NULL. `cppcheck` is not able to infer this
    // from the code, because we are using a macro, NOTE_C_STATIC, to remove
    // the static keyword in the public header during testing.

    J *head = NULL; /* linked list head */
    J *current_item = NULL;

    // cppcheck-suppress nullPointerRedundantCheck
    if (input_buffer->depth >= N_CJSON_NESTING_LIMIT) {
        return false; /* to deeply nested */
    }
    // cppcheck-suppress nullPointerRedundantCheck
    input_buffer->depth++;

    if (cannot_access_at_index(input_buffer, 0) || (buffer_at_offset(input_buffer)[0] != '{')) {
        goto fail; /* not an object */
    }

    // _buffer_skip_whitespace() will verify the input_buffer is not NULL at
    // the new offset.
    // cppcheck-suppress nullPointerRedundantCheck
    input_buffer->offset++;
    _buffer_skip_whitespace(input_buffer);
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == '}')) {
        goto success; /* empty object */
    }

    /* check if we skipped to the end of the buffer */
    if (cannot_access_at_index(input_buffer, 0)) {
        // _buffer_skip_whitespace() has moved us one beyond the end, therefore
        // we need to move back one, and we know we can access it.
        // cppcheck-suppress nullPointerRedundantCheck
        input_buffer->offset--;
        goto fail;
    }

    /* step back to character in front of the first element */
    // cppcheck-suppress nullPointerRedundantCheck
    input_buffer->offset--;

    /* loop through the comma separated array elements */
    do {
        J *new_item = NULL;
        const unsigned char *keyEnd = NULL;
        size_t keyMax = 0;
        size_t valMax = 0;
        Jbool valueIsString = false;

        /* Measure the key BEFORE allocating the node, then look ahead at the
         * value, so the node, its key AND a short string value can all be
         * carved out of a single allocation. Both measurements are upper bounds
         * (escape sequences only shrink), so content containing escapes
         * over-reserves by a few bytes -- still strictly better than the extra
         * heap blocks it replaces. */
        // _buffer_skip_whitespace() will verify the input_buffer is not NULL at
        // the new offset.
        // cppcheck-suppress nullPointerRedundantCheck
        input_buffer->offset++;
        _buffer_skip_whitespace(input_buffer);
        if (!_j_parse_string_measure(input_buffer, &keyMax, &keyEnd)) {
            goto fail; /* failed to parse name */
        }

        /* Look ahead past the ':' for the value. Only its FIRST byte is needed
         * to know whether it is a string, and if it is, it can be measured
         * without consuming anything. The input offset is restored either way. */
        {
            // cppcheck cannot infer that input_buffer is non-NULL here, because
            // NOTE_C_STATIC removes the static keyword during test builds. Same
            // suppressions as the rest of this function.
            // cppcheck-suppress nullPointerRedundantCheck
            size_t savedOffset = input_buffer->offset;
            // cppcheck-suppress nullPointerRedundantCheck
            size_t afterKey = (size_t)(keyEnd - input_buffer->content) + 1;
            const unsigned char *valEnd = NULL;

            // cppcheck-suppress nullPointerRedundantCheck
            input_buffer->offset = afterKey;
            _buffer_skip_whitespace(input_buffer);
            if (can_access_at_index(input_buffer, 0)
                    && (buffer_at_offset(input_buffer)[0] == ':')) {
                input_buffer->offset++;
                _buffer_skip_whitespace(input_buffer);
                if (can_access_at_index(input_buffer, 0)
                        && (buffer_at_offset(input_buffer)[0] == '\"')) {
                    if (_j_parse_string_measure(input_buffer, &valMax, &valEnd)) {
                        valueIsString = true;
                    } else {
                        valMax = 0;
                    }
                }
            }
            // cppcheck-suppress nullPointerRedundantCheck
            input_buffer->offset = savedOffset;
        }

        /* allocate next item, sized for its key and (if a string) its value */
        new_item = _j_new_item_sized(_j_size_for(valueIsString, valMax, keyMax));
        if (new_item == NULL) {
            goto fail; /* allocation failure */
        }
        if (valueIsString) {
            /* Declare the type now so the storage helpers place the key after
             * the value inside the inline region rather than past the struct. */
            _j_set_type(new_item, JString);
        }

        /* attach next item to list */
        if (head == NULL) {
            /* start the linked list */
            current_item = head = new_item;
        } else {
            /* add to the end and advance */
            current_item->next = new_item;
            new_item->prev = current_item;
            current_item = new_item;
        }

        /* Reserve the value slot first when the value is a string, so the key
         * lands after it inside the same allocation. The value bytes are filled
         * in later by _parse_value; only the placement is decided here. */
        if (valueIsString) {
            if (_j_valuestring_reserve(current_item, valMax, true) == NULL) {
                goto fail; /* allocation failure */
            }
            /* Keep the reserved region a valid empty string until it is filled,
             * so an early failure cannot leave unterminated bytes behind. */
            current_item->valuestring[0] = '\0';
        }

        /* Decode the key straight into the node's own storage. In the unpacked
         * build _j_key_storage() hands back a heap block instead, so this loop
         * is identical in both builds.
         *
         * The key follows the RESERVED value slot for a string member, and the
         * struct for anything else (whose numeric members are live). */
        {
            size_t keyOffset;
            char *keyDest;
#ifdef NOTE_C_STORAGE_OPTIMIZATION
            keyOffset = valueIsString ? (J_INLINE_OFF + valMax) : sizeof(J);
#else
            keyOffset = 0;
#endif
            keyDest = _j_key_storage(current_item, keyMax, keyOffset);
            if (keyDest == NULL) {
                goto fail; /* allocation failure */
            }
            if (!_j_parse_string_decode(input_buffer, keyEnd, (unsigned char *)keyDest)) {
                goto fail; /* failed to parse name */
            }
        }
        _buffer_skip_whitespace(input_buffer);

        if (cannot_access_at_index(input_buffer, 0) || (buffer_at_offset(input_buffer)[0] != ':')) {
            goto fail; /* invalid object */
        }

        /* parse the value */
        input_buffer->offset++;
        _buffer_skip_whitespace(input_buffer);
        if (!_parse_value(current_item, input_buffer)) {
            goto fail; /* failed to parse value */
        }
        _buffer_skip_whitespace(input_buffer);
    } while (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == ','));

    if (cannot_access_at_index(input_buffer, 0) || (buffer_at_offset(input_buffer)[0] != '}')) {
        goto fail; /* expected end of object */
    }

success:
    input_buffer->depth--;

    _j_set_type(item, JObject);
    item->child = head;

    input_buffer->offset++;
    return true;

fail:
    if (head != NULL) {
        JDelete(head);
    }

    return false;
}

/* See if there is another non-omitted item looking forward */
NOTE_C_STATIC bool _last_non_omitted_object(J * item, printbuffer * const output_buffer)
{
    if (!output_buffer->omitempty) {
        return (item->next == 0);
    }
    while (item->next != 0) {
        item = item->next;
        int type = JGetItemType(item);
        if (type != JTYPE_BOOL_FALSE && type != JTYPE_NUMBER_ZERO && type != JTYPE_STRING_BLANK) {
            return false;
        }
    }
    return true;
}

/* Render an object to text. */
NOTE_C_STATIC Jbool _print_object(const J * const item, printbuffer * const output_buffer)
{
    unsigned char *output_pointer = NULL;
    size_t length = 0;
    J *current_item = item->child;

    if (output_buffer == NULL) {
        return false;
    }

    /* Compose the output: */
    length = (size_t) (output_buffer->format ? 2 : 1); /* fmt: {\n */
    output_pointer = _ensure(output_buffer, length + 1);
    if (output_pointer == NULL) {
        return false;
    }

    *output_pointer++ = '{';
    output_buffer->depth++;
    if (output_buffer->format) {
        *output_pointer++ = '\n';
    }
    output_buffer->offset += length;

    while (current_item) {

        /* See if it should be eliminated because of omitempty.
         *
         * This must be decided BEFORE any indentation is emitted. Emitting the
         * indentation first left orphaned whitespace behind for every elided
         * member -- {"a":0} rendered as "{\n    }" -- and consumed buffer the
         * caller never budgeted for. It also meant formatted omit-empty output
         * was not a fixed point: re-printing a re-parsed document produced
         * different whitespace. */
        bool omit = false;
        if (output_buffer->omitempty) {
            int type = JGetItemType(current_item);
            omit =(type == JTYPE_BOOL_FALSE || type == JTYPE_NUMBER_ZERO || type == JTYPE_STRING_BLANK);
        }

        /* print item only if not omitted */
        if (!omit) {

            if (output_buffer->format) {
                size_t i;
#if (PRINT_TAB_CHARS == 0)
                size_t needed = output_buffer->depth;
#else
                size_t needed = output_buffer->depth * PRINT_TAB_CHARS;
#endif
                output_pointer = _ensure(output_buffer, needed);
                if (output_pointer == NULL) {
                    return false;
                }
                for (i = 0; i < output_buffer->depth; i++) {
#if (PRINT_TAB_CHARS == 0)
                    *output_pointer++ = '\t';
                    output_buffer->offset++;
#else
                    for (int tc=0; tc<PRINT_TAB_CHARS; tc++) {
                        *output_pointer++ = ' ';
                        output_buffer->offset++;
                    }
#endif
                }
            }

            /* print key */
            if (!_print_string_ptr((unsigned char*)current_item->string, output_buffer)) {
                return false;
            }
            _update_offset(output_buffer);

            length = (size_t) (output_buffer->format ? 2 : 1);
            output_pointer = _ensure(output_buffer, length);
            if (output_pointer == NULL) {
                return false;
            }
            *output_pointer++ = ':';
            if (output_buffer->format) {
#if (PRINT_TAB_CHARS == 0)
                *output_pointer++ = '\t';
#else
                *output_pointer++ = ' ';
#endif
            }
            output_buffer->offset += length;

            /* print value */
            if (!_print_value(current_item, output_buffer)) {
                return false;
            }
            _update_offset(output_buffer);

            /* print comma if not last */
            bool more_fields_coming = !_last_non_omitted_object(current_item, output_buffer);
            length = (size_t) ((output_buffer->format ? 1 : 0) + (more_fields_coming ? 1 : 0));
            output_pointer = _ensure(output_buffer, length + 1);
            if (output_pointer == NULL) {
                return false;
            }
            if (more_fields_coming) {
                *output_pointer++ = ',';
            }

            if (output_buffer->format) {
                *output_pointer++ = '\n';
            }
            *output_pointer = '\0';
            output_buffer->offset += length;
        }

        current_item = current_item->next;
    }

#if (PRINT_TAB_CHARS == 0)
    int needed = output_buffer->format ? (output_buffer->depth - 1) : 0;
#else
    int needed = output_buffer->format ? ((output_buffer->depth - 1) * PRINT_TAB_CHARS) : 0;
#endif
    needed += 2; // }\0
    output_pointer = _ensure(output_buffer, needed);
    if (output_pointer == NULL) {
        return false;
    }
    if (output_buffer->format) {
        size_t i;
        for (i = 0; i < (output_buffer->depth - 1); i++) {
#if (PRINT_TAB_CHARS == 0)
            *output_pointer++ = '\t';
#else
            for (int tc=0; tc<PRINT_TAB_CHARS; tc++) {
                *output_pointer++ = ' ';
            }
#endif
        }
    }
    *output_pointer++ = '}';
    *output_pointer = '\0';
    output_buffer->depth--;

    return true;
}

/* Get Array size/item / object item. */
N_CJSON_PUBLIC(int) JGetArraySize(const J *array)
{
    J *child = NULL;
    size_t size = 0;

    if (array == NULL) {
        return 0;
    }

    child = array->child;

    while(child != NULL) {
        size++;
        child = child->next;
    }

    /* FIXME: Can overflow here. Cannot be fixed without breaking the API */

    return (int)size;
}

NOTE_C_STATIC J* _get_array_item(const J *array, size_t index)
{
    J *current_child = NULL;

    if (array == NULL) {
        return NULL;
    }

    current_child = array->child;
    while ((current_child != NULL) && (index > 0)) {
        index--;
        current_child = current_child->next;
    }

    return current_child;
}

N_CJSON_PUBLIC(J *) JGetArrayItem(const J *array, int index)
{
    if (array == NULL) {
        return NULL;
    }
    if (index < 0) {
        return NULL;
    }

    return _get_array_item(array, (size_t)index);
}

NOTE_C_STATIC J *_get_object_item(const J * const object, const char * const name, const Jbool case_sensitive)
{
    J *current_element = NULL;

    if ((object == NULL) || (name == NULL)) {
        return NULL;
    }

    current_element = object->child;
    if (case_sensitive) {
        while ((current_element != NULL) && (strcmp(name, current_element->string) != 0)) {
            current_element = current_element->next;
        }
    } else {
        while ((current_element != NULL) && (_case_insensitive_strcmp((const unsigned char*)name, (const unsigned char*)(current_element->string)) != 0)) {
            current_element = current_element->next;
        }
    }

    return current_element;
}

N_CJSON_PUBLIC(J *) JGetObjectItem(const J * const object, const char * const string)
{
    if (object == NULL) {
        return NULL;
    }
    return _get_object_item(object, string, false);
}

N_CJSON_PUBLIC(J *) JGetObjectItemCaseSensitive(const J * const object, const char * const string)
{
    if (object == NULL) {
        return NULL;
    }
    return _get_object_item(object, string, true);
}

N_CJSON_PUBLIC(Jbool) JHasObjectItem(const J *object, const char *string)
{
    if (object == NULL) {
        return false;
    }
    return JGetObjectItem(object, string) ? 1 : 0;
}

/* Utility for array list handling. */
NOTE_C_STATIC void _suffix_object(J *prev, J *item)
{
    prev->next = item;
    item->prev = prev;
}

/* Utility for handling references. */
NOTE_C_STATIC J *_create_reference(const J *item)
{
    J *reference = NULL;
    if (item == NULL) {
        return NULL;
    }

    reference = _jNew_Item();
    if (reference == NULL) {
        return NULL;
    }

    memcpy(reference, item, sizeof(J));
    reference->string = NULL;
    reference->type |= JIsReference;
    reference->next = reference->prev = NULL;
    /* The key was just dropped, so no key-ownership flag may survive the copy.
     * JStringIsConst on a NULL key is merely inert, but JKeyInline would be an
     * active lie -- it asserts that reference->string addresses THIS node. */
    reference->type &= ~(JStringIsConst | JKeyInline);
#ifdef NOTE_C_STORAGE_OPTIMIZATION
    /* The memcpy above copied the SOURCE's packing state into a node that has
     * its own, smaller allocation. Left uncorrected, objlen would claim this
     * node is as large as the source, and JValueInline would claim a
     * valuestring pointing into the SOURCE lies inside THIS node -- from which
     * _j_key_inline_offset() would compute a key offset relative to the wrong
     * allocation, and _j_key_set() would then write the key at that offset.
     * Reset both to describe the reference itself: its own allocation is
     * exactly sizeof(J), and its value string is borrowed, which is what
     * JIsReference above already records. */
    reference->objlen = (uint16_t)sizeof(J);
    reference->type &= ~JValueInline;
#endif
    /* NOTE: this is a shallow copy, so if the source's value string lives in the
     * source's own allocation the reference points into it. That is the same
     * borrowed-pointer relationship the unpacked build has with the source's
     * separate heap block -- in both layouts a reference must not outlive the
     * item it references. JIsReference is set below, so the reference never
     * frees it either way. */
    return reference;
}

NOTE_C_STATIC Jbool _add_item_to_array(J *array, J *item)
{
    J *child = NULL;

    if ((item == NULL) || (array == NULL)) {
        return false;
    }

    child = array->child;

    if (child == NULL) {
        /* list is empty, start new one */
        array->child = item;
    } else {
        /* append to the end */
        while (child->next) {
            child = child->next;
        }
        _suffix_object(child, item);
    }

    return true;
}

/* Add item to array/object. */
N_CJSON_PUBLIC(void) JAddItemToArray(J *array, J *item)
{
    if (array == NULL || item == NULL) {
        JDelete(item);
        return;
    }
    if (!_add_item_to_array(array, item)) {
        JDelete(item);
    }
}

#if defined(__clang__) || (defined(__GNUC__)  && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 5))))
#pragma GCC diagnostic push
#endif
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif
/* helper function to cast away const */
NOTE_C_STATIC void* _cast_away_const(const void* string)
{
    return (void*)string;
}
#if defined(__clang__) || (defined(__GNUC__)  && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 5))))
#pragma GCC diagnostic pop
#endif


NOTE_C_STATIC Jbool _add_item_to_object(J * const object, const char * const string, J * const item, const Jbool constant_key)
{
    if ((object == NULL) || (string == NULL) || (item == NULL)) {
        return false;
    }

    if (!_j_key_set(item, string, constant_key)) {
        return false;
    }

    return _add_item_to_array(object, item);
}

N_CJSON_PUBLIC(void) JAddItemToObject(J *object, const char *string, J *item)
{
    if (object == NULL || string == NULL || item == NULL) {
        JDelete(item);
        return;
    }
    if (!_add_item_to_object(object, string, item, false)) {
        JDelete(item);
    }
}

/* Add an item to an object with constant string as key */
N_CJSON_PUBLIC(void) JAddItemToObjectCS(J *object, const char *string, J *item)
{
    if (object == NULL || string == NULL || item == NULL) {
        JDelete(item);
        return;
    }
    if (!_add_item_to_object(object, string, item, true)) {
        JDelete(item);
    }
}

N_CJSON_PUBLIC(void) JAddItemReferenceToArray(J *array, J *item)
{
    if (array == NULL || item == NULL) {
        return;
    }
    J *ref = _create_reference(item);
    if (!_add_item_to_array(array, ref)) {
        JDelete(ref);
    }
}

N_CJSON_PUBLIC(void) JAddItemReferenceToObject(J *object, const char *string, J *item)
{
    if (object == NULL || string == NULL || item == NULL) {
        return;
    }
    J *ref = _create_reference(item);
    if (!_add_item_to_object(object, string, ref, false)) {
        JDelete(ref);
    }
}

/* Create an item sized to hold `name` as an inline key, and `strval` as an
 * inline value when the type is a string type.
 *
 * The JAdd<T>ToObject helpers know the key at the moment they construct the
 * item, so unlike the generic JAddItemToObject path they can reserve room for
 * it. Without this a JAddNumberToObject() key always needed its own heap block,
 * because JCreateNumber() allocates exactly sizeof(J) and a node can never grow
 * afterwards -- its address has already been handed to the caller. */
NOTE_C_STATIC J *_j_new_keyed(int type, const char *name, const char *strval)
{
    size_t klen = (name != NULL) ? strlen(name) + 1 : 0;
    size_t vlen = (strval != NULL) ? strlen(strval) + 1 : 0;
    Jbool isString = (type == JString) || (type == JRaw);
    J *item = _j_new_item_sized(_j_size_for(isString, vlen, klen));

    if (item != NULL) {
        item->type = (uint16_t)type;
        if (isString && !_j_valuestring_set(item, strval, true)) {
            JDelete(item);
            return NULL;
        }
    }

    return item;
}

N_CJSON_PUBLIC(J*) JAddTrueToObject(J * const object, const char * const name)
{
    if (object == NULL) {
        return NULL;
    }

    J *true_item = _j_new_keyed(JTrue, name, NULL);
    if (_add_item_to_object(object, name, true_item, false)) {
        return true_item;
    }

    JDelete(true_item);
    return NULL;
}

N_CJSON_PUBLIC(J*) JAddFalseToObject(J * const object, const char * const name)
{
    if (object == NULL) {
        return NULL;
    }

    J *false_item = _j_new_keyed(JFalse, name, NULL);
    if (_add_item_to_object(object, name, false_item, false)) {
        return false_item;
    }

    JDelete(false_item);
    return NULL;
}

/*!
 @brief Add a boolean field to a `J` object.

 @param object The object to add the field to.
 @param name The name of the field.
 @param boolean The value of the field.

 @returns A pointer to the newly-added boolean field or NULL on error.
 */
N_CJSON_PUBLIC(J*) JAddBoolToObject(J * const object, const char * const name, const Jbool boolean)
{
    if (object == NULL) {
        return NULL;
    }

    J *bool_item = _j_new_keyed(boolean ? JTrue : JFalse, name, NULL);
    if (_add_item_to_object(object, name, bool_item, false)) {
        return bool_item;
    }

    JDelete(bool_item);
    return NULL;
}

/*!
 @brief Add a number field to a `J` object.

 @param object The object to add the field to.
 @param name The name of the field.
 @param number The value of the field.

 @returns A pointer to the newly-added number field or NULL on error.
 */
N_CJSON_PUBLIC(J*) JAddNumberToObject(J * const object, const char * const name, const JNUMBER number)
{
    if (object == NULL) {
        return NULL;
    }

    J *number_item = _j_new_keyed(JNumber, name, NULL);
    if (number_item != NULL) {
        JSetNumberHelper(number_item, number);
    }
    if (_add_item_to_object(object, name, number_item, false)) {
        return number_item;
    }

    JDelete(number_item);
    return NULL;
}

N_CJSON_PUBLIC(J*) JAddIntToObject(J * const object, const char * const name, const JINTEGER integer)
{
    if (object == NULL) {
        return NULL;
    }

    J *integer_item = _j_new_keyed(JNumber, name, NULL);
    if (integer_item != NULL) {
        JSetIntHelper(integer_item, integer);
    }
    if (_add_item_to_object(object, name, integer_item, false)) {
        return integer_item;
    }

    JDelete(integer_item);
    return NULL;
}

/*!
 @brief Add a string field to a `J` object.

 @param object The object to add the field to.
 @param name The name of the field.
 @param string The value of the field.

 @returns A pointer to the newly-added string field or NULL on error.
 */
N_CJSON_PUBLIC(J*) JAddStringToObject(J * const object, const char * const name, const char * const string)
{
    if (object == NULL || string == NULL) {
        return NULL;
    }

    J *string_item = _j_new_keyed(JString, name, string);
    if (_add_item_to_object(object, name, string_item, false)) {
        return string_item;
    }

    JDelete(string_item);
    return NULL;
}

N_CJSON_PUBLIC(J*) JAddRawToObject(J * const object, const char * const name, const char * const raw)
{
    if (object == NULL || raw == NULL) {
        return NULL;
    }

    J *raw_item = _j_new_keyed(JRaw, name, raw);
    if (_add_item_to_object(object, name, raw_item, false)) {
        return raw_item;
    }

    JDelete(raw_item);
    return NULL;
}

/*!
 @brief Add an object field to another `J` object.

 @param object The object to add the field to.
 @param name The name of the field.

 @returns A pointer to the newly-added object field or NULL on error.
 */
N_CJSON_PUBLIC(J*) JAddObjectToObject(J * const object, const char * const name)
{
    if (object == NULL) {
        return NULL;
    }

    J *object_item = _j_new_keyed(JObject, name, NULL);
    if (_add_item_to_object(object, name, object_item, false)) {
        return object_item;
    }

    JDelete(object_item);
    return NULL;
}

/*!
 @brief Add an array field to a `J` object.

 @param object The object to add the field to.
 @param name The name of the field.

 @returns A pointer to the newly-added array field or NULL on error.
 */
N_CJSON_PUBLIC(J*) JAddArrayToObject(J * const object, const char * const name)
{
    if (object == NULL) {
        return NULL;
    }

    J *array = _j_new_keyed(JArray, name, NULL);
    if (_add_item_to_object(object, name, array, false)) {
        return array;
    }

    JDelete(array);
    return NULL;
}

N_CJSON_PUBLIC(J *) JDetachItemViaPointer(J *parent, J * const item)
{
    if (parent == NULL || item == NULL) {
        return NULL;
    }

    if (item->prev != NULL) {
        /* not the first element */
        item->prev->next = item->next;
    }
    if (item->next != NULL) {
        /* not the last element */
        item->next->prev = item->prev;
    }

    if (item == parent->child) {
        /* first element */
        parent->child = item->next;
    }
    /* make sure the detached item doesn't point anywhere anymore */
    item->prev = NULL;
    item->next = NULL;

    return item;
}

N_CJSON_PUBLIC(J *) JDetachItemFromArray(J *array, int which)
{
    if (array == NULL) {
        return NULL;
    }
    if (which < 0) {
        return NULL;
    }

    return JDetachItemViaPointer(array, _get_array_item(array, (size_t)which));
}

N_CJSON_PUBLIC(void) JDeleteItemFromArray(J *array, int which)
{
    if (array == NULL) {
        return;
    }
    JDelete(JDetachItemFromArray(array, which));
}

N_CJSON_PUBLIC(J *) JDetachItemFromObject(J *object, const char *string)
{
    if (object == NULL) {
        return NULL;
    }

    J *to_detach = JGetObjectItem(object, string);

    return JDetachItemViaPointer(object, to_detach);
}

N_CJSON_PUBLIC(J *) JDetachItemFromObjectCaseSensitive(J *object, const char *string)
{
    if (object == NULL) {
        return NULL;
    }

    J *to_detach = JGetObjectItemCaseSensitive(object, string);

    return JDetachItemViaPointer(object, to_detach);
}

N_CJSON_PUBLIC(void) JDeleteItemFromObject(J *object, const char *string)
{
    if (object == NULL) {
        return;
    }
    JDelete(JDetachItemFromObject(object, string));
}

N_CJSON_PUBLIC(void) JDeleteItemFromObjectCaseSensitive(J *object, const char *string)
{
    if (object == NULL) {
        return;
    }
    JDelete(JDetachItemFromObjectCaseSensitive(object, string));
}

/* Replace array/object items with new ones. */
N_CJSON_PUBLIC(void) JInsertItemInArray(J *array, int which, J *newitem)
{
    if (array == NULL || newitem == NULL) {
        JDelete(newitem);
        return;
    }

    J *after_inserted = NULL;

    if (which < 0) {
        JDelete(newitem);
        return;
    }

    after_inserted = _get_array_item(array, (size_t)which);
    if (after_inserted == NULL) {
        if (!_add_item_to_array(array, newitem)) {
            JDelete(newitem);
        }
        return;
    }

    newitem->next = after_inserted;
    newitem->prev = after_inserted->prev;
    after_inserted->prev = newitem;
    if (after_inserted == array->child) {
        array->child = newitem;
    } else {
        newitem->prev->next = newitem;
    }
}

N_CJSON_PUBLIC(Jbool) JReplaceItemViaPointer(J * const parent, J * const item, J * replacement)
{
    if (parent == NULL || replacement == NULL || item == NULL) {
        return false;
    }

    if (replacement == item) {
        return true;
    }

    replacement->next = item->next;
    replacement->prev = item->prev;

    if (replacement->next != NULL) {
        replacement->next->prev = replacement;
    }
    if (replacement->prev != NULL) {
        replacement->prev->next = replacement;
    }
    if (parent->child == item) {
        parent->child = replacement;
    }

    item->next = NULL;
    item->prev = NULL;
    JDelete(item);

    return true;
}

N_CJSON_PUBLIC(void) JReplaceItemInArray(J *array, int which, J *newitem)
{
    if (array == NULL || newitem == NULL) {
        JDelete(newitem);
        return;
    }

    if (which < 0) {
        JDelete(newitem);
        return;
    }

    if (!JReplaceItemViaPointer(array, _get_array_item(array, (size_t)which), newitem)) {
        JDelete(newitem);
    }
}

NOTE_C_STATIC Jbool _replace_item_in_object(J *object, const char *string, J *replacement, Jbool case_sensitive)
{
    if (object == NULL || replacement == NULL || string == NULL) {
        return false;
    }

    J *existing = _get_object_item(object, string, case_sensitive);
    if (existing == NULL) {
        return false;
    }

    /* replace the name in the replacement */
    if (!_j_key_set(replacement, string, false)) {
        return false;
    }

    JReplaceItemViaPointer(object, existing, replacement);

    return true;
}

N_CJSON_PUBLIC(void) JReplaceItemInObject(J *object, const char *string, J *newitem)
{
    if (object == NULL || newitem == NULL) {
        JDelete(newitem);
        return;
    }
    if (!_replace_item_in_object(object, string, newitem, false)) {
        JDelete(newitem);
    }
}

N_CJSON_PUBLIC(void) JReplaceItemInObjectCaseSensitive(J *object, const char *string, J *newitem)
{
    if (object == NULL || newitem == NULL) {
        JDelete(newitem);
        return;
    }
    if (!_replace_item_in_object(object, string, newitem, true)) {
        JDelete(newitem);
    }
}

N_CJSON_PUBLIC(J *) JCreateTrue(void)
{
    J *item = _jNew_Item();
    if(item) {
        item->type = JTrue;
    }
    return item;
}

N_CJSON_PUBLIC(J *) JCreateFalse(void)
{
    J *item = _jNew_Item();
    if(item) {
        item->type = JFalse;
    }
    return item;
}

N_CJSON_PUBLIC(J *) JCreateBool(Jbool b)
{
    J *item = _jNew_Item();
    if(item) {
        item->type = b ? JTrue : JFalse;
    }
    return item;
}

N_CJSON_PUBLIC(J *) JCreateNumber(JNUMBER num)
{
    J *item = _jNew_Item();
    if(item) {
        item->type = JNumber;
        item->valuenumber = num;

        // Saturate valueint in the case of overflow.
        // Cast integer limits to JNUMBER: INT64_MAX is not exactly representable as
        // double, and newer AppleClang rejects the implicit conversion under -Werror.
        if (num >= (JNUMBER)JINTEGER_MAX) {
            item->valueint = JINTEGER_MAX;
        } else if (num <= (JNUMBER)JINTEGER_MIN) {
            item->valueint = JINTEGER_MIN;
        } else {
            item->valueint = (JINTEGER)num;
        }
    }
    return item;
}

N_CJSON_PUBLIC(J *) JCreateInteger(JINTEGER integer)
{
    J *item = _jNew_Item();
    if(item) {
        item->type = JNumber;
        item->valuenumber = (JNUMBER)integer;
        item->valueint = integer;
    }
    return item;
}

N_CJSON_PUBLIC(J *) JCreateString(const char *string)
{
    J *item;
    size_t len;

    if (string == NULL) {
        return NULL;
    }

    /* Reserve trailing room up front for a value too long to inline, so the
     * string lands in the node's own allocation instead of a second block. */
    len = strlen(string) + 1;
    item = _j_new_item_sized(_j_size_for(true, len, 0));
    if (item) {
        item->type = JString;
        if (!_j_valuestring_set(item, string, true)) {
            JDelete(item);
            return NULL;
        }
    }
    return item;
}

N_CJSON_PUBLIC(J *) JCreateStringValue(const char *string)
{
    J *item = _jNew_Item();
    if (item != NULL) {
        item->type = JString;
        item->valuestring = (char*)_cast_away_const(string);
    }
    return item;
}

N_CJSON_PUBLIC(J *) JCreateStringReference(const char *string)
{
    J *item = _jNew_Item();
    if (item != NULL) {
        item->type = JString | JIsReference;
        item->valuestring = (char*)_cast_away_const(string);
    }
    return item;
}

N_CJSON_PUBLIC(J *) JCreateObjectReference(const J *child)
{
    if (child == NULL) {
        return NULL;
    }
    J *item = _jNew_Item();
    if (item != NULL) {
        item->type = JObject | JIsReference;
        item->child = (J*)_cast_away_const(child);
    }
    return item;
}

N_CJSON_PUBLIC(J *) JCreateArrayReference(const J *child)
{
    if (child == NULL) {
        return NULL;
    }
    J *item = _jNew_Item();
    if (item != NULL) {
        item->type = JArray | JIsReference;
        item->child = (J*)_cast_away_const(child);
    }
    return item;
}

N_CJSON_PUBLIC(J *) JCreateRaw(const char *raw)
{
    J *item;
    size_t len;

    if (raw == NULL) {
        return NULL;
    }

    len = strlen(raw) + 1;
    item = _j_new_item_sized(_j_size_for(true, len, 0));
    if(item) {
        item->type = JRaw;
        if (!_j_valuestring_set(item, raw, true)) {
            JDelete(item);
            return NULL;
        }
    }
    return item;
}

N_CJSON_PUBLIC(J *) JCreateArray(void)
{
    J *item = _jNew_Item();
    if(item) {
        item->type=JArray;
    }
    return item;
}

/*!
 @brief Create a new `J` object.

 To free the object, use `JDelete`.

 @returns A pointer to the newly-created object.
 */
N_CJSON_PUBLIC(J *) JCreateObject(void)
{
    J *item = _jNew_Item();
    if (item) {
        item->type = JObject;
    }
    return item;
}

/* Create Arrays: */
N_CJSON_PUBLIC(J *) JCreateIntArray(const long int *numbers, int count)
{
    size_t i = 0;
    J *n = NULL;
    J *p = NULL;
    J *a = NULL;

    if ((count < 0) || (numbers == NULL)) {
        return NULL;
    }

    a = JCreateArray();
    for(i = 0; a && (i < (size_t)count); i++) {
        n = JCreateNumber(numbers[i]);
        if (!n) {
            JDelete(a);
            return NULL;
        }
        if(!i) {
            a->child = n;
        } else {
            _suffix_object(p, n);
        }
        p = n;
    }

    return a;
}

N_CJSON_PUBLIC(J *) JCreateNumberArray(const JNUMBER *numbers, int count)
{
    size_t i = 0;
    J *n = NULL;
    J *p = NULL;
    J *a = NULL;

    if ((count < 0) || (numbers == NULL)) {
        return NULL;
    }

    a = JCreateArray();

    for(i = 0; a && (i < (size_t)count); i++) {
        n = JCreateNumber(numbers[i]);
        if(!n) {
            JDelete(a);
            return NULL;
        }
        if(!i) {
            a->child = n;
        } else {
            _suffix_object(p, n);
        }
        p = n;
    }

    return a;
}

N_CJSON_PUBLIC(J *) JCreateStringArray(const char **strings, int count)
{
    size_t i = 0;
    J *n = NULL;
    J *p = NULL;
    J *a = NULL;

    if ((count < 0) || (strings == NULL)) {
        return NULL;
    }

    a = JCreateArray();

    for (i = 0; a && (i < (size_t)count); i++) {
        n = JCreateString(strings[i]);
        if(!n) {
            JDelete(a);
            return NULL;
        }
        if(!i) {
            a->child = n;
        } else {
            _suffix_object(p,n);
        }
        p = n;
    }

    return a;
}

/* Duplication */
N_CJSON_PUBLIC(J *) JDuplicate(const J *item, Jbool recurse)
{
    J *newitem = NULL;
    J *child = NULL;
    J *next = NULL;
    J *newchild = NULL;

    /* Bail on bad ptr */
    if (!item) {
        goto fail;
    }
    /* Create new item, reserving room for a value string too long to inline so
     * the duplicate packs exactly as a freshly constructed node would. */
    newitem = _j_new_item_sized(
                  _j_size_for(item->valuestring != NULL,
                              item->valuestring != NULL ? strlen(item->valuestring) + 1 : 0,
                              item->string != NULL ? strlen(item->string) + 1 : 0));
    if (!newitem) {
        goto fail;
    }
    /* Copy over all vars.
     *
     * Guarded on whether the source's numeric members currently hold numbers,
     * not on its base type. A JString node can carry live numeric state -- a
     * setter writes it without changing the type -- and the historical
     * JDuplicate copied it. Keying this on the type silently dropped it, in the
     * default layout too. When the source is lending those bytes to an inline
     * value or key there is no number to copy, and the value is reinstated
     * below anyway. */
    newitem->type = (uint16_t)(item->type & ~(JIsReference | JStringIsConst | JValueInline | JKeyInline));
    if (_jNumericIsLive(item)) {
        newitem->valueint = item->valueint;
        newitem->valuenumber = item->valuenumber;
    }
    if (item->valuestring) {
        /* Inline the copy only if the SOURCE's value was inline. A string
         * node whose value has been evacuated to the heap has live numeric
         * members -- that is what a setter leaves behind -- and those share
         * bytes with the inline region. Inlining here would overwrite the
         * numbers copied just above with the value's characters, which is
         * how the duplicate ended up with a garbage valueint. Preserving
         * the source's arrangement keeps both, and costs nothing: a source
         * whose value is inline still produces an inline copy. */
        if (!_j_valuestring_set(newitem, item->valuestring,
                                (item->type & JValueInline) != 0)) {
            goto fail;
        }
    }
    if (item->string) {
        /* Always materialize an independent key rather than sharing the
         * source's pointer when JStringIsConst is set. That flag also covers
         * keys living inside the SOURCE node's allocation (JKeyInline), so
         * sharing would leave the duplicate dangling once the source is freed.
         * This mirrors what this function has always done for valuestring. */
        if (!_j_key_set(newitem, item->string, false)) {
            goto fail;
        }
    }
    /* If non-recursive, then we're done! */
    if (!recurse) {
        return newitem;
    }
    /* Walk the ->next chain for the child. */
    child = item->child;
    while (child != NULL) {
        newchild = JDuplicate(child, true); /* Duplicate (with recurse) each item in the ->next chain */
        if (!newchild) {
            goto fail;
        }
        if (next != NULL) {
            /* If newitem->child already set, then crosswire ->prev and ->next and move on */
            next->next = newchild;
            newchild->prev = next;
            next = newchild;
        } else {
            /* Set newitem->child and move to it */
            newitem->child = newchild;
            next = newchild;
        }
        child = child->next;
    }

    return newitem;

fail:
    if (newitem != NULL) {
        JDelete(newitem);
    }

    return NULL;
}

N_CJSON_PUBLIC(void) JMinify(char *json)
{
    unsigned char *into = (unsigned char*)json;

    if (json == NULL) {
        return;
    }

    while (*json) {
        if (*json == ' ') {
            json++;
        } else if (*json == '\t') {
            /* Whitespace characters. */
            json++;
        } else if (*json == '\r') {
            json++;
        } else if (*json=='\n') {
            json++;
        } else if ((*json == '/') && (json[1] == '/')) {
            /* double-slash comments, to end of line. */
            while (*json && (*json != '\n')) {
                json++;
            }
        } else if ((*json == '/') && (json[1] == '*')) {
            /* multiline comments. */
            while (*json && !((*json == '*') && (json[1] == '/'))) {
                json++;
            }
            json += 2;
        } else if (*json == '\"') {
            /* string literals, which are \" sensitive. */
            *into++ = (unsigned char)*json++;
            while (*json && (*json != '\"')) {
                if (*json == '\\') {
                    *into++ = (unsigned char)*json++;
                }
                *into++ = (unsigned char)*json++;
            }
            *into++ = (unsigned char)*json++;
        } else {
            /* All other characters. */
            *into++ = (unsigned char)*json++;
        }
    }

    /* and null-terminate. */
    *into = '\0';
}

N_CJSON_PUBLIC(Jbool) JIsInvalid(const J * const item)
{
    if (item == NULL) {
        return false;
    }
    return (item->type & 0xFF) == JInvalid;
}

N_CJSON_PUBLIC(Jbool) JIsFalse(const J * const item)
{
    if (item == NULL) {
        return false;
    }
    return (item->type & 0xFF) == JFalse;
}

N_CJSON_PUBLIC(Jbool) JIsTrue(const J * const item)
{
    if (item == NULL) {
        return false;
    }
    return (item->type & 0xff) == JTrue;
}


N_CJSON_PUBLIC(Jbool) JIsBool(const J * const item)
{
    if (item == NULL) {
        return false;
    }
    return (item->type & (JTrue | JFalse)) != 0;
}
N_CJSON_PUBLIC(Jbool) JIsNull(const J * const item)
{
    if (item == NULL) {
        return false;
    }
    return (item->type & 0xFF) == JNULL;
}

N_CJSON_PUBLIC(Jbool) JIsNumber(const J * const item)
{
    if (item == NULL) {
        return false;
    }
    return (item->type & 0xFF) == JNumber;
}

N_CJSON_PUBLIC(Jbool) JIsString(const J * const item)
{
    if (item == NULL) {
        return false;
    }
    return (item->type & 0xFF) == JString;
}

N_CJSON_PUBLIC(Jbool) JIsArray(const J * const item)
{
    if (item == NULL) {
        return false;
    }
    return (item->type & 0xFF) == JArray;
}

N_CJSON_PUBLIC(Jbool) JIsObject(const J * const item)
{
    if (item == NULL) {
        return false;
    }
    return (item->type & 0xFF) == JObject;
}

N_CJSON_PUBLIC(Jbool) JIsRaw(const J * const item)
{
    if (item == NULL) {
        return false;
    }
    return (item->type & 0xFF) == JRaw;
}

N_CJSON_PUBLIC(Jbool) JCompare(const J * const a, const J * const b, const Jbool case_sensitive)
{
    if ((a == NULL) || (b == NULL) || ((a->type & 0xFF) != (b->type & 0xFF)) || JIsInvalid(a)) {
        return false;
    }

    /* check if type is valid */
    switch (a->type & 0xFF) {
    case JFalse:
    case JTrue:
    case JNULL:
    case JNumber:
    case JString:
    case JRaw:
    case JArray:
    case JObject:
        break;

    default:
        return false;
    }

    /* identical objects are equal */
    if (a == b) {
        return true;
    }

    switch (a->type & 0xFF) {
    /* in these cases and equal type is enough */
    case JFalse:
    case JTrue:
    case JNULL:
        return true;

    case JNumber:
        if (a->valuenumber == b->valuenumber) {
            return true;
        }
        return false;

    case JString:
    case JRaw:
        if ((a->valuestring == NULL) || (b->valuestring == NULL)) {
            return false;
        }
        if (strcmp(a->valuestring, b->valuestring) == 0) {
            return true;
        }

        return false;

    case JArray: {
        J *a_element = a->child;
        J *b_element = b->child;

        for (; (a_element != NULL) && (b_element != NULL);) {
            if (!JCompare(a_element, b_element, case_sensitive)) {
                return false;
            }

            a_element = a_element->next;
            b_element = b_element->next;
        }

        /* one of the arrays is longer than the other */
        if (a_element != b_element) {
            return false;
        }

        return true;
    }

    case JObject: {
        J *a_element = NULL;
        J *b_element = NULL;
        JArrayForEach(a_element, a) {
            /* TODO This has O(n^2) runtime, which is horrible! */
            b_element = _get_object_item(b, a_element->string, case_sensitive);
            if (b_element == NULL) {
                return false;
            }

            if (!JCompare(a_element, b_element, case_sensitive)) {
                return false;
            }
        }

        /* doing this twice, once on a and b to prevent true comparison if a subset of b
         * TODO: Do this the proper way, this is just a fix for now */
        JArrayForEach(b_element, b) {
            a_element = _get_object_item(a, b_element->string, case_sensitive);
            if (a_element == NULL) {
                return false;
            }

            if (!JCompare(b_element, a_element, case_sensitive)) {
                return false;
            }
        }

        return true;
    }

    default:
        return false;
    }
}
