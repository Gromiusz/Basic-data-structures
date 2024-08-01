#include <gtest/gtest.h>
#include "list.hpp"


class ListElementTest : public testing::Test {
protected:
    ListElement<int> listElement;

    ListElementTest(): listElement(4) {}
};

TEST_F(ListElementTest, BasicTest){
    ASSERT_EQ(listElement.value, 4);

    size_t* counter = new size_t(0);
    listElement.count(counter);
    ASSERT_EQ(*counter, 0);
    delete counter;
}

// class IntListTest : public testing::Test {
// protected:
//     List<int> list;

//     IntListTest(): list(5) {

//     }
// }
