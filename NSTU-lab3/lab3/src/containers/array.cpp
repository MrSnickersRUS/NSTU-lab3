#ifndef ARRAY_CPP
#define ARRAY_CPP

#include <fstream>
#include "../binary_serialization.h"
#include "linear.h"

template<typename T>
Array<T>::Array(size_t initialCapacity) 
    : capacity(initialCapacity), length(0) {
    data = new T[capacity];
}

template<typename T>
Array<T>::~Array() {
    delete[] data;
}

template<typename T>
void Array<T>::resize() {
    capacity *= 2;
    T* newData = new T[capacity];
    for (size_t i = 0; i < length; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
}

template<typename T>
void Array<T>::push(const T& value) {
    if (length >= capacity) {
        resize();
    }
    data[length++] = value;
}

template<typename T>
T Array<T>::get(size_t index) const {
    if (index >= length) {
        throw std::out_of_range("Индекс за пределами массива");
    }
    return data[index];
}

template<typename T>
void Array<T>::insert(size_t index, const T& value) {
    if (index > length) {
        throw std::out_of_range("Индекс за пределами массива");
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

template<typename T>
void Array<T>::remove(size_t index) {
    if (index >= length) {
        throw std::out_of_range("Индекс за пределами массива");
    }
    for (size_t i = index; i < length - 1; i++) {
        data[i] = data[i + 1];
    }
    length--;
}

template<typename T>
void Array<T>::replace(size_t index, const T& value) {
    if (index >= length) {
        throw std::out_of_range("Индекс за пределами массива");
    }
    data[index] = value;
}

template<typename T>
void Array<T>::clear() {
    length = 0;
}

template<typename T>
size_t Array<T>::size() const {
    return length;
}

template<typename T>
bool Array<T>::empty() const {
    return length == 0;
}

template<typename T>
void Array<T>::print(std::ostream& os) const {
    os << "[";
    for (size_t i = 0; i < length; i++) {
        os << data[i];
        if (i < length - 1) os << ", ";
    }
    os << "]";
}

// Бинарная сериализация для Array<string>
template<>
void Array<std::string>::saveToBinary(std::ofstream& out) const {
    // Записываем размер
    uint32_t sz = static_cast<uint32_t>(length);
    out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    
    // Записываем каждый элемент
    for (size_t i = 0; i < length; i++) {
        uint32_t strLen = static_cast<uint32_t>(data[i].length());
        out.write(reinterpret_cast<const char*>(&strLen), sizeof(strLen));
        out.write(data[i].c_str(), strLen);
    }
}

template<>
void Array<std::string>::loadFromBinary(std::ifstream& in) {
    clear();
    
    // Читаем размер
    uint32_t sz;
    in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    
    // Читаем каждый элемент
    for (uint32_t i = 0; i < sz; i++) {
        uint32_t strLen;
        in.read(reinterpret_cast<char*>(&strLen), sizeof(strLen));
        std::string str(strLen, '\0');
        in.read(&str[0], strLen);
        push(str);
    }
}

// Общий шаблон для других типов
template<typename T>
void Array<T>::saveToBinary(std::ofstream& out) const {
    uint32_t sz = static_cast<uint32_t>(length);
    out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    out.write(reinterpret_cast<const char*>(data), length * sizeof(T));
}

template<typename T>
void Array<T>::loadFromBinary(std::ifstream& in) {
    clear();
    uint32_t sz;
    in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    
    for (uint32_t i = 0; i < sz; i++) {
        T value;
        in.read(reinterpret_cast<char*>(&value), sizeof(T));
        push(value);
    }
}

#endif
