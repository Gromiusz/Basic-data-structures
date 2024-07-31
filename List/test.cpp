#include <gtest/gtest.h>
#include "list.hpp"


class ListElementTest : public testing::Test {
protected:
    ListElement<int> listElement;

    ListElementTest(): listElement(4) {}
};

TEST_F(ListElementTest, BasicTest){
    ASSERT_EQ(listElement.value, 4);

}

