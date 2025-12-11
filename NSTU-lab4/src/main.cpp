#include <iostream>
#include <limits>
#include "task1_race.hpp"
#include "task2_dates.hpp"
#include "task3_philosophers.hpp"

using namespace std;

void print_menu() {
    cout << "\n";
    cout << "      Лаб. 4: Многопоточность в C++     \n";
    cout << "\n";
    cout << "1. Задача 1: Анализ состояния гонки\n";
    cout << "2. Задача 2: Многопоточная проверка дат\n";
    cout << "3. Задача 3: Задача обедающих философов\n";
    cout << "0. Выход\n";
    cout << "\n";
    cout << "Выберите опцию: ";
}

void clear_input() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    int choice;
    
    while (true) {
        print_menu();
        
        if (!(cin >> choice)) {
            clear_input();
            cout << "Некорректный ввод. Введите число.\n";
            continue;
        }
        
        switch (choice) {
            case 1:
                run_task1();
                break;
                
            case 2:
                run_task2();
                break;
                
            case 3:
                run_task3();
                break;
             
            case 0:
                cout << "Выход...\n";
                return 0;
                
            default:
                cout << "Некорректный выбор. Попробуйте снова.\n";
        }
        
        if (choice != 0) {
            cout << "\nНажмите Enter для продолжения...";
            clear_input();
            cin.get();
        }
    }
    
    return 0;
}
