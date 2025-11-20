#ifndef SET_CPP
#define SET_CPP

#include "hash.h"

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
