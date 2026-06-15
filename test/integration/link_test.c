// Integration test: verify that note_c_lib links correctly and key
// symbols are callable from a downstream project.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "note.h"

int main(void)
{
    // Exercise a representative set of note-c API functions to prove
    // the static library links and the public headers are usable.

    // Register standard malloc/free so note-c can allocate memory.
    NoteSetFnDefault(malloc, free, NULL, NULL);

    // NoteNewRequest creates a JSON request object.
    J *req = NoteNewRequest("hub.status");
    if (req == NULL) {
        fprintf(stderr, "NoteNewRequest returned NULL\n");
        return 1;
    }

    // JAddStringToObject exercises cJSON helpers.
    JAddStringToObject(req, "test", "value");

    // Read it back.
    const char *val = JGetString(req, "test");
    if (val == NULL || strcmp(val, "value") != 0) {
        fprintf(stderr, "JGetString mismatch\n");
        JDelete(req);
        return 1;
    }

    JDelete(req);

    printf("note_c_lib link test passed\n");
    return 0;
}
