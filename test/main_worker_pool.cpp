#include "../libftpp.hpp"
#include <iostream>
#include <atomic>
#include <chrono>
#include <vector>
#include <random>

// Global counters for testing
std::atomic<int> jobCounter{0};
std::atomic<int> completedJobs{0};
std::atomic<int> errorCount{0};

void test_original() {
    std::cout << "\n=== Original Test - 1000 Simple Jobs ===" << std::endl;
    
    jobCounter.store(0);
    completedJobs.store(0);
    
    WorkerPool pool(4);

    auto job = []() {
        threadSafeCout << "Executing job on thread: " << std::this_thread::get_id() << std::endl;
        completedJobs.fetch_add(1);
    };

    for (int i = 0; i < 1000; ++i) {
        pool.addJob(job);
    }

    std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for jobs to finish
    
    std::cout << "Completed jobs: " << completedJobs.load() << std::endl;
}

void test_variable_duration_jobs() {
    std::cout << "\n=== Variable Duration Jobs Test ===" << std::endl;
    
    completedJobs.store(0);
    WorkerPool pool(3);

    // Fast jobs
    for (int i = 0; i < 10; ++i) {
        pool.addJob([i]() {
            threadSafeCout << "Fast job " << i << " on thread: " << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            completedJobs.fetch_add(1);
        });
    }

    // Medium jobs
    for (int i = 0; i < 5; ++i) {
        pool.addJob([i]() {
            threadSafeCout << "Medium job " << i << " on thread: " << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            completedJobs.fetch_add(1);
        });
    }

    // Slow jobs
    for (int i = 0; i < 3; ++i) {
        pool.addJob([i]() {
            threadSafeCout << "Slow job " << i << " on thread: " << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            completedJobs.fetch_add(1);
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Variable duration jobs completed: " << completedJobs.load() << std::endl;
}

void test_computational_jobs() {
    std::cout << "\n=== Computational Jobs Test ===" << std::endl;
    
    completedJobs.store(0);
    WorkerPool pool(4);

    // Mathematical computation jobs
    for (int jobId = 0; jobId < 8; ++jobId) {
        pool.addJob([jobId]() {
            threadSafeCout << "Starting computation job " << jobId 
                          << " on thread: " << std::this_thread::get_id() << std::endl;
            
            double result = 0.0;
            for (int i = 1; i <= 100000; ++i) {
                result += std::sin(i) * std::cos(i) / (i + 1);
            }
            
            threadSafeCout << "Computation job " << jobId 
                          << " completed with result: " << result << std::endl;
            completedJobs.fetch_add(1);
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Computational jobs completed: " << completedJobs.load() << std::endl;
}

void test_batch_processing() {
    std::cout << "\n=== Batch Processing Test ===" << std::endl;
    
    completedJobs.store(0);
    WorkerPool pool(4);

    // Simulate processing files in batches
    std::vector<std::string> files = {
        "data1.txt", "data2.txt", "data3.txt", "config.json", 
        "log1.txt", "log2.txt", "backup.zip", "image1.png",
        "image2.png", "document.pdf", "script.sh", "database.db"
    };

    for (size_t i = 0; i < files.size(); ++i) {
        pool.addJob([i, &files]() {
            threadSafeCout << "Processing file: " << files[i] 
                          << " on thread: " << std::this_thread::get_id() << std::endl;
            
            // Simulate file processing time based on "file type"
            std::string file = files[i];
            int processingTime = 100; // base time
            
            // C++17 compatible way to check file extensions
            if ((file.size() >= 4 && file.substr(file.size() - 4) == ".png") ||
                (file.size() >= 4 && file.substr(file.size() - 4) == ".zip")) {
                processingTime = 300; // images and archives take longer
            } else if (file.size() >= 3 && file.substr(file.size() - 3) == ".db") {
                processingTime = 500; // database operations are slow
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(processingTime));
            
            threadSafeCout << "Completed processing: " << files[i] << std::endl;
            completedJobs.fetch_add(1);
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "Batch processing completed: " << completedJobs.load() << " files" << std::endl;
}

void test_producer_consumer_pattern() {
    std::cout << "\n=== Producer-Consumer Pattern Test ===" << std::endl;
    
    completedJobs.store(0);
    std::atomic<int> producedItems{0};
    WorkerPool pool(2); // 2 workers for processing

    // Producer thread that adds jobs continuously
    std::thread producer([&]() {
        for (int i = 0; i < 20; ++i) {
            pool.addJob([i, &producedItems]() {
                threadSafeCout << "Consuming item " << i 
                              << " on thread: " << std::this_thread::get_id() << std::endl;
                
                // Simulate item processing
                std::this_thread::sleep_for(std::chrono::milliseconds(150));
                
                threadSafeCout << "Finished consuming item " << i << std::endl;
                completedJobs.fetch_add(1);
            });
            
            producedItems.fetch_add(1);
            threadSafeCout << "Producer added item " << i << " (total: " << producedItems.load() << ")" << std::endl;
            
            // Producer adds items at intervals
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    producer.join();
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Let workers finish
    
    std::cout << "Produced: " << producedItems.load() << ", Consumed: " << completedJobs.load() << std::endl;
}

void test_different_pool_sizes() {
    std::cout << "\n=== Different Pool Sizes Test ===" << std::endl;
    
    const int jobsPerTest = 50;
    
    // Test with 1 worker
    {
        std::cout << "Testing with 1 worker..." << std::endl;
        completedJobs.store(0);
        auto start = std::chrono::high_resolution_clock::now();
        
        WorkerPool pool(1);
        for (int i = 0; i < jobsPerTest; ++i) {
            pool.addJob([i]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
                completedJobs.fetch_add(1);
            });
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "1 worker: " << completedJobs.load() << " jobs in " << duration.count() << "ms" << std::endl;
    }
    
    // Test with 4 workers
    {
        std::cout << "Testing with 4 workers..." << std::endl;
        completedJobs.store(0);
        auto start = std::chrono::high_resolution_clock::now();
        
        WorkerPool pool(4);
        for (int i = 0; i < jobsPerTest; ++i) {
            pool.addJob([i]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
                completedJobs.fetch_add(1);
            });
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "4 workers: " << completedJobs.load() << " jobs in " << duration.count() << "ms" << std::endl;
    }

    // Test with 8 workers
    {
        std::cout << "Testing with 8 workers..." << std::endl;
        completedJobs.store(0);
        auto start = std::chrono::high_resolution_clock::now();
        
        WorkerPool pool(8);
        for (int i = 0; i < jobsPerTest; ++i) {
            pool.addJob([i]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
                completedJobs.fetch_add(1);
            });
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "8 workers: " << completedJobs.load() << " jobs in " << duration.count() << "ms" << std::endl;
    }
}

void test_concurrent_job_addition() {
    std::cout << "\n=== Concurrent Job Addition Test ===" << std::endl;
    
    completedJobs.store(0);
    std::atomic<int> addedJobs{0};
    WorkerPool pool(3);

    // Multiple threads adding jobs concurrently
    std::vector<std::thread> adders;
    
    for (int threadId = 0; threadId < 3; ++threadId) {
        adders.emplace_back([&pool, threadId, &addedJobs]() {
            for (int i = 0; i < 15; ++i) {
                pool.addJob([threadId, i]() {
                    threadSafeCout << "Job from adder-thread " << threadId 
                                  << ", job " << i << " executing on: " 
                                  << std::this_thread::get_id() << std::endl;
                    
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    completedJobs.fetch_add(1);
                });
                
                addedJobs.fetch_add(1);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        });
    }

    // Wait for all adders to finish
    for (auto& adder : adders) {
        adder.join();
    }

    // Wait for workers to complete jobs
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    std::cout << "Added jobs: " << addedJobs.load() << ", Completed: " << completedJobs.load() << std::endl;
}

void test_mixed_workload() {
    std::cout << "\n=== Mixed Workload Test ===" << std::endl;
    
    completedJobs.store(0);
    WorkerPool pool(4);

    // Add a mix of different job types
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> workTypeDist(1, 4);
    std::uniform_int_distribution<> durationDist(50, 300);

    for (int i = 0; i < 30; ++i) {
        int workType = workTypeDist(gen);
        int duration = durationDist(gen);
        
        pool.addJob([i, workType, duration]() {
            std::string jobType;
            switch (workType) {
                case 1: jobType = "CPU-intensive"; break;
                case 2: jobType = "I/O simulation"; break;
                case 3: jobType = "Memory operation"; break;
                case 4: jobType = "Network simulation"; break;
            }
            
            threadSafeCout << "Job " << i << " (" << jobType << ") starting on thread: " 
                          << std::this_thread::get_id() << std::endl;
            
            // Simulate different types of work
            if (workType == 1) {
                // CPU intensive
                volatile double result = 0;
                for (int j = 0; j < 10000; ++j) {
                    result += std::sqrt(j) * std::sin(j);
                }
            } else {
                // I/O or waiting simulation
                std::this_thread::sleep_for(std::chrono::milliseconds(duration));
            }
            
            threadSafeCout << "Job " << i << " (" << jobType << ") completed" << std::endl;
            completedJobs.fetch_add(1);
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "Mixed workload completed: " << completedJobs.load() << " jobs" << std::endl;
}

void test_stress_test() {
    std::cout << "\n=== Stress Test - High Job Volume ===" << std::endl;
    
    completedJobs.store(0);
    WorkerPool pool(6);

    const int totalJobs = 5000;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < totalJobs; ++i) {
        pool.addJob([i]() {
            // Very minimal work per job
            volatile int result = i * 2 + 1;
            (void)result; // Suppress unused variable warning
            completedJobs.fetch_add(1);
        });
    }

    // Wait for completion
    while (completedJobs.load() < totalJobs && 
           std::chrono::duration_cast<std::chrono::seconds>(
               std::chrono::high_resolution_clock::now() - start).count() < 10) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Progress: " << completedJobs.load() << "/" << totalJobs << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Stress test completed: " << completedJobs.load() << "/" << totalJobs 
              << " jobs in " << duration.count() << "ms" << std::endl;
    std::cout << "Throughput: " << (completedJobs.load() * 1000.0 / duration.count()) 
              << " jobs/second" << std::endl;
}

int main() {
    std::cout << "Starting comprehensive WorkerPool testing..." << std::endl;
    
    test_original();
    test_variable_duration_jobs();
    test_computational_jobs();
    test_batch_processing();
    test_producer_consumer_pattern();
    test_different_pool_sizes();
    test_concurrent_job_addition();
    test_mixed_workload();
    test_stress_test();
    
    std::cout << "\nAll WorkerPool tests completed!" << std::endl;
    return 0;
}
