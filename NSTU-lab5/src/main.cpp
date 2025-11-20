#include "simulation.h"
#include "prey.h"
#include "predator.h"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <random>

Direction intToDirection(int d) {
    switch (d) {
        case 0: return Direction::UP;
        case 1: return Direction::RIGHT;
        case 2: return Direction::DOWN;
        case 3: return Direction::LEFT;
        default: return Direction::UP;
    }
}

void printMenu() {
    std::cout << "\nВыберите режим работы:\n";
    std::cout << "  1. Загрузить из файла\n";
    std::cout << "  2. Ввести данные вручную\n";
    std::cout << "  3. Сгенерировать случайную симуляцию\n";
    std::cout << "  4. Запустить тестовый пример 1 (3×3, 3 хода)\n";
    std::cout << "  5. Запустить тестовый пример 2 (4×4, 20 ходов)\n";
    std::cout << "  0. Выход\n";
    std::cout << "\nВаш выбор: ";
}

void generateRandomSimulation(Simulation& sim, int N, int M, int preyCount, int predatorCount) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, N - 1);
    std::uniform_int_distribution<> disY(0, M - 1);
    std::uniform_int_distribution<> disDir(0, 3);
    std::uniform_int_distribution<> disK(1, 5);
    
    std::cout << "\nГенерация случайных животных...\n";
    
    // Генерация жертв
    for (int i = 0; i < preyCount; i++) {
        int x = disX(gen);
        int y = disY(gen);
        int d = disDir(gen);
        int k = disK(gen);
        sim.addAnimal(std::make_unique<Prey>(x, y, intToDirection(d), k));
    }
    
    // Генерация хищников
    for (int i = 0; i < predatorCount; i++) {
        int x = disX(gen);
        int y = disY(gen);
        int d = disDir(gen);
        int k = disK(gen);
        sim.addAnimal(std::make_unique<Predator>(x, y, intToDirection(d), k));
    }
    
    std::cout << "Создано " << preyCount << " жертв и " << predatorCount << " хищников\n";
}

bool askVisualization() {
    std::cout << "\nВключить визуализацию? (y/n): ";
    char choice;
    std::cin >> choice;
    return (choice == 'y' || choice == 'Y');
}

int getPositiveInt(const std::string& prompt, int minVal = 1, int maxVal = 1000) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minVal && value <= maxVal) {
            return value;
        }
        std::cout << "Ошибка! Введите число от " << minVal << " до " << maxVal << "\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main(int argc, char* argv[]) {
    // Если есть аргументы командной строки - старый режим работы
    if (argc > 1) {
        bool visualize = false;
        std::string inputFile = "";
        
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "-v" || arg == "--visualize") {
                visualize = true;
            } else if (arg == "-f" || arg == "--file") {
                if (i + 1 < argc) {
                    inputFile = argv[++i];
                }
            }
        }
        
        std::istream* input = &std::cin;
        std::ifstream file;
        
        if (!inputFile.empty()) {
            file.open(inputFile);
            if (file.is_open()) {
                input = &file;
            } else {
                std::cerr << "Не удалось открыть файл: " << inputFile << "\n";
                return 1;
            }
        }
        
        int N, M, T, R, W;
        *input >> N >> M >> T;
        *input >> R >> W;
        
        Simulation sim(N, M, T);
        
        for (int i = 0; i < R; i++) {
            int x, y, d, k;
            *input >> x >> y >> d >> k;
            sim.addAnimal(std::make_unique<Prey>(x, y, intToDirection(d), k));
        }
        
        for (int i = 0; i < W; i++) {
            int x, y, d, k;
            *input >> x >> y >> d >> k;
            sim.addAnimal(std::make_unique<Predator>(x, y, intToDirection(d), k));
        }
        
        sim.run(visualize);
        
        if (!visualize) {
            sim.printFinalState();
        } else {
            std::cout << "\n=== Финальное состояние ===\n";
            sim.printFinalState();
        }
        
        if (file.is_open()) {
            file.close();
        }
        
        return 0;
    }
    
    // Интерактивный режим с меню
    while (true) {
        printMenu();
        
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Некорректный ввод!\n";
            continue;
        }
        
        if (choice == 0) {
            std::cout << "\nДо свидания!\n";
            break;
        }
        
        int N, M, T;
        bool visualize = false;
        
        switch (choice) {
            case 1: {
                std::cout << "\nВведите имя файла: ";
                std::string filename;
                std::cin >> filename;
                
                std::ifstream file(filename);
                if (!file.is_open()) {
                    std::cerr << "Не удалось открыть файл: " << filename << "\n";
                    break;
                }
                
                int R, W;
                file >> N >> M >> T >> R >> W;
                
                visualize = askVisualization();
                Simulation sim(N, M, T);
                
                for (int i = 0; i < R; i++) {
                    int x, y, d, k;
                    file >> x >> y >> d >> k;
                    sim.addAnimal(std::make_unique<Prey>(x, y, intToDirection(d), k));
                }
                
                for (int i = 0; i < W; i++) {
                    int x, y, d, k;
                    file >> x >> y >> d >> k;
                    sim.addAnimal(std::make_unique<Predator>(x, y, intToDirection(d), k));
                }
                
                file.close();
                
                std::cout << "\nЗапуск симуляции " << N << "×" << M << " на " << T << " ходов...\n";
                sim.run(visualize);
                std::cout << "\nФинальное состояние\n";
                sim.printFinalState();
                break;
            }
            
            case 2: {  // Ручной ввод
                N = getPositiveInt("Ширина поля (N): ", 1, 100);
                M = getPositiveInt("Высота поля (M): ", 1, 100);
                T = getPositiveInt("Количество ходов (T): ", 1, 1000);
                
                int R = getPositiveInt("Количество жертв: ", 0, 100);
                int W = getPositiveInt("Количество хищников: ", 0, 100);
                
                visualize = askVisualization();
                Simulation sim(N, M, T);
                
                std::cout << "\nВвод данных для жертв (x y direction k):\n";
                std::cout << "   direction: 0=вверх, 1=вправо, 2=вниз, 3=влево\n";
                for (int i = 0; i < R; i++) {
                    std::cout << "Жертва " << (i + 1) << ": ";
                    int x, y, d, k;
                    std::cin >> x >> y >> d >> k;
                    sim.addAnimal(std::make_unique<Prey>(x, y, intToDirection(d), k));
                }
                
                std::cout << "\nВвод данных для хищников (x y direction k):\n";
                for (int i = 0; i < W; i++) {
                    std::cout << "Хищник " << (i + 1) << ": ";
                    int x, y, d, k;
                    std::cin >> x >> y >> d >> k;
                    sim.addAnimal(std::make_unique<Predator>(x, y, intToDirection(d), k));
                }
                
                std::cout << "\nЗапуск симуляции...\n";
                sim.run(visualize);
                std::cout << "\nФинальное состояние\n";
                sim.printFinalState();
                break;
            }
            
            case 3: {  // Случайная генерация
                N = getPositiveInt("Ширина поля (N): ", 1, 100);
                M = getPositiveInt("Высота поля (M): ", 1, 100);
                T = getPositiveInt("Количество ходов (T): ", 1, 1000);
                
                int R = getPositiveInt("Количество жертв: ", 0, 100);
                int W = getPositiveInt("Количество хищников: ", 0, 100);
                
                visualize = askVisualization();
                Simulation sim(N, M, T);
                
                generateRandomSimulation(sim, N, M, R, W);
                
                std::cout << "\nЗапуск симуляции...\n";
                sim.run(visualize);
                std::cout << "\nФинальное состояние\n";
                sim.printFinalState();
                break;
            }
            
            case 4: {  // Тестовый пример 1
                std::cout << "\nЗагрузка тестового примера 1 (3×3, 3 хода)...\n";
                visualize = askVisualization();
                
                Simulation sim(3, 3, 3);
                sim.addAnimal(std::make_unique<Prey>(1, 2, Direction::RIGHT, 1));
                sim.addAnimal(std::make_unique<Prey>(1, 1, Direction::UP, 2));
                sim.addAnimal(std::make_unique<Predator>(0, 2, Direction::RIGHT, 2));
                
                std::cout << "Запуск симуляции...\n";
                sim.run(visualize);
                std::cout << "\nФинальное состояние\n";
                sim.printFinalState();
                break;
            }
            
            case 5: {  // Тестовый пример 2
                std::cout << "\nЗагрузка тестового примера 2 (4×4, 20 ходов)...\n";
                visualize = askVisualization();
                
                Simulation sim(4, 4, 20);
                sim.addAnimal(std::make_unique<Prey>(0, 0, Direction::RIGHT, 100));
                sim.addAnimal(std::make_unique<Predator>(0, 3, Direction::UP, 100));
                
                std::cout << "Запуск симуляции...\n";
                sim.run(visualize);
                std::cout << "\nФинальное состояние\n";
                sim.printFinalState();
                break;
            }
            
            default:
                std::cout << "Неверный выбор! Попробуйте еще раз.\n";
        }
        
        std::cout << "Нажмите Enter для продолжения...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
    
    return 0;
}
