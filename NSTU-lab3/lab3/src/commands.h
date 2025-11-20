#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <map>
#include "containers/linear.h"
#include "containers/hash.h"
#include "containers/trees.h"

// Типы контейнеров
enum ContainerType {
    ARRAY,
    SINGLE_LIST,
    DOUBLE_LIST,
    STACK,
    QUEUE,
    HASHMAP,
    SET,
    AVLTREE
};

// Операции
enum Operation {
    // Общие
    PUSH, POP, GET, DEL, SIZE, CLEAR, PRINT,
    // Массивы
    INSERT, REMOVE, REPLACE,
    // Списки
    ADDHEAD, ADDTAIL, CONTAINS,
    // Хэш-таблицы
    PUT, GETVAL, REMOVEKEY,
    // Множества
    ADD,
    // Деревья
    SEARCH,
    // Операции с файлами
    SAVE, LOAD
};

// Глобальные хранилища для именованных структур данных
extern std::map<std::string, Array<std::string>> arrays;
extern std::map<std::string, SingleList<std::string>> singleLists;
extern std::map<std::string, DoubleList<std::string>> doubleLists;
extern std::map<std::string, Stack<std::string>> stacks;
extern std::map<std::string, Queue<std::string>> queues;
extern std::map<std::string, HashMap<std::string, std::string>> hashmaps;
extern std::map<std::string, Set<std::string>> sets;
extern std::map<std::string, AVLTree<std::string>> trees;

// Основные функции
void processCommand(const std::string& command);
void saveAllContainers(const std::string& filePath);
void loadAllContainers(const std::string& filePath);
void saveAllContainersBinary(const std::string& filePath);
void loadAllContainersBinary(const std::string& filePath);
void printHelp();

// Вспомогательные функции для парсинга
ContainerType parseContainerType(const std::string& type);

#endif