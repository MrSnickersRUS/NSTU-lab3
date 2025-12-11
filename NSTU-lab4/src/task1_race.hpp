#ifndef TASK1_RACE_HPP
#define TASK1_RACE_HPP

#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <mutex>
#include <semaphore>
#include <barrier>
#include "sync_primitives.hpp"
#include "benchmark.hpp"

constexpr int NUM_THREADS = 10;
constexpr int ITERATIONS_PER_THREAD = 1000;

// Генератор случайных ASCII символов
char generate_random_ascii() {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    static thread_local std::uniform_int_distribution<> dis(33, 126);
    return static_cast<char>(dis(gen));
}

// Тест без синхронизации (состояние гонки)
void test_no_sync(int /*thread_id*/, std::string& shared_string) {
    for (int i = 0; i < ITERATIONS_PER_THREAD; ++i) {
        shared_string += generate_random_ascii();
    }
}

// Тест с Mutex
void test_mutex(int /*thread_id*/, std::string& shared_string, std::mutex& mtx) {
    for (int i = 0; i < ITERATIONS_PER_THREAD; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        shared_string += generate_random_ascii();
    }
}

// Тест с Semaphore
void test_semaphore(int /*thread_id*/, std::string& shared_string, 
                   std::counting_semaphore<1>& sem) {
    for (int i = 0; i < ITERATIONS_PER_THREAD; ++i) {
        sem.acquire();
        shared_string += generate_random_ascii();
        sem.release();
    }
}

// Тест с Barrier
void test_barrier(int /*thread_id*/, std::string& shared_string, 
                    std::barrier<>& barrier, std::mutex& mtx) {
    for (int i = 0; i < ITERATIONS_PER_THREAD; ++i) {
        barrier.arrive_and_wait();
        std::lock_guard<std::mutex> lock(mtx);
        shared_string += generate_random_ascii();
    }
}

// Тест с SpinLock
void test_spinlock(int /*thread_id*/, std::string& shared_string, SpinLock& spinlock) {
    for (int i = 0; i < ITERATIONS_PER_THREAD; ++i) {
        spinlock.lock();
        shared_string += generate_random_ascii();
        spinlock.unlock();
    }
}

// Тест с SpinWait
void test_spinwait(int thread_id, std::string& shared_string,
                  SpinWait& /*spinwait*/, std::atomic<int>& turn) {
    for (int i = 0; i < ITERATIONS_PER_THREAD; ++i) {
        while (turn.load() != thread_id) {
            // Активное ожидание своей очереди
        }
        shared_string += generate_random_ascii();
        turn.store((thread_id + 1) % NUM_THREADS);
    }
}

// Тест с Monitor
void test_monitor(int /*thread_id*/, std::string& shared_string, Monitor& monitor) {
    for (int i = 0; i < ITERATIONS_PER_THREAD; ++i) {
        monitor.lock();
        shared_string += generate_random_ascii();
        monitor.unlock();
    }
}

void run_task1() {
    std::cout << "\nЗАДАЧА 1: Состояние гонки с примитивами синхронизации\n\n";
    std::cout << "Количество потоков: " << NUM_THREADS << std::endl;
    std::cout << "Итераций на поток: " << ITERATIONS_PER_THREAD << std::endl;
    std::cout << "Ожидаемая длина строки: " << NUM_THREADS * ITERATIONS_PER_THREAD << "\n\n";

    // Тест 1: Без синхронизации (состояние гонки)
    {
        std::cout << "1. Без синхронизации (состояние гонки):\n";
        std::string shared_string;
        shared_string.reserve(NUM_THREADS * ITERATIONS_PER_THREAD);
        
        StopWatch sw;
        sw.start();
        
        std::vector<std::thread> threads;
        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back(test_no_sync, i, std::ref(shared_string));
        }
        for (auto& t : threads) {
            t.join();
        }
        
        sw.stop();
        std::cout << "   Время: " << sw.elapsed_milliseconds() << " мс\n";
        std::cout << "   Длина результата: " << shared_string.size() 
                  << " (ожидалось: " << NUM_THREADS * ITERATIONS_PER_THREAD << ")\n";
        std::cout << "   Состояние гонки обнаружено: " 
                  << (shared_string.size() != NUM_THREADS * ITERATIONS_PER_THREAD ? "ДА" : "НЕТ") 
                  << "\n\n";
    }

    // Тест 2: Mutex
    {
        std::cout << "2. Mutex:\n";
        std::string shared_string;
        shared_string.reserve(NUM_THREADS * ITERATIONS_PER_THREAD);
        std::mutex mtx;
        
        StopWatch sw;
        sw.start();
        
        std::vector<std::thread> threads;
        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back(test_mutex, i, std::ref(shared_string), std::ref(mtx));
        }
        for (auto& t : threads) {
            t.join();
        }
        
        sw.stop();
        std::cout << "   Время: " << sw.elapsed_milliseconds() << " мс\n";
        std::cout << "   Длина результата: " << shared_string.size() << "\n";
        std::cout << "   Корректно: " 
                  << (shared_string.size() == NUM_THREADS * ITERATIONS_PER_THREAD ? "ДА" : "НЕТ") 
                  << "\n\n";
    }

    // Тест 3: Semaphore
    {
        std::cout << "3. Semaphore:\n";
        std::string shared_string;
        shared_string.reserve(NUM_THREADS * ITERATIONS_PER_THREAD);
        std::counting_semaphore<1> sem(1);
        
        StopWatch sw;
        sw.start();
        
        std::vector<std::thread> threads;
        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back(test_semaphore, i, std::ref(shared_string), std::ref(sem));
        }
        for (auto& t : threads) {
            t.join();
        }
        
        sw.stop();
        std::cout << "   Время: " << sw.elapsed_milliseconds() << " мс\n";
        std::cout << "   Длина результата: " << shared_string.size() << "\n";
        std::cout << "   Корректно: " 
                  << (shared_string.size() == NUM_THREADS * ITERATIONS_PER_THREAD ? "ДА" : "НЕТ") 
                  << "\n\n";
    }

    // Тест 4: Barrier (с mutex для синхронизации)
    {
        std::cout << "4. Barrier (с Mutex):\n";
        std::string shared_string;
        shared_string.reserve(NUM_THREADS * ITERATIONS_PER_THREAD);
        std::barrier barrier(NUM_THREADS);
        std::mutex mtx;
        
        StopWatch sw;
        sw.start();
        
        std::vector<std::thread> threads;
        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back(test_barrier, i, std::ref(shared_string), 
                               std::ref(barrier), std::ref(mtx));
        }
        for (auto& t : threads) {
            t.join();
        }
        
        sw.stop();
        std::cout << "   Время: " << sw.elapsed_milliseconds() << " мс\n";
        std::cout << "   Длина результата: " << shared_string.size() << "\n";
        std::cout << "   Корректно: " 
                  << (shared_string.size() == NUM_THREADS * ITERATIONS_PER_THREAD ? "ДА" : "НЕТ") 
                  << "\n\n";
    }

    // Тест 5: SpinLock
    {
        std::cout << "5. SpinLock:\n";
        std::string shared_string;
        shared_string.reserve(NUM_THREADS * ITERATIONS_PER_THREAD);
        SpinLock spinlock;
        
        StopWatch sw;
        sw.start();
        
        std::vector<std::thread> threads;
        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back(test_spinlock, i, std::ref(shared_string), std::ref(spinlock));
        }
        for (auto& t : threads) {
            t.join();
        }
        
        sw.stop();
        std::cout << "   Время: " << sw.elapsed_milliseconds() << " мс\n";
        std::cout << "   Длина результата: " << shared_string.size() << "\n";
        std::cout << "   Корректно: " 
                  << (shared_string.size() == NUM_THREADS * ITERATIONS_PER_THREAD ? "ДА" : "НЕТ") 
                  << "\n\n";
    }

    // Тест 6: SpinWait
    {
        std::cout << "6. SpinWait (по очереди):\n";
        std::string shared_string;
        shared_string.reserve(NUM_THREADS * ITERATIONS_PER_THREAD);
        SpinWait spinwait;
        std::atomic<int> turn(0);
        
        StopWatch sw;
        sw.start();
        
        std::vector<std::thread> threads;
        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back(test_spinwait, i, std::ref(shared_string), 
                               std::ref(spinwait), std::ref(turn));
        }
        for (auto& t : threads) {
            t.join();
        }
        
        sw.stop();
        std::cout << "   Время: " << sw.elapsed_milliseconds() << " мс\n";
        std::cout << "   Длина результата: " << shared_string.size() << "\n";
        std::cout << "   Корректно: " 
                  << (shared_string.size() == NUM_THREADS * ITERATIONS_PER_THREAD ? "ДА" : "НЕТ") 
                  << "\n\n";
    }

    // Тест 7: Monitor
    {
        std::cout << "7. Monitor:\n";
        std::string shared_string;
        shared_string.reserve(NUM_THREADS * ITERATIONS_PER_THREAD);
        Monitor monitor;
        
        StopWatch sw;
        sw.start();
        
        std::vector<std::thread> threads;
        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back(test_monitor, i, std::ref(shared_string), std::ref(monitor));
        }
        for (auto& t : threads) {
            t.join();
        }
        
        sw.stop();
        std::cout << "   Время: " << sw.elapsed_milliseconds() << " мс\n";
        std::cout << "   Длина результата: " << shared_string.size() << "\n";
        std::cout << "   Корректно: " 
                  << (shared_string.size() == NUM_THREADS * ITERATIONS_PER_THREAD ? "ДА" : "НЕТ") 
                  << "\n\n";
    }
}

#endif // TASK1_RACE_HPP
