#include <gtest/gtest.h>
#include "../src/containers/linear.h"
#include <sstream>

class QueueTest : public ::testing::Test {
protected:
    Queue<std::string>* queue;
    
    void SetUp() override {
        queue = new Queue<std::string>();
    }
    
    void TearDown() override {
        delete queue;
    }
};

TEST_F(QueueTest, DefaultConstructor) {
    EXPECT_EQ(queue->size(), 0);
    EXPECT_TRUE(queue->empty());
}

TEST_F(QueueTest, EnqueueAndSize) {
    queue->enqueue("first");
    EXPECT_EQ(queue->size(), 1);
    EXPECT_FALSE(queue->empty());
    
    queue->enqueue("second");
    EXPECT_EQ(queue->size(), 2);
}

TEST_F(QueueTest, EnqueueAndDequeue) {
    queue->enqueue("first");
    queue->enqueue("second");
    queue->enqueue("third");
    
    EXPECT_EQ(queue->dequeue(), "first");
    EXPECT_EQ(queue->size(), 2);
    EXPECT_EQ(queue->dequeue(), "second");
    EXPECT_EQ(queue->dequeue(), "third");
    EXPECT_TRUE(queue->empty());
}

TEST_F(QueueTest, Peek) {
    queue->enqueue("first");
    queue->enqueue("second");
    
    EXPECT_EQ(queue->peek(), "first");
    EXPECT_EQ(queue->size(), 2); // Peek не удаляет
}

TEST_F(QueueTest, DequeueEmpty) {
    EXPECT_THROW(queue->dequeue(), std::runtime_error);
}

TEST_F(QueueTest, PeekEmpty) {
    EXPECT_THROW(queue->peek(), std::runtime_error);
}

TEST_F(QueueTest, Clear) {
    queue->enqueue("first");
    queue->enqueue("second");
    queue->enqueue("third");
    
    queue->clear();
    EXPECT_EQ(queue->size(), 0);
    EXPECT_TRUE(queue->empty());
}

TEST_F(QueueTest, FIFO_Order) {
    // First In First Out
    for (int i = 0; i < 10; i++) {
        queue->enqueue(std::to_string(i));
    }
    
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(queue->dequeue(), std::to_string(i));
    }
}

TEST_F(QueueTest, Print) {
    queue->enqueue("first");
    queue->enqueue("second");
    queue->enqueue("third");
    
    std::ostringstream oss;
    queue->print(oss);
    
    EXPECT_EQ(oss.str(), "[FRONT first <- second <- third REAR]");
}

TEST_F(QueueTest, BinarySerialization) {
    queue->enqueue("first");
    queue->enqueue("second");
    queue->enqueue("third");
    
    // Сохранение
    std::ofstream out("test_queue.bin", std::ios::binary);
    queue->saveToBinary(out);
    out.close();
    
    // Загрузка
    Queue<std::string> queue2;
    std::ifstream in("test_queue.bin", std::ios::binary);
    queue2.loadFromBinary(in);
    in.close();
    
    EXPECT_EQ(queue2.size(), 3);
    EXPECT_EQ(queue2.dequeue(), "first");
    EXPECT_EQ(queue2.dequeue(), "second");
    EXPECT_EQ(queue2.dequeue(), "third");
    
    std::remove("test_queue.bin");
}

TEST_F(QueueTest, MultipleOperations) {
    // Добавляем и удаляем в цикле
    for (int i = 0; i < 50; i++) {
        queue->enqueue("item" + std::to_string(i));
    }
    EXPECT_EQ(queue->size(), 50);
    
    for (int i = 0; i < 25; i++) {
        queue->dequeue();
    }
    EXPECT_EQ(queue->size(), 25);
    
    // Добавляем еще
    for (int i = 50; i < 75; i++) {
        queue->enqueue("item" + std::to_string(i));
    }
    EXPECT_EQ(queue->size(), 50);
}
