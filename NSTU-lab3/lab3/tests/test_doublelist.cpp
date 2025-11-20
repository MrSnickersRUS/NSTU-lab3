#include <gtest/gtest.h>
#include "../src/containers/linear.h"
#include <sstream>

class DoubleListTest : public ::testing::Test {
protected:
    DoubleList<int>* list;

    void SetUp() override {
        list = new DoubleList<int>();
    }

    void TearDown() override {
        delete list;
    }
};

TEST_F(DoubleListTest, DefaultConstructor) {
    EXPECT_TRUE(list->empty());
    EXPECT_EQ(list->size(), 0);
}

TEST_F(DoubleListTest, AddHead) {
    list->addHead(10);
    EXPECT_FALSE(list->empty());
    EXPECT_EQ(list->size(), 1);
}

TEST_F(DoubleListTest, AddTail) {
    list->addTail(10);
    list->addTail(20);
    list->addTail(30);
    EXPECT_EQ(list->size(), 3);
}

TEST_F(DoubleListTest, AddHeadMultiple) {
    list->addHead(1);
    list->addHead(2);
    list->addHead(3);
    EXPECT_EQ(list->size(), 3);
}

TEST_F(DoubleListTest, RemoveElement) {
    list->addTail(10);
    list->addTail(20);
    list->addTail(30);
    EXPECT_TRUE(list->remove(20));
    EXPECT_EQ(list->size(), 2);
}

TEST_F(DoubleListTest, RemoveHead) {
    list->addTail(10);
    list->addTail(20);
    EXPECT_TRUE(list->remove(10));
    EXPECT_EQ(list->size(), 1);
}

TEST_F(DoubleListTest, RemoveTail) {
    list->addTail(10);
    list->addTail(20);
    list->addTail(30);
    EXPECT_TRUE(list->remove(30));
    EXPECT_EQ(list->size(), 2);
}

TEST_F(DoubleListTest, RemoveNonExistent) {
    list->addTail(10);
    EXPECT_FALSE(list->remove(999));
    EXPECT_EQ(list->size(), 1);
}

TEST_F(DoubleListTest, Clear) {
    list->addTail(10);
    list->addTail(20);
    list->addTail(30);
    list->clear();
    EXPECT_TRUE(list->empty());
    EXPECT_EQ(list->size(), 0);
}

TEST_F(DoubleListTest, Print) {
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
