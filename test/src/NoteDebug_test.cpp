/*!
 * @file NoteDebug_test.cpp
 *
 * Written by the Blues Inc. team.
 *
 * Copyright (c) 2023 Blues Inc. MIT License. Use of this source code is
 * governed by licenses granted by the copyright holder including that found in
 * the
 * <a href="https://github.com/blues/note-c/blob/master/LICENSE">LICENSE</a>
 * file.
 *
 */

#ifdef TEST

#include <catch2/catch_test_macros.hpp>

#include "n_lib.h"

namespace
{

typedef struct {
    char debugBuf[NOTE_C_DEBUG_W_LEVEL_MAX_BYTES];
    size_t debugBufIdx;
    bool debugOutputCalled;
} TestState;

TestState state;

size_t MyDebugOutput(const char *line)
{
    size_t len = 0;
    state.debugOutputCalled = true;

    if (line != NULL) {
        len = strlcpy(state.debugBuf + state.debugBufIdx, line,
                      sizeof(state.debugBuf) - state.debugBufIdx);
        state.debugBufIdx += len;
    }

    return len;
}

TEST_CASE("NoteDebug")
{
    memset(&state, 0, sizeof(state));

    SECTION("Hook not set") {
        CHECK(!NoteIsDebugOutputActive());

        NoteDebug(NULL);
        CHECK(!state.debugOutputCalled);
    }

    SECTION("Hook set") {
        NoteSetFnDebugOutput(MyDebugOutput);

        SECTION("Active") {
            CHECK(NoteIsDebugOutputActive());
        }

        SECTION("Hook called") {
            NoteDebug(NULL);

#ifdef NOTE_NODEBUG
            CHECK(!state.debugOutputCalled);
#else
            CHECK(state.debugOutputCalled);
#endif
        }

        SECTION("NoteDebugln") {
            NoteDebugln("test");
            CHECK(!strcmp(state.debugBuf, "test\r\n"));
        }

        SECTION("NoteDebugIntln") {
            const char* expectedString;

            SECTION("Just number") {
                expectedString = "1\r\n";
                NoteDebugIntln(NULL, 1);
            }

            SECTION("String and number") {
                expectedString = "test1\r\n";
                NoteDebugIntln("test", 1);
            }

#ifdef NOTE_NODEBUG
            CHECK(!state.debugOutputCalled);
#else
            CHECK(state.debugOutputCalled);
            CHECK(!strcmp(state.debugBuf, expectedString));
#endif
        }

        SECTION("NoteDebugWithLogLevel") {
            const char file[] = __FILE__;
            const int lineNo = __LINE__;
            const char *lineNoStr = std::to_string(lineNo).c_str();
            const char msg[] = "my message";
            char tooLargeBuf[NOTE_C_DEBUG_W_LEVEL_MAX_BYTES + 1];
            memset(tooLargeBuf, 'a', sizeof(tooLargeBuf));
            tooLargeBuf[NOTE_C_DEBUG_W_LEVEL_MAX_BYTES] = '\0';

            SECTION("Info level messages NOT logged by default") {
                NoteDebugWithLevel(NOTE_C_LOG_LEVEL_INFO, file, lineNo, msg);

                CHECK(!state.debugOutputCalled);
            }

            SECTION("Message with invalid log level dropped") {
                const uint8_t invalidLogLevel = 42;
                NoteSetMaxLogLevel(invalidLogLevel);
                NoteDebugWithLevel(invalidLogLevel, file, lineNo, msg);

                CHECK(!state.debugOutputCalled);
            }

            SECTION("File name too big") {
                NoteDebugWithLevel(NOTE_C_LOG_LEVEL_ERROR, tooLargeBuf, lineNo,
                                   msg);

                CHECK(!state.debugOutputCalled);
            }

            SECTION("Message too big") {
                NoteDebugWithLevel(NOTE_C_LOG_LEVEL_ERROR, file, lineNo,
                                   tooLargeBuf);

                CHECK(!state.debugOutputCalled);
            }

            SECTION("Sufficient log level") {
                uint8_t level;
                const char *levelStr;

                SECTION("NOTE_C_LOG_LEVEL_ERROR") {
                    level = NOTE_C_LOG_LEVEL_ERROR;
                    levelStr = "ERROR";
                }

                SECTION("NOTE_C_LOG_LEVEL_WARN") {
                    level = NOTE_C_LOG_LEVEL_WARN;
                    levelStr = "WARN";
                }

                SECTION("NOTE_C_LOG_LEVEL_INFO") {
                    level = NOTE_C_LOG_LEVEL_INFO;
                    levelStr = "INFO";
                }

                SECTION("NOTE_C_LOG_LEVEL_DEBUG") {
                    level = NOTE_C_LOG_LEVEL_DEBUG;
                    levelStr = "DEBUG";
                }

                NoteSetMaxLogLevel(level);
                NoteDebugWithLevel(level, file, lineNo, msg);

#ifdef NOTE_NODEBUG
                CHECK(!state.debugOutputCalled);
#else
                CHECK(state.debugOutputCalled);
                // Make sure the level, message, file, and line number were all
                // in the debug output.
                CHECK(strstr(state.debugBuf, levelStr) != NULL);
                CHECK(strstr(state.debugBuf, file) != NULL);
                CHECK(strstr(state.debugBuf, lineNoStr) != NULL);
                CHECK(strstr(state.debugBuf, lineNoStr) != NULL);
#endif
            }

            // Set max log level back to the default before the next test
            // runs.
            NoteSetMaxLogLevel(NOTE_C_DEBUG_W_LEVEL_DEFAULT_MAX);
        }
    }
}

}

#endif // TEST
