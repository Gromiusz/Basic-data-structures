#include <gtest/gtest.h>
#include "list.hpp"

class ListElementTest : public testing::Test
{
protected:
    ListElement<int> listElement;

    ListElementTest() : listElement(4) {}
};

TEST_F(ListElementTest, AllTestsOnElementList)
{
    ASSERT_EQ(listElement.value, 4);

    size_t *counter = new size_t(0);
    listElement.count_size(counter);
    ASSERT_EQ(*counter, 0);
    delete counter;
}

TEST(EmptySize, EmptyList)
{
    List<int> list;
    ASSERT_TRUE(list.empty());
    ASSERT_EQ(list.size(), 0);
}

TEST(EmptySize, NotEmptyList)
{
    List<int> list(2);
    ASSERT_FALSE(list.empty());
    ASSERT_EQ(list.size(), 1);
}

TEST(BeginEnd, NotEmptyList)
{
    List<int> list(2);
    ASSERT_NE(list.begin(), List<int>::Iterator(nullptr));
    ASSERT_EQ(list.end(), List<int>::Iterator(nullptr));
}

TEST(IteratorPostincrementation, NotEmptyList)
{
    List<int> list(2);
    auto it = list.begin();
    EXPECT_NE(it, nullptr);

    it++;
    EXPECT_EQ(it, nullptr);
}

TEST(Pushback, EmptyList)
{
    List<int> list;

    list.push_back(5);

    ASSERT_FALSE(list.empty());
    ASSERT_EQ(list.size(), 1);
    auto it = list.begin();
    EXPECT_EQ(*it, 5);

    ++it;
    EXPECT_EQ(it, list.end());
}

TEST(Pushback, NotEmptyList)
{
    List<int> list(2);

    list.push_back(5);

    ASSERT_FALSE(list.empty());
    ASSERT_EQ(list.size(), 2);
    auto it = list.begin();
    EXPECT_EQ(*it, 2);

    ++it;
    EXPECT_EQ(*it, 5);

    ++it;
    EXPECT_EQ(it, list.end());
}

TEST(Pushfront, EmptyList)
{
    List<int> list;

    list.push_front(5);

    ASSERT_FALSE(list.empty());
    ASSERT_EQ(list.size(), 1);
    auto it = list.begin();
    EXPECT_EQ(*it, 5);
}

// TEST(Pushfront, NotEmptyList)
// {
//     List<int> list(2);

//     list.push_front(5);

//     ASSERT_FALSE(list.empty());
//     EXPECT_EQ(list.size(), 2);
//     auto it = list.begin();
//     EXPECT_NE(it, nullptr);
//     EXPECT_EQ(*it, 5);

//     ++it;
//     EXPECT_NE(it, nullptr);
//     EXPECT_EQ(*it, 2);

//     ++it;
//     EXPECT_EQ(it, nullptr);
// }

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