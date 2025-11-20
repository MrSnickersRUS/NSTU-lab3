#include "commands.h"
#include "json_utils.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

// Глобальные хранилища
map<string, Array<string>> arrays;
map<string, SingleList<string>> singleLists;
map<string, DoubleList<string>> doubleLists;
map<string, Stack<string>> stacks;
map<string, Queue<string>> queues;
map<string, HashMap<string, string>> hashmaps;
map<string, Set<string>> sets;
map<string, AVLTree<string>> trees;

// Парсинг типа контейнера
ContainerType parseContainerType(const string& type) {
    if (type == "ARRAY" || type == "M") return ARRAY;
    if (type == "SLIST" || type == "F") return SINGLE_LIST;
    if (type == "DLIST" || type == "L") return DOUBLE_LIST;
    if (type == "STACK" || type == "S") return STACK;
    if (type == "QUEUE" || type == "Q") return QUEUE;
    if (type == "HASHMAP" || type == "H") return HASHMAP;
    if (type == "SET" || type == "E") return SET;
    if (type == "TREE" || type == "T") return AVLTREE;
    throw runtime_error("Неизвестный тип контейнера: " + type);
}

// Сохранение всех контейнеров в JSON
void saveAllContainers(const string& filePath) {
    json j;
    
    // Сохраняем массивы
    for (const auto& [name, arr] : arrays) {
        vector<string> vec;
        for (size_t i = 0; i < arr.size(); i++) {
            vec.push_back(arr.get(i));
        }
        j["arrays"][name] = vec;
    }
    
    // Сохраняем стеки
    for (const auto& [name, stack] : stacks) {
        (void)stack; // unused
        j["stacks"][name] = json::array();
    }
    
    // Сохраняем очереди
    for (const auto& [name, queue] : queues) {
        (void)queue; // unused
        j["queues"][name] = json::array();
    }
    
    // Сохраняем HashMaps
    for (const auto& [name, map] : hashmaps) {
        j["hashmaps"][name] = json::object();
        // HashMap не имеет итератора, поэтому пока сохраняем пустой объект
    }
    
    for (const auto& [name, tree] : trees) {
        j["trees"][name] = json::array();
    }
    
    // Записываем в файл
    ofstream outFile(filePath);
    if (!outFile) {
        throw runtime_error("Не удалось открыть файл для записи: " + filePath);
    }
    outFile << j.dump(2);
    outFile.close();
    
    cout << "✓ Все контейнеры сохранены в " << filePath << endl;
}

// Загрузка всех контейнеров из JSON
void loadAllContainers(const string& filePath) {
    ifstream inFile(filePath);
    if (!inFile) {
        throw runtime_error("Не удалось открыть файл для чтения: " + filePath);
    }
    
    json j;
    inFile >> j;
    inFile.close();
    
    arrays.clear();
    stacks.clear();
    queues.clear();
    hashmaps.clear();
    trees.clear();
    
    // Загружаем массивы
    if (j.contains("arrays")) {
        for (auto it = j["arrays"].begin(); it != j["arrays"].end(); ++it) {
            string name = it.key();
            arrays.emplace(std::piecewise_construct,
                          std::forward_as_tuple(name),
                          std::forward_as_tuple());
            
            for (const auto& val : it.value()) {
                arrays.at(name).push(val.get<string>());
            }
        }
    }
    
    // Загружаем стеки
    if (j.contains("stacks")) {
        for (auto it = j["stacks"].begin(); it != j["stacks"].end(); ++it) {
            string name = it.key();
            stacks.emplace(std::piecewise_construct,
                          std::forward_as_tuple(name),
                          std::forward_as_tuple());
            
            for (const auto& val : it.value()) {
                stacks.at(name).push(val.get<string>());
            }
        }
    }
    
    // Загружаем очереди
    if (j.contains("queues")) {
        for (auto it = j["queues"].begin(); it != j["queues"].end(); ++it) {
            string name = it.key();
            queues.emplace(std::piecewise_construct,
                          std::forward_as_tuple(name),
                          std::forward_as_tuple());
            
            for (const auto& val : it.value()) {
                queues.at(name).enqueue(val.get<string>());
            }
        }
    }
    
    cout << "✓ Контейнеры загружены из " << filePath << endl;
}

// Сохранение в бинарный формат
void saveAllContainersBinary(const string& filePath) {
    ofstream out(filePath, ios::binary);
    if (!out) {
        throw runtime_error("Не удалось открыть файл для бинарной записи: " + filePath);
    }
    
    // Записываем количество каждого типа контейнеров
    uint32_t count;
    
    // Arrays
    count = static_cast<uint32_t>(arrays.size());
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (const auto& [name, arr] : arrays) {
        // Записываем имя
        uint32_t nameLen = static_cast<uint32_t>(name.length());
        out.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
        out.write(name.c_str(), nameLen);
        // Записываем данные
        arr.saveToBinary(out);
    }
    
    // Stacks
    count = static_cast<uint32_t>(stacks.size());
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (const auto& [name, stack] : stacks) {
        uint32_t nameLen = static_cast<uint32_t>(name.length());
        out.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
        out.write(name.c_str(), nameLen);
        stack.saveToBinary(out);
    }
    
    // Queues
    count = static_cast<uint32_t>(queues.size());
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (const auto& [name, queue] : queues) {
        uint32_t nameLen = static_cast<uint32_t>(name.length());
        out.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
        out.write(name.c_str(), nameLen);
        queue.saveToBinary(out);
    }
    
    // HashMaps
    count = static_cast<uint32_t>(hashmaps.size());
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (const auto& [name, map] : hashmaps) {
        uint32_t nameLen = static_cast<uint32_t>(name.length());
        out.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
        out.write(name.c_str(), nameLen);
        map.saveToBinary(out);
    }
    
    // Trees
    count = static_cast<uint32_t>(trees.size());
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (const auto& [name, tree] : trees) {
        uint32_t nameLen = static_cast<uint32_t>(name.length());
        out.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
        out.write(name.c_str(), nameLen);
        tree.saveToBinary(out);
    }
    
    out.close();
    cout << "✓ Все контейнеры сохранены в бинарный файл " << filePath << endl;
}

// Загрузка из бинарного формата
void loadAllContainersBinary(const string& filePath) {
    ifstream in(filePath, ios::binary);
    if (!in) {
        throw runtime_error("Не удалось открыть файл для бинарного чтения: " + filePath);
    }
    
    // Очищаем все контейнеры
    arrays.clear();
    stacks.clear();
    queues.clear();
    hashmaps.clear();
    trees.clear();
    
    uint32_t count;
    
    // Массивы
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    for (uint32_t i = 0; i < count; i++) {
        uint32_t nameLen;
        in.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        string name(nameLen, '\0');
        in.read(&name[0], nameLen);
        
        Array<string> arr;
        arr.loadFromBinary(in);
        arrays.emplace(name, std::move(arr));
    }
    
    // Стеки
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    for (uint32_t i = 0; i < count; i++) {
        uint32_t nameLen;
        in.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        string name(nameLen, '\0');
        in.read(&name[0], nameLen);
        
        Stack<string> stack;
        stack.loadFromBinary(in);
        stacks.emplace(name, std::move(stack));
    }
    
    // Очереди
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    for (uint32_t i = 0; i < count; i++) {
        uint32_t nameLen;
        in.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        string name(nameLen, '\0');
        in.read(&name[0], nameLen);
        
        Queue<string> queue;
        queue.loadFromBinary(in);
        queues.emplace(name, std::move(queue));
    }
    
    // Хэш-таблицы
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    for (uint32_t i = 0; i < count; i++) {
        uint32_t nameLen;
        in.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        string name(nameLen, '\0');
        in.read(&name[0], nameLen);
        
        HashMap<string, string> map;
        map.loadFromBinary(in);
        hashmaps.emplace(name, std::move(map));
    }
    
    // Деревья
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    for (uint32_t i = 0; i < count; i++) {
        uint32_t nameLen;
        in.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        string name(nameLen, '\0');
        in.read(&name[0], nameLen);
        
        AVLTree<string> tree;
        tree.loadFromBinary(in);
        trees.emplace(name, std::move(tree));
    }
    
    in.close();
    cout << "✓ Контейнеры загружены из бинарного файла " << filePath << endl;
}

// Парсинг команды нового формата: MPUSH, SPOP, QPEEK, HPUT и т.д.
struct ParsedCommand {
    char containerPrefix;     // M, S, Q, H, T, E, F, L
    string operation;         // PUSH, POP, GET и т.д.
    string containerName;
    vector<string> args;
    bool isValid = false;
};

ParsedCommand parseNewFormat(const string& cmd, istringstream& iss) {
    ParsedCommand result;
    
    if (cmd.length() < 2) {
        return result;
    }
    
    // Первый символ - префикс контейнера
    result.containerPrefix = cmd[0];
    
    // Остальное - операция
    result.operation = cmd.substr(1);
    
    // Читаем имя контейнера
    iss >> result.containerName;
    
    // Читаем остальные аргументы
    string arg;
    while (iss >> arg) {
        result.args.push_back(arg);
    }
    
    result.isValid = !result.containerName.empty();
    return result;
}

// Обработка команд
void processCommand(const string& command) {
    istringstream iss(command);
    string cmd;
    iss >> cmd;
    
    if (cmd == "HELP") {
        printHelp();
        return;
    }
    
    if (cmd == "SAVE") {
        string filename;
        iss >> filename;
        if (filename.empty()) filename = "containers.json";
        saveAllContainers(filename);
        return;
    }
    
    if (cmd == "LOAD") {
        string filename;
        iss >> filename;
        if (filename.empty()) filename = "containers.json";
        loadAllContainers(filename);
        return;
    }
    
    if (cmd == "BSAVE") {
        string filename;
        iss >> filename;
        if (filename.empty()) filename = "containers.bin";
        saveAllContainersBinary(filename);
        return;
    }
    
    if (cmd == "BLOAD") {
        string filename;
        iss >> filename;
        if (filename.empty()) filename = "containers.bin";
        loadAllContainersBinary(filename);
        return;
    }
    
    if (cmd == "CREATE") {
        string containerType, containerName;
        iss >> containerType >> containerName;
        
        if (containerType.empty() || containerName.empty()) {
            throw runtime_error("Неверный формат CREATE. Используйте: CREATE <ТИП> <ИМЯ>");
        }
        
        ContainerType type = parseContainerType(containerType);
        
        switch (type) {
            case ARRAY:
                if (arrays.find(containerName) == arrays.end()) {
                    arrays.emplace(std::piecewise_construct,
                                  std::forward_as_tuple(containerName),
                                  std::forward_as_tuple());
                    cout << "✓ Создан пустой массив '" << containerName << "'" << endl;
                } else {
                    cout << "⚠ Массив '" << containerName << "' уже существует" << endl;
                }
                break;
            case SINGLE_LIST:
                if (singleLists.find(containerName) == singleLists.end()) {
                    singleLists.emplace(std::piecewise_construct,
                                       std::forward_as_tuple(containerName),
                                       std::forward_as_tuple());
                    cout << "✓ Создан пустой односвязный список '" << containerName << "'" << endl;
                } else {
                    cout << "⚠ Список '" << containerName << "' уже существует" << endl;
                }
                break;
            case DOUBLE_LIST:
                if (doubleLists.find(containerName) == doubleLists.end()) {
                    doubleLists.emplace(std::piecewise_construct,
                                       std::forward_as_tuple(containerName),
                                       std::forward_as_tuple());
                    cout << "✓ Создан пустой двусвязный список '" << containerName << "'" << endl;
                } else {
                    cout << "⚠ Список '" << containerName << "' уже существует" << endl;
                }
                break;
            case STACK:
                if (stacks.find(containerName) == stacks.end()) {
                    stacks.emplace(std::piecewise_construct,
                                  std::forward_as_tuple(containerName),
                                  std::forward_as_tuple());
                    cout << "✓ Создан пустой стек '" << containerName << "'" << endl;
                } else {
                    cout << "⚠ Стек '" << containerName << "' уже существует" << endl;
                }
                break;
            case QUEUE:
                if (queues.find(containerName) == queues.end()) {
                    queues.emplace(std::piecewise_construct,
                                  std::forward_as_tuple(containerName),
                                  std::forward_as_tuple());
                    cout << "✓ Создана пустая очередь '" << containerName << "'" << endl;
                } else {
                    cout << "⚠ Очередь '" << containerName << "' уже существует" << endl;
                }
                break;
            case HASHMAP:
                if (hashmaps.find(containerName) == hashmaps.end()) {
                    hashmaps.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(containerName),
                                    std::forward_as_tuple());
                    cout << "✓ Создана пустая хеш-таблица '" << containerName << "'" << endl;
                } else {
                    cout << "⚠ Хеш-таблица '" << containerName << "' уже существует" << endl;
                }
                break;
            case SET:
                if (sets.find(containerName) == sets.end()) {
                    sets.emplace(std::piecewise_construct,
                                std::forward_as_tuple(containerName),
                                std::forward_as_tuple());
                    cout << "✓ Создано пустое множество '" << containerName << "'" << endl;
                } else {
                    cout << "⚠ Множество '" << containerName << "' уже существует" << endl;
                }
                break;
            case AVLTREE:
                if (trees.find(containerName) == trees.end()) {
                    trees.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(containerName),
                                 std::forward_as_tuple());
                    cout << "✓ Создано пустое AVL-дерево '" << containerName << "'" << endl;
                } else {
                    cout << "⚠ Дерево '" << containerName << "' уже существует" << endl;
                }
                break;
        }
        return;
    }
    
    // Команда DELETE - удаление контейнера
    if (cmd == "DELETE") {
        string containerType, containerName;
        iss >> containerType >> containerName;
        
        if (containerType.empty() || containerName.empty()) {
            throw runtime_error("Неверный формат DELETE. Используйте: DELETE <ТИП> <ИМЯ>");
        }
        
        ContainerType type = parseContainerType(containerType);
        
        switch (type) {
            case ARRAY:
                if (arrays.erase(containerName) > 0) {
                    cout << "✓ Массив '" << containerName << "' удален" << endl;
                } else {
                    cout << "⚠ Массив '" << containerName << "' не найден" << endl;
                }
                break;
            case SINGLE_LIST:
                if (singleLists.erase(containerName) > 0) {
                    cout << "✓ Односвязный список '" << containerName << "' удален" << endl;
                } else {
                    cout << "⚠ Список '" << containerName << "' не найден" << endl;
                }
                break;
            case DOUBLE_LIST:
                if (doubleLists.erase(containerName) > 0) {
                    cout << "✓ Двусвязный список '" << containerName << "' удален" << endl;
                } else {
                    cout << "⚠ Список '" << containerName << "' не найден" << endl;
                }
                break;
            case STACK:
                if (stacks.erase(containerName) > 0) {
                    cout << "✓ Стек '" << containerName << "' удален" << endl;
                } else {
                    cout << "⚠ Стек '" << containerName << "' не найден" << endl;
                }
                break;
            case QUEUE:
                if (queues.erase(containerName) > 0) {
                    cout << "✓ Очередь '" << containerName << "' удалена" << endl;
                } else {
                    cout << "⚠ Очередь '" << containerName << "' не найдена" << endl;
                }
                break;
            case HASHMAP:
                if (hashmaps.erase(containerName) > 0) {
                    cout << "✓ Хеш-таблица '" << containerName << "' удалена" << endl;
                } else {
                    cout << "⚠ Хеш-таблица '" << containerName << "' не найдена" << endl;
                }
                break;
            case SET:
                if (sets.erase(containerName) > 0) {
                    cout << "✓ Множество '" << containerName << "' удалено" << endl;
                } else {
                    cout << "⚠ Множество '" << containerName << "' не найдено" << endl;
                }
                break;
            case AVLTREE:
                if (trees.erase(containerName) > 0) {
                    cout << "✓ AVL-дерево '" << containerName << "' удалено" << endl;
                } else {
                    cout << "⚠ Дерево '" << containerName << "' не найдено" << endl;
                }
                break;
        }
        return;
    }
    
    // Команда LIST - показать все контейнеры
    if (cmd == "LIST") {
        if (!arrays.empty()) {
            cout << "📦 Массивы (" << arrays.size() << "):" << endl;
            for (const auto& [name, arr] : arrays) {
                cout << "  - " << name << " (размер: " << arr.size() << ")" << endl;
            }
            cout << endl;
        }
        
        if (!singleLists.empty()) {
            cout << "📝 Односвязные списки (" << singleLists.size() << "):" << endl;
            for (const auto& [name, list] : singleLists) {
                cout << "  - " << name << " (размер: " << list.size() << ")" << endl;
            }
            cout << endl;
        }
        
        if (!doubleLists.empty()) {
            cout << "📋 Двусвязные списки (" << doubleLists.size() << "):" << endl;
            for (const auto& [name, list] : doubleLists) {
                cout << "  - " << name << " (размер: " << list.size() << ")" << endl;
            }
            cout << endl;
        }
        
        if (!stacks.empty()) {
            cout << "📚 Стеки (" << stacks.size() << "):" << endl;
            for (const auto& [name, stack] : stacks) {
                cout << "  - " << name << " (размер: " << stack.size() << ")" << endl;
            }
            cout << endl;
        }
        
        if (!queues.empty()) {
            cout << "🎫 Очереди (" << queues.size() << "):" << endl;
            for (const auto& [name, queue] : queues) {
                cout << "  - " << name << " (размер: " << queue.size() << ")" << endl;
            }
            cout << endl;
        }
        
        if (!hashmaps.empty()) {
            cout << "🗂️  Хеш-таблицы (" << hashmaps.size() << "):" << endl;
            for (const auto& [name, map] : hashmaps) {
                cout << "  - " << name << " (размер: " << map.size() << ")" << endl;
            }
            cout << endl;
        }
        
        if (!sets.empty()) {
            cout << "🎯 Множества (" << sets.size() << "):" << endl;
            for (const auto& [name, set] : sets) {
                cout << "  - " << name << " (размер: " << set.size() << ")" << endl;
            }
            cout << endl;
        }
        
        if (!trees.empty()) {
            cout << "🌲 AVL-деревья (" << trees.size() << "):" << endl;
            for (const auto& [name, tree] : trees) {
                cout << "  - " << name << " (размер: " << tree.size() << ", высота: " << tree.height() << ")" << endl;
            }
            cout << endl;
        }
        
        size_t total = arrays.size() + singleLists.size() + doubleLists.size() + 
                      stacks.size() + queues.size() + hashmaps.size() + 
                      sets.size() + trees.size();
        
        if (total == 0) {
            cout << "  (Нет созданных контейнеров)" << endl;
        }
        return;
    }
    
    // Парсим команду нового формата
    ParsedCommand parsed = parseNewFormat(cmd, iss);
    
    if (!parsed.isValid) {
        throw runtime_error("Invalid command format. Use: <PREFIX><OPERATION> <NAME> [ARGS]\nExample: MPUSH myarray value");
    }
    
    // Определяем тип контейнера по префиксу
    ContainerType type;
    if (parsed.containerPrefix == 'M') type = ARRAY;
    else if (parsed.containerPrefix == 'S') type = STACK;
    else if (parsed.containerPrefix == 'Q') type = QUEUE;
    else if (parsed.containerPrefix == 'H') type = HASHMAP;
    else if (parsed.containerPrefix == 'T') type = AVLTREE;
    else if (parsed.containerPrefix == 'E') type = SET;
    else if (parsed.containerPrefix == 'F') type = SINGLE_LIST;
    else if (parsed.containerPrefix == 'L') type = DOUBLE_LIST;
    else {
        throw runtime_error("Неизвестный префикс контейнера: " + string(1, parsed.containerPrefix));
    }
    
    // Обработка по типу контейнера
    string& containerName = parsed.containerName;
    string& operation = parsed.operation;
    auto& args = parsed.args;
    
    switch (type) {
        case ARRAY: {
            if (arrays.find(containerName) == arrays.end()) {
                arrays.emplace(std::piecewise_construct,
                              std::forward_as_tuple(containerName),
                              std::forward_as_tuple());
            }
            
            auto& arr = arrays.at(containerName);
            
            if (operation == "PUSH") {
                if (args.empty()) throw runtime_error("MPUSH требует значение");
                arr.push(args[0]);
                cout << "✓ Добавлено: " << args[0] << endl;
            }
            else if (operation == "GET") {
                if (args.empty()) throw runtime_error("MGET требует индекс");
                size_t index = std::stoul(args[0]);
                cout << "arr[" << index << "] = " << arr.get(index) << endl;
            }
            else if (operation == "INSERT") {
                if (args.size() < 2) throw runtime_error("MINSERT требует индекс и значение");
                size_t index = std::stoul(args[0]);
                arr.insert(index, args[1]);
                cout << "✓ Вставлено на позицию " << index << endl;
            }
            else if (operation == "REMOVE") {
                if (args.empty()) throw runtime_error("MREMOVE требует индекс");
                size_t index = std::stoul(args[0]);
                arr.remove(index);
                cout << "✓ Удалено с позиции " << index << endl;
            }
            else if (operation == "REPLACE") {
                if (args.size() < 2) throw runtime_error("MREPLACE требует индекс и значение");
                size_t index = std::stoul(args[0]);
                arr.replace(index, args[1]);
                cout << "✓ Заменено на позиции " << index << endl;
            }
            else if (operation == "SIZE") {
                cout << "Размер: " << arr.size() << endl;
            }
            else if (operation == "PRINT") {
                arr.print();
                cout << endl;
            }
            else if (operation == "CLEAR") {
                arr.clear();
                cout << "✓ Массив очищен" << endl;
            }
            else {
                throw runtime_error("Неизвестная операция для ARRAY: " + operation);
            }
            break;
        }
        
        case STACK: {
            if (stacks.find(containerName) == stacks.end()) {
                stacks.emplace(std::piecewise_construct,
                              std::forward_as_tuple(containerName),
                              std::forward_as_tuple());
            }
            
            auto& stack = stacks.at(containerName);
            
            if (operation == "PUSH") {
                if (args.empty()) throw runtime_error("SPUSH требует значение");
                stack.push(args[0]);
                cout << "✓ Добавлено в стек: " << args[0] << endl;
            }
            else if (operation == "POP") {
                string val = stack.pop();
                cout << "Извлечено: " << val << endl;
            }
            else if (operation == "PEEK") {
                cout << "Вершина стека: " << stack.peek() << endl;
            }
            else if (operation == "SIZE") {
                cout << "Размер: " << stack.size() << endl;
            }
            else if (operation == "PRINT") {
                stack.print();
                cout << endl;
            }
            else if (operation == "CLEAR") {
                stack.clear();
                cout << "✓ Стек очищен" << endl;
            }
            else {
                throw runtime_error("Неизвестная операция для STACK: " + operation);
            }
            break;
        }
        
        case QUEUE: {
            if (queues.find(containerName) == queues.end()) {
                queues.emplace(std::piecewise_construct,
                              std::forward_as_tuple(containerName),
                              std::forward_as_tuple());
            }
            
            auto& queue = queues.at(containerName);
            
            if (operation == "PUSH" || operation == "ENQUEUE") {
                if (args.empty()) throw runtime_error("QPUSH требует значение");
                queue.enqueue(args[0]);
                cout << "✓ Добавлено в очередь: " << args[0] << endl;
            }
            else if (operation == "POP" || operation == "DEQUEUE") {
                string val = queue.dequeue();
                cout << "Извлечено: " << val << endl;
            }
            else if (operation == "PEEK") {
                cout << "Первый элемент: " << queue.peek() << endl;
            }
            else if (operation == "SIZE") {
                cout << "Размер: " << queue.size() << endl;
            }
            else if (operation == "PRINT") {
                queue.print();
                cout << endl;
            }
            else if (operation == "CLEAR") {
                queue.clear();
                cout << "✓ Очередь очищена" << endl;
            }
            else {
                throw runtime_error("Неизвестная операция для QUEUE: " + operation);
            }
            break;
        }
        
        case HASHMAP: {
            if (hashmaps.find(containerName) == hashmaps.end()) {
                hashmaps.emplace(std::piecewise_construct,
                                std::forward_as_tuple(containerName),
                                std::forward_as_tuple());
            }
            
            auto& map = hashmaps.at(containerName);
            
            if (operation == "PUT") {
                if (args.size() < 2) throw runtime_error("HPUT требует ключ и значение");
                map.put(args[0], args[1]);
                cout << "✓ Добавлено: " << args[0] << " => " << args[1] << endl;
            }
            else if (operation == "GET") {
                if (args.empty()) throw runtime_error("HGET требует ключ");
                cout << map.get(args[0]) << endl;
            }
            else if (operation == "CONTAINS") {
                if (args.empty()) throw runtime_error("HCONTAINS требует ключ");
                cout << (map.contains(args[0]) ? "Да" : "Нет") << endl;
            }
            else if (operation == "REMOVE") {
                if (args.empty()) throw runtime_error("HREMOVE требует ключ");
                map.remove(args[0]);
                cout << "✓ Удалено: " << args[0] << endl;
            }
            else if (operation == "SIZE") {
                cout << "Размер: " << map.size() << endl;
            }
            else if (operation == "PRINT") {
                map.print();
                cout << endl;
            }
            else if (operation == "CLEAR") {
                map.clear();
                cout << "✓ HashMap очищена" << endl;
            }
            else {
                throw runtime_error("Неизвестная операция для HASHMAP: " + operation);
            }
            break;
        }
        
        case AVLTREE: {
            if (trees.find(containerName) == trees.end()) {
                trees.emplace(std::piecewise_construct,
                             std::forward_as_tuple(containerName),
                             std::forward_as_tuple());
            }
            
            auto& tree = trees.at(containerName);
            
            if (operation == "INSERT" || operation == "PUSH") {
                if (args.empty()) throw runtime_error("TINSERT требует значение");
                tree.insert(args[0]);
                cout << "✓ Добавлено в дерево: " << args[0] << endl;
            }
            else if (operation == "SEARCH") {
                if (args.empty()) throw runtime_error("TSEARCH требует значение");
                cout << (tree.search(args[0]) ? "Найдено" : "Не найдено") << endl;
            }
            else if (operation == "REMOVE") {
                if (args.empty()) throw runtime_error("TREMOVE требует значение");
                tree.remove(args[0]);
                cout << "✓ Удалено: " << args[0] << endl;
            }
            else if (operation == "SIZE") {
                cout << "Размер: " << tree.size() << endl;
            }
            else if (operation == "HEIGHT") {
                cout << "Высота: " << tree.height() << endl;
            }
            else if (operation == "PRINT") {
                tree.print();
                cout << endl;
            }
            else if (operation == "CLEAR") {
                tree.clear();
                cout << "✓ Дерево очищено" << endl;
            }
            else {
                throw runtime_error("Неизвестная операция для TREE: " + operation);
            }
            break;
        }
        
        case SINGLE_LIST:
        case DOUBLE_LIST:
        case SET:
            throw runtime_error("Тип контейнера еще не полностью реализован");
        
        default:
            throw runtime_error("Неизвестный тип контейнера");
    }
}

// Справка
void printHelp() {  
    cout << "НОВЫЙ ФОРМАТ КОМАНД:" << endl;
    cout << "  <PREFIX><OPERATION> <NAME> [ARGS...]\n" << endl;
    
    cout << "Префиксы контейнеров:" << endl;
    cout << "  M - Array (массив)" << endl;
    cout << "  S - Stack (стек)" << endl;
    cout << "  Q - Queue (очередь)" << endl;
    cout << "  H - HashMap (хеш-таблица)" << endl;
    cout << "  T - Tree (AVL-дерево)" << endl;
    cout << "  E - Set (множество)" << endl;
    cout << "  F - SingleList (односвязный список)" << endl;
    cout << "  L - DoubleList (двусвязный список)\n" << endl;
    
    cout << "Операции для ARRAY (M):" << endl;
    cout << "  MPUSH <name> <value>           - Добавить элемент" << endl;
    cout << "  MGET <name> <index>            - Получить элемент" << endl;
    cout << "  MINSERT <name> <index> <value> - Вставить элемент" << endl;
    cout << "  MREMOVE <name> <index>         - Удалить элемент" << endl;
    cout << "  MREPLACE <name> <index> <val>  - Заменить элемент" << endl;
    cout << "  MSIZE <name>                   - Размер массива" << endl;
    cout << "  MPRINT <name>                  - Вывести массив" << endl;
    cout << "  MCLEAR <name>                  - Очистить массив\n" << endl;
    
    cout << "Операции для STACK (S):" << endl;
    cout << "  SPUSH <name> <value>  - Добавить элемент" << endl;
    cout << "  SPOP <name>           - Извлечь элемент" << endl;
    cout << "  SPEEK <name>          - Посмотреть вершину" << endl;
    cout << "  SSIZE <name>          - Размер стека" << endl;
    cout << "  SPRINT <name>         - Вывести стек" << endl;
    cout << "  SCLEAR <name>         - Очистить стек\n" << endl;
    
    cout << "Операции для QUEUE (Q):" << endl;
    cout << "  QPUSH <name> <value>  - Добавить элемент" << endl;
    cout << "  QPOP <name>           - Извлечь элемент" << endl;
    cout << "  QPEEK <name>          - Посмотреть первый" << endl;
    cout << "  QSIZE <name>          - Размер очереди" << endl;
    cout << "  QPRINT <name>         - Вывести очередь" << endl;
    cout << "  QCLEAR <name>         - Очистить очередь\n" << endl;
    
    cout << "Операции для HASHMAP (H):" << endl;
    cout << "  HPUT <name> <key> <value> - Добавить пару" << endl;
    cout << "  HGET <name> <key>         - Получить значение" << endl;
    cout << "  HCONTAINS <name> <key>    - Проверить наличие" << endl;
    cout << "  HREMOVE <name> <key>      - Удалить пару" << endl;
    cout << "  HSIZE <name>              - Размер таблицы" << endl;
    cout << "  HPRINT <name>             - Вывести таблицу" << endl;
    cout << "  HCLEAR <name>             - Очистить таблицу\n" << endl;
    
    cout << "Операции для TREE (T):" << endl;
    cout << "  TINSERT <name> <value> - Добавить элемент" << endl;
    cout << "  TSEARCH <name> <value> - Найти элемент" << endl;
    cout << "  TREMOVE <name> <value> - Удалить элемент" << endl;
    cout << "  TSIZE <name>           - Количество узлов" << endl;
    cout << "  THEIGHT <name>         - Высота дерева" << endl;
    cout << "  TPRINT <name>          - Вывести дерево" << endl;
    cout << "  TCLEAR <name>          - Очистить дерево\n" << endl;
    
    cout << "Управление контейнерами:" << endl;
    cout << "  CREATE <TYPE> <NAME>  - Создать пустой контейнер" << endl;
    cout << "  DELETE <TYPE> <NAME>  - Удалить контейнер" << endl;
    cout << "  LIST                  - Показать все контейнеры\n" << endl;
    
    cout << "Работа с файлами:" << endl;
    cout << "  SAVE [filename]  - Сохранить все контейнеры в JSON (по умолчанию containers.json)" << endl;
    cout << "  LOAD [filename]  - Загрузить все контейнеры из JSON (по умолчанию containers.json)" << endl;
    cout << "  BSAVE [filename] - Сохранить все контейнеры в бинарный файл (по умолчанию containers.bin)" << endl;
    cout << "  BLOAD [filename] - Загрузить все контейнеры из бинарного файла (по умолчанию containers.bin)\n" << endl;
    
    cout << "Другие команды:" << endl;
    cout << "  HELP            - Показать эту справку" << endl;
    cout << "  EXIT            - Выход из программы\n" << endl;
    
    cout << "Примеры:" << endl;
    cout << "  CREATE ARRAY myarr        - Создать пустой массив" << endl;
    cout << "  MPUSH myarr hello         - Добавить элемент в массив" << endl;
    cout << "  SPUSH mystack world       - Добавить в стек" << endl;
    cout << "  HPUT mymap name John      - Добавить пару в таблицу" << endl;
    cout << "  TINSERT mytree 42         - Добавить в дерево" << endl;
    cout << "  MPRINT myarr              - Вывести массив" << endl;
    cout << "  LIST                      - Показать все контейнеры" << endl;
    cout << "  DELETE ARRAY myarr        - Удалить массив" << endl;
    cout << "  SAVE data.json            - Сохранить в JSON" << endl;
    cout << "  BSAVE data.bin            - Сохранить в бинарный файл" << endl;
    cout << "  BLOAD data.bin            - Загрузить из бинарного файла" << endl;
}
