#ifndef CUCKOO_CPP
#define CUCKOO_CPP

#include "hash.h"

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

#endif
