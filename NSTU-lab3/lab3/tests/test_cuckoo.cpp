#include <gtest/gtest.h>
#include "../src/containers/hash.h"
#include <sstream>

class CuckooHashMapTest : public ::testing::Test {
protected:
    CuckooHashMap<std::string, int>* map;

    void SetUp() override {
        map = new CuckooHashMap<std::string, int>();
    }

    void TearDown() override {
        delete map;
    }
};

TEST_F(CuckooHashMapTest, DefaultConstructor) {
    EXPECT_TRUE(map->empty());
    EXPECT_EQ(map->size(), 0);
}

TEST_F(CuckooHashMapTest, PutAndGet) {
    map->put("key1", 100);
    EXPECT_EQ(map->get("key1"), 100);
}

TEST_F(CuckooHashMapTest, PutMultiple) {
    map->put("a", 1);
    map->put("b", 2);
    map->put("c", 3);
    EXPECT_EQ(map->size(), 3);
    EXPECT_EQ(map->get("a"), 1);
    EXPECT_EQ(map->get("b"), 2);
    EXPECT_EQ(map->get("c"), 3);
}

TEST_F(CuckooHashMapTest, UpdateValue) {
    map->put("key", 10);
    map->put("key", 20);
    EXPECT_EQ(map->get("key"), 20);
    EXPECT_EQ(map->size(), 1);
}

TEST_F(CuckooHashMapTest, Contains) {
    map->put("key1", 100);
    EXPECT_TRUE(map->contains("key1"));
    EXPECT_FALSE(map->contains("key2"));
}

TEST_F(CuckooHashMapTest, Remove) {
    map->put("key1", 100);
    map->put("key2", 200);
    EXPECT_TRUE(map->remove("key1"));
    EXPECT_EQ(map->size(), 1);
    EXPECT_FALSE(map->contains("key1"));
}

TEST_F(CuckooHashMapTest, RemoveNonExistent) {
    EXPECT_FALSE(map->remove("nonexistent"));
}

TEST_F(CuckooHashMapTest, Clear) {
    map->put("a", 1);
    map->put("b", 2);
    map->clear();
    EXPECT_TRUE(map->empty());
    EXPECT_EQ(map->size(), 0);
}

TEST_F(CuckooHashMapTest, Print) {
    map->put("key1", 100);
    std::ostringstream oss;
    map->print(oss);
    std::string output = oss.str();
    EXPECT_FALSE(output.empty());
}

TEST_F(CuckooHashMapTest, CollisionHandling) {
    for (int i = 0; i < 10; i++) {
        map->put("key" + std::to_string(i), i);
    }
    EXPECT_EQ(map->size(), 10);
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(map->get("key" + std::to_string(i)), i);
    }
}
