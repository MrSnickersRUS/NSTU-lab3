#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <chrono>
#include <iostream>
#include <string>

// Класс для измерения времени выполнения
class StopWatch {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    bool running;

public:
    StopWatch() : running(false) {}

    void start() {
        start_time = std::chrono::high_resolution_clock::now();
        running = true;
    }

    void stop() {
        end_time = std::chrono::high_resolution_clock::now();
        running = false;
    }

    double elapsed_milliseconds() const {
        auto end = running ? std::chrono::high_resolution_clock::now() : end_time;
        return std::chrono::duration<double, std::milli>(end - start_time).count();
    }

    double elapsed_microseconds() const {
        auto end = running ? std::chrono::high_resolution_clock::now() : end_time;
        return std::chrono::duration<double, std::micro>(end - start_time).count();
    }

    void print_elapsed(const std::string& label) const {
        std::cout << label << ": " << elapsed_milliseconds() << " ms" << std::endl;
    }
};

#endif