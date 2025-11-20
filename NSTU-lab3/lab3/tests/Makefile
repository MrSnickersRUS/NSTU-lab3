# Makefile для тестов с покрытием кода

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I../src
TESTFLAGS = -lgtest -lgtest_main -pthread
COVFLAGS = -fprofile-arcs -ftest-coverage --coverage

# Директории
SRC_DIR = ../src
CONT_DIR = $(SRC_DIR)/containers
TEST_DIR = .
BUILD_DIR = build
COV_DIR = coverage

# CPP файлы для покрытия (только не-шаблонные)
# Для шаблонных классов покрытие будет собираться из заголовочных файлов
# commands.cpp не включаем, так как это отдельная логика приложения, не контейнеры
OTHER_SOURCES = 

# Все исходники для покрытия
ALL_SOURCES = $(OTHER_SOURCES)

# Тестовые файлы
TEST_SOURCES = test_array.cpp \
               test_stack.cpp \
               test_queue.cpp \
               test_hashmap.cpp \
               test_avltree.cpp \
               test_singlelist.cpp \
               test_doublelist.cpp \
               test_set.cpp \
               test_cuckoo.cpp

# Исполняемые файлы тестов (по одному на каждый тест)
TEST_EXECS = $(patsubst %.cpp,$(BUILD_DIR)/%,$(TEST_SOURCES))

.PHONY: all clean test coverage html

all: $(TEST_EXECS)

# Создание директории build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Компиляция и линковка каждого теста отдельно с исходниками контейнеров
$(BUILD_DIR)/%: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(COVFLAGS) $< -o $@ $(TESTFLAGS)

# Запуск всех тестов
test: $(TEST_EXECS)
	@echo "=== Запуск тестов ==="
	@for test in $(TEST_EXECS); do \
		echo ""; \
		echo "Запуск $$test"; \
		./$$test || exit 1; \
	done

# Генерация покрытия
coverage: test
	@echo ""
	@echo "=== Генерация покрытия кода ==="
	@echo "Данные покрытия собраны в .gcda файлах"

# HTML отчет
html: coverage
	@echo ""
	@echo "=== Создание HTML отчета ==="
	@mkdir -p $(COV_DIR)
	lcov --capture --directory $(BUILD_DIR) --output-file $(COV_DIR)/coverage.info --ignore-errors mismatch,inconsistent
	lcov --remove $(COV_DIR)/coverage.info '/usr/*' '*/test_*.cpp' '*/commands.cpp' '*/main.cpp' --output-file $(COV_DIR)/coverage_filtered.info --ignore-errors unused
	lcov --extract $(COV_DIR)/coverage_filtered.info '*/src/*.h' '*/containers/*.h' '*/containers/*.cpp' --output-file $(COV_DIR)/coverage_final.info --ignore-errors unused
	genhtml $(COV_DIR)/coverage_final.info --output-directory $(COV_DIR)/html --ignore-errors source
	@echo ""
	@echo "HTML отчет создан в $(COV_DIR)/html/index.html"
	@echo "Откройте файл в браузере для просмотра"

# Очистка
clean:
	rm -rf $(BUILD_DIR) $(COV_DIR)
	rm -f *.gcov *.gcda *.gcno
	rm -f test_*.bin

# Помощь
help:
	@echo "Доступные команды:"
	@echo "  make          - компиляция тестов"
	@echo "  make test     - запуск тестов"
	@echo "  make coverage - запуск тестов с генерацией покрытия"
	@echo "  make html     - создание HTML отчета покрытия"
	@echo "  make clean    - очистка"
	@echo "  make help     - эта справка"
