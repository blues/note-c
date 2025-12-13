/*!
 * @file n_cjson_helpers.c
 *
 * Written by Ray Ozzie and Blues Inc. team.
 *
 * Copyright (c) 2019 Blues Inc. MIT License. Use of this source code is
 * governed by licenses granted by the copyright holder including that found in
 * the
 * <a href="https://github.com/blues/note-c/blob/master/LICENSE">LICENSE</a>
 * file.
 *
 */

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "n_lib.h"

bool JIsPresent(J *json, const char *field)
{
    if (json == NULL) {
        return false;
    }
    return (JGetObjectItem(json, field) != NULL);
}

char *JGetString(J *json, const char *field)
{
    if (json == NULL) {
        return (char *) c_nullstring;
    }
    J *item = JGetObjectItem(json, field);
    if (item == NULL) {
        return (char *) c_nullstring;
    }
    if (!JIsString(item)) {
        return (char *) c_nullstring;
    }
    if (item->valuestring == NULL) {
        return (char *) c_nullstring;
    }
    return item->valuestring;
}

J *JGetArray(J *json, const char *field)
{
    if (json == NULL) {
        return NULL;
    }
    J *item = JGetObjectItem(json, field);
    if (item == NULL) {
        return NULL;
    }
    if (!JIsArray(item)) {
        return NULL;
    }
    return item;
}

J *JGetObject(J *json, const char *field)
{
    if (json == NULL) {
        return NULL;
    }
    J *item = JGetObjectItem(json, field);
    if (item == NULL) {
        return NULL;
    }
    if (!JIsObject(item)) {
        return NULL;
    }
    return item;
}

bool JBoolValue(J *item)
{
    if (item == NULL) {
        return false;
    }
    return ((item->type & 0xff) == JTrue);
}

char *JStringValue(J *item)
{
    if (item == NULL) {
        return (char *)"";
    }
    return item->valuestring;
}

JNUMBER JNumberValue(J *item)
{
    if (item == NULL) {
        return 0.0;
    }
    return item->valuenumber;
}

JNUMBER JGetNumber(J *json, const char *field)
{
    if (json == NULL) {
        return 0.0;
    }
    J *item = JGetObjectItem(json, field);
    if (item == NULL) {
        return 0.0;
    }
    if (!JIsNumber(item)) {
        return 0.0;
    }
    return JNumberValue(item);
}

JINTEGER JIntValue(J *item)
{
    if (item == NULL) {
        return 0;
    }
    return item->valueint;
}

JINTEGER JGetInt(J *json, const char *field)
{
    if (json == NULL) {
        return 0;
    }
    J *item = JGetObjectItem(json, field);
    if (item == NULL) {
        return 0;
    }
    if (!JIsNumber(item)) {
        return 0;
    }
    return JIntValue(item);
}

bool JGetBool(J *json, const char *field)
{
    if (json == NULL) {
        return false;
    }
    J *item = JGetObjectItem(json, field);
    if (item == NULL) {
        return false;
    }
    if (!JIsBool(item)) {
        return false;
    }
    return JIsTrue(item);
}

bool JIsNullString(J *json, const char *field)
{
    if (json == NULL) {
        return false;
    }
    J *item = JGetObjectItem(json, field);
    if (item == NULL) {
        return true;
    }
    if (!JIsString(item)) {
        return false;
    }
    if (item->valuestring == NULL) {
        return true;
    }
    if (item->valuestring[0] == '\0') {
        return true;
    }
    return false;
}

bool JIsExactString(J *json, const char *field, const char *teststr)
{
    if (json == NULL) {
        return false;
    }
    J *item = JGetObjectItem(json, field);
    if (item == NULL) {
        return false;
    }
    if (!JIsString(item)) {
        return false;
    }
    if (item->valuestring == NULL) {
        return false;
    }
    if (strlen(teststr) == 0) {
        return false;
    }
    return (strcmp(item->valuestring, teststr) == 0);
}

bool JContainsString(J *json, const char *field, const char *substr)
{
    if (json == NULL) {
        return false;
    }
    J *item = JGetObjectItem(json, field);
    if (item == NULL) {
        return false;
    }
    if (!JIsString(item)) {
        return false;
    }
    if (item->valuestring == NULL) {
        return false;
    }
    if (strlen(substr) == 0) {
        return false;
    }
    return (strstr(item->valuestring, substr) != NULL);
}

bool JAddBinaryToObject(J *json, const char *fieldName, const void *binaryData, uint32_t binaryDataLen)
{
    if (json == NULL) {
        return false;
    }
    unsigned stringDataLen = JB64EncodeLen(binaryDataLen);
    char *stringData = (char *) _Malloc(stringDataLen);
    if (stringData == NULL) {
        return false;
    }
    JB64Encode(stringData, binaryData, binaryDataLen);
    J *stringItem = JCreateStringValue(stringData);
    if (stringItem == NULL) {
        _Free(stringData);
        return false;
    }
    JAddItemToObject(json, fieldName, stringItem);
    return true;
}

bool JGetBinaryFromObject(J *json, const char *fieldName, uint8_t **retBinaryData, uint32_t *retBinaryDataLen)
{
    // Initialize the return values to NULL and zero.
    *retBinaryData = NULL;
    *retBinaryDataLen = 0;

    if (json == NULL) {
        return false;
    }

    // In some cases, the caller may already have extracted the string from a different field, in which
    // case "json" will be set to the payload pointer.
    char *payload;
    if (fieldName == NULL) {
        payload = (char *) json;
    } else {
        payload = JGetString(json, fieldName);
    }
    if (payload[0] == '\0') {
        return false;
    }

    // Allocate a buffer for the payload, with an extra 'convenience byte' for null termination.  (see below)
    char *p = (char *) _Malloc(JB64DecodeLen(payload)+1);
    if (p == NULL) {
        return false;
    }
    uint32_t actualLen = JB64Decode(p, payload);

    // As a convenience to the caller, null-terminate the returned buffer in case it's a string.
    // (If we didn't do this, the caller would be forced to alloc another buffer of length+1 and
    // copy it to add the null terminator, while it's easy for us to do it here.)
    p[actualLen] = '\0';

    // Return the binary to the caller
    *retBinaryData = (uint8_t *)p;
    *retBinaryDataLen = actualLen;
    return true;

}

const char *JGetItemName(const J * item)
{
    if (item == NULL || item->string == NULL) {
        return "";
    }
    return item->string;
}

void JItoA(JINTEGER n, char *s)
{
    char c;
    // Conversion to unsigned is required to handle the case where n is
    // JINTEGER_MIN. In that case, applying the unary minus operator to the
    // signed version of n overflows and the behavior is undefined. By changing
    // n to be unsigned, the unary minus operator behaves differently, and there
    // is no overflow. See https://stackoverflow.com/q/8026694.
    JUINTEGER unsignedN = n;
    long int i, j;
    if (n < 0) {
        unsignedN = -unsignedN;
    }
    i = 0;
    do {
        s[i++] = unsignedN % 10 + '0';
    } while ((unsignedN /= 10) > 0);
    if (n < 0) {
        s[i++] = '-';
    }
    s[i] = '\0';
    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

JINTEGER JAtoI(const char *string)
{
    JINTEGER result = 0;
    unsigned int digit;
    int sign;
    while (*string == ' ') {
        string += 1;
    }
    if (*string == '-') {
        sign = 1;
        string += 1;
    } else {
        sign = 0;
        if (*string == '+') {
            string += 1;
        }
    }
    for ( ; ; string += 1) {
        digit = *string - '0';
        if (digit > 9) {
            break;
        }
        result = (10*result) + digit;
    }
    if (sign) {
        result = -result;
    }
    return result;
}

char *JAllocString(uint8_t *buffer, uint32_t len)
{
    char *buf = _Malloc(len+1);
    if (buf == NULL) {
        return NULL;
    }
    if (len > 0) {
        memcpy(buf, buffer, len);
    }
    buf[len] = '\0';
    return buf;
}

const char *JType(J *item)
{
    if (item == NULL) {
        return "";
    }
    switch (item->type & 0xff) {
    case JTrue:
    case JFalse:
        return "bool";
    case JNULL:
        return "null";
    case JNumber:
        return "number";
    case JRaw:
    case JString:
        return "string";
    case JObject:
        return "object";
    case JArray:
        return "array";
    }
    return "invalid";
}

int JGetType(J *json, const char *field)
{
    if (json == NULL || field == NULL) {
        return JTYPE_NOT_PRESENT;
    }
    return JGetItemType(JGetObjectItem(json, field));
}

int JGetItemType(J *item)
{
    const char *v;
    if (item == NULL) {
        return JTYPE_NOT_PRESENT;
    }
    switch (item->type & 0xff) {
    case JTrue:
        return JTYPE_BOOL_TRUE;
    case JFalse:
        return JTYPE_BOOL_FALSE;
    case JNULL:
        return JTYPE_NULL;
    case JNumber:
        if (item->valueint == 0 && item->valuenumber == 0) {
            return JTYPE_NUMBER_ZERO;
        }
        return JTYPE_NUMBER;
    case JRaw:
    case JString: {
        v = item->valuestring;
        if (v == NULL || v[0] == 0) {
            return JTYPE_STRING_BLANK;
        }
        int vlen = strlen(v);
        char *endstr;
        JNUMBER value = JAtoN(v, &endstr);
        if (endstr[0] == 0) {
            if (value == 0) {
                return JTYPE_STRING_ZERO;
            }
            return JTYPE_STRING_NUMBER;
        }
        if (vlen == 4 && (
                    (v[0] == 't' || v[0] == 'T')
                    && (v[1] == 'r' || v[1] == 'R')
                    && (v[2] == 'u' || v[2] == 'U')
                    && (v[3] == 'e' || v[3] == 'E'))) {
            return JTYPE_STRING_BOOL_TRUE;
        }
        if (vlen == 5 && (
                    (v[0] == 'f' || v[0] == 'F')
                    && (v[1] == 'a' || v[1] == 'A')
                    && (v[2] == 'l' || v[2] == 'L')
                    && (v[3] == 's' || v[3] == 'S')
                    && (v[4] == 'e' || v[4] == 'E'))) {
            return JTYPE_STRING_BOOL_FALSE;
        }
        return JTYPE_STRING;
    }
    case JObject:
        return JTYPE_OBJECT;
    case JArray:
        return JTYPE_ARRAY;
    }
    return JTYPE_NOT_PRESENT;
}

// Coalesce to the base types
int JBaseItemType(int type)
{
    switch (type) {
    case JTYPE_BOOL_TRUE:
        return JTYPE_BOOL;
    case JTYPE_BOOL_FALSE:
        return JTYPE_BOOL;
    case JTYPE_NUMBER_ZERO:
        return JTYPE_NUMBER;
    case JTYPE_STRING_BLANK:
        return JTYPE_STRING;
    case JTYPE_STRING_ZERO:
        return JTYPE_STRING;
    case JTYPE_STRING_NUMBER:
        return JTYPE_STRING;
    case JTYPE_STRING_BOOL_TRUE:
        return JTYPE_STRING;
    case JTYPE_STRING_BOOL_FALSE:
        return JTYPE_STRING;
    }
    return type;
}

void JMerge(J *target, J *source)
{
    if (source == NULL) {
        return;
    }
    if (target == NULL) {
        // Target is NULL but source is not - delete source to avoid leak
        JDelete(source);
        return;
    }
    J *item = source->child;
    while (item != NULL) {
        J *next = item->next;
        const char *name = item->string;

        // Remove from source
        JDetachItemViaPointer(source, item);

        // Delete existing field in target if present
        JDeleteItemFromObject(target, name);

        // Add to target
        JAddItemToObject(target, name, item);

        item = next;
    }
    JDelete(source);
}

/*****************************************************************************
 *
 * JObjectf / JAddf - printf-style JSON object construction
 *
 *****************************************************************************
 *
 * OVERVIEW
 * --------
 * JObjectf creates a JSON object using a printf-inspired format string syntax.
 * This provides a concise, readable way to construct JSON objects inline,
 * similar to how printf provides a concise way to format strings.
 *
 * Just as printf("name: %s, age: %d", name, 42) formats a string,
 * JObjectf("name:%s age:%d", name, 42) creates {"name":"...","age":42}
 *
 * BASIC SYNTAX
 * ------------
 * The format string is a whitespace-separated list of field definitions.
 * Each field definition has the form:  name:value
 *
 * Whitespace includes: space, tab, newline (\n), carriage return (\r), comma (,)
 * This means you can write formats in multiple styles:
 *
 *     "name:%s age:%d"           // space-separated
 *     "name:%s, age:%d"          // comma-separated (JSON-like)
 *     "name:%s,age:%d"           // compact
 *     "name:%s\nage:%d"          // multi-line in source
 *
 * FIELD NAMES
 * -----------
 * Field names can be:
 *   - A literal identifier: letters, digits, underscore (e.g., "myField")
 *   - A format specifier %s: the name is taken from the next argument
 *
 * Examples:
 *     JObjectf("status:%s", "ok")              // {"status":"ok"}
 *     JObjectf("%s:%s", "status", "ok")        // {"status":"ok"}
 *     JObjectf("%s:%d", fieldName, 42)         // {<fieldName>:42}
 *
 * VALUE SPECIFIERS
 * ----------------
 * Values can be format specifiers (pulling from arguments) or literals:
 *
 * Format specifiers (consume an argument):
 *     %s  - String (const char*)
 *     %d  - Integer (JINTEGER, typically long long)
 *     %f  - Floating point number (JNUMBER, typically double)
 *     %b  - Boolean (int: 0=false, non-zero=true)
 *     %o  - JSON object/array (J*) - object is MOVED, not copied
 *     %a  - Synonym for %o (for "array", but accepts any J*)
 *
 * Literal values (no argument consumed):
 *     true        - Boolean true
 *     false       - Boolean false
 *     123         - Integer (decimal digits, optional leading minus)
 *     123.45      - Floating point (contains decimal point)
 *     'text'      - String literal with single quotes (escape: \' and \\)
 *     "text"      - String literal with double quotes (escape: \" and \\)
 *     word        - Unquoted string (letters/digits/underscore/dot, ends at whitespace)
 *                   e.g., status:ok, mode:continuous, file:data.qo
 *
 * EXAMPLES
 * --------
 *
 * Basic usage with format specifiers:
 *
 *     J *obj = JObjectf("name:%s age:%d active:%b",
 *                       "Alice", 30, true);
 *     // Creates: {"name":"Alice","age":30,"active":true}
 *
 * Using literal values (no arguments needed):
 *
 *     J *obj = JObjectf("status:'pending' count:0 enabled:false");
 *     // Creates: {"status":"pending","count":0,"enabled":false}
 *
 * Unquoted string values (simple words without quotes):
 *
 *     J *obj = JObjectf("status:ok mode:continuous file:data.qo");
 *     // Creates: {"status":"ok","mode":"continuous","file":"data.qo"}
 *
 * Mixing literals and format specifiers:
 *
 *     J *obj = JObjectf("type:'sensor' id:%s reading:%f",
 *                       sensorId, temperature);
 *     // Creates: {"type":"sensor","id":"...","reading":23.5}
 *
 * Dynamic field names:
 *
 *     J *obj = JObjectf("%s:%d %s:%d", "x", 10, "y", 20);
 *     // Creates: {"x":10,"y":20}
 *
 * Nested objects (object is MOVED into result):
 *
 *     J *inner = JObjectf("lat:%f lon:%f", 40.7128, -74.0060);
 *     J *obj = JObjectf("name:%s location:%o", "NYC", inner);
 *     // Creates: {"name":"NYC","location":{"lat":40.7128,"lon":-74.006}}
 *     // NOTE: 'inner' is now owned by 'obj', do not JDelete(inner)!
 *
 * String literals with embedded quotes (single or double quotes work):
 *
 *     J *obj = JObjectf("message:'It\\'s working!'");
 *     // Creates: {"message":"It's working!"}
 *
 *     J *obj = JObjectf("message:\"It's working!\"");
 *     // Creates: {"message":"It's working!"}  (no escape needed)
 *
 *     J *obj = JObjectf("quote:\"She said \\\"Hello\\\"\"");
 *     // Creates: {"quote":"She said \"Hello\""}
 *
 * Multi-line format for readability:
 *
 *     J *obj = JObjectf(
 *         "req:note.add"
 *         "file:%s"
 *         "body:%o",
 *         filename, bodyObj);
 *
 * Building Notecard requests and commands:
 *
 *     // A Notecard request (expects response)
 *     J *req = JObjectf("req:hub.set product:com.blues.app mode:continuous");
 *     // Creates: {"req":"hub.set","product":"com.blues.app","mode":"continuous"}
 *
 *     // A Notecard command (no response expected)
 *     J *cmd = JObjectf("cmd:card.led mode:breathe");
 *     // Creates: {"cmd":"card.led","mode":"breathe"}
 *
 *     // Full request example with mixed values
 *     J *req = JObjectf("req:note.add file:sensors.qo body:%o", sensorData);
 *     // Creates: {"req":"note.add","file":"sensors.qo","body":{...}}
 *
 * USING JAddf
 * -----------
 * JAddf is a convenience macro that merges fields into an existing object:
 *
 *     J *obj = JCreateObject();
 *     JAddStringToObject(obj, "base", "value");
 *     JAddf(obj, "extra:%d flag:%b", 42, true);
 *     // obj now has: {"base":"value","extra":42,"flag":true}
 *
 * JAddf overwrites existing fields with the same name:
 *
 *     J *obj = JObjectf("status:'pending'");
 *     JAddf(obj, "status:'complete'");
 *     // obj now has: {"status":"complete"}
 *
 * ERROR HANDLING
 * --------------
 * - If format is NULL, returns an empty object.
 * - If memory allocation fails, returns NULL.
 * - If format is malformed (e.g., missing ':'), parsing stops and returns
 *   a partial object containing all fields successfully parsed so far.
 * - If %o/%a argument is NULL, that field is silently skipped.
 * - If a duplicate field name appears, the later value overwrites the earlier.
 *
 * MEMORY MANAGEMENT
 * -----------------
 * - The returned J* must be freed with JDelete() when no longer needed.
 * - Objects passed via %o/%a are MOVED into the result and should NOT be
 *   freed separately. They become owned by the returned object.
 * - String arguments (%s for values) are copied; the original strings
 *   are not modified and can be freed independently.
 *
 *****************************************************************************/

/*!
 * @brief Check if a character is a field separator (whitespace or comma).
 *
 * Field separators in the JObjectf format string include all traditional
 * whitespace characters plus comma, allowing flexible formatting styles.
 *
 * @param c The character to check.
 * @return true if c is a separator, false otherwise.
 */
static bool _jObjectf_isSeparator(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == ',';
}

/*!
 * @brief Skip past any separator characters in the format string.
 *
 * @param p Pointer to current position in format string.
 * @return Pointer to next non-separator character (or end of string).
 */
static const char *_jObjectf_skipSeparators(const char *p)
{
    while (*p && _jObjectf_isSeparator(*p)) {
        p++;
    }
    return p;
}

/*!
 * @brief Check if a character is valid in a literal field name.
 *
 * Valid characters are: letters (a-z, A-Z), digits (0-9), underscore (_).
 *
 * @param c The character to check.
 * @return true if c is valid in a field name, false otherwise.
 */
static bool _jObjectf_isNameChar(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           c == '_';
}

/*!
 * @brief Check if a character is valid in an unquoted string value.
 *
 * Unquoted string values can contain letters, digits, underscore, and dot.
 * This allows values like: ok, continuous, data.qo, hub.set, com.blues.app
 *
 * @param c The character to check.
 * @return true if c is valid in an unquoted string value, false otherwise.
 */
static bool _jObjectf_isUnquotedStringChar(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           c == '_' ||
           c == '.';
}

/*!
 * @brief Parse a field name from the format string.
 *
 * The field name is either:
 * - A literal identifier (letters, digits, underscore)
 * - The format specifier %s (name taken from va_list argument)
 *
 * @param p         Pointer to current position in format string.
 * @param nameBuf   Buffer to store the parsed field name.
 * @param bufSize   Size of nameBuf.
 * @param args      Pointer to va_list for extracting %s argument.
 * @return Pointer to character after the parsed name, or NULL on error.
 */
static const char *_jObjectf_parseName(const char *p, char *nameBuf,
                                       size_t bufSize, va_list *args)
{
    // Check for %s format specifier (name from argument)
    if (p[0] == '%' && p[1] == 's') {
        const char *argName = va_arg(*args, const char *);
        if (argName == NULL) {
            nameBuf[0] = '\0';
            return NULL;
        }
        size_t len = strlen(argName);
        if (len >= bufSize) {
            len = bufSize - 1;
        }
        memcpy(nameBuf, argName, len);
        nameBuf[len] = '\0';
        return p + 2;
    }

    // Parse literal identifier
    size_t i = 0;
    while (_jObjectf_isNameChar(*p) && i < bufSize - 1) {
        nameBuf[i++] = *p++;
    }
    nameBuf[i] = '\0';

    if (i == 0) {
        // No valid name characters found
        return NULL;
    }

    return p;
}

/*!
 * @brief Parse a literal number from the format string.
 *
 * Parses decimal integers or floating point numbers.
 * Integer: optional minus, followed by digits (e.g., "123", "-42")
 * Float: integer part, decimal point, fractional part (e.g., "3.14", "-0.5")
 *
 * @param p           Pointer to current position in format string.
 * @param value       Pointer to store the parsed J* value.
 * @param endPtr      Pointer to store position after parsed number.
 * @return true if a number was successfully parsed, false otherwise.
 */
static bool _jObjectf_parseNumber(const char *p, J **value, const char **endPtr)
{
    const char *start = p;
    bool isFloat = false;
    bool hasDigits = false;

    // Optional leading minus
    if (*p == '-') {
        p++;
    }

    // Integer part
    while (*p >= '0' && *p <= '9') {
        p++;
        hasDigits = true;
    }

    // Check for decimal point
    if (*p == '.') {
        isFloat = true;
        p++;
        // Fractional part
        while (*p >= '0' && *p <= '9') {
            p++;
            hasDigits = true;
        }
    }

    if (!hasDigits) {
        return false;
    }

    // Parse the number
    if (isFloat) {
        JNUMBER num = JAtoN(start, NULL);
        *value = JCreateNumber(num);
    } else {
        JINTEGER num = JAtoI(start);
        *value = JCreateInteger(num);
    }

    *endPtr = p;
    return (*value != NULL);
}

/*!
 * @brief Parse a quoted string literal from the format string.
 *
 * String literals can be enclosed in either single quotes ('text') or
 * double quotes ("text"). The opening and closing quote must match.
 *
 * Escape sequences:
 * - \' for single quote (in single-quoted strings)
 * - \" for double quote (in double-quoted strings)
 * - \\ for backslash (in either)
 *
 * @param p           Pointer to opening quote character (' or ").
 * @param value       Pointer to store the parsed J* string value.
 * @param endPtr      Pointer to store position after closing quote.
 * @return true if string was successfully parsed, false otherwise.
 */
static bool _jObjectf_parseQuotedString(const char *p, J **value,
                                        const char **endPtr)
{
    char quoteChar = *p;
    if (quoteChar != '\'' && quoteChar != '"') {
        return false;
    }
    p++; // Skip opening quote

    // First pass: calculate length needed
    const char *scan = p;
    size_t len = 0;
    while (*scan && *scan != quoteChar) {
        if (*scan == '\\' && (scan[1] == quoteChar || scan[1] == '\\')) {
            scan += 2;
        } else {
            scan++;
        }
        len++;
    }

    if (*scan != quoteChar) {
        // Unterminated string
        return false;
    }

    // Allocate and copy with escape processing
    char *buf = _Malloc(len + 1);
    if (buf == NULL) {
        return false;
    }

    size_t i = 0;
    while (*p && *p != quoteChar) {
        if (*p == '\\' && (p[1] == quoteChar || p[1] == '\\')) {
            buf[i++] = p[1];
            p += 2;
        } else {
            buf[i++] = *p++;
        }
    }
    buf[i] = '\0';

    // Skip closing quote
    if (*p == quoteChar) {
        p++;
    }

    *value = JCreateString(buf);
    _Free(buf);
    *endPtr = p;

    return (*value != NULL);
}

/*!
 * @brief Parse a value from the format string.
 *
 * Values can be:
 * - Format specifiers: %s (string), %d (int), %f (float), %b (bool), %o/%a (object)
 * - Literals: true, false, numbers (123, -45.67), quoted strings ('text' or "text")
 * - Unquoted strings: words starting with a letter (e.g., ok, continuous, hub.set)
 *
 * @param p           Pointer to current position in format string.
 * @param value       Pointer to store the parsed J* value (NULL if skipped).
 * @param args        Pointer to va_list for extracting arguments.
 * @param endPtr      Pointer to store position after parsed value.
 * @return true if value was successfully parsed (even if skipped), false on error.
 */
static bool _jObjectf_parseValue(const char *p, J **value, va_list *args,
                                 const char **endPtr)
{
    *value = NULL;

    // Check for format specifiers
    if (*p == '%') {
        char spec = p[1];
        p += 2;
        *endPtr = p;

        switch (spec) {
        case 's': {
            const char *str = va_arg(*args, const char *);
            if (str != NULL) {
                *value = JCreateString(str);
            }
            return true;
        }
        case 'd': {
            JINTEGER num = va_arg(*args, JINTEGER);
            *value = JCreateInteger(num);
            return (*value != NULL);
        }
        case 'f': {
            JNUMBER num = va_arg(*args, JNUMBER);
            *value = JCreateNumber(num);
            return (*value != NULL);
        }
        case 'b': {
            int bval = va_arg(*args, int);
            *value = JCreateBool(bval ? true : false);
            return (*value != NULL);
        }
        case 'o':
        case 'a': {
            J *obj = va_arg(*args, J *);
            // If NULL, we skip this field (value stays NULL, return true)
            *value = obj;
            return true;
        }
        default:
            // Unknown format specifier
            return false;
        }
    }

    // Check for boolean literals
    if (strncmp(p, "true", 4) == 0 &&
        !_jObjectf_isNameChar(p[4])) {
        *value = JCreateBool(true);
        *endPtr = p + 4;
        return (*value != NULL);
    }
    if (strncmp(p, "false", 5) == 0 &&
        !_jObjectf_isNameChar(p[5])) {
        *value = JCreateBool(false);
        *endPtr = p + 5;
        return (*value != NULL);
    }

    // Check for quoted string literal (single or double quotes)
    if (*p == '\'' || *p == '"') {
        return _jObjectf_parseQuotedString(p, value, endPtr);
    }

    // Check for numeric literal
    if ((*p >= '0' && *p <= '9') || (*p == '-' && p[1] >= '0' && p[1] <= '9')) {
        return _jObjectf_parseNumber(p, value, endPtr);
    }

    // Check for unquoted string (must start with a letter)
    // This allows values like: ok, continuous, hub.set, com.blues.app, data.qo
    if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z')) {
        const char *start = p;
        while (_jObjectf_isUnquotedStringChar(*p)) {
            p++;
        }
        size_t len = p - start;
        char *buf = _Malloc(len + 1);
        if (buf == NULL) {
            return false;
        }
        memcpy(buf, start, len);
        buf[len] = '\0';
        *value = JCreateString(buf);
        _Free(buf);
        *endPtr = p;
        return (*value != NULL);
    }

    // Unrecognized value format
    return false;
}

/*!
 @brief Create a JSON object using a printf-style format string.

 JObjectf provides a concise way to construct JSON objects inline, similar to
 how printf formats strings. The format string contains field definitions
 separated by whitespace (space, tab, newline, carriage return, or comma).

 Each field definition has the form `name:value` where:
 - name is a literal identifier or %s (from argument)
 - value is a format specifier (%s, %d, %f, %b, %o, %a) or a literal

 Field name specifiers:
 - literal: Letters, digits, underscore (e.g., myField)
 - %s: Name taken from argument (const char*)

 Value format specifiers:
 - %s: String (const char*)
 - %d: Integer (JINTEGER)
 - %f: Floating point (JNUMBER)
 - %b: Boolean (int, 0=false, non-zero=true)
 - %o: JSON object/array (J*) - object is MOVED, not copied
 - %a: Synonym for %o

 Literal values:
 - true / false: Boolean literals
 - 123 or -45: Integer literals
 - 3.14 or -0.5: Floating point literals
 - 'text': String literal with single quotes (use \\' for embedded quote)
 - "text": String literal with double quotes (use \\" for embedded quote)
 - word: Unquoted string starting with letter (e.g., ok, hub.set, data.qo)

 @param format The format string describing the object structure.
 @param ... Arguments corresponding to format specifiers.

 @return A newly allocated J* object, or NULL on memory allocation failure.
         The caller is responsible for calling JDelete() on the returned object.

 @note Objects passed via %o/%a are MOVED into the result and should not be
       freed separately.
 @note If the format string is malformed, returns a partial object with all
       fields successfully parsed before the error.
 @note If a %o/%a argument is NULL, that field is silently skipped.

 Example usage:
 @code
 // Simple object with mixed types
 J *obj = JObjectf("name:%s age:%d active:%b", "Alice", 30, true);

 // Using unquoted string values
 J *obj = JObjectf("status:ok mode:continuous file:data.qo");

 // Notecard request
 J *req = JObjectf("req:hub.set product:com.blues.app mode:continuous");

 // Nested object
 J *loc = JObjectf("lat:%f lon:%f", 40.7128, -74.0060);
 J *obj = JObjectf("city:%s location:%o", "NYC", loc);
 @endcode
 */
J *JObjectf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    J *result = JObjectfv(format, args);
    va_end(args);
    return result;
}

/*!
 @brief Create a JSON object using a printf-style format string (va_list version).

 This is the va_list variant of JObjectf, useful when wrapping JObjectf
 in another variadic function.

 @param format The format string describing the object structure.
 @param args   The va_list containing arguments for format specifiers.

 @return A newly allocated J* object, or NULL on memory allocation failure.

 @see JObjectf for detailed documentation and examples.
 */
J *JObjectfv(const char *format, va_list args)
{
    J *result = JCreateObject();
    if (result == NULL) {
        return NULL;
    }

    if (format == NULL) {
        return result;
    }

    // Buffer for field names - sized to be reasonable for typical use
    // Field names in JSON are typically short identifiers
    char nameBuf[256];

    const char *p = format;

    while (*p) {
        // Skip leading separators
        p = _jObjectf_skipSeparators(p);
        if (*p == '\0') {
            break;
        }

        // Parse field name
        const char *afterName = _jObjectf_parseName(p, nameBuf,
                                sizeof(nameBuf), (va_list *)&args);
        if (afterName == NULL || nameBuf[0] == '\0') {
            // Parse error or empty name - stop parsing
            break;
        }
        p = afterName;

        // Expect colon separator
        if (*p != ':') {
            // Malformed - stop parsing
            break;
        }
        p++; // Skip colon

        // Parse value
        J *value = NULL;
        const char *afterValue;
        if (!_jObjectf_parseValue(p, &value, (va_list *)&args, &afterValue)) {
            // Parse error - stop parsing
            break;
        }
        p = afterValue;

        // Add field to object (if value is not NULL)
        // For %o/%a with NULL argument, we skip the field
        if (value != NULL) {
            // Delete any existing field with same name (to support overwrites)
            JDeleteItemFromObject(result, nameBuf);
            JAddItemToObject(result, nameBuf, value);
        }
    }

    return result;
}
