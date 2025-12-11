#ifndef TASK3_PHILOSOPHERS_HPP
#define TASK3_PHILOSOPHERS_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <random>
#include <memory>
#include <atomic>

class DiningPhilosophers {
private:
    int num_philosophers;
    std::vector<std::unique_ptr<std::mutex>> forks;
    std::vector<int> eating_count;
    std::mutex output_mutex;
    std::atomic<bool> running;
    
    void print_status(int philosopher_id, const std::string& status) {
        std::lock_guard<std::mutex> lock(output_mutex);
        std::cout << "Философ " << philosopher_id << " " << status << std::endl;
    }
    
    void think(int philosopher_id) {
        print_status(philosopher_id, "размышляет...");
        std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 200));
    }
    
    void eat(int philosopher_id) {
        print_status(philosopher_id, "ест.");
        eating_count[philosopher_id]++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 200));
    }
    
    void philosopher_lifecycle(int id) {
        while (running.load()) {
            think(id);
            
            // Проверяем running перед попыткой взять вилки
            if (!running.load()) break;
            
            int left_fork = id;
            int right_fork = (id + 1) % num_philosophers;
            
            // Всегда берем сначала вилку с меньшим номером
            int first_fork = std::min(left_fork, right_fork);
            int second_fork = std::max(left_fork, right_fork);
            
            // Пытаемся взять вилки
            print_status(id, "голоден и пытается взять вилки.");
            
            // Используем try_lock с проверкой running
            while (running.load() && !forks[first_fork]->try_lock()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            
            if (!running.load()) break;
            
            print_status(id, "взял вилку " + std::to_string(first_fork));
            
            // Пытаемся взять вторую вилку
            while (running.load() && !forks[second_fork]->try_lock()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            
            if (!running.load()) {
                forks[first_fork]->unlock();
                break;
            }
            
            print_status(id, "взял вилку " + std::to_string(second_fork));
            
            // Едим
            eat(id);
            
            // Кладем вилки
            forks[second_fork]->unlock();
            print_status(id, "положил вилку " + std::to_string(second_fork));
            
            forks[first_fork]->unlock();
            print_status(id, "положил вилку " + std::to_string(first_fork));
            
            print_status(id, "закончил есть (всего: " + 
                        std::to_string(eating_count[id]) + " раз)");
        }
    }

public:
    DiningPhilosophers(int n) : num_philosophers(n), running(false) {
        for (int i = 0; i < n; ++i) {
            forks.push_back(std::make_unique<std::mutex>());
        }
        eating_count.resize(n, 0);
    }
    
    void start(int duration_seconds) {
        std::cout << "\nЗАДАЧА 3: Задача обедающих философов\n\n";
        std::cout << "Количество философов: " << num_philosophers << std::endl;
        std::cout << "Длительность симуляции: " << duration_seconds << " секунд\n";
        std::cout << "Решение: Упорядочивание ресурсов (берем сначала вилку с меньшим номером)\n\n";
        
        running.store(true);
        std::vector<std::thread> philosophers;
        
        // Запускаем потоки философов
        for (int i = 0; i < num_philosophers; ++i) {
            philosophers.emplace_back(&DiningPhilosophers::philosopher_lifecycle, this, i);
        }
        
        // Пусть обедают указанное время
        std::this_thread::sleep_for(std::chrono::seconds(duration_seconds));
        
        // Останавливаем симуляцию
        running.store(false);

        std::cout << "\nОстанавливаем симуляцию...\n";
        
        // Ждем завершения всех философов
        for (auto& t : philosophers) {
            t.join();
        }
        
        // Выводим статистику
        std::cout << "\nСимуляция завершена\n";
        std::cout << "Статистика приемов пищи:\n";
        int total_meals = 0;
        for (int i = 0; i < num_philosophers; ++i) {
            std::cout << "  Философ " << i << ": " << eating_count[i] << " приемов пищи\n";
            total_meals += eating_count[i];
        }
        std::cout << "Всего приемов пищи: " << total_meals << std::endl;
        std::cout << "Среднее на философа: " 
                  << (double)total_meals / num_philosophers << std::endl;
        std::cout << "\nДедлока не произошло! ✓\n";
    }
};

void run_task3() {
    int num_philosophers;
    int duration;
    
    std::cout << "\nВведите количество философов (по умолчанию 5): ";
    std::cin >> num_philosophers;
    if (num_philosophers < 2) num_philosophers = 5;
    
    std::cout << "Введите длительность симуляции в секундах (по умолчанию 10): ";
    std::cin >> duration;
    if (duration < 1) duration = 10;
    
    DiningPhilosophers dining(num_philosophers);
    dining.start(duration);
}

#endif