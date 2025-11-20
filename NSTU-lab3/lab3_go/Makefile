.PHONY: all build run test coverage coverage-report html clean help fmt lint

# Цели сборки
all: build

build:
	@echo "🔨 Компиляция..."
	@go build -o lab3_go .
	@echo "✅ Сборка завершена!"

run: build
	@echo "🚀 Запуск программы..."
	@./lab3_go

# Тестирование
test:
	@echo "🧪 Запуск тестов..."
	@go test ./tests -v

# Coverage с HTML отчётом
coverage:
	@echo "📊 Генерация отчёта о покрытии..."
	@go test ./tests -coverprofile=coverage.out -coverpkg=./containers
	@go tool cover -html=coverage.out -o coverage.html
	@echo "✅ HTML отчёт сохранён в coverage.html"
	@go tool cover -func=coverage.out | tail -1

# Показать процент покрытия
coverage-report:
	@go test ./tests -coverprofile=coverage.out -coverpkg=./containers
	@echo "\n📈 Покрытие по файлам:"
	@go tool cover -func=coverage.out

# Генерация HTML отчёта (алиас для coverage)
html: coverage

# Очистка
clean:
	@echo "🧹 Очистка..."
	@rm -f lab3_go coverage.out coverage.html
	@echo "✅ Очистка завершена!"

# Форматирование кода
fmt:
	@echo "✨ Форматирование кода..."
	@go fmt ./...

# Проверка кода
lint:
	@echo "🔍 Проверка кода..."
	@go vet ./...

# Справка
help:
	@echo "Доступные команды:"
	@echo "  make build          - Скомпилировать проект"
	@echo "  make run            - Запустить программу"
	@echo "  make test           - Запустить тесты"
	@echo "  make coverage       - Генерация HTML отчёта о покрытии"
	@echo "  make html           - Генерация HTML отчёта (алиас для coverage)"
	@echo "  make coverage-report - Показать детальный отчёт о покрытии"
	@echo "  make clean          - Удалить скомпилированные файлы"
	@echo "  make fmt            - Форматировать код"
	@echo "  make lint           - Проверить код"
	@echo "  make help           - Показать эту справку"
