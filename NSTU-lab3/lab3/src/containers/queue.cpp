#ifndef QUEUE_CPP
#define QUEUE_CPP

#include <cstdint>
#include <fstream>
#include "../binary_serialization.h"
#include "linear.h"

template<typename T>
Queue<T>::Node::Node(const T& value) : data(value), next(nullptr) {}

template<typename T>
Queue<T>::Queue() : front(nullptr), rear(nullptr), length(0) {}

template<typename T>
Queue<T>::~Queue() {
    clear();
}

template<typename T>
void Queue<T>::enqueue(const T& value) {
    Node* newNode = new Node(value);
    if (rear == nullptr) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    length++;
}

template<typename T>
T Queue<T>::dequeue() {
    if (front == nullptr) {
        throw std::runtime_error("Очередь пуста");
    }
    Node* temp = front;
    T value = temp->data;
    front = front->next;
    if (front == nullptr) {
        rear = nullptr;
    }
    delete temp;
    length--;
    return value;
}

template<typename T>
T Queue<T>::peek() const {
    if (front == nullptr) {
        throw std::runtime_error("Очередь пуста");
    }
    return front->data;
}

template<typename T>
void Queue<T>::clear() {
    while (front != nullptr) {
        Node* temp = front;
        front = front->next;
        delete temp;
    }
    rear = nullptr;
    length = 0;
}

template<typename T>
size_t Queue<T>::size() const {
    return length;
}

template<typename T>
bool Queue<T>::empty() const {
    return front == nullptr;
}

template<typename T>
void Queue<T>::print(std::ostream& os) const {
    Node* current = front;
    os << "[ПЕРЕД ";
    while (current != nullptr) {
        os << current->data;
        if (current->next != nullptr) os << " <- ";
        current = current->next;
    }
    os << " ЗАД]";
}

// Бинарная сериализация для Queue<string>
template<>
void Queue<std::string>::saveToBinary(std::ofstream& out) const {
    // Записываем размер
    uint32_t sz = static_cast<uint32_t>(length);
    out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    
    // Записываем элементы от front к rear
    Node* current = front;
    while (current != nullptr) {
        uint32_t strLen = static_cast<uint32_t>(current->data.length());
        out.write(reinterpret_cast<const char*>(&strLen), sizeof(strLen));
        out.write(current->data.c_str(), strLen);
        current = current->next;
    }
}

template<>
void Queue<std::string>::loadFromBinary(std::ifstream& in) {
    clear();
    
    // Читаем размер
    uint32_t sz;
    in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    
    // Читаем элементы и добавляем в очередь
    for (uint32_t i = 0; i < sz; i++) {
        uint32_t strLen;
        in.read(reinterpret_cast<char*>(&strLen), sizeof(strLen));
        std::string str(strLen, '\0');
        in.read(&str[0], strLen);
        enqueue(str);
    }
}

// Общий шаблон
template<typename T>
void Queue<T>::saveToBinary(std::ofstream& out) const {
    uint32_t sz = static_cast<uint32_t>(length);
    out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    
    Node* current = front;
    while (current != nullptr) {
        out.write(reinterpret_cast<const char*>(&current->data), sizeof(T));
        current = current->next;
    }
}

template<typename T>
void Queue<T>::loadFromBinary(std::ifstream& in) {
    clear();
    uint32_t sz;
    in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    
    for (uint32_t i = 0; i < sz; i++) {
        T value;
        in.read(reinterpret_cast<char*>(&value), sizeof(T));
        enqueue(value);
    }
}

#endif
