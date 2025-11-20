#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <cstddef>
#include <stdexcept>
#include <string>


// Определяет общие операции, которые должны поддерживать все структуры данных
template<typename T>
class Container {
public:
    virtual ~Container() = default;
    
    virtual void clear() = 0;
    
    virtual size_t size() const = 0;
    
    virtual bool empty() const = 0;
};

#endif
