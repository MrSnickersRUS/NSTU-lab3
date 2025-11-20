#include <gtest/gtest.h>
#include "../src/containers/hash.h"
#include <sstream>

class SetTest : public ::testing::Test {
protected:
    Set<int>* set;

    void SetUp() override {
        set = new Set<int>();
    }

    void TearDown() override {
        delete set;
    }
};

TEST_F(SetTest, DefaultConstructor) {
    EXPECT_TRUE(set->empty());
    EXPECT_EQ(set->size(), 0);
}

TEST_F(SetTest, Add) {
    set->add(10);
    EXPECT_FALSE(set->empty());
    EXPECT_EQ(set->size(), 1);
    EXPECT_TRUE(set->contains(10));
}

TEST_F(SetTest, AddMultiple) {
    set->add(10);
    set->add(20);
    set->add(30);
    EXPECT_EQ(set->size(), 3);
    EXPECT_TRUE(set->contains(10));
    EXPECT_TRUE(set->contains(20));
    EXPECT_TRUE(set->contains(30));
}

TEST_F(SetTest, AddDuplicate) {
    set->add(10);
    set->add(10);
    EXPECT_EQ(set->size(), 1);
}

TEST_F(SetTest, Contains) {
    set->add(10);
    EXPECT_TRUE(set->contains(10));
    EXPECT_FALSE(set->contains(999));
}

TEST_F(SetTest, Remove) {
    set->add(10);
    set->add(20);
    EXPECT_TRUE(set->remove(10));
    EXPECT_EQ(set->size(), 1);
    EXPECT_FALSE(set->contains(10));
}

TEST_F(SetTest, RemoveNonExistent) {
    set->add(10);
    EXPECT_FALSE(set->remove(999));
    EXPECT_EQ(set->size(), 1);
}

TEST_F(SetTest, Clear) {
    set->add(10);
    set->add(20);
    set->add(30);
    set->clear();
    EXPECT_TRUE(set->empty());
    EXPECT_EQ(set->size(), 0);
}

TEST_F(SetTest, Print) {
    set->add(10);
    set->add(20);
    std::ostringstream oss;
    set->print(oss);
    std::string output = oss.str();
    EXPECT_FALSE(output.empty());
}
