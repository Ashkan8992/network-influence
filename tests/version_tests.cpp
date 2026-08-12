#include "influence/version.hpp"
#include <gtest/gtest.h>

TEST(VersionTest, ReturnsCurrentVersion) {
    EXPECT_STREQ(influence::version(), "0.1.0");
    /* TEST(...)
    EXPECT_EQ(...)
    EXPECT_TRUE(...)
    EXPECT_FALSE(...)
    EXPECT_THROW(...) */
}
