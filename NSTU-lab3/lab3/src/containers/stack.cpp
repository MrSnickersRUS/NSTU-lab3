#ifndef STACK_CPP
#define STACK_CPP

#include <vector>
#include <cstdint>
#include <fstream>
#include "../binary_serialization.h"
#include "linear.h"

template<typename T>
Stack<T>::Node::Node(const T& value) : data(value), next(nullptr) {}

template<typename T>
Stack<T>::Stack() : top(nullptr), length(0) {}

template<typename T>
Stack<T>::~Stack() {
    clear();
}

template<typename T>
void Stack<T>::push(const T& value) {
    Node* newNode = new Node(value);
    newNode->next = top;
    top = newNode;
    length++;
}

template<typename T>
T Stack<T>::pop() {
    if (top == nullptr) {
        throw std::runtime_error("Stack is empty");
    }
    Node* temp = top;
    T value = temp->data;
    top = top->next;
    delete temp;
    length--;
    return value;
}

template<typename T>
T Stack<T>::peek() const {
    if (top == nullptr) {
        throw std::runtime_error("Стек пуст");
    }
    return top->data;
}

template<typename T>
void Stack<T>::clear() {
    while (top != nullptr) {
        Node* temp = top;
        top = top->next;
        delete temp;
    }
    length = 0;
}

template<typename T>
size_t Stack<T>::size() const {
    return length;
}

template<typename T>
bool Stack<T>::empty() const {
    return top == nullptr;
}

template<typename T>
void Stack<T>::print(std::ostream& os) const {
    Node* current = top;
    os << "[TOP ";
    while (current != nullptr) {
        os << current->data;
        if (current->next != nullptr) os << " | ";
        current = current->next;
    }
    os << "]";
}

// Бинарная сериализация для Stack<string>
template<>
void Stack<std::string>::saveToBinary(std::ofstream& out) const {
    // Записываем размер
    uint32_t sz = static_cast<uint32_t>(length);
    out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    
    // Нужно сохранить элементы в порядке от дна к вершине
    std::vector<std::string> elements;
    Node* current = top;
    while (current != nullptr) {
        elements.push_back(current->data);
        current = current->next;
    }
    
    // Записываем в обратном порядке (от дна к вершине)
    for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
        uint32_t strLen = static_cast<uint32_t>(it->length());
        out.write(reinterpret_cast<const char*>(&strLen), sizeof(strLen));
        out.write(it->c_str(), strLen);
    }
}

template<>
void Stack<std::string>::loadFromBinary(std::ifstream& in) {
    clear();
    
    // Читаем размер
    uint32_t sz;
    in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    
    // Читаем элементы (они сохранены от дна к вершине)
    std::vector<std::string> elements;
    for (uint32_t i = 0; i < sz; i++) {
        uint32_t strLen;
        in.read(reinterpret_cast<char*>(&strLen), sizeof(strLen));
        std::string str(strLen, '\0');
        in.read(&str[0], strLen);
        elements.push_back(str);
    }
    
    // Пушим в стек (последний стал первым)
    for (const auto& elem : elements) {
        push(elem);
    }
}

// Общий шаблон
template<typename T>
void Stack<T>::saveToBinary(std::ofstream& out) const {
    uint32_t sz = static_cast<uint32_t>(length);
    out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    
    std::vector<T> elements;
    Node* current = top;
    while (current != nullptr) {
        elements.push_back(current->data);
        current = current->next;
    }
    
    for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
        out.write(reinterpret_cast<const char*>(&(*it)), sizeof(T));
    }
}

template<typename T>
void Stack<T>::loadFromBinary(std::ifstream& in) {
    clear();
    uint32_t sz;
    in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    
    std::vector<T> elements;
    for (uint32_t i = 0; i < sz; i++) {
        T value;
        in.read(reinterpret_cast<char*>(&value), sizeof(T));
        elements.push_back(value);
    }
    
    for (const auto& elem : elements) {
        push(elem);
    }
}

#endif
