#include <gtest/gtest.h>
#include "deque_lib.hpp"

TEST(Basic, Init)
{
    Deque<int> queue;
    Deque<int> queue_i{3, 2};

    EXPECT_EQ(queue.size(), 0);

    EXPECT_EQ(queue_i.front(), 3);
    EXPECT_EQ(queue_i.back(), 2);
    EXPECT_EQ(queue_i.size(), 2);
}