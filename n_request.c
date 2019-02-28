// Copyright 2018 Inca Roads LLC.  All rights reserved.
// Use of this source code is governed by licenses granted by the
// copyright holder including that found in the LICENSE file.

#include "n_lib.h"

// For flow tracing
#define SHOW_TRANSACTIONS

// Flag that gets set whenever an error occurs that should force a reset
static bool resetRequired = true;

// Create an error response document
static J *errDoc(char *errmsg) {
    J *rspdoc = JCreateObject();
    if (rspdoc != NULL)
        JAddStringToObject(rspdoc, "err", errmsg);
    return rspdoc;
}

// Lock for mutual exclusion, not only because access to the card must be serialized, but also because
// both C++ and ArduinoJSON call malloc() which is not a thread-safe operation.
J *NotecardNewRequest(char *request) {
    J *reqdoc = JCreateObject();
    if (reqdoc != NULL)
        JAddStringToObject(reqdoc, "req", request);
    return reqdoc;
}

// Initiate a transaction to the card using reqdoc, and return the result in rspdoc
J *NotecardTransaction(J *req) {

    // If a reset of the module is required for any reason, do it now.
    // We must do this before acquiring lock.
    if (resetRequired) {
        resetRequired = false;
        if (!NotecardReset())
            return NULL;
    }

    // Lock
    _LockNotecard();

    // Serialize the JSON requet
    char *json = JPrintUnformatted(req);
    if (json == NULL) {
        _UnlockNotecard();
        return errDoc("can't convert to JSON");
    }
    
#ifdef SHOW_TRANSACTIONS
    _Debug("%s\n", json);
#endif

    // Pertform the transaction
    char *responseJSON;
    char *errStr = _Transaction(json, &responseJSON);

    // Free the json
    JFree(json);

    // If error, queue up a reset
    if (errStr != NULL) {
        resetRequired = true;
        _UnlockNotecard();
        return errDoc(errStr);
    }

    // Parse the reply from the card on the input stream
    J *rspdoc = JParse(responseJSON);
    if (rspdoc == NULL) {
        _Free(responseJSON);
        _Debug("unable to parse %d-byte response JSON: \"%s\"\n", strlen(responseJSON), responseJSON);
        _UnlockNotecard();
        return errDoc("unrecognized response from card");
    }

    // Debug
#ifdef SHOW_TRANSACTIONS
    _Debug("%s\n", responseJSON);
#endif

    // Discard the buffer now that it's parsed
    _Free(responseJSON);

    // Unlock
    _UnlockNotecard();

    // Done
    return rspdoc;
    
}

// Initialize or re-initialize the module, returning false if anything fails
bool NotecardReset() {
    _LockNotecard();
    bool success = _NotecardReset();
    _UnlockNotecard();
    return success;
}
