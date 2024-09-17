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

TEST(Assign, BrackettoExistingBuckle)
{
    Deque<int> queue_bracket(5, 4);
    Deque<int> queue_buckle{3, 2};

    queue_buckle = queue_bracket;
    EXPECT_EQ(queue_buckle.front(), 4);
    EXPECT_EQ(queue_buckle.back(), 4);
    EXPECT_EQ(queue_buckle.size(), 5);
    EXPECT_FALSE(queue_buckle.empty());
}

TEST(Assign, BuckleToExistingBracket)
{
    Deque<int> queue_bracket(5, 4);
    Deque<int> queue_buckle{3, 2};

    queue_bracket = queue_buckle;
    EXPECT_EQ(queue_buckle.front(), 3);
    EXPECT_EQ(queue_buckle.back(), 2);
    EXPECT_EQ(queue_buckle.size(), 2);
    EXPECT_FALSE(queue_buckle.empty());
}

TEST(Assign, Normal)
{
    Deque<int> queue_bracket(5, 4);

    queue_bracket.assign(2, 1);
    EXPECT_EQ(queue_bracket.front(), 1);
    EXPECT_EQ(queue_bracket.back(), 1);
    EXPECT_EQ(queue_bracket.size(), 2);
    EXPECT_FALSE(queue_bracket.empty());
}

TEST(Assign, InitializerList)
{
    Deque<int> queue_bracket(5, 4);

    queue_bracket.assign({4, 3, 3, 2, 1, 6, 7});
    EXPECT_EQ(queue_bracket.front(), 4);
    EXPECT_EQ(queue_bracket.back(), 7);
    EXPECT_EQ(queue_bracket.size(), 7);
    EXPECT_FALSE(queue_bracket.empty());
}

TEST(Assign, Iterators)
{
    Deque<int> queue_bracket(5, 4);
    Deque<int> queue_buckle{3, 2};

    queue_bracket.assign(queue_buckle.begin(), queue_buckle.end());
    EXPECT_EQ(queue_bracket.front(), 3);
    EXPECT_EQ(queue_bracket.back(), 2);
    EXPECT_EQ(queue_bracket.size(), 2);
    EXPECT_FALSE(queue_bracket.empty());
}