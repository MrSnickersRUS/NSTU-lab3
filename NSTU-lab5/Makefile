# Makefile для проекта "Хищник-жертва"

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include
LDFLAGS = 

# Директории
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = .

# Исходные файлы
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGET = $(BIN_DIR)/predator_prey

.PHONY: all clean run run-interactive run-visual run2 run2-visual help

all: $(TARGET)

# Создание директории для объектных файлов
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Компиляция объектных файлов
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Линковка исполняемого файла
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Запуск в интерактивном режиме (с меню)
run: $(TARGET)
	@echo "🎮 Запуск интерактивного режима..."
	./$(TARGET)

# Запуск интерактивного режима (явная команда)
run-interactive: $(TARGET)
	@echo "🎮 Запуск интерактивного режима..."
	./$(TARGET)

# Запуск с примером из задания (3x3, 3 хода) - через аргументы
run-test1: $(TARGET)
	@echo "=== Запуск примера 1 (3x3, 3 хода) ==="
	@echo "3 3 3" > input1.txt
	@echo "2 1" >> input1.txt
	@echo "1 2 1 1" >> input1.txt
	@echo "1 1 0 2" >> input1.txt
	@echo "0 2 1 2" >> input1.txt
	./$(TARGET) -f input1.txt

# Запуск с визуализацией
run-visual: $(TARGET)
	@echo "Запуск с визуализацией"
	@echo "3 3 3" > input1.txt
	@echo "2 1" >> input1.txt
	@echo "1 2 1 1" >> input1.txt
	@echo "1 1 0 2" >> input1.txt
	@echo "0 2 1 2" >> input1.txt
	./$(TARGET) -f input1.txt -v

# Запуск второго примера (4x4, 20 ходов)
run2: $(TARGET)
	@echo "Запуск примера 2 (4x4, 20 ходов)"
	@echo "4 4 20" > input2.txt
	@echo "1 1" >> input2.txt
	@echo "0 0 1 100" >> input2.txt
	@echo "0 3 0 100" >> input2.txt
	./$(TARGET) -f input2.txt

# Запуск второго примера с визуализацией
run2-visual: $(TARGET)
	@echo "Запуск примера 2 с визуализацией"
	@echo "4 4 20" > input2.txt
	@echo "1 1" >> input2.txt
	@echo "0 0 1 100" >> input2.txt
	@echo "0 3 0 100" >> input2.txt
	./$(TARGET) -f input2.txt -v

# Очистка
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	rm -f input*.txt

# Помощь
help:
	@echo "Доступные команды:"
	@echo "  make                  - компиляция проекта"
	@echo "  make run              - запуск в интерактивном режиме (с меню)"
	@echo "  make run-interactive  - запуск в интерактивном режиме"
	@echo "  make run-test1        - запуск тестового примера 1 (3×3, 3 хода)"
	@echo "  make run-test1-visual - запуск примера 1 с визуализацией"
	@echo "  make run-test2        - запуск тестового примера 2 (4×4, 20 ходов)"
	@echo "  make run-test2-visual - запуск примера 2 с визуализацией"
	@echo "  make clean            - очистка проекта"
	@echo "  make help             - эта справка"
