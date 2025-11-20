#ifndef AVL_CPP
#define AVL_CPP

#include <cstdint>
#include <fstream>
#include "../binary_serialization.h"
#include "trees.h"

template<typename T>
AVLTree<T>::Node::Node(const T& value) 
    : data(value), height(1), left(nullptr), right(nullptr) {}

template<typename T>
AVLTree<T>::AVLTree() : root(nullptr), count(0) {}

template<typename T>
AVLTree<T>::~AVLTree() {
    clear();
}

template<typename T>
int AVLTree<T>::getHeight(Node* node) const {
    return node ? node->height : 0;
}

template<typename T>
int AVLTree<T>::getBalance(Node* node) const {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

template<typename T>
void AVLTree<T>::updateHeight(Node* node) {
    if (node) {
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::rotateRight(Node* y) {
    Node* x = y->left;
    Node* B = x->right;
    
    x->right = y;
    y->left = B;
    
    updateHeight(y);
    updateHeight(x);
    
    return x;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::rotateLeft(Node* x) {
    Node* y = x->right;
    Node* B = y->left;
    
    y->left = x;
    x->right = B;
    
    updateHeight(x);
    updateHeight(y);
    
    return y;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::rebalance(Node* node) {
    if (!node) return nullptr;
    
    int balance = getBalance(node);
    
    if (balance > 1) {
        if (getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    
    if (balance < -1) {
        if (getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }
    
    return node;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::insertNode(Node* node, const T& value) {
    if (!node) {
        count++;
        return new Node(value);
    }
    
    if (value < node->data) {
        node->left = insertNode(node->left, value);
    } else if (value > node->data) {
        node->right = insertNode(node->right, value);
    } else {
        return node;
    }
    
    updateHeight(node);
    return rebalance(node);
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::findMin(Node* node) const {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::deleteNode(Node* node, const T& value) {
    if (!node) return nullptr;
    
    if (value < node->data) {
        node->left = deleteNode(node->left, value);
    } else if (value > node->data) {
        node->right = deleteNode(node->right, value);
    } else {
        if (!node->left || !node->right) {
            Node* temp = node->left ? node->left : node->right;
            if (!temp) {
                temp = node;
                node = nullptr;
            } else {
                *node = *temp;
            }
            delete temp;
            count--;
        } else {
            Node* temp = findMin(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data);
        }
    }
    
    if (!node) return nullptr;
    
    updateHeight(node);
    return rebalance(node);
}

template<typename T>
bool AVLTree<T>::searchNode(Node* node, const T& value) const {
    if (!node) return false;
    if (value == node->data) return true;
    if (value < node->data) {
        return searchNode(node->left, value);
    } else {
        return searchNode(node->right, value);
    }
}

template<typename T>
void AVLTree<T>::clearNode(Node* node) {
    if (!node) return;
    clearNode(node->left);
    clearNode(node->right);
    delete node;
}

template<typename T>
void AVLTree<T>::inorderTraversal(Node* node, std::ostream& os) const {
    if (!node) return;
    inorderTraversal(node->left, os);
    os << node->data << " ";
    inorderTraversal(node->right, os);
}

template<typename T>
void AVLTree<T>::insert(const T& value) {
    root = insertNode(root, value);
}

template<typename T>
void AVLTree<T>::remove(const T& value) {
    root = deleteNode(root, value);
}

template<typename T>
bool AVLTree<T>::search(const T& value) const {
    return searchNode(root, value);
}

template<typename T>
void AVLTree<T>::clear() {
    clearNode(root);
    root = nullptr;
    count = 0;
}

template<typename T>
size_t AVLTree<T>::size() const {
    return count;
}

template<typename T>
bool AVLTree<T>::empty() const {
    return root == nullptr;
}

template<typename T>
int AVLTree<T>::height() const {
    return getHeight(root);
}

template<typename T>
void AVLTree<T>::print(std::ostream& os) const {
    os << "AVLTree [";
    inorderTraversal(root, os);
    os << "] (size: " << count << ", height: " << height() << ")";
}

template<typename T>
void AVLTree<T>::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Не удалось открыть файл для записи");
    }
    inorderTraversal(root, file);
    file.close();
}

template<typename T>
void AVLTree<T>::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Не удалось открыть файл для чтения");
    }
    clear();
    T value;
    while (file >> value) {
        insert(value);
    }
    file.close();
}

// Бинарная сериализация для AVLTree<string>
template<>
inline void AVLTree<std::string>::saveBinaryNode(std::ofstream& out, Node* node) const {
    if (node == nullptr) {
        uint8_t null_marker = 0;
        out.write(reinterpret_cast<const char*>(&null_marker), sizeof(null_marker));
        return;
    }
    
    uint8_t node_marker = 1;
    out.write(reinterpret_cast<const char*>(&node_marker), sizeof(node_marker));
    
    // Записываем данные узла
    uint32_t strLen = static_cast<uint32_t>(node->data.length());
    out.write(reinterpret_cast<const char*>(&strLen), sizeof(strLen));
    out.write(node->data.c_str(), strLen);
    
    // Рекурсивно записываем детей
    saveBinaryNode(out, node->left);
    saveBinaryNode(out, node->right);
}

template<>
inline AVLTree<std::string>::Node* AVLTree<std::string>::loadBinaryNode(std::ifstream& in) {
    uint8_t marker;
    in.read(reinterpret_cast<char*>(&marker), sizeof(marker));
    
    if (marker == 0) {
        return nullptr;
    }
    
    // Читаем данные узла
    uint32_t strLen;
    in.read(reinterpret_cast<char*>(&strLen), sizeof(strLen));
    std::string data(strLen, '\0');
    in.read(&data[0], strLen);
    
    Node* node = new Node(data);
    node->left = loadBinaryNode(in);
    node->right = loadBinaryNode(in);
    
    updateHeight(node);
    
    return node;
}

template<>
inline void AVLTree<std::string>::saveToBinary(std::ofstream& out) const {
    // Записываем количество элементов
    uint32_t sz = static_cast<uint32_t>(count);
    out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    
    // Записываем дерево
    saveBinaryNode(out, root);
}

template<>
inline void AVLTree<std::string>::loadFromBinary(std::ifstream& in) {
    clear();
    
    // Читаем количество элементов
    uint32_t sz;
    in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    
    count = sz;
    root = loadBinaryNode(in);
}

// Общий шаблон для других типов
template<typename T>
void AVLTree<T>::saveBinaryNode(std::ofstream& out, Node* node) const {
    if (node == nullptr) {
        uint8_t null_marker = 0;
        out.write(reinterpret_cast<const char*>(&null_marker), sizeof(null_marker));
        return;
    }
    
    uint8_t node_marker = 1;
    out.write(reinterpret_cast<const char*>(&node_marker), sizeof(node_marker));
    out.write(reinterpret_cast<const char*>(&node->data), sizeof(T));
    
    saveBinaryNode(out, node->left);
    saveBinaryNode(out, node->right);
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::loadBinaryNode(std::ifstream& in) {
    uint8_t marker;
    in.read(reinterpret_cast<char*>(&marker), sizeof(marker));
    
    if (marker == 0) {
        return nullptr;
    }
    
    T data;
    in.read(reinterpret_cast<char*>(&data), sizeof(T));
    
    Node* node = new Node(data);
    node->left = loadBinaryNode(in);
    node->right = loadBinaryNode(in);
    
    updateHeight(node);
    
    return node;
}

template<typename T>
void AVLTree<T>::saveToBinary(std::ofstream& out) const {
    uint32_t sz = static_cast<uint32_t>(count);
    out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    saveBinaryNode(out, root);
}

template<typename T>
void AVLTree<T>::loadFromBinary(std::ifstream& in) {
    clear();
    uint32_t sz;
    in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    count = sz;
    root = loadBinaryNode(in);
}

#endif
