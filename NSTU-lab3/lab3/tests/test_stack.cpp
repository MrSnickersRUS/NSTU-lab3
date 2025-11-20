#include <gtest/gtest.h>
#include "../src/containers/linear.h"
#include <sstream>

class StackTest : public ::testing::Test {
protected:
    Stack<std::string>* stack;
    
    void SetUp() override {
        stack = new Stack<std::string>();
    }
    
    void TearDown() override {
        delete stack;
    }
};

TEST_F(StackTest, DefaultConstructor) {
    EXPECT_EQ(stack->size(), 0);
    EXPECT_TRUE(stack->empty());
}

TEST_F(StackTest, PushAndSize) {
    stack->push("first");
    EXPECT_EQ(stack->size(), 1);
    EXPECT_FALSE(stack->empty());
    
    stack->push("second");
    EXPECT_EQ(stack->size(), 2);
}

TEST_F(StackTest, PushAndPop) {
    stack->push("first");
    stack->push("second");
    stack->push("third");
    
    EXPECT_EQ(stack->pop(), "third");
    EXPECT_EQ(stack->size(), 2);
    EXPECT_EQ(stack->pop(), "second");
    EXPECT_EQ(stack->pop(), "first");
    EXPECT_TRUE(stack->empty());
}

TEST_F(StackTest, Peek) {
    stack->push("first");
    stack->push("second");
    
    EXPECT_EQ(stack->peek(), "second");
    EXPECT_EQ(stack->size(), 2); // Peek не удаляет элемент
}

TEST_F(StackTest, PopEmpty) {
    EXPECT_THROW(stack->pop(), std::runtime_error);
}

TEST_F(StackTest, PeekEmpty) {
    EXPECT_THROW(stack->peek(), std::runtime_error);
}

TEST_F(StackTest, Clear) {
    stack->push("first");
    stack->push("second");
    stack->push("third");
    
    stack->clear();
    EXPECT_EQ(stack->size(), 0);
    EXPECT_TRUE(stack->empty());
}

TEST_F(StackTest, LIFO_Order) {
    // Last In First Out
    for (int i = 0; i < 10; i++) {
        stack->push(std::to_string(i));
    }
    
    for (int i = 9; i >= 0; i--) {
        EXPECT_EQ(stack->pop(), std::to_string(i));
    }
}

TEST_F(StackTest, Print) {
    stack->push("bottom");
    stack->push("middle");
    stack->push("top");
    
    std::ostringstream oss;
    stack->print(oss);
    
    EXPECT_EQ(oss.str(), "[TOP top | middle | bottom]");
}

TEST_F(StackTest, BinarySerialization) {
    stack->push("first");
    stack->push("second");
    stack->push("third");
    
    // Сохранение
    std::ofstream out("test_stack.bin", std::ios::binary);
    stack->saveToBinary(out);
    out.close();
    
    // Загрузка
    Stack<std::string> stack2;
    std::ifstream in("test_stack.bin", std::ios::binary);
    stack2.loadFromBinary(in);
    in.close();
    
    EXPECT_EQ(stack2.size(), 3);
    EXPECT_EQ(stack2.pop(), "third");
    EXPECT_EQ(stack2.pop(), "second");
    EXPECT_EQ(stack2.pop(), "first");
    
    std::remove("test_stack.bin");
}

TEST_F(StackTest, MultipleOperations) {
    for (int i = 0; i < 100; i++) {
        stack->push("item" + std::to_string(i));
    }
    EXPECT_EQ(stack->size(), 100);
    
    for (int i = 0; i < 50; i++) {
        stack->pop();
    }
    EXPECT_EQ(stack->size(), 50);
}
