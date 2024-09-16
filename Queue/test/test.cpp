#include <gtest/gtest.h>
#include "deque_lib.hpp"

TEST(Constructors, Constructors)
{
    Deque<int> queue;
    Deque<int> queue_ii(5, 4);
    Deque<int> queue_i{3, 2};

    EXPECT_EQ(queue.size(), 0);
    EXPECT_TRUE(queue.empty());

    EXPECT_EQ(queue_i.front(), 3);
    EXPECT_EQ(queue_i.back(), 2);
    EXPECT_EQ(queue_i.size(), 2);
    EXPECT_FALSE(queue_i.empty());

    EXPECT_EQ(queue_ii.front(), 4);
    EXPECT_EQ(queue_ii.back(), 4);
    EXPECT_EQ(queue_ii.size(), 5);
    EXPECT_FALSE(queue_ii.empty());
}