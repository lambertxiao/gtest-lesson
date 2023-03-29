#include <gtest/gtest.h>

int add(int x, int y) {
  return x + y;
}

TEST(AdditionTest, testAdd) {
  EXPECT_EQ(add(1, 2), 3);
}

