#ifndef LINEAR_H
#define LINEAR_H

#include "containers.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <cstdint>
#include <vector>

template<typename T>
class Array : public Container<T> {
private:
    T* data;
    size_t capacity;
    size_t length;
    
    void resize() {
        capacity *= 2;
        T* newData = new T[capacity];
        for (size_t i = 0; i < length; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }
    
public:
    explicit Array(size_t initialCapacity = 10) 
        : capacity(initialCapacity), length(0) {
        data = new T[capacity];
    }
    
    ~Array() override {
        delete[] data;
    }
    
    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;
    
    Array(Array&& other) noexcept : data(other.data), capacity(other.capacity), length(other.length) {
        other.data = nullptr;
        other.capacity = 0;
        other.length = 0;
    }
    
    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            capacity = other.capacity;
            length = other.length;
            other.data = nullptr;
            other.capacity = 0;
            other.length = 0;
        }
        return *this;
    }
    
    void push(const T& value) {
        if (length >= capacity) {
            resize();
        }
        data[length++] = value;
    }
    
    T get(size_t index) const {
        if (index >= length) {
            throw std::out_of_range("Индекс массива выходит за границы");
        }
        return data[index];
    }
    
    void insert(size_t index, const T& value) {
        if (index > length) {
            throw std::out_of_range("Индекс массива выходит за границы");
        }
        if (length >= capacity) {
            resize();
        }
        for (size_t i = length; i > index; i--) {
            data[i] = data[i - 1];
        }
        data[index] = value;
        length++;
    }
    
    void remove(size_t index) {
        if (index >= length) {
            throw std::out_of_range("Индекс массива выходит за границы");
        }
        for (size_t i = index; i < length - 1; i++) {
            data[i] = data[i + 1];
        }
        length--;
    }
    
    void replace(size_t index, const T& value) {
        if (index >= length) {
            throw std::out_of_range("Индекс массива выходит за границы");
        }
        data[index] = value;
    }
    
    void clear() override {
        length = 0;
    }
    
    size_t size() const override {
        return length;
    }
    
    bool empty() const override {
        return length == 0;
    }
    
    void print(std::ostream& os = std::cout) const {
        os << "[";
        for (size_t i = 0; i < length; i++) {
            os << data[i];
            if (i < length - 1) os << ", ";
        }
        os << "]";
    }
    
    // Бинарная сериализация - общий шаблон для POD типов
    void saveToBinary(std::ofstream& out) const {
        uint32_t sz = static_cast<uint32_t>(length);
        out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
        out.write(reinterpret_cast<const char*>(data), length * sizeof(T));
    }
    
    void loadFromBinary(std::ifstream& in) {
        clear();
        uint32_t sz;
        in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
        
        for (uint32_t i = 0; i < sz; i++) {
            T value;
            in.read(reinterpret_cast<char*>(&value), sizeof(T));
            push(value);
        }
    }
};

// Специализация для std::string
template<>
inline void Array<std::string>::saveToBinary(std::ofstream& out) const {
    uint32_t sz = static_cast<uint32_t>(length);
    out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    
    for (size_t i = 0; i < length; i++) {
        uint32_t strLen = static_cast<uint32_t>(data[i].length());
        out.write(reinterpret_cast<const char*>(&strLen), sizeof(strLen));
        out.write(data[i].c_str(), strLen);
    }
}

template<>
inline void Array<std::string>::loadFromBinary(std::ifstream& in) {
    clear();
    uint32_t sz;
    in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    
    for (uint32_t i = 0; i < sz; i++) {
        uint32_t strLen;
        in.read(reinterpret_cast<char*>(&strLen), sizeof(strLen));
        std::string str(strLen, '\0');
        in.read(&str[0], strLen);
        push(str);
    }
}

template<typename T>
class SingleList : public Container<T> {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value);
    };
    Node* head;
    size_t length;
public:
    SingleList();
    ~SingleList() override;
    void addHead(const T& value);
    void addTail(const T& value);
    bool remove(const T& value);
    bool contains(const T& value) const;
    void clear() override;
    size_t size() const override;
    bool empty() const override;
    void print(std::ostream& os = std::cout) const;
};

template<typename T>
class DoubleList : public Container<T> {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        Node(const T& value);
    };
    Node* head;
    Node* tail;
    size_t length;
public:
    DoubleList();
    ~DoubleList() override;
    void addHead(const T& value);
    void addTail(const T& value);
    bool remove(const T& value);
    void clear() override;
    size_t size() const override;
    bool empty() const override;
    void print(std::ostream& os = std::cout) const;
};

template<typename T>
class Stack : public Container<T> {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };
    Node* top;
    size_t length;
public:
    Stack() : top(nullptr), length(0) {}
    
    ~Stack() override {
        clear();
    }
    
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;
    
    Stack(Stack&& other) noexcept : top(other.top), length(other.length) {
        other.top = nullptr;
        other.length = 0;
    }
    
    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            clear();
            top = other.top;
            length = other.length;
            other.top = nullptr;
            other.length = 0;
        }
        return *this;
    }
    
    void push(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
        length++;
    }
    
    T pop() {
        if (top == nullptr) {
            throw std::runtime_error("Стек пуст");
        }
        Node* temp = top;
        T value = temp->data;
        top = top->next;
        delete temp;
        length--;
        return value;
    }
    
    T peek() const {
        if (top == nullptr) {
            throw std::runtime_error("Стек пуст");
        }
        return top->data;
    }
    
    void clear() override {
        while (top != nullptr) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
        length = 0;
    }
    
    size_t size() const override {
        return length;
    }
    
    bool empty() const override {
        return top == nullptr;
    }
    
    void print(std::ostream& os = std::cout) const {
        Node* current = top;
        os << "[TOP ";
        while (current != nullptr) {
            os << current->data;
            if (current->next != nullptr) os << " | ";
            current = current->next;
        }
        os << "]";
    }
    
    // Бинарная сериализация - общий шаблон для POD типов
    void saveToBinary(std::ofstream& out) const {
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
    
    void loadFromBinary(std::ifstream& in) {
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
};

// Специализация Stack для std::string
template<>
inline void Stack<std::string>::saveToBinary(std::ofstream& out) const {
    uint32_t sz = static_cast<uint32_t>(length);
    out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    
    std::vector<std::string> elements;
    Node* current = top;
    while (current != nullptr) {
        elements.push_back(current->data);
        current = current->next;
    }
    
    for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
        uint32_t strLen = static_cast<uint32_t>(it->length());
        out.write(reinterpret_cast<const char*>(&strLen), sizeof(strLen));
        out.write(it->c_str(), strLen);
    }
}

template<>
inline void Stack<std::string>::loadFromBinary(std::ifstream& in) {
    clear();
    uint32_t sz;
    in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    
    std::vector<std::string> elements;
    for (uint32_t i = 0; i < sz; i++) {
        uint32_t strLen;
        in.read(reinterpret_cast<char*>(&strLen), sizeof(strLen));
        std::string str(strLen, '\0');
        in.read(&str[0], strLen);
        elements.push_back(str);
    }
    
    for (const auto& elem : elements) {
        push(elem);
    }
}

template<typename T>
class Queue : public Container<T> {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };
    Node* front;
    Node* rear;
    size_t length;
public:
    Queue() : front(nullptr), rear(nullptr), length(0) {}
    
    ~Queue() override {
        clear();
    }
    
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;
    
    Queue(Queue&& other) noexcept : front(other.front), rear(other.rear), length(other.length) {
        other.front = nullptr;
        other.rear = nullptr;
        other.length = 0;
    }
    
    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            clear();
            front = other.front;
            rear = other.rear;
            length = other.length;
            other.front = nullptr;
            other.rear = nullptr;
            other.length = 0;
        }
        return *this;
    }
    
    void enqueue(const T& value) {
        Node* newNode = new Node(value);
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        length++;
    }
    
    T dequeue() {
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
    
    T peek() const {
        if (front == nullptr) {
            throw std::runtime_error("Очередь пуста");
        }
        return front->data;
    }
    
    void clear() override {
        while (front != nullptr) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
        rear = nullptr;
        length = 0;
    }
    
    size_t size() const override {
        return length;
    }
    
    bool empty() const override {
        return front == nullptr;
    }
    
    void print(std::ostream& os = std::cout) const {
        Node* current = front;
        os << "[FRONT ";
        while (current != nullptr) {
            os << current->data;
            if (current->next != nullptr) os << " <- ";
            current = current->next;
        }
        os << " REAR]";
    }
    
    // Бинарная сериализация - общий шаблон для POD типов
    void saveToBinary(std::ofstream& out) const {
        uint32_t sz = static_cast<uint32_t>(length);
        out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
        
        Node* current = front;
        while (current != nullptr) {
            out.write(reinterpret_cast<const char*>(&current->data), sizeof(T));
            current = current->next;
        }
    }
    
    void loadFromBinary(std::ifstream& in) {
        clear();
        uint32_t sz;
        in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
        
        for (uint32_t i = 0; i < sz; i++) {
            T value;
            in.read(reinterpret_cast<char*>(&value), sizeof(T));
            enqueue(value);
        }
    }
};

// Специализация Queue для std::string
template<>
inline void Queue<std::string>::saveToBinary(std::ofstream& out) const {
    uint32_t sz = static_cast<uint32_t>(length);
    out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    
    Node* current = front;
    while (current != nullptr) {
        uint32_t strLen = static_cast<uint32_t>(current->data.length());
        out.write(reinterpret_cast<const char*>(&strLen), sizeof(strLen));
        out.write(current->data.c_str(), strLen);
        current = current->next;
    }
}

template<>
inline void Queue<std::string>::loadFromBinary(std::ifstream& in) {
    clear();
    uint32_t sz;
    in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    
    for (uint32_t i = 0; i < sz; i++) {
        uint32_t strLen;
        in.read(reinterpret_cast<char*>(&strLen), sizeof(strLen));
        std::string str(strLen, '\0');
        in.read(&str[0], strLen);
        enqueue(str);
    }
}

#include "singlelist.cpp"
#include "doublelist.cpp"

#endif
