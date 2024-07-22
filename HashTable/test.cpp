#include <gtest/gtest.h>
#include "hash.hpp"

class HashTableTest : public testing::Test {
protected:
    HashTable h1;
    HashTable h2;

    HashTableTest(): h1(10), h2(10){
        h1.insert("av");
        h1.insert("bc");
    }


};




TEST_F(HashTableTest, SizeGetter)
{
    ASSERT_EQ(h1.get_size(), 2);
}

TEST_F(HashTableTest, Finder)
{
    auto [if_found, kubek, pozycja] = h1.find("av");
    ASSERT_TRUE(if_found);
    EXPECT_GE(kubek,0);
    EXPECT_LE(kubek,9);
    EXPECT_EQ(pozycja, 0);

    std::tie(if_found, kubek, pozycja) = h1.find("bc");
    ASSERT_TRUE(if_found);
    EXPECT_GE(kubek,0);
    EXPECT_LE(kubek,9);
    EXPECT_GE(pozycja, 0);
    EXPECT_LE(pozycja, 1);

    std::tie(if_found, kubek, pozycja) = h2.find("av");
    ASSERT_FALSE(if_found);
    EXPECT_EQ(kubek, -1);
    EXPECT_EQ(pozycja, -1);
}

TEST_F(HashTableTest, Deleter)
{
    ASSERT_EQ(h1.get_size(), 2);
    h1.delete_("av");
    auto [if_found, kubek, pozycja] = h1.find("av");
    ASSERT_FALSE(if_found);
    ASSERT_EQ(h1.get_size(), 1);

    // Attempt of deleting record that was previously deleted
    h1.delete_("av");
    ASSERT_EQ(h1.get_size(), 1);

    // Deleting something else
    h1.delete_("bc");
    std::tie(if_found, kubek, pozycja) = h1.find("bc");
    ASSERT_FALSE(if_found);
    ASSERT_EQ(h1.get_size(), 0);

    //Deleting something taht not have been in structure
    ASSERT_EQ(h1.get_size(), 0);
    h1.delete_("cd");
    ASSERT_EQ(h1.get_size(), 0);
}

TEST_F(HashTableTest, Deleterh2)
{
    ASSERT_EQ(h2.get_size(), 0);
    h2.delete_("cd");
    ASSERT_EQ(h2.get_size(), 0);
}


class HashTableParamTest : public testing::TestWithParam<std::string>{
protected:
    HashTable h1;

    HashTableParamTest(): h1(20) {}
};

TEST_P(HashTableParamTest, FinderParam)
{
    auto string = GetParam();
    h1.insert(string);
    auto [if_found,kubek,pozycja] = h1.find(string);
    EXPECT_TRUE(if_found);
    EXPECT_GE(kubek,0);
    EXPECT_LE(kubek,20);
    EXPECT_GE(pozycja, 0);
    EXPECT_LE(pozycja, 1);
}

INSTANTIATE_TEST_SUITE_P(Nazwa,
                        HashTableParamTest,
                        testing::Values("bdhs",
                                        "dfsf",
                                        "fgh",
                                        "juyjy",
                                        "ef")
                        );