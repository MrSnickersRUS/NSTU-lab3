#ifndef HASHMAP_CPP
#define HASHMAP_CPP

#include <cstdint>
#include <fstream>
#include "../binary_serialization.h"
#include "hash.h"

template<typename K, typename V>
HashMap<K, V>::Entry::Entry() : occupied(false) {}

template<typename K, typename V>
HashMap<K, V>::Entry::Entry(const K& k, const V& v) : key(k), value(v), occupied(true) {}

template<typename K, typename V>
HashMap<K, V>::HashMap(size_t initialCapacity) 
    : capacity(initialCapacity), count(0) {
    table.resize(capacity);
}

template<typename K, typename V>
size_t HashMap<K, V>::hash1(const K& key) const {
    std::hash<K> hasher;
    return hasher(key) % capacity;
}

template<typename K, typename V>
size_t HashMap<K, V>::hash2(const K& key) const {
    std::hash<K> hasher;
    size_t h = hasher(key) % (capacity - 1);
    return h == 0 ? 1 : h;
}

template<typename K, typename V>
bool HashMap<K, V>::findIndex(const K& key, size_t& index) const {
    size_t h1 = hash1(key);
    size_t h2 = hash2(key);
    index = h1;
    
    for (size_t i = 0; i < capacity; i++) {
        if (!table[index].occupied) {
            return false;
        }
        if (table[index].key == key) {
            return true;
        }
        index = (h1 + (i + 1) * h2) % capacity;
    }
    throw std::runtime_error("Хеш-таблица заполнена");
}

template<typename K, typename V>
void HashMap<K, V>::put(const K& key, const V& value) {
    if (count >= capacity) {
        throw std::runtime_error("Хеш-таблица заполнена");
    }
    size_t index;
    bool found = findIndex(key, index);
    if (found) {
        table[index].value = value;
    } else {
        table[index] = Entry(key, value);
        count++;
    }
}

template<typename K, typename V>
V HashMap<K, V>::get(const K& key) const {
    size_t index;
    if (findIndex(key, index)) {
        return table[index].value;
    }
    throw std::runtime_error("Ключ не найден");
}

template<typename K, typename V>
bool HashMap<K, V>::contains(const K& key) const {
    size_t index;
    return findIndex(key, index);
}

template<typename K, typename V>
bool HashMap<K, V>::remove(const K& key) {
    size_t index;
    if (findIndex(key, index)) {
        table[index].occupied = false;
        count--;
        return true;
    }
    return false;
}

template<typename K, typename V>
void HashMap<K, V>::clear() {
    for (auto& entry : table) {
        entry.occupied = false;
    }
    count = 0;
}

template<typename K, typename V>
size_t HashMap<K, V>::size() const {
    return count;
}

template<typename K, typename V>
bool HashMap<K, V>::empty() const {
    return count == 0;
}

template<typename K, typename V>
void HashMap<K, V>::print(std::ostream& os) const {
    os << "HashMap {\n";
    for (size_t i = 0; i < capacity; i++) {
        if (table[i].occupied) {
            os << "  [" << i << "] " << table[i].key 
               << " => " << table[i].value << "\n";
        }
    }
    os << "} (size: " << count << ")";
}

// Бинарная сериализация для HashMap<string, string>
template<>
inline void HashMap<std::string, std::string>::saveToBinary(std::ofstream& out) const {
    // Записываем количество элементов
    uint32_t sz = static_cast<uint32_t>(count);
    out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    
    // Записываем каждую пару ключ-значение
    for (size_t i = 0; i < capacity; i++) {
        if (table[i].occupied) {
            // Записываем ключ
            uint32_t keyLen = static_cast<uint32_t>(table[i].key.length());
            out.write(reinterpret_cast<const char*>(&keyLen), sizeof(keyLen));
            out.write(table[i].key.c_str(), keyLen);
            
            // Записываем значение
            uint32_t valLen = static_cast<uint32_t>(table[i].value.length());
            out.write(reinterpret_cast<const char*>(&valLen), sizeof(valLen));
            out.write(table[i].value.c_str(), valLen);
        }
    }
}

template<>
inline void HashMap<std::string, std::string>::loadFromBinary(std::ifstream& in) {
    clear();
    
    // Читаем количество элементов
    uint32_t sz;
    in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    
    // Читаем каждую пару
    for (uint32_t i = 0; i < sz; i++) {
        // Читаем ключ
        uint32_t keyLen;
        in.read(reinterpret_cast<char*>(&keyLen), sizeof(keyLen));
        std::string key(keyLen, '\0');
        in.read(&key[0], keyLen);
        
        // Читаем значение
        uint32_t valLen;
        in.read(reinterpret_cast<char*>(&valLen), sizeof(valLen));
        std::string value(valLen, '\0');
        in.read(&value[0], valLen);
        
        put(key, value);
    }
}

// Общий шаблон для других типов
template<typename K, typename V>
void HashMap<K, V>::saveToBinary(std::ofstream& out) const {
    uint32_t sz = static_cast<uint32_t>(count);
    out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    
    for (size_t i = 0; i < capacity; i++) {
        if (table[i].occupied) {
            out.write(reinterpret_cast<const char*>(&table[i].key), sizeof(K));
            out.write(reinterpret_cast<const char*>(&table[i].value), sizeof(V));
        }
    }
}

template<typename K, typename V>
void HashMap<K, V>::loadFromBinary(std::ifstream& in) {
    clear();
    uint32_t sz;
    in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    
    for (uint32_t i = 0; i < sz; i++) {
        K key;
        V value;
        in.read(reinterpret_cast<char*>(&key), sizeof(K));
        in.read(reinterpret_cast<char*>(&value), sizeof(V));
        put(key, value);
    }
}

#endif
