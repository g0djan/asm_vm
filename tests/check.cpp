extern "C" {
#include "../asm/f.h"
}

#include "gtest/gtest.h"

TEST(func, Sanity) {
    ASSERT_TRUE(f() == 5);
}
