#include <gtest/gtest.h>
#include "lib.hpp"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(InsertTest, Basic){
    BinaryTree b1(3);
    EXPECT_TRUE(BTM::is_right_nullptr(&b1));
    EXPECT_TRUE(BTM::is_left_nullptr(&b1));
    BTM::insert(&b1, 4);
    EXPECT_FALSE(BTM::is_right_nullptr(&b1));
    EXPECT_TRUE(BTM::is_left_nullptr(&b1));
}

class BinaryTreeTest : public testing::Test {
protected:
    BinaryTree b;

    BinaryTreeTest(): b(15) {
      BTM::insert(&b, 12);
      BTM::insert(&b, 3);
      BTM::insert(&b, 5);
      BTM::insert(&b, 23);
      BTM::insert(&b, 6);
      BTM::insert(&b, 18);
    }
};

TEST_F(BinaryTreeTest, Pointers) {
    ASSERT_FALSE(BTM::is_left_nullptr(&b));
    ASSERT_FALSE(BTM::is_right_nullptr(&b));
}

TEST_F(BinaryTreeTest, Searching) {
    ASSERT_EQ(BTM::get_value(BTM::search(&b, 12)), 12);
    ASSERT_EQ(BTM::get_value(BTM::search(&b, 18)), 18);
}

class BinaryTreeParamTest : public testing::TestWithParam<int> {
  protected:
    BinaryTree b;

    BinaryTreeParamTest(): b(15) {
      BTM::insert(&b, 12);
      BTM::insert(&b, 3);
      BTM::insert(&b, 5);
      BTM::insert(&b, 23);
      BTM::insert(&b, 6);
      BTM::insert(&b, 18);
    }
};

TEST_P(BinaryTreeParamTest, Searching) {
    int value = GetParam();
    ASSERT_EQ(BTM::get_value(BTM::search(&b, value)), value);
}

INSTANTIATE_TEST_SUITE_P(Nazwa, BinaryTreeParamTest, testing::Values(12, 3, 5, 23, 6, 18));

