#ifndef TASK2_DATES_HPP
#define TASK2_DATES_HPP

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <algorithm>
#include "benchmark.hpp"

struct Date {
    int day;
    int month;
    int year;

    Date(int d, int m, int y) : day(d), month(m), year(y) {}
};

// Проверка високосного года
bool is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Проверка корректности даты
bool is_valid_date(const Date& date) {
    if (date.year < 1 || date.year > 9999) return false;
    if (date.month < 1 || date.month > 12) return false;
    
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (is_leap_year(date.year)) {
        days_in_month[1] = 29;
    }
    
    if (date.day < 1 || date.day > days_in_month[date.month - 1]) {
        return false;
    }
    
    return true;
}

// Генерация случайных дат (включая некоторые некорректные)
std::vector<Date> generate_dates(size_t count) {
    std::vector<Date> dates;
    dates.reserve(count);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> day_dis(1, 35);    // Некоторые некорректные дни
    std::uniform_int_distribution<> month_dis(1, 13);  // Некоторые некорректные месяцы
    std::uniform_int_distribution<> year_dis(1990, 2030);
    
    for (size_t i = 0; i < count; ++i) {
        dates.emplace_back(day_dis(gen), month_dis(gen), year_dis(gen));
    }
    
    return dates;
}

// Однопоточная проверка
std::vector<Date> validate_dates_single_thread(const std::vector<Date>& dates) {
    std::vector<Date> invalid_dates;
    
    for (const auto& date : dates) {
        if (!is_valid_date(date)) {
            invalid_dates.push_back(date);
        }
    }
    
    return invalid_dates;
}

// Вспомогательная функция для многопоточной проверки
void validate_dates_worker(const std::vector<Date>& dates, 
                          size_t start, size_t end,
                          std::vector<Date>& invalid_dates,
                          std::mutex& mtx) {
    std::vector<Date> local_invalid;
    
    for (size_t i = start; i < end; ++i) {
        if (!is_valid_date(dates[i])) {
            local_invalid.push_back(dates[i]);
        }
    }
    
    // Блокировка только при добавлении в общий вектор
    std::lock_guard<std::mutex> lock(mtx);
    invalid_dates.insert(invalid_dates.end(), local_invalid.begin(), local_invalid.end());
}

// Многопоточная проверка
std::vector<Date> validate_dates_multi_thread(const std::vector<Date>& dates, int num_threads) {
    std::vector<Date> invalid_dates;
    std::mutex mtx;
    std::vector<std::thread> threads;
    
    size_t chunk_size = dates.size() / num_threads;
    
    for (int i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_threads - 1) ? dates.size() : (i + 1) * chunk_size;
        
        threads.emplace_back(validate_dates_worker, std::cref(dates), 
                           start, end, std::ref(invalid_dates), std::ref(mtx));
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    return invalid_dates;
}

void print_date(const Date& date) {
    std::cout << date.day << "/" << date.month << "/" << date.year;
}

void run_task2() {
    std::cout << "\nЗАДАЧА 2: Многопоточная проверка дат\n\n";
    
    size_t array_size;
    int num_threads;
    
    std::cout << "Введите размер массива: ";
    std::cin >> array_size;
    
    std::cout << "Введите количество потоков: ";
    std::cin >> num_threads;
    
    std::cout << "\nГенерация " << array_size << " случайных дат...\n";
    auto dates = generate_dates(array_size);
    
    // Однопоточная проверка
    std::cout << "\n--- Однопоточная проверка ---\n";
    StopWatch sw_single;
    sw_single.start();
    auto invalid_single = validate_dates_single_thread(dates);
    sw_single.stop();
    
    std::cout << "Время: " << sw_single.elapsed_milliseconds() << " мс\n";
    std::cout << "Найдено некорректных дат: " << invalid_single.size() << "\n";
    
    // Многопоточная проверка
    std::cout << "\n--- Многопоточная проверка (потоков: " << num_threads << ") ---\n";
    StopWatch sw_multi;
    sw_multi.start();
    auto invalid_multi = validate_dates_multi_thread(dates, num_threads);
    sw_multi.stop();
    
    std::cout << "Время: " << sw_multi.elapsed_milliseconds() << " мс\n";
    std::cout << "Найдено некорректных дат: " << invalid_multi.size() << "\n";
    
    // Расчёт ускорения
    double speedup = sw_single.elapsed_milliseconds() / sw_multi.elapsed_milliseconds();
    std::cout << "\nУскорение: " << speedup << "x\n";
    
    // Вывод первых 10 некорректных дат
    std::cout << "\nПервые " << std::min(size_t(10), invalid_single.size()) 
              << " некорректных дат:\n";
    for (size_t i = 0; i < std::min(size_t(10), invalid_single.size()); ++i) {
        std::cout << "  ";
        print_date(invalid_single[i]);
        std::cout << "\n";
    }
    
    std::cout << "\n";
}

#endif // TASK2_DATES_HPP
