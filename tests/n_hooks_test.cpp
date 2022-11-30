#include <gtest/gtest.h>

#include "note.h"
#include "n_lib.h"

namespace
{

TEST(NoteMallocTest, NullIfNoHook)
{
    uint8_t* buf = (uint8_t*)NoteMalloc(10);
    EXPECT_EQ(buf, nullptr);
}

TEST(NoteMallocTest, NotNullIfHook)
{
    NoteSetFnDefault(malloc, free, NULL, NULL);
    uint8_t* buf = (uint8_t*)NoteMalloc(10);
    EXPECT_NE(buf, nullptr);
    NoteFree(buf);
}

TEST(NoteDebugOutputTest, DebugInactive)
{
    EXPECT_FALSE(NoteIsDebugOutputActive());
}

size_t DebugOut(const char *msg)
{
    size_t result = 0;

    if (msg) {
        printf("%s\n", msg);
        result = strlen(msg) + 1; // +1 for newline
    }

    return result;
}

TEST(NoteDebugOutputTest, DebugActive)
{
    NoteSetFnDebugOutput(DebugOut);
    EXPECT_TRUE(NoteIsDebugOutputActive());
}

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

