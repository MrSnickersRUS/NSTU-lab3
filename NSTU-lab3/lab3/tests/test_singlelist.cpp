#include <gtest/gtest.h>
#include "../src/containers/linear.h"
#include <sstream>

class SingleListTest : public ::testing::Test {
protected:
    SingleList<int>* list;

    void SetUp() override {
        list = new SingleList<int>();
    }

    void TearDown() override {
        delete list;
    }
};

TEST_F(SingleListTest, DefaultConstructor) {
    EXPECT_TRUE(list->empty());
    EXPECT_EQ(list->size(), 0);
}

TEST_F(SingleListTest, AddHead) {
    list->addHead(10);
    EXPECT_FALSE(list->empty());
    EXPECT_EQ(list->size(), 1);
    EXPECT_TRUE(list->contains(10));
}

TEST_F(SingleListTest, AddTail) {
    list->addTail(10);
    list->addTail(20);
    list->addTail(30);
    EXPECT_EQ(list->size(), 3);
    EXPECT_TRUE(list->contains(10));
    EXPECT_TRUE(list->contains(20));
    EXPECT_TRUE(list->contains(30));
}

TEST_F(SingleListTest, AddHeadMultiple) {
    list->addHead(1);
    list->addHead(2);
    list->addHead(3);
    EXPECT_EQ(list->size(), 3);
}

TEST_F(SingleListTest, RemoveElement) {
    list->addTail(10);
    list->addTail(20);
    list->addTail(30);
    EXPECT_TRUE(list->remove(20));
    EXPECT_EQ(list->size(), 2);
    EXPECT_FALSE(list->contains(20));
}

TEST_F(SingleListTest, RemoveHead) {
    list->addTail(10);
    list->addTail(20);
    EXPECT_TRUE(list->remove(10));
    EXPECT_EQ(list->size(), 1);
    EXPECT_FALSE(list->contains(10));
    EXPECT_TRUE(list->contains(20));
}

TEST_F(SingleListTest, RemoveTail) {
    list->addTail(10);
    list->addTail(20);
    list->addTail(30);
    EXPECT_TRUE(list->remove(30));
    EXPECT_EQ(list->size(), 2);
}

TEST_F(SingleListTest, RemoveNonExistent) {
    list->addTail(10);
    EXPECT_FALSE(list->remove(999));
    EXPECT_EQ(list->size(), 1);
}

TEST_F(SingleListTest, Contains) {
    list->addTail(10);
    list->addTail(20);
    EXPECT_TRUE(list->contains(10));
    EXPECT_TRUE(list->contains(20));
    EXPECT_FALSE(list->contains(999));
}

TEST_F(SingleListTest, Clear) {
    list->addTail(10);
    list->addTail(20);
    list->addTail(30);
    list->clear();
    EXPECT_TRUE(list->empty());
    EXPECT_EQ(list->size(), 0);
}

TEST_F(SingleListTest, Print) {
    list->addTail(10);
    list->addTail(20);
    list->addTail(30);
    std::ostringstream oss;
    list->print(oss);
    std::string output = oss.str();
    EXPECT_TRUE(output.find("10") != std::string::npos);
    EXPECT_TRUE(output.find("20") != std::string::npos);
    EXPECT_TRUE(output.find("30") != std::string::npos);
}
