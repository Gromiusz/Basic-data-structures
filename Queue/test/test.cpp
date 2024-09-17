#include <gtest/gtest.h>
#include "deque_lib.hpp"

class BracketBuckleTest : public testing::Test 
{
public:
    Deque<int> queue_bracket;
    Deque<int> queue_buckle;
    BracketBuckleTest() : queue_bracket(5, 4), queue_buckle{3, 2} {}   
};

class BigQueueTest : public testing::Test 
{
public:
    Deque<int> big_queue; // size = 14
    BigQueueTest() : big_queue{3, 2, 6, 5, 1, 3, 2, 2, 8, 5, 6, 22, -5, 6} {}   
};

TEST(Constructors, Constructors)
{
    Deque<int> queue;

    EXPECT_EQ(queue.size(), 0);
    EXPECT_TRUE(queue.empty());
}

TEST_F(BracketBuckleTest, Constructors)
{
    EXPECT_EQ(queue_buckle.front(), 3);
    EXPECT_EQ(queue_buckle.back(), 2);
    EXPECT_EQ(queue_buckle.size(), 2);
    EXPECT_FALSE(queue_buckle.empty());

    EXPECT_EQ(queue_bracket.front(), 4);
    EXPECT_EQ(queue_bracket.back(), 4);
    EXPECT_EQ(queue_bracket.size(), 5);
    EXPECT_FALSE(queue_bracket.empty());
}

TEST_F(BracketBuckleTest, AssignBrackettoExistingBuckle)
{
    queue_buckle = queue_bracket;
    EXPECT_EQ(queue_buckle.front(), 4);
    EXPECT_EQ(queue_buckle.back(), 4);
    EXPECT_EQ(queue_buckle.size(), 5);
    EXPECT_FALSE(queue_buckle.empty());
}

TEST_F(BracketBuckleTest, AssignBuckleToExistingBracket)
{
    queue_bracket = queue_buckle;
    EXPECT_EQ(queue_buckle.front(), 3);
    EXPECT_EQ(queue_buckle.back(), 2);
    EXPECT_EQ(queue_buckle.size(), 2);
    EXPECT_FALSE(queue_buckle.empty());
}

TEST_F(BracketBuckleTest, AssignNormal)
{
    queue_bracket.assign(2, 1);
    EXPECT_EQ(queue_bracket.front(), 1);
    EXPECT_EQ(queue_bracket.back(), 1);
    EXPECT_EQ(queue_bracket.size(), 2);
    EXPECT_FALSE(queue_bracket.empty());
}

TEST_F(BracketBuckleTest, AssignInitializerList)
{
    queue_bracket.assign({4, 3, 3, 2, 1, 6, 7});
    EXPECT_EQ(queue_bracket.front(), 4);
    EXPECT_EQ(queue_bracket.back(), 7);
    EXPECT_EQ(queue_bracket.size(), 7);
    EXPECT_FALSE(queue_bracket.empty());
}

TEST_F(BracketBuckleTest, AssignIterators)
{
    auto it_begin = queue_buckle.begin();
    auto it_end = queue_buckle.end();

    queue_bracket.assign(it_begin, it_end);
    EXPECT_EQ(queue_bracket.front(), 3);
    EXPECT_EQ(queue_bracket.back(), 2);
    EXPECT_EQ(queue_bracket.size(), 2);
    EXPECT_FALSE(queue_bracket.empty());
}

TEST_F(BracketBuckleTest, DirectAcess)
{
    EXPECT_EQ(queue_buckle[0], 3);
    EXPECT_EQ(queue_buckle[1], 2);
}

TEST_F(BracketBuckleTest, DirectAcessWithChange)
{
    ASSERT_EQ(queue_buckle[0], 3);
    queue_buckle[0] = 8;
    EXPECT_EQ(queue_buckle[0], 8);
    EXPECT_EQ(queue_buckle[1], 2);
}

TEST_F(BigQueueTest, DirectAcessWithChange)
{
    EXPECT_EQ(big_queue[0], 3);
    EXPECT_EQ(big_queue[13], 6);

    ASSERT_EQ(big_queue[8], 8);
    big_queue[8] = -20;
    EXPECT_EQ(big_queue[8], -20);
}




