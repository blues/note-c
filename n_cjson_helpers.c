// Copyright 2018 Inca Roads LLC.  All rights reserved.
// Use of this source code is governed by licenses granted by the
// copyright holder including that found in the LICENSE file.

#include <time.h>
#include "n_lib.h"

// Return true if a field is present in the response
bool JIsPresent(J *rsp, char *field) {
    if (rsp == NULL)
        return false;
    return (JGetObjectItem(rsp, field) != NULL);
}

// Return a string from the specified JSON object, or "" if it's not present
char *JGetString(J *rsp, char *field) {
    if (rsp == NULL)
        return "";
    J *item = JGetObjectItem(rsp, field);
    if (item == NULL)
        return "";
    if (!JIsString(item))
        return "";
    if (item->valuestring == NULL)
        return "";
    return item->valuestring;
}

// Return a number from the specified JSON object, or 0 if it's not present
double JGetDouble(J *rsp, char *field) {
    if (rsp == NULL)
        return 0.0;
    J *item = JGetObjectItem(rsp, field);
    if (item == NULL)
        return 0.0;
    if (!JIsNumber(item))
        return 0.0;
    return item->valuedouble;
}

// Return a number from the JSON object, or 0 if it's not present
int JGetInt(J *rsp, char *field) {
    if (rsp == NULL)
        return 0.0;
    J *item = JGetObjectItem(rsp, field);
    if (item == NULL)
        return 0.0;
    if (!JIsNumber(item))
        return 0.0;
    return item->valueint;
}

// Return a bool from the JSON object, or false if it's not present
bool JGetBool(J *rsp, char *field) {
    if (rsp == NULL)
        return false;
    J *item = JGetObjectItem(rsp, field);
    if (item == NULL)
        return false;
    if (!JIsBool(item))
        return 0.0;
    return JIsTrue(item);
}

// Return true if the object is valid and if the field is not present or if it's null
bool JIsNullString(J *rsp, char *field) {
    if (rsp == NULL)
        return false;
    J *item = JGetObjectItem(rsp, field);
    if (item == NULL)
        return true;
    if (!JIsString(item))
        return false;
    if (item->valuestring == NULL)
        return true;
    if (item->valuestring[0] == '\0')
        return true;
    return false;
}

// Return true if a field exists, and is a string, and is exactly text specified
bool JIsExactString(J *rsp, char *field, char *teststr) {
    if (rsp == NULL)
        return false;
    J *item = JGetObjectItem(rsp, field);
    if (item == NULL)
        return false;
    if (!JIsString(item))
        return false;
    if (item->valuestring == NULL)
        return false;
    uint32_t teststrlen = strlen(teststr);
    if (strlen(teststr) == 0)
        return false;
    return (strcmp(item->valuestring, teststr) == 0);
}

// Return true if a field exists, and is a string, and contains the text as a substring
bool JContainsString(J *rsp, char *field, char *substr) {
    if (rsp == NULL)
        return false;
    J *item = JGetObjectItem(rsp, field);
    if (item == NULL)
        return false;
    if (!JIsString(item))
        return false;
    if (item->valuestring == NULL)
        return false;
    uint32_t substrlen = strlen(substr);
    if (strlen(substr) == 0)
        return false;
    return (strstr(item->valuestring, substr) != NULL);
}
