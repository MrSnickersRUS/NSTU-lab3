#ifndef HASH_H
#define HASH_H

#include "containers.h"
#include <iostream>
#include <functional>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <fstream>
#include "../binary_serialization.h"

template<typename K, typename V>
class HashMap {
private:
    static const size_t DEFAULT_CAPACITY = 101;
    struct Entry {
        K key;
        V value;
        bool occupied;
        Entry();
        Entry(const K& k, const V& v);
    };
    std::vector<Entry> table;
    size_t capacity;
    size_t count;
    
    size_t hash1(const K& key) const;
    size_t hash2(const K& key) const;
    bool findIndex(const K& key, size_t& index) const;

public:
    explicit HashMap(size_t initialCapacity = DEFAULT_CAPACITY);
    
    HashMap(const HashMap&) = delete;
    HashMap& operator=(const HashMap&) = delete;
    
    HashMap(HashMap&& other) noexcept 
        : table(std::move(other.table)), capacity(other.capacity), count(other.count) {
        other.capacity = 0;
        other.count = 0;
    }
    
    HashMap& operator=(HashMap&& other) noexcept {
        if (this != &other) {
            table = std::move(other.table);
            capacity = other.capacity;
            count = other.count;
            other.capacity = 0;
            other.count = 0;
        }
        return *this;
    }
    
    void put(const K& key, const V& value);
    V get(const K& key) const;
    bool contains(const K& key) const;
    bool remove(const K& key);
    void clear();
    size_t size() const;
    bool empty() const;
    void print(std::ostream& os = std::cout) const;
    
    // Бинарная сериализация
    void saveToBinary(std::ofstream& out) const;
    void loadFromBinary(std::ifstream& in);
};

template<typename K, typename V>
class CuckooHashMap {
private:
    static const size_t DEFAULT_CAPACITY = 101;
    static const int MAX_ITERATIONS = 500;
    
    struct Entry {
        K key;
        V value;
        bool occupied;
        Entry();
        Entry(const K& k, const V& v);
    };
    
    std::vector<Entry> table1;
    std::vector<Entry> table2;
    size_t capacity;
    size_t count;
    
    size_t hash1(const K& key) const;
    size_t hash2(const K& key) const;
    bool insertInternal(const K& key, const V& value, int iterations);

public:
    explicit CuckooHashMap(size_t initialCapacity = DEFAULT_CAPACITY);
    void put(const K& key, const V& value);
    V get(const K& key) const;
    bool contains(const K& key) const;
    bool remove(const K& key);
    void clear();
    size_t size() const;
    bool empty() const;
    void print(std::ostream& os = std::cout) const;
};

template<typename T>
class Set {
private:
    HashMap<T, bool> map;
    
public:
    explicit Set(size_t initialCapacity = 101);
    void add(const T& value);
    bool contains(const T& value) const;
    bool remove(const T& value);
    void clear();
    size_t size() const;
    bool empty() const;
    void print(std::ostream& os = std::cout) const;
};

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

// Специализация для std::string
template<>
inline void HashMap<std::string, std::string>::saveToBinary(std::ofstream& out) const {
    uint32_t sz = static_cast<uint32_t>(count);
    out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    
    for (size_t i = 0; i < capacity; i++) {
        if (table[i].occupied) {
            uint32_t keyLen = static_cast<uint32_t>(table[i].key.length());
            out.write(reinterpret_cast<const char*>(&keyLen), sizeof(keyLen));
            out.write(table[i].key.c_str(), keyLen);
            
            uint32_t valLen = static_cast<uint32_t>(table[i].value.length());
            out.write(reinterpret_cast<const char*>(&valLen), sizeof(valLen));
            out.write(table[i].value.c_str(), valLen);
        }
    }
}

template<>
inline void HashMap<std::string, std::string>::loadFromBinary(std::ifstream& in) {
    clear();
    uint32_t sz;
    in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    
    for (uint32_t i = 0; i < sz; i++) {
        uint32_t keyLen;
        in.read(reinterpret_cast<char*>(&keyLen), sizeof(keyLen));
        std::string key(keyLen, '\0');
        in.read(&key[0], keyLen);
        
        uint32_t valLen;
        in.read(reinterpret_cast<char*>(&valLen), sizeof(valLen));
        std::string value(valLen, '\0');
        in.read(&value[0], valLen);
        
        put(key, value);
    }
}

// Реализация CuckooHashMap
template<typename K, typename V>
CuckooHashMap<K, V>::Entry::Entry() : occupied(false) {}

template<typename K, typename V>
CuckooHashMap<K, V>::Entry::Entry(const K& k, const V& v) : key(k), value(v), occupied(true) {}

template<typename K, typename V>
CuckooHashMap<K, V>::CuckooHashMap(size_t initialCapacity) 
    : capacity(initialCapacity), count(0) {
    table1.resize(capacity);
    table2.resize(capacity);
}

template<typename K, typename V>
size_t CuckooHashMap<K, V>::hash1(const K& key) const {
    std::hash<K> hasher;
    return hasher(key) % capacity;
}

template<typename K, typename V>
size_t CuckooHashMap<K, V>::hash2(const K& key) const {
    std::hash<K> hasher;
    return (hasher(key) / capacity) % capacity;
}

template<typename K, typename V>
bool CuckooHashMap<K, V>::insertInternal(const K& key, const V& value, int iterations) {
    if (iterations >= MAX_ITERATIONS) {
        throw std::runtime_error("CuckooHashMap: слишком много итераций, требуется рехеширование");
    }
    
    size_t idx1 = hash1(key);
    
    if (!table1[idx1].occupied) {
        table1[idx1] = Entry(key, value);
        return true;
    }
    
    if (table1[idx1].key == key) {
        table1[idx1].value = value;
        return true;
    }
    
    K oldKey = table1[idx1].key;
    V oldValue = table1[idx1].value;
    table1[idx1] = Entry(key, value);
    
    size_t idx2 = hash2(oldKey);
    
    if (!table2[idx2].occupied) {
        table2[idx2] = Entry(oldKey, oldValue);
        return true;
    }
    
    if (table2[idx2].key == oldKey) {
        table2[idx2].value = oldValue;
        return true;
    }
    
    K nextKey = table2[idx2].key;
    V nextValue = table2[idx2].value;
    table2[idx2] = Entry(oldKey, oldValue);
    
    return insertInternal(nextKey, nextValue, iterations + 1);
}

template<typename K, typename V>
void CuckooHashMap<K, V>::put(const K& key, const V& value) {
    if (count >= capacity * 2) {
        throw std::runtime_error("CuckooHashMap заполнена");
    }
    
    if (contains(key)) {
        size_t idx1 = hash1(key);
        size_t idx2 = hash2(key);
        
        if (table1[idx1].occupied && table1[idx1].key == key) {
            table1[idx1].value = value;
        } else if (table2[idx2].occupied && table2[idx2].key == key) {
            table2[idx2].value = value;
        }
        return;
    }
    
    if (insertInternal(key, value, 0)) {
        count++;
    }
}

template<typename K, typename V>
V CuckooHashMap<K, V>::get(const K& key) const {
    size_t idx1 = hash1(key);
    size_t idx2 = hash2(key);
    
    if (table1[idx1].occupied && table1[idx1].key == key) {
        return table1[idx1].value;
    }
    if (table2[idx2].occupied && table2[idx2].key == key) {
        return table2[idx2].value;
    }
    throw std::runtime_error("Ключ не найден");
}

template<typename K, typename V>
bool CuckooHashMap<K, V>::contains(const K& key) const {
    size_t idx1 = hash1(key);
    size_t idx2 = hash2(key);
    
    return (table1[idx1].occupied && table1[idx1].key == key) ||
           (table2[idx2].occupied && table2[idx2].key == key);
}

template<typename K, typename V>
bool CuckooHashMap<K, V>::remove(const K& key) {
    size_t idx1 = hash1(key);
    size_t idx2 = hash2(key);
    
    if (table1[idx1].occupied && table1[idx1].key == key) {
        table1[idx1].occupied = false;
        count--;
        return true;
    }
    if (table2[idx2].occupied && table2[idx2].key == key) {
        table2[idx2].occupied = false;
        count--;
        return true;
    }
    return false;
}

template<typename K, typename V>
void CuckooHashMap<K, V>::clear() {
    for (auto& entry : table1) {
        entry.occupied = false;
    }
    for (auto& entry : table2) {
        entry.occupied = false;
    }
    count = 0;
}

template<typename K, typename V>
size_t CuckooHashMap<K, V>::size() const {
    return count;
}

template<typename K, typename V>
bool CuckooHashMap<K, V>::empty() const {
    return count == 0;
}

template<typename K, typename V>
void CuckooHashMap<K, V>::print(std::ostream& os) const {
    os << "CuckooHashMap {\n  Table 1:\n";
    for (size_t i = 0; i < capacity; i++) {
        if (table1[i].occupied) {
            os << "    [" << i << "] " << table1[i].key << " => " << table1[i].value << "\n";
        }
    }
    os << "  Table 2:\n";
    for (size_t i = 0; i < capacity; i++) {
        if (table2[i].occupied) {
            os << "    [" << i << "] " << table2[i].key << " => " << table2[i].value << "\n";
        }
    }
    os << "} (size: " << count << ")";
}

// Реализация Set

template<typename T>
Set<T>::Set(size_t initialCapacity) : map(initialCapacity) {}

template<typename T>
void Set<T>::add(const T& value) {
    map.put(value, true);
}

template<typename T>
bool Set<T>::contains(const T& value) const {
    return map.contains(value);
}

template<typename T>
bool Set<T>::remove(const T& value) {
    return map.remove(value);
}

template<typename T>
void Set<T>::clear() {
    map.clear();
}

template<typename T>
size_t Set<T>::size() const {
    return map.size();
}

template<typename T>
bool Set<T>::empty() const {
    return map.empty();
}

template<typename T>
void Set<T>::print(std::ostream& os) const {
    os << "Set { size: " << size() << " }";
}

#endif
