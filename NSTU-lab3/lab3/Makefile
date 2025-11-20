# Makefile для Lab3 - Классы контейнеров (C++)
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -I./src -I./include
TARGET = lab3
TEST_JSON = test_json
TEST_MEMORY = test_memory
DBMS = dbms

# Исходные файлы
MAIN_SOURCES = src/main.cpp src/commands.cpp
HEADERS = src/containers/containers.h \
          src/containers/linear.h \
          src/containers/hash.h \
          src/containers/trees.h \
          src/binary_serialization.h \
          src/commands.h \
          src/json_utils_simple.h

# Реализации контейнеров (не шаблонные)
CONTAINER_IMPL = src/containers/singlelist.cpp \
                 src/containers/doublelist.cpp \
                 src/containers/hashmap.cpp \
                 src/containers/cuckoo.cpp \
                 src/containers/set.cpp \
                 src/containers/avl.cpp

# ОСНОВНЫЕ ЦЕЛИ

# Компиляция всего проекта
all: $(TARGET)

$(TARGET): $(MAIN_SOURCES) $(HEADERS)
	@echo "🔨 Компиляция $(TARGET)..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(MAIN_SOURCES)
	@echo "✅ Сборка завершена!"

# Запуск программы
run: $(TARGET)
	@echo "🚀 Запуск $(TARGET)..."
	@echo ""
	./$(TARGET)

# Компиляция и запуск
build-run: all run

# Тест JSON сериализации (удален, используем Google Test в tests/)

# Интерактивная СУБД с командами (теперь это main)
dbms: $(TARGET)
	@echo "🚀 Запуск интерактивной СУБД..."
	@echo ""
	./$(TARGET)

# Проверка с Valgrind
valgrind: $(TARGET)
	@echo "🔍 Проверка утечек памяти с Valgrind..."
	@echo "EXIT" | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

# Запуск Google Test
test:
	@echo "🧪 Запуск Google Test..."
	@cd tests && $(MAKE) test

# Очистка скомпилированных файлов
clean:
	@echo "🧹 Очистка..."
	rm -f $(TARGET) *.json *.bin
	@cd tests && $(MAKE) clean
	@echo "✅ Очистка завершена!"

# Компиляция с отладочной информацией
debug: CXXFLAGS += -g -DDEBUG
debug: clean $(TARGET)
	@echo "🐛 Отладочная версия готова!"

# Компиляция с оптимизацией
release: CXXFLAGS += -O3 -DNDEBUG
release: clean $(TARGET)
	@echo "⚡ Оптимизированная версия готова!"

# Проверка синтаксиса без компоновки
check:
	@echo "🔍 Проверка синтаксиса..."
	$(CXX) $(CXXFLAGS) -fsyntax-only $(MAIN_SOURCES)
	@echo "✅ Синтаксис корректен!"

# Показать информацию о проекте
info:
	@echo "  Lab3 - Классы контейнеров (C++)"
	@echo "Компилятор: $(CXX)"
	@echo "Флаги:      $(CXXFLAGS)"
	@echo "Цель:       $(TARGET)"
	@echo ""
	@echo "Структуры данных:"
	@echo "  ✓ Массив (Array)"
	@echo "  ✓ Односвязный список (SingleList)"
	@echo "  ✓ Двусвязный список (DoubleList)"
	@echo "  ✓ Стек (Stack)"
	@echo "  ✓ Очередь (Queue)"
	@echo "  ✓ Хеш-таблица (HashMap)"
	@echo "  ✓ Множество (Set)"
	@echo "  ✓ АВЛ-дерево (AVLTree)"

# Помощь
help:
	@echo "Доступные команды:"
	@echo "  make           - Скомпилировать проект"
	@echo "  make run       - Запустить программу"
	@echo "  make build-run - Скомпилировать и запустить"
	@echo "  make dbms      - Запустить интерактивную СУБД (=run)"
	@echo "  make test      - Запустить Google Test"
	@echo "  make valgrind  - Проверить утечки памяти с Valgrind"
	@echo "  make clean     - Удалить скомпилированные файлы"
	@echo "  make debug     - Собрать с отладочной информацией"
	@echo "  make release   - Собрать с оптимизацией"
	@echo "  make check     - Проверить синтаксис"
	@echo "  make info      - Показать информацию о проекте"
	@echo "  make help      - Показать эту справку"

.PHONY: all run build-run dbms test valgrind clean debug release check info help
