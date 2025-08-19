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

// Test class that implements IJobs interface
class SimpleTestJob : public WorkerPool::IJobs {
private:
	int jobId;
	std::string jobName;
	
public:
	SimpleTestJob(int id, const std::string& name) : jobId(id), jobName(name) {}
	
	void execute() override {
		threadSafeCout << "SimpleTestJob " << jobId << " (" << jobName 
					  << ") executing on thread: " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		completedJobs.fetch_add(1);
	}
};

class ComputationTestJob : public WorkerPool::IJobs {
private:
	int iterations;
	std::string taskName;
	
public:
	ComputationTestJob(int iter, const std::string& name) : iterations(iter), taskName(name) {}
	
	void execute() override {
		threadSafeCout << "ComputationTestJob (" << taskName << ") starting with " 
					  << iterations << " iterations on thread: " << std::this_thread::get_id() << std::endl;
		
		double result = 0.0;
		for (int i = 0; i < iterations; ++i) {
			result += std::sin(i) * std::cos(i);
		}
		
		threadSafeCout << "ComputationTestJob (" << taskName << ") completed. Result: " 
					  << result << std::endl;
		completedJobs.fetch_add(1);
	}
};

class FileProcessingJob : public WorkerPool::IJobs {
private:
	std::string filename;
	size_t fileSize;
	
public:
	FileProcessingJob(const std::string& name, size_t size) : filename(name), fileSize(size) {}
	
	void execute() override {
		threadSafeCout << "Processing file: " << filename << " (size: " << fileSize 
					  << " bytes) on thread: " << std::this_thread::get_id() << std::endl;
		
		// Simulate processing time based on file size
		int processingTime = static_cast<int>(fileSize / 1000) + 50; // 1ms per KB + base time
		std::this_thread::sleep_for(std::chrono::milliseconds(processingTime));
		
		threadSafeCout << "Completed processing file: " << filename << std::endl;
		completedJobs.fetch_add(1);
	}
};

// Test different addJob method overloads
void test_addjob_overloads() {
	std::cout << "\n=== AddJob Method Overloads Test ===" << std::endl;
	
	completedJobs.store(0);
	WorkerPool pool(4);

	std::cout << "\n--- Testing std::function<void()> (by value) ---" << std::endl;
	
	// Test 1: std::function<void()> by value
	std::function<void()> func1 = []() {
		threadSafeCout << "Function by value executing on thread: " 
					  << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		completedJobs.fetch_add(1);
	};
	
	for (int i = 0; i < 3; ++i) {
		pool.addJob(func1);
	}

	std::cout << "\n--- Testing const std::function<void()>& (by const reference) ---" << std::endl;
	
	// Test 2: const std::function<void()>& by const reference
	const std::function<void()> constFunc = []() {
		threadSafeCout << "Function by const reference executing on thread: " 
					  << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		completedJobs.fetch_add(1);
	};
	
	for (int i = 0; i < 3; ++i) {
		pool.addJob(constFunc);
	}

	std::cout << "\n--- Testing std::unique_ptr<IJobs> ---" << std::endl;
	
	// Test 3: std::unique_ptr<IJobs>
	for (int i = 0; i < 3; ++i) {
		auto job = std::make_unique<SimpleTestJob>(i, "UniquePtr-Job-" + std::to_string(i));
		pool.addJob(std::move(job));
	}

	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "AddJob overloads test completed: " << completedJobs.load() << " jobs" << std::endl;
}

void test_ijobs_interface() {
	std::cout << "\n=== IJobs Interface Test ===" << std::endl;
	
	completedJobs.store(0);
	WorkerPool pool(3);

	std::cout << "\n--- Testing SimpleTestJob ---" << std::endl;
	
	// Test SimpleTestJob
	for (int i = 0; i < 5; ++i) {
		auto job = std::make_unique<SimpleTestJob>(i, "SimpleJob-" + std::to_string(i));
		pool.addJob(std::move(job));
	}

	std::cout << "\n--- Testing ComputationTestJob ---" << std::endl;
	
	// Test ComputationTestJob
	std::vector<int> computationSizes = {1000, 5000, 10000};
	for (size_t i = 0; i < computationSizes.size(); ++i) {
		auto job = std::make_unique<ComputationTestJob>(
			computationSizes[i], 
			"Computation-" + std::to_string(i)
		);
		pool.addJob(std::move(job));
	}

	std::cout << "\n--- Testing FileProcessingJob ---" << std::endl;
	
	// Test FileProcessingJob
	std::vector<std::pair<std::string, size_t>> files = {
		{"document.pdf", 50000},
		{"image.png", 150000},
		{"video.mp4", 500000},
		{"config.json", 5000}
	};
	
	for (const auto& file : files) {
		auto job = std::make_unique<FileProcessingJob>(file.first, file.second);
		pool.addJob(std::move(job));
	}

	std::this_thread::sleep_for(std::chrono::seconds(4));
	std::cout << "IJobs interface test completed: " << completedJobs.load() << " jobs" << std::endl;
}

void test_mixed_job_types() {
	std::cout << "\n=== Mixed Job Types Test ===" << std::endl;
	
	completedJobs.store(0);
	WorkerPool pool(4);

	// Mix all three types of jobs
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> jobTypeDist(1, 3);

	for (int i = 0; i < 20; ++i) {
		int jobType = jobTypeDist(gen);
		
		switch (jobType) {
			case 1: {
				// Lambda function job
				std::function<void()> lambdaJob = [i]() {
					threadSafeCout << "Lambda job " << i << " executing on thread: " 
								  << std::this_thread::get_id() << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(80));
					completedJobs.fetch_add(1);
				};
				pool.addJob(lambdaJob);
				break;
			}
			case 2: {
				// Const reference function job
				const std::function<void()> constJob = [i]() {
					threadSafeCout << "Const ref job " << i << " executing on thread: " 
								  << std::this_thread::get_id() << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					completedJobs.fetch_add(1);
				};
				pool.addJob(constJob);
				break;
			}
			case 3: {
				// IJobs interface job
				auto job = std::make_unique<SimpleTestJob>(i, "Mixed-IJobs-" + std::to_string(i));
				pool.addJob(std::move(job));
				break;
			}
		}
		
		// Small delay between job additions
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	std::this_thread::sleep_for(std::chrono::seconds(3));
	std::cout << "Mixed job types test completed: " << completedJobs.load() << " jobs" << std::endl;
}

void test_concurrent_job_addition_overloads() {
	std::cout << "\n=== Concurrent Job Addition with All Overloads Test ===" << std::endl;
	
	completedJobs.store(0);
	std::atomic<int> addedJobs{0};
	WorkerPool pool(4);

	// Multiple threads adding different types of jobs concurrently
	std::vector<std::thread> adders;
	
	// Thread 1: Adding function jobs by value
	adders.emplace_back([&pool, &addedJobs]() {
		for (int i = 0; i < 10; ++i) {
			std::function<void()> job = [i]() {
				threadSafeCout << "Function-by-value job " << i << " on thread: " 
							  << std::this_thread::get_id() << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				completedJobs.fetch_add(1);
			};
			pool.addJob(job);
			addedJobs.fetch_add(1);
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
	});
	
	// Thread 2: Adding function jobs by const reference
	adders.emplace_back([&pool, &addedJobs]() {
		for (int i = 0; i < 10; ++i) {
			const std::function<void()> constJob = [i]() {
				threadSafeCout << "Function-by-const-ref job " << i << " on thread: " 
							  << std::this_thread::get_id() << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(120));
				completedJobs.fetch_add(1);
			};
			pool.addJob(constJob);
			addedJobs.fetch_add(1);
			std::this_thread::sleep_for(std::chrono::milliseconds(40));
		}
	});
	
	// Thread 3: Adding IJobs interface jobs
	adders.emplace_back([&pool, &addedJobs]() {
		for (int i = 0; i < 8; ++i) {
			auto job = std::make_unique<SimpleTestJob>(i, "Concurrent-IJobs-" + std::to_string(i));
			pool.addJob(std::move(job));
			addedJobs.fetch_add(1);
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	});

	// Wait for all adders to finish
	for (auto& adder : adders) {
		adder.join();
	}

	// Wait for workers to complete jobs
	std::this_thread::sleep_for(std::chrono::seconds(4));
	
	std::cout << "Concurrent addition test - Added: " << addedJobs.load() 
			  << ", Completed: " << completedJobs.load() << std::endl;
}

// Complex job for polymorphism testing
class DatabaseJob : public WorkerPool::IJobs {
private:
	std::string operation;
	int recordCount;
	
public:
	DatabaseJob(const std::string& op, int records) : operation(op), recordCount(records) {}
	
	void execute() override {
		threadSafeCout << "Database " << operation << " operation with " << recordCount 
					  << " records on thread: " << std::this_thread::get_id() << std::endl;
		
		// Simulate database operation time
		int operationTime = recordCount * 2; // 2ms per record
		std::this_thread::sleep_for(std::chrono::milliseconds(operationTime));
		
		threadSafeCout << "Completed database " << operation << " operation" << std::endl;
		completedJobs.fetch_add(1);
	}
};

class NetworkJob : public WorkerPool::IJobs {
private:
	std::string endpoint;
	int dataSize;
	
public:
	NetworkJob(const std::string& ep, int size) : endpoint(ep), dataSize(size) {}
	
	void execute() override {
		threadSafeCout << "Network request to " << endpoint << " (" << dataSize 
					  << " bytes) on thread: " << std::this_thread::get_id() << std::endl;
		
		// Simulate network latency
		std::this_thread::sleep_for(std::chrono::milliseconds(150 + (dataSize / 1000)));
		
		threadSafeCout << "Completed network request to " << endpoint << std::endl;
		completedJobs.fetch_add(1);
	}
};

void test_polymorphic_jobs() {
	std::cout << "\n=== Polymorphic Jobs Test ===" << std::endl;
	
	completedJobs.store(0);
	WorkerPool pool(3);

	// Mix different types of IJobs implementations
	std::vector<std::unique_ptr<WorkerPool::IJobs>> jobs;
	
	// Create various job types
	jobs.push_back(std::make_unique<DatabaseJob>("SELECT", 100));
	jobs.push_back(std::make_unique<NetworkJob>("api.example.com/users", 5000));
	jobs.push_back(std::make_unique<SimpleTestJob>(1, "PolymorphicTest"));
	jobs.push_back(std::make_unique<ComputationTestJob>(2000, "PolymorphicComputation"));
	jobs.push_back(std::make_unique<DatabaseJob>("INSERT", 50));
	jobs.push_back(std::make_unique<NetworkJob>("cdn.example.com/images", 15000));
	jobs.push_back(std::make_unique<FileProcessingJob>("report.xlsx", 75000));

	// Add all jobs to the pool
	for (auto& job : jobs) {
		pool.addJob(std::move(job));
	}

	std::this_thread::sleep_for(std::chrono::seconds(3));
	std::cout << "Polymorphic jobs test completed: " << completedJobs.load() << " jobs" << std::endl;
}

void test_performance_comparison() {
	std::cout << "\n=== Performance Comparison Between Job Types ===" << std::endl;
	
	const int jobsPerType = 100;
	
	// Test lambda functions performance
	{
		std::cout << "\nTesting lambda function performance..." << std::endl;
		completedJobs.store(0);
		auto start = std::chrono::high_resolution_clock::now();
		
		WorkerPool pool(4);
		for (int i = 0; i < jobsPerType; ++i) {
			std::function<void()> job = [i]() {
				volatile int result = i * 2;
				(void)result;
				completedJobs.fetch_add(1);
			};
			pool.addJob(job);
		}
		
		while (completedJobs.load() < jobsPerType) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout << "Lambda functions: " << completedJobs.load() << " jobs in " 
				  << duration.count() << "ms" << std::endl;
	}
	
	// Test IJobs interface performance
	{
		std::cout << "\nTesting IJobs interface performance..." << std::endl;
		completedJobs.store(0);
		auto start = std::chrono::high_resolution_clock::now();
		
		WorkerPool pool(4);
		for (int i = 0; i < jobsPerType; ++i) {
			auto job = std::make_unique<SimpleTestJob>(i, "PerfTest");
			pool.addJob(std::move(job));
		}
		
		while (completedJobs.load() < jobsPerType) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout << "IJobs interface: " << completedJobs.load() << " jobs in " 
				  << duration.count() << "ms" << std::endl;
	}
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

	test_addjob_overloads();
	test_ijobs_interface();
	test_mixed_job_types();
	test_concurrent_job_addition_overloads();
	
	test_polymorphic_jobs();
	test_performance_comparison();
	
	std::cout << "\nAll WorkerPool tests completed!" << std::endl;
	return 0;
}

