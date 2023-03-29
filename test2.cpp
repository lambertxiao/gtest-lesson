#include <gtest/gtest.h>

class Math {
 public:
  int add(int x, int y) {
    return x + y;
  }
  int sub(int x, int y) {
    return x - y;
  }
};

class MathTestSuite : public testing::Test {
 protected:
  Math math;
};

TEST_F(MathTestSuite, testAdd) {
  EXPECT_EQ(math.add(1, 2), 3);
}

TEST_F(MathTestSuite, testSub) {
  EXPECT_EQ(math.sub(3, 1), 2);
}

