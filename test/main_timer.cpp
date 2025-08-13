#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <chrono>
#include "../inc/libftpp.hpp"

void printDuration(const std::string& label, const Timer::duration_ms& duration) {
    std::cout << std::left << std::setw(20) << label << ": " 
              << std::fixed << std::setprecision(2) << duration.count() << " ms" << std::endl;
}

void test_basic_functionality() {
    std::cout << "=== Testing Basic Functionality ===" << std::endl;
    
    Timer timer;
    
    // Test initial state
    std::cout << "Initial state:" << std::endl;
    std::cout << "Running: " << timer.isRunning() << std::endl;
    std::cout << "Timed out: " << timer.hasTimedOut() << std::endl;
    printDuration("Elapsed", timer.elapsed());
    
    // Test start without timeout
    std::cout << "\nStarting timer (no timeout)..." << std::endl;
    timer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    std::cout << "Running: " << timer.isRunning() << std::endl;
    std::cout << "Timed out: " << timer.hasTimedOut() << std::endl;
    printDuration("Elapsed", timer.elapsed());
    
    // Test stop
    std::cout << "\nStopping timer..." << std::endl;
    timer.stop();
    std::cout << "Running: " << timer.isRunning() << std::endl;
    std::cout << "Timed out: " << timer.hasTimedOut() << std::endl;
    printDuration("Elapsed", timer.elapsed());
    
    std::cout << std::endl;
}

void test_timeout_functionality() {
    std::cout << "=== Testing Timeout Functionality ===" << std::endl;
    
    Timer timer;
    
    // Set timeout and start
    timer.setTimeout(Timer::duration_ms(100));
    printDuration("Timeout set to", timer.getTimeout());
    
    timer.start(Timer::duration_ms(150)); // Override timeout
    printDuration("New timeout", timer.getTimeout());
    
    std::cout << "\nBefore timeout:" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "Running: " << timer.isRunning() << std::endl;
    std::cout << "Timed out: " << timer.hasTimedOut() << std::endl;
    printDuration("Elapsed", timer.elapsed());
    
    std::cout << "\nAfter timeout (manual update):" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(120));
    timer.update();
    std::cout << "Running: " << timer.isRunning() << std::endl;
    std::cout << "Timed out: " << timer.hasTimedOut() << std::endl;
    printDuration("Elapsed", timer.elapsed());
    
    std::cout << std::endl;
}

void test_reset_functionality() {
    std::cout << "=== Testing Reset Functionality ===" << std::endl;
    
    Timer timer;
    
    timer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    std::cout << "Before reset:" << std::endl;
    printDuration("Elapsed", timer.elapsed());
    
    timer.reset();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    std::cout << "After reset:" << std::endl;
    std::cout << "Running: " << timer.isRunning() << std::endl;
    printDuration("Elapsed", timer.elapsed());
    
    timer.stop();
    std::cout << std::endl;
}

void test_callback_startAfter() {
    std::cout << "=== Testing startAfter Callbacks ===" << std::endl;
    
    Timer timer;
    std::atomic<int> callback_count{0};
    std::vector<std::future<void>> futures;
    
    auto start_time = std::chrono::steady_clock::now();
    
    // Schedule multiple callbacks
    for (int i = 1; i <= 3; ++i) {
        auto delay = Timer::duration_ms(i * 50);
        futures.push_back(timer.startAfter(delay, [&callback_count, i, start_time]() {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<Timer::duration_ms>(now - start_time);
            std::cout << "Callback " << i << " executed after " 
                      << std::fixed << std::setprecision(1) << elapsed.count() << "ms" << std::endl;
            callback_count++;
        }));
    }
    
    // Wait for all callbacks
    for (auto& future : futures) {
        future.wait();
    }
    
    std::cout << "Total callbacks executed: " << callback_count.load() << std::endl;
    std::cout << std::endl;
}

void test_callback_startAt() {
    std::cout << "=== Testing startAt Callbacks ===" << std::endl;
    
    Timer timer;
    auto start_time = std::chrono::steady_clock::now();
    auto target_time = start_time + std::chrono::milliseconds(100);
    
    std::cout << "Scheduling callback for 100ms from now..." << std::endl;
    
    auto future = timer.startAt(target_time, [start_time]() {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<Timer::duration_ms>(now - start_time);
        std::cout << "startAt callback executed after " 
                  << std::fixed << std::setprecision(1) << elapsed.count() << "ms" << std::endl;
    });
    
    future.wait();
    std::cout << std::endl;
}

void test_callback_every_infinite() {
    std::cout << "=== Testing every() Infinite Callbacks ===" << std::endl;
    
    Timer timer;
    std::atomic<int> callback_count{0};
    auto start_time = std::chrono::steady_clock::now();
    
    auto future = timer.every(Timer::duration_ms(30), [&callback_count, start_time]() {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<Timer::duration_ms>(now - start_time);
        callback_count++;
        std::cout << "Every callback #" << callback_count.load() 
                  << " at " << std::fixed << std::setprecision(1) << elapsed.count() << "ms" << std::endl;
    });
    
    // Let it run for a while
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    
    std::cout << "Stopping infinite callbacks..." << std::endl;
    // Future will be destroyed here, but callbacks continue until timer destructor
    
    std::cout << std::endl;
}

void test_callback_every_limited() {
    std::cout << "=== Testing every() Limited Callbacks ===" << std::endl;
    
    Timer timer;
    std::atomic<int> callback_count{0};
    auto start_time = std::chrono::steady_clock::now();
    
    auto future = timer.every(Timer::duration_ms(40), [&callback_count, start_time]() {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<Timer::duration_ms>(now - start_time);
        callback_count++;
        std::cout << "Limited callback #" << callback_count.load() 
                  << " at " << std::fixed << std::setprecision(1) << elapsed.count() << "ms" << std::endl;
    }, 3); // Only 3 iterations
    
    future.wait();
    std::cout << "Limited callbacks completed. Total: " << callback_count.load() << std::endl;
    std::cout << std::endl;
}

void test_wait_functions() {
    std::cout << "=== Testing Wait Functions ===" << std::endl;
    
    Timer timer;
    auto start_time = std::chrono::steady_clock::now();
    
    std::cout << "Testing waitFor(50ms)..." << std::endl;
    timer.waitFor(Timer::duration_ms(50));
    auto elapsed = std::chrono::duration_cast<Timer::duration_ms>(
        std::chrono::steady_clock::now() - start_time);
    printDuration("Actual wait time", elapsed);
    
    start_time = std::chrono::steady_clock::now();
    auto target_time = start_time + std::chrono::milliseconds(75);
    
    std::cout << "\nTesting waitUntil(75ms from now)..." << std::endl;
    timer.waitUntil(target_time);
    elapsed = std::chrono::duration_cast<Timer::duration_ms>(
        std::chrono::steady_clock::now() - start_time);
    printDuration("Actual wait time", elapsed);
    
    std::cout << std::endl;
}

void test_concurrent_operations() {
    std::cout << "=== Testing Concurrent Operations ===" << std::endl;
    
    Timer timer;
    std::atomic<int> operations_completed{0};
    std::vector<std::thread> threads;
    
    // Thread 1: Start/stop operations
    threads.emplace_back([&timer, &operations_completed]() {
        for (int i = 0; i < 10; ++i) {
            timer.start(Timer::duration_ms(50));
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            timer.stop();
            operations_completed++;
        }
    });
    
    // Thread 2: Reset operations
    threads.emplace_back([&timer, &operations_completed]() {
        for (int i = 0; i < 10; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            timer.reset();
            operations_completed++;
        }
    });
    
    // Thread 3: Status queries
    threads.emplace_back([&timer, &operations_completed]() {
        for (int i = 0; i < 20; ++i) {
            timer.isRunning();
            timer.hasTimedOut();
            timer.elapsed();
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            operations_completed++;
        }
    });
    
    // Thread 4: Timeout setting
    threads.emplace_back([&timer, &operations_completed]() {
        for (int i = 0; i < 10; ++i) {
            timer.setTimeout(Timer::duration_ms(i * 10 + 20));
            timer.getTimeout();
            std::this_thread::sleep_for(std::chrono::milliseconds(3));
            operations_completed++;
        }
    });
    
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Concurrent operations completed: " << operations_completed.load() << std::endl;
    std::cout << "Final state - Running: " << timer.isRunning() 
              << ", TimedOut: " << timer.hasTimedOut() << std::endl;
    std::cout << std::endl;
}

void test_edge_cases() {
    std::cout << "=== Testing Edge Cases ===" << std::endl;
    
    Timer timer;
    
    // Test very short timeout
    std::cout << "Testing very short timeout (1ms)..." << std::endl;
    timer.start(Timer::duration_ms(1));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    timer.update();
    std::cout << "Running: " << timer.isRunning() << std::endl;
    std::cout << "Timed out: " << timer.hasTimedOut() << std::endl;
    
    timer.stop();
    
    // Test zero timeout
    std::cout << "\nTesting zero timeout..." << std::endl;
    timer.start(Timer::duration_ms(0));
    timer.update();
    std::cout << "Running: " << timer.isRunning() << std::endl;
    std::cout << "Timed out: " << timer.hasTimedOut() << std::endl;
    
    // Test operations on stopped timer
    std::cout << "\nTesting operations on stopped timer..." << std::endl;
    timer.stop();
    timer.reset(); // Should do nothing harmful
    timer.update(); // Should do nothing harmful
    printDuration("Elapsed when stopped", timer.elapsed());
    
    // Test callback with exception
    std::cout << "\nTesting callback with exception..." << std::endl;
    try {
        auto future = timer.startAfter(Timer::duration_ms(10), []() {
            throw std::runtime_error("Test exception");
        });
        future.wait();
    } catch (const std::exception& e) {
        std::cout << "Caught exception in callback: " << e.what() << std::endl;
    }
    
    std::cout << std::endl;
}

void test_performance() {
    std::cout << "=== Testing Performance ===" << std::endl;
    
    Timer timer;
    const int iterations = 1000;
    
    auto start = std::chrono::steady_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        timer.start();
        timer.elapsed();
        timer.isRunning();
        timer.hasTimedOut();
        timer.stop();
    }
    
    auto end = std::chrono::steady_clock::now();
    auto total_time = std::chrono::duration_cast<Timer::duration_ms>(end - start);
    
    std::cout << "Performed " << iterations << " timer operations in " 
              << std::fixed << std::setprecision(3) << total_time.count() << "ms" << std::endl;
    std::cout << "Average time per operation: " 
              << std::fixed << std::setprecision(6) << (total_time.count() / iterations) << "ms" << std::endl;
    
    std::cout << std::endl;
}

int main() {
    std::cout << "Timer Class Comprehensive Test Suite" << std::endl;
    std::cout << "====================================" << std::endl;
    
    try {
        test_basic_functionality();
        test_timeout_functionality();
        test_reset_functionality();
        test_callback_startAfter();
        test_callback_startAt();
        test_callback_every_limited();
        test_wait_functions();
        test_concurrent_operations();
        test_edge_cases();
        test_performance();
        
        // Note: test_callback_every_infinite() is commented out to prevent infinite execution
        // Uncomment if you want to test infinite callbacks (remember to kill the process)
        
        std::cout << "=== All Tests Completed Successfully ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}