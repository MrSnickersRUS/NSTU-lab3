#include <gtest/gtest.h>
#include "../src/containers/trees.h"
#include <sstream>

class AVLTreeTest : public ::testing::Test {
protected:
    AVLTree<int>* tree;
    
    void SetUp() override {
        tree = new AVLTree<int>();
    }
    
    void TearDown() override {
        delete tree;
    }
};

TEST_F(AVLTreeTest, DefaultConstructor) {
    EXPECT_EQ(tree->size(), 0);
    EXPECT_TRUE(tree->empty());
}

TEST_F(AVLTreeTest, Insert) {
    tree->insert(10);
    EXPECT_EQ(tree->size(), 1);
    EXPECT_TRUE(tree->search(10));
}

TEST_F(AVLTreeTest, InsertMultiple) {
    tree->insert(10);
    tree->insert(5);
    tree->insert(15);
    tree->insert(3);
    tree->insert(7);
    
    EXPECT_EQ(tree->size(), 5);
    EXPECT_TRUE(tree->search(10));
    EXPECT_TRUE(tree->search(5));
    EXPECT_TRUE(tree->search(15));
    EXPECT_TRUE(tree->search(3));
    EXPECT_TRUE(tree->search(7));
}

TEST_F(AVLTreeTest, InsertDuplicate) {
    tree->insert(10);
    tree->insert(10);
    
    EXPECT_EQ(tree->size(), 1);
}

TEST_F(AVLTreeTest, Remove) {
    tree->insert(10);
    tree->insert(5);
    tree->insert(15);
    
    tree->remove(5);
    EXPECT_EQ(tree->size(), 2);
    EXPECT_FALSE(tree->search(5));
}

TEST_F(AVLTreeTest, RemoveNonExistent) {
    tree->insert(10);
    tree->remove(20);  // remove возвращает void
    EXPECT_EQ(tree->size(), 1);
}

TEST_F(AVLTreeTest, RemoveFromEmpty) {
    tree->remove(10);  // remove возвращает void
    EXPECT_EQ(tree->size(), 0);
}

TEST_F(AVLTreeTest, Search) {
    tree->insert(10);
    tree->insert(5);
    tree->insert(15);
    
    EXPECT_TRUE(tree->search(10));
    EXPECT_TRUE(tree->search(5));
    EXPECT_TRUE(tree->search(15));
    EXPECT_FALSE(tree->search(20));
}

TEST_F(AVLTreeTest, Clear) {
    tree->insert(10);
    tree->insert(5);
    tree->insert(15);
    
    tree->clear();
    EXPECT_EQ(tree->size(), 0);
    EXPECT_TRUE(tree->empty());
}

TEST_F(AVLTreeTest, BalanceLeftRotation) {
    // Вставка в возрастающем порядке должна вызвать левые вращения
    tree->insert(1);
    tree->insert(2);
    tree->insert(3);
    tree->insert(4);
    tree->insert(5);
    
    EXPECT_EQ(tree->size(), 5);
    // Проверяем, что все элементы присутствуют
    for (int i = 1; i <= 5; i++) {
        EXPECT_TRUE(tree->search(i));
    }
}

TEST_F(AVLTreeTest, BalanceRightRotation) {
    // Вставка в убывающем порядке должна вызвать правые вращения
    tree->insert(5);
    tree->insert(4);
    tree->insert(3);
    tree->insert(2);
    tree->insert(1);
    
    EXPECT_EQ(tree->size(), 5);
    // Проверяем, что все элементы присутствуют
    for (int i = 1; i <= 5; i++) {
        EXPECT_TRUE(tree->search(i));
    }
}

TEST_F(AVLTreeTest, BalanceLeftRightRotation) {
    tree->insert(10);
    tree->insert(5);
    tree->insert(7);
    
    EXPECT_EQ(tree->size(), 3);
    EXPECT_TRUE(tree->search(10));
    EXPECT_TRUE(tree->search(5));
    EXPECT_TRUE(tree->search(7));
}

TEST_F(AVLTreeTest, BalanceRightLeftRotation) {
    tree->insert(10);
    tree->insert(15);
    tree->insert(12);
    
    EXPECT_EQ(tree->size(), 3);
    EXPECT_TRUE(tree->search(10));
    EXPECT_TRUE(tree->search(15));
    EXPECT_TRUE(tree->search(12));
}

TEST_F(AVLTreeTest, MultipleOperations) {
    // Вставка 100 элементов
    for (int i = 0; i < 100; i++) {
        tree->insert(i);
    }
    EXPECT_EQ(tree->size(), 100);
    
    // Проверка всех элементов
    for (int i = 0; i < 100; i++) {
        EXPECT_TRUE(tree->search(i));
    }
    
    // Удаление половины элементов
    for (int i = 0; i < 50; i++) {
        tree->remove(i);
    }
    EXPECT_EQ(tree->size(), 50);
    
    // Проверка оставшихся элементов
    for (int i = 50; i < 100; i++) {
        EXPECT_TRUE(tree->search(i));
    }
}

TEST_F(AVLTreeTest, BinarySerialization) {
    tree->insert(10);
    tree->insert(5);
    tree->insert(15);
    tree->insert(3);
    tree->insert(7);
    tree->insert(12);
    tree->insert(20);
    
    // Сохранение
    std::ofstream out("test_avltree.bin", std::ios::binary);
    tree->saveToBinary(out);
    out.close();
    
    // Загрузка
    AVLTree<int> tree2;
    std::ifstream in("test_avltree.bin", std::ios::binary);
    tree2.loadFromBinary(in);
    in.close();
    
    EXPECT_EQ(tree2.size(), 7);
    EXPECT_TRUE(tree2.search(10));
    EXPECT_TRUE(tree2.search(5));
    EXPECT_TRUE(tree2.search(15));
    EXPECT_TRUE(tree2.search(3));
    EXPECT_TRUE(tree2.search(7));
    EXPECT_TRUE(tree2.search(12));
    EXPECT_TRUE(tree2.search(20));
    
    std::remove("test_avltree.bin");
}

TEST_F(AVLTreeTest, RemoveRootNode) {
    tree->insert(10);
    tree->insert(5);
    tree->insert(15);
    tree->remove(10);
    EXPECT_EQ(tree->size(), 2);
    EXPECT_FALSE(tree->search(10));
}

TEST_F(AVLTreeTest, RemoveLeafNodes) {
    tree->insert(10);
    tree->insert(5);
    tree->insert(15);
    tree->remove(5);
    tree->remove(15);
    EXPECT_EQ(tree->size(), 1);
    EXPECT_TRUE(tree->search(10));
}

TEST_F(AVLTreeTest, Print) {
    tree->insert(10);
    tree->insert(5);
    tree->insert(15);
    std::ostringstream oss;
    tree->print(oss);
    std::string output = oss.str();
    EXPECT_TRUE(output.find("5") != std::string::npos);
    EXPECT_TRUE(output.find("10") != std::string::npos);
    EXPECT_TRUE(output.find("15") != std::string::npos);
}

TEST_F(AVLTreeTest, SearchInEmpty) {
    EXPECT_FALSE(tree->search(10));
}

TEST_F(AVLTreeTest, ComplexRotations) {
    tree->insert(50);
    tree->insert(25);
    tree->insert(75);
    tree->insert(10);
    tree->insert(30);
    tree->insert(60);
    tree->insert(80);
    tree->insert(5);
    tree->insert(15);
    tree->insert(27);
    tree->insert(35);
    
    EXPECT_EQ(tree->size(), 11);
    for (auto val : {50, 25, 75, 10, 30, 60, 80, 5, 15, 27, 35}) {
        EXPECT_TRUE(tree->search(val));
    }
}
