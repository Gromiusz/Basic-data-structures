#include <gtest/gtest.h>
#include "list.hpp"

class ListElementTest : public testing::Test
{
protected:
    ListElement<int> listElement;

    ListElementTest() : listElement(4) {}
};

TEST_F(ListElementTest, BasicTest)
{
    ASSERT_EQ(listElement.value, 4);

    size_t *counter = new size_t(0);
    listElement.count_size(counter);
    ASSERT_EQ(*counter, 0);
    delete counter;
}

TEST(EmptyAndSizeOnEmptyList, EmptySize)
{
    List<int> list;
    ASSERT_TRUE(list.empty());
    ASSERT_EQ(list.size(), 0);
}

TEST(EmptyAndSizeTest, EmptySize)
{
    List<int> list(2);
    ASSERT_FALSE(list.empty());
    ASSERT_EQ(list.size(), 1);
}

TEST(beginAndEndTest, beginAndEndd)
{
    List<int> list(2);
    ASSERT_NE(list.begin(), List<int>::Iterator(nullptr));
    ASSERT_EQ(list.end(), List<int>::Iterator(nullptr));
}

TEST(PushBackTestOnEmptyList, PushBackOnEmptyList)
{
    List<int> list;

    list.push_back(5);

    ASSERT_FALSE(list.empty());
    ASSERT_EQ(list.size(), 1);
    auto it = list.begin();
    EXPECT_EQ(*it, 5);
}

TEST(PushBackTest, PushBackOnEmptyList)
{
    List<int> list(2);

    list.push_back(5);

    ASSERT_FALSE(list.empty());
    ASSERT_EQ(list.size(), 2);
    auto it = list.begin();
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 5);
}

class IntListTest : public testing::Test
{
protected:
    List<int> list;

    IntListTest() : list(5)
    {
        list.push_back(2);
        list.push_back(8);
        // list.push_front(7);
        list.push_back(7);
    }
};

TEST_F(IntListTest, sizeTest)
{
    EXPECT_FALSE(list.empty());
    ASSERT_EQ(list.size(), 4);
}

TEST_F(IntListTest, find)
{
    auto it = std::find(list.begin(), list.end(), 2);
    ASSERT_NE(it, nullptr);
    EXPECT_EQ(*it, 2);
}

TEST_F(IntListTest, beginAndEnd)
{
    ASSERT_NE(list.begin(), List<int>::Iterator(nullptr));
    ASSERT_EQ(list.end(), List<int>::Iterator(nullptr));
}