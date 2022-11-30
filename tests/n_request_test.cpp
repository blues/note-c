#include <gtest/gtest.h>

#include "note.h"

namespace
{

class Environment : public ::testing::Environment
{
public:
    ~Environment() override {}

    void SetUp() override
    {
        NoteSetFnDefault(malloc, free, NULL, NULL);
    }

    void TearDown() override {}
};

TEST(NoteNewRequestTest, NotNull)
{
    J *req = NoteNewRequest("hub.set");
    EXPECT_NE(req, nullptr);
    JDelete(req);
}

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    testing::AddGlobalTestEnvironment(new Environment);

    return RUN_ALL_TESTS();
}

