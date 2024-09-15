#include <gtest/gtest.h>
#include "deque_lib.hpp"

TEST(Basic, Init)
{
    Deque<int> queue;
    Deque<int> queue_i{3, 2};

    ASSERT_EQ(queue_i.front(), 3);
}