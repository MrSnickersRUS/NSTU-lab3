#include <gtest/gtest.h>
#include "../src/containers/hash.h"
#include <sstream>

class HashMapTest : public ::testing::Test {
protected:
    HashMap<std::string, std::string>* map;
    
    void SetUp() override {
        map = new HashMap<std::string, std::string>();
    }
    
    void TearDown() override {
        delete map;
    }
};

TEST_F(HashMapTest, DefaultConstructor) {
    EXPECT_EQ(map->size(), 0);
    EXPECT_TRUE(map->empty());
}

TEST_F(HashMapTest, PutAndGet) {
    map->put("key1", "value1");
    EXPECT_EQ(map->size(), 1);
    EXPECT_EQ(map->get("key1"), "value1");
}

TEST_F(HashMapTest, PutMultiple) {
    map->put("key1", "value1");
    map->put("key2", "value2");
    map->put("key3", "value3");
    
    EXPECT_EQ(map->size(), 3);
    EXPECT_EQ(map->get("key1"), "value1");
    EXPECT_EQ(map->get("key2"), "value2");
    EXPECT_EQ(map->get("key3"), "value3");
}

TEST_F(HashMapTest, UpdateValue) {
    map->put("key1", "value1");
    map->put("key1", "newvalue");
    
    EXPECT_EQ(map->size(), 1);
    EXPECT_EQ(map->get("key1"), "newvalue");
}

TEST_F(HashMapTest, Contains) {
    map->put("key1", "value1");
    
    EXPECT_TRUE(map->contains("key1"));
    EXPECT_FALSE(map->contains("key2"));
}

TEST_F(HashMapTest, Remove) {
    map->put("key1", "value1");
    map->put("key2", "value2");
    
    EXPECT_TRUE(map->remove("key1"));
    EXPECT_EQ(map->size(), 1);
    EXPECT_FALSE(map->contains("key1"));
    EXPECT_TRUE(map->contains("key2"));
}

TEST_F(HashMapTest, RemoveNonExistent) {
    EXPECT_FALSE(map->remove("nonexistent"));
}

TEST_F(HashMapTest, GetNonExistent) {
    EXPECT_THROW(map->get("nonexistent"), std::runtime_error);
}

TEST_F(HashMapTest, Clear) {
    map->put("key1", "value1");
    map->put("key2", "value2");
    map->put("key3", "value3");
    
    map->clear();
    EXPECT_EQ(map->size(), 0);
    EXPECT_TRUE(map->empty());
}

TEST_F(HashMapTest, CollisionHandling) {
    // Добавляем много элементов для проверки обработки коллизий
    for (int i = 0; i < 50; i++) {
        std::string key = "key" + std::to_string(i);
        std::string value = "value" + std::to_string(i);
        map->put(key, value);
    }
    
    EXPECT_EQ(map->size(), 50);
    
    for (int i = 0; i < 50; i++) {
        std::string key = "key" + std::to_string(i);
        std::string value = "value" + std::to_string(i);
        EXPECT_EQ(map->get(key), value);
    }
}

TEST_F(HashMapTest, BinarySerialization) {
    map->put("key1", "value1");
    map->put("key2", "value2");
    map->put("key3", "value3");
    
    // Сохранение
    std::ofstream out("test_hashmap.bin", std::ios::binary);
    map->saveToBinary(out);
    out.close();
    
    // Загрузка
    HashMap<std::string, std::string> map2;
    std::ifstream in("test_hashmap.bin", std::ios::binary);
    map2.loadFromBinary(in);
    in.close();
    
    EXPECT_EQ(map2.size(), 3);
    EXPECT_EQ(map2.get("key1"), "value1");
    EXPECT_EQ(map2.get("key2"), "value2");
    EXPECT_EQ(map2.get("key3"), "value3");
    
    std::remove("test_hashmap.bin");
}

TEST_F(HashMapTest, EmptyOperations) {
    EXPECT_FALSE(map->remove("key"));
    EXPECT_FALSE(map->contains("key"));
}

TEST_F(HashMapTest, Print) {
    map->put("key1", "value1");
    map->put("key2", "value2");
    std::ostringstream oss;
    map->print(oss);
    std::string output = oss.str();
    EXPECT_FALSE(output.empty());
}

TEST_F(HashMapTest, RemoveAndReinsert) {
    map->put("key", "value1");
    map->remove("key");
    map->put("key", "value2");
    EXPECT_EQ(map->get("key"), "value2");
    EXPECT_EQ(map->size(), 1);
}

TEST_F(HashMapTest, IntegerHashMap) {
    HashMap<int, int> intMap;
    intMap.put(1, 100);
    intMap.put(2, 200);
    intMap.put(3, 300);
    
    EXPECT_EQ(intMap.get(1), 100);
    EXPECT_EQ(intMap.get(2), 200);
    EXPECT_EQ(intMap.get(3), 300);
}
