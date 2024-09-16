#include <gtest/gtest.h>
#include "deque_lib.hpp"

TEST(Constructors, Constructors)
{
    Deque<int> queue;
    Deque<int> queue_bracket(5, 4);
    Deque<int> queue_buckle{3, 2};

    EXPECT_EQ(queue.size(), 0);
    EXPECT_TRUE(queue.empty());

    EXPECT_EQ(queue_buckle.front(), 3);
    EXPECT_EQ(queue_buckle.back(), 2);
    EXPECT_EQ(queue_buckle.size(), 2);
    EXPECT_FALSE(queue_buckle.empty());

    EXPECT_EQ(queue_bracket.front(), 4);
    EXPECT_EQ(queue_bracket.back(), 4);
    EXPECT_EQ(queue_bracket.size(), 5);
    EXPECT_FALSE(queue_bracket.empty());
}

TEST(Assign, toExisting)
{
    Deque<int> queue_bracket(5, 4);
    Deque<int> queue_buckle{3, 2};

    queue_buckle = queue_bracket;
    EXPECT_EQ(queue_buckle.front(), 4);
    EXPECT_EQ(queue_buckle.back(), 4);
    EXPECT_EQ(queue_buckle.size(), 5);
    EXPECT_FALSE(queue_buckle.empty());
}