#ifndef TREES_H
#define TREES_H

#include "containers.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <fstream>

template<typename T>
class AVLTree : public Container<T> {
private:
    struct Node {
        T data;
        int height;
        Node* left;
        Node* right;
        Node(const T& value);
    };
    
    Node* root;
    size_t count;
    
    int getHeight(Node* node) const;
    int getBalance(Node* node) const;
    void updateHeight(Node* node);
    Node* rotateRight(Node* y);
    Node* rotateLeft(Node* x);
    Node* rebalance(Node* node);
    Node* insertNode(Node* node, const T& value);
    Node* findMin(Node* node) const;
    Node* deleteNode(Node* node, const T& value);
    bool searchNode(Node* node, const T& value) const;
    void clearNode(Node* node);
    void inorderTraversal(Node* node, std::ostream& os) const;

public:
    AVLTree();
    ~AVLTree() override;
    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;
    
    AVLTree(AVLTree&& other) noexcept : root(other.root), count(other.count) {
        other.root = nullptr;
        other.count = 0;
    }
    
    AVLTree& operator=(AVLTree&& other) noexcept {
        if (this != &other) {
            clear();
            root = other.root;
            count = other.count;
            other.root = nullptr;
            other.count = 0;
        }
        return *this;
    }
    
    void insert(const T& value);
    void remove(const T& value);
    bool search(const T& value) const;
    void clear() override;
    size_t size() const override;
    bool empty() const override;
    int height() const;
    void print(std::ostream& os = std::cout) const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    
    // Бинарная сериализация
    void saveToBinary(std::ofstream& out) const;
    void loadFromBinary(std::ifstream& in);
private:
    void saveBinaryNode(std::ofstream& out, Node* node) const;
    Node* loadBinaryNode(std::ifstream& in);
};

#include "avl.cpp"

#endif
