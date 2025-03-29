#include <gtest/gtest.h>

TEST(HelloTest, BasicAssertions){
    EXPECT_STRNE("Hello","World!");
}
