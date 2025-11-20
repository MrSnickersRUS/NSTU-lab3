#ifndef JSON_UTILS
#define JSON_UTILS

#include <string>
#include <fstream>
#include <vector>
#include "../include/json.hpp"

using json = nlohmann::json;

// Простые обертки для работы с JSON
// Сохранение вектора в JSON файл
template<typename T>
void saveVectorToJson(const std::string& filename, const std::vector<T>& vec, const std::string& name = "data") {
    json j;
    j[name] = vec;
    j["size"] = vec.size();
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
    }
    file << j.dump(2);
    file.close();
}

// Загрузка вектора из JSON файла
template<typename T>
std::vector<T> loadVectorFromJson(const std::string& filename, const std::string& name = "data") {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для чтения: " + filename);
    }
    
    json j;
    file >> j;
    file.close();
    
    if (!j.contains(name)) {
        throw std::runtime_error("JSON не содержит поле: " + name);
    }
    
    return j[name].get<std::vector<T>>();
}

// Сохранение объекта (map-like) в JSON
template<typename K, typename V>
void saveMapToJson(const std::string& filename, const std::vector<std::pair<K, V>>& pairs) {
    json j = json::object();
    
    for (const auto& pair : pairs) {
        std::ostringstream keyStr;
        keyStr << pair.first;
        std::ostringstream valStr;
        valStr << pair.second;
        j[keyStr.str()] = valStr.str();
    }
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
    }
    file << j.dump(2);
    file.close();
}

// Загрузка map из JSON
template<typename K, typename V>
std::vector<std::pair<K, V>> loadMapFromJson(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для чтения: " + filename);
    }
    
    json j;
    file >> j;
    file.close();
    
    std::vector<std::pair<K, V>> result;
    
    for (auto it = j.begin(); it != j.end(); ++it) {
        K key;
        V value;
        
        std::istringstream keyStream(it.key());
        keyStream >> key;
        
        std::string valStr = it.value().get<std::string>();
        std::istringstream valStream(valStr);
        valStream >> value;
        
        result.push_back({key, value});
    }
    
    return result;
}

#endif
