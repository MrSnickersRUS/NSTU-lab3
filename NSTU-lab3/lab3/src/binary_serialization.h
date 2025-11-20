#ifndef BINARY_SERIALIZATION_H
#define BINARY_SERIALIZATION_H

#include <fstream>
#include <string>
#include <cstdint>
#include <vector>

// Утилиты для бинарной сериализации

// Запись примитивных типов
inline void writeBinary(std::ofstream& out, uint32_t value) {
    out.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

inline void writeBinary(std::ofstream& out, size_t value) {
    uint32_t val32 = static_cast<uint32_t>(value);
    out.write(reinterpret_cast<const char*>(&val32), sizeof(val32));
}

inline void writeBinary(std::ofstream& out, bool value) {
    uint8_t val = value ? 1 : 0;
    out.write(reinterpret_cast<const char*>(&val), sizeof(val));
}

// Запись строки: длина (4 байта) + данные
inline void writeBinary(std::ofstream& out, const std::string& str) {
    uint32_t length = static_cast<uint32_t>(str.length());
    out.write(reinterpret_cast<const char*>(&length), sizeof(length));
    out.write(str.c_str(), length);
}

// Чтение примитивных типов
inline uint32_t readUint32(std::ifstream& in) {
    uint32_t value;
    in.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

inline size_t readSize(std::ifstream& in) {
    uint32_t value;
    in.read(reinterpret_cast<char*>(&value), sizeof(value));
    return static_cast<size_t>(value);
}

inline bool readBool(std::ifstream& in) {
    uint8_t value;
    in.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value != 0;
}

// Чтение строки
inline std::string readString(std::ifstream& in) {
    uint32_t length = readUint32(in);
    std::string str(length, '\0');
    in.read(&str[0], length);
    return str;
}

#endif