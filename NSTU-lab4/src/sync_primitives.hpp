#ifndef SYNC_PRIMITIVES_HPP
#define SYNC_PRIMITIVES_HPP

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <semaphore>
#include <barrier>

// Реализация SpinLock
class SpinLock {
private:
    std::atomic_flag lock_flag = ATOMIC_FLAG_INIT;

public:
    void lock() {
        while (lock_flag.test_and_set(std::memory_order_acquire)) {
            // Активное ожидание
        }
    }

    void unlock() {
        lock_flag.clear(std::memory_order_release);
    }
};

// Реализация SpinWait
class SpinWait {
private:
    std::atomic<bool> flag{false};

public:
    void wait() {
        while (!flag.load(std::memory_order_acquire)) {
            // Активное ожидание
        }
    }

    void signal() {
        flag.store(true, std::memory_order_release);
    }

    void reset() {
        flag.store(false, std::memory_order_release);
    }
};

// Реализация Monitor (использует mutex и condition variable)
class Monitor {
private:
    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;

public:
    void lock() {
        mtx.lock();
    }

    void unlock() {
        mtx.unlock();
    }

    void wait() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return ready; });
    }

    void notify_all() {
        std::unique_lock<std::mutex> lock(mtx);
        ready = true;
        cv.notify_all();
    }

    void reset() {
        std::unique_lock<std::mutex> lock(mtx);
        ready = false;
    }
};

#endif // SYNC_PRIMITIVES_HPP
