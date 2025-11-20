#include <gtest/gtest.h>
#include "../src/containers/linear.h"
#include <sstream>

// Тесты для Array
class ArrayTest : public ::testing::Test {
protected:
    Array<std::string>* arr;
    
    void SetUp() override {
        arr = new Array<std::string>();
    }
    
    void TearDown() override {
        delete arr;
    }
};

TEST_F(ArrayTest, DefaultConstructor) {
    EXPECT_EQ(arr->size(), 0);
    EXPECT_TRUE(arr->empty());
}

TEST_F(ArrayTest, PushAndSize) {
    arr->push("first");
    EXPECT_EQ(arr->size(), 1);
    EXPECT_FALSE(arr->empty());
    
    arr->push("second");
    EXPECT_EQ(arr->size(), 2);
}

TEST_F(ArrayTest, GetElement) {
    arr->push("first");
    arr->push("second");
    arr->push("third");
    
    EXPECT_EQ(arr->get(0), "first");
    EXPECT_EQ(arr->get(1), "second");
    EXPECT_EQ(arr->get(2), "third");
}

TEST_F(ArrayTest, GetOutOfBounds) {
    arr->push("first");
    EXPECT_THROW(arr->get(1), std::out_of_range);
    EXPECT_THROW(arr->get(100), std::out_of_range);
}

TEST_F(ArrayTest, Insert) {
    arr->push("first");
    arr->push("third");
    arr->insert(1, "second");
    
    EXPECT_EQ(arr->size(), 3);
    EXPECT_EQ(arr->get(0), "first");
    EXPECT_EQ(arr->get(1), "second");
    EXPECT_EQ(arr->get(2), "third");
}

TEST_F(ArrayTest, InsertAtBeginning) {
    arr->push("second");
    arr->insert(0, "first");
    
    EXPECT_EQ(arr->get(0), "first");
    EXPECT_EQ(arr->get(1), "second");
}

TEST_F(ArrayTest, InsertAtEnd) {
    arr->push("first");
    arr->insert(1, "second");
    
    EXPECT_EQ(arr->get(1), "second");
}

TEST_F(ArrayTest, Remove) {
    arr->push("first");
    arr->push("second");
    arr->push("third");
    arr->remove(1);
    
    EXPECT_EQ(arr->size(), 2);
    EXPECT_EQ(arr->get(0), "first");
    EXPECT_EQ(arr->get(1), "third");
}

TEST_F(ArrayTest, Replace) {
    arr->push("first");
    arr->push("second");
    arr->replace(1, "SECOND");
    
    EXPECT_EQ(arr->get(1), "SECOND");
    EXPECT_EQ(arr->size(), 2);
}

TEST_F(ArrayTest, Clear) {
    arr->push("first");
    arr->push("second");
    arr->clear();
    
    EXPECT_EQ(arr->size(), 0);
    EXPECT_TRUE(arr->empty());
}

TEST_F(ArrayTest, MultipleOperations) {
    for (int i = 0; i < 100; i++) {
        arr->push("item" + std::to_string(i));
    }
    EXPECT_EQ(arr->size(), 100);
    
    for (int i = 0; i < 50; i++) {
        arr->remove(0);
    }
    EXPECT_EQ(arr->size(), 50);
    EXPECT_EQ(arr->get(0), "item50");
}

TEST_F(ArrayTest, Print) {
    arr->push("hello");
    arr->push("world");
    
    std::ostringstream oss;
    arr->print(oss);
    
    EXPECT_EQ(oss.str(), "[hello, world]");
}

TEST_F(ArrayTest, BinarySerialization) {
    arr->push("test1");
    arr->push("test2");
    arr->push("test3");
    
    // Сохранение
    std::ofstream out("test_array.bin", std::ios::binary);
    arr->saveToBinary(out);
    out.close();
    
    // Загрузка в новый массив
    Array<std::string> arr2;
    std::ifstream in("test_array.bin", std::ios::binary);
    arr2.loadFromBinary(in);
    in.close();
    
    EXPECT_EQ(arr2.size(), 3);
    EXPECT_EQ(arr2.get(0), "test1");
    EXPECT_EQ(arr2.get(1), "test2");
    EXPECT_EQ(arr2.get(2), "test3");
    
    // Удаляем тестовый файл
    std::remove("test_array.bin");
}
