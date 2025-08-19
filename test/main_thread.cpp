#include <iostream>
#include <chrono>
#include <vector>
#include <atomic>
#include <random>
#include "../libftpp.hpp"

// Global counters for testing
std::atomic<int> globalCounter{0};
std::atomic<int> sharedResource{0};

void myFunction1() {
	for (int i = 0; i < 5; ++i) {
		threadSafeCout << "Hello from Function1, iteration " << i << std::endl;
	}
}

void myFunction2() {
	for (int i = 0; i < 5; ++i) {
		threadSafeCout << "Hello from Function2, iteration " << i << std::endl;
	}
}

void test_original() {
	threadSafeCout << "\n=== Original Test ===" << std::endl;
	
	Thread thread1("Thread1", myFunction1);
	Thread thread2("Thread2", myFunction2);

	thread1.start();
	thread2.start();

	// Let threads run for a bit
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	thread1.stop();
	thread2.stop();
}

void counterTask() {
	for (int i = 0; i < 10; ++i) {
		int current = globalCounter.fetch_add(1);
		threadSafeCout << "Counter incremented to: " << (current + 1) << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void test_atomic_counter() {
	threadSafeCout << "\n=== Atomic Counter Test ===" << std::endl;
	
	globalCounter.store(0);
	
	Thread counter1("Counter1", counterTask);
	Thread counter2("Counter2", counterTask);
	Thread counter3("Counter3", counterTask);

	threadSafeCout << "Starting 3 threads to increment counter..." << std::endl;
	
	counter1.start();
	counter2.start();
	counter3.start();

	// Let them run for a while
	std::this_thread::sleep_for(std::chrono::seconds(2));

	counter1.stop();
	counter2.stop();
	counter3.stop();

	threadSafeCout << "Final counter value: " << globalCounter.load() << std::endl;
}

void producerTask() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 100);
	
	for (int i = 0; i < 8; ++i) {
		int value = dis(gen);
		sharedResource.store(value);
		threadSafeCout << "Produced: " << value << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

void consumerTask() {
	for (int i = 0; i < 8; ++i) {
		int value = sharedResource.load();
		threadSafeCout << "Consumed: " << value << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
	}
}

void test_producer_consumer() {
	threadSafeCout << "\n=== Producer-Consumer Test ===" << std::endl;
	
	sharedResource.store(0);
	
	Thread producer("Producer", producerTask);
	Thread consumer("Consumer", consumerTask);

	threadSafeCout << "Starting producer and consumer threads..." << std::endl;
	
	producer.start();
	consumer.start();

	// Let them run
	std::this_thread::sleep_for(std::chrono::seconds(3));

	producer.stop();
	consumer.stop();
}

void workerTask(int workerId) {
	auto worker = [workerId]() {
		for (int i = 0; i < 5; ++i) {
			threadSafeCout << "Worker " << workerId << " processing task " << i << std::endl;
			
			// Simulate work with variable duration
			std::this_thread::sleep_for(std::chrono::milliseconds(100 + (workerId * 50)));
		}
		threadSafeCout << "Worker " << workerId << " finished all tasks" << std::endl;
	};
	
	worker();
}

void test_multiple_workers() {
	threadSafeCout << "\n=== Multiple Workers Test ===" << std::endl;
	
	std::vector<Thread> workers;
	workers.reserve(4);

	// Create worker threads with lambda capture
	for (int i = 1; i <= 4; ++i) {
		workers.emplace_back("Worker" + std::to_string(i), 
						   [i]() { workerTask(i); });
	}

	threadSafeCout << "Starting 4 worker threads..." << std::endl;
	
	// Start all workers
	for (auto& worker : workers) {
		worker.start();
	}

	// Let them work
	std::this_thread::sleep_for(std::chrono::seconds(3));

	// Stop all workers
	for (auto& worker : workers) {
		worker.stop();
	}
}

void infiniteTask() {
	int counter = 0;
	while (true) {
		threadSafeCout << "Infinite task iteration: " << counter++ << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		
		// Check if we should stop (this depends on your Thread implementation)
		if (counter > 20) {  // Safety limit for demo
			break;
		}
	}
}

void test_thread_lifecycle() {
	threadSafeCout << "\n=== Thread Lifecycle Test ===" << std::endl;
	
	Thread infiniteThread("InfiniteTask", infiniteTask);

	threadSafeCout << "Starting infinite task thread..." << std::endl;
	infiniteThread.start();

	threadSafeCout << "Letting it run for 2 seconds..." << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));

	threadSafeCout << "Stopping infinite task thread..." << std::endl;
	infiniteThread.stop();

	threadSafeCout << "Thread stopped successfully" << std::endl;
}

void heavyComputationTask() {
	threadSafeCout << "Starting heavy computation..." << std::endl;
	
	// Simulate CPU-intensive work
	long long sum = 0;
	for (int i = 0; i < 1000000; ++i) {
		sum += i * i;
		
		// Occasionally report progress
		if (i % 200000 == 0) {
			threadSafeCout << "Computation progress: " << (i / 10000) << "%" << std::endl;
		}
	}
	
	threadSafeCout << "Heavy computation completed. Result: " << sum << std::endl;
}

void lightTask() {
	for (int i = 0; i < 10; ++i) {
		threadSafeCout << "Light task step: " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void test_mixed_workloads() {
	threadSafeCout << "\n=== Mixed Workloads Test ===" << std::endl;
	
	Thread heavyThread("HeavyComputation", heavyComputationTask);
	Thread lightThread1("LightTask1", lightTask);
	Thread lightThread2("LightTask2", lightTask);

	threadSafeCout << "Starting mixed workload threads..." << std::endl;
	
	heavyThread.start();
	lightThread1.start();
	lightThread2.start();

	// Let them run
	std::this_thread::sleep_for(std::chrono::seconds(4));

	heavyThread.stop();
	lightThread1.stop();
	lightThread2.stop();
}

void timeStampTask() {
	for (int i = 0; i < 5; ++i) {
		auto now = std::chrono::system_clock::now();
		auto time_t = std::chrono::system_clock::to_time_t(now);
		
		threadSafeCout << "Timestamp task - iteration " << i 
					  << " at time: " << time_t << std::endl;
		
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void test_thread_synchronization() {
	threadSafeCout << "\n=== Thread Synchronization Test ===" << std::endl;
	
	Thread timeThread1("TimeStamp1", timeStampTask);
	Thread timeThread2("TimeStamp2", timeStampTask);
	Thread timeThread3("TimeStamp3", timeStampTask);

	threadSafeCout << "Testing thread-safe output synchronization..." << std::endl;
	
	timeThread1.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	timeThread2.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	timeThread3.start();

	// Let them run
	std::this_thread::sleep_for(std::chrono::seconds(3));

	timeThread1.stop();
	timeThread2.stop();
	timeThread3.stop();
}

void stressTask() {
	for (int i = 0; i < 50; ++i) {
		threadSafeCout << "Stress test message " << i << " with some longer text to test output handling" << std::endl;
		
		// Very short sleep to create high-frequency output
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void test_stress_output() {
	threadSafeCout << "\n=== Stress Test - High Frequency Output ===" << std::endl;
	
	Thread stress1("Stress1", stressTask);
	Thread stress2("Stress2", stressTask);
	Thread stress3("Stress3", stressTask);

	threadSafeCout << "Starting stress test with high-frequency output..." << std::endl;
	
	stress1.start();
	stress2.start();
	stress3.start();

	// Short run time for stress test
	std::this_thread::sleep_for(std::chrono::seconds(2));

	stress1.stop();
	stress2.stop();
	stress3.stop();

	threadSafeCout << "Stress test completed" << std::endl;
}

void singleRunTask() {
	threadSafeCout << "This task runs once and completes" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	threadSafeCout << "Single run task finished" << std::endl;
}

void userControlledLoopTask() {
	for (int i = 0; i < 3; ++i) {
		threadSafeCout << "User-controlled loop iteration " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	threadSafeCout << "User-controlled loop completed" << std::endl;
}

void conditionalLoopTask() {
	int count = 0;
	while (count < 5) {
		threadSafeCout << "Conditional loop count: " << count << std::endl;
		count++;
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
	}
	threadSafeCout << "Conditional loop finished" << std::endl;
}

void test_user_controlled_execution() {
	threadSafeCout << "\n=== User-Controlled Execution Test ===" << std::endl;
	
	Thread singleRun("SingleRun", singleRunTask);
	Thread userLoop("UserLoop", userControlledLoopTask);
	Thread conditionalLoop("ConditionalLoop", conditionalLoopTask);

	threadSafeCout << "Starting threads with different execution patterns..." << std::endl;
	
	singleRun.start();
	userLoop.start();
	conditionalLoop.start();

	// Give them time to complete their work
	std::this_thread::sleep_for(std::chrono::seconds(2));

	singleRun.stop();
	userLoop.stop();
	conditionalLoop.stop();
}

void batchProcessingTask() {
	threadSafeCout << "Processing batch of 10 items..." << std::endl;
	
	for (int i = 1; i <= 10; ++i) {
		threadSafeCout << "Processing item " << i << "/10" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	
	threadSafeCout << "Batch processing completed!" << std::endl;
}

void quickTask() {
	threadSafeCout << "Quick task: Computing 2+2 = " << (2+2) << std::endl;
}

void mediumTask() {
	threadSafeCout << "Medium task started" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	threadSafeCout << "Medium task completed" << std::endl;
}

void test_different_durations() {
	threadSafeCout << "\n=== Different Task Durations Test ===" << std::endl;
	
	Thread quick("QuickTask", quickTask);
	Thread medium("MediumTask", mediumTask);
	Thread batch("BatchTask", batchProcessingTask);

	threadSafeCout << "Starting tasks with different execution times..." << std::endl;
	
	quick.start();
	medium.start();
	batch.start();

	// Give enough time for all to complete
	std::this_thread::sleep_for(std::chrono::seconds(3));

	quick.stop();
	medium.stop();
	batch.stop();
}

void dataProcessingTask(int dataSize) {
	auto processor = [dataSize]() {
		threadSafeCout << "Processing " << dataSize << " data items..." << std::endl;
		
		for (int i = 0; i < dataSize; ++i) {
			// Simulate data processing
			int result = i * i + i;
			
			if (i % (dataSize / 4) == 0) {  // Report progress 4 times
				threadSafeCout << "Progress: " << i << "/" << dataSize 
							  << " (sample result: " << result << ")" << std::endl;
			}
			
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		
		threadSafeCout << "Data processing completed for " << dataSize << " items" << std::endl;
	};
	
	processor();
}

void test_parameterized_tasks() {
	threadSafeCout << "\n=== Parameterized Tasks Test ===" << std::endl;
	
	Thread smallData("SmallData", [](){ dataProcessingTask(8); });
	Thread mediumData("MediumData", [](){ dataProcessingTask(12); });
	Thread largeData("LargeData", [](){ dataProcessingTask(16); });

	threadSafeCout << "Starting parameterized data processing tasks..." << std::endl;
	
	smallData.start();
	mediumData.start();
	largeData.start();

	// Give them time to complete
	std::this_thread::sleep_for(std::chrono::seconds(4));

	smallData.stop();
	mediumData.stop();
	largeData.stop();
}

void fileSimulationTask() {
	std::vector<std::string> files = {"config.txt", "data.csv", "log.txt", "backup.zip"};
	
	for (const auto& file : files) {
		threadSafeCout << "Simulating processing of " << file << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		threadSafeCout << "Completed processing " << file << std::endl;
	}
	
	threadSafeCout << "All files processed!" << std::endl;
}

void networkSimulationTask() {
	std::vector<std::string> urls = {"api.example.com", "data.service.com", "backup.cloud.com"};
	
	for (const auto& url : urls) {
		threadSafeCout << "Simulating request to " << url << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(400));
		threadSafeCout << "Response received from " << url << std::endl;
	}
	
	threadSafeCout << "All network requests completed!" << std::endl;
}

void test_realistic_scenarios() {
	threadSafeCout << "\n=== Realistic Scenarios Test ===" << std::endl;
	
	Thread fileProcessor("FileProcessor", fileSimulationTask);
	Thread networkClient("NetworkClient", networkSimulationTask);

	threadSafeCout << "Starting realistic file and network processing scenarios..." << std::endl;
	
	fileProcessor.start();
	networkClient.start();

	// Give them time to complete their specific tasks
	std::this_thread::sleep_for(std::chrono::seconds(3));

	fileProcessor.stop();
	networkClient.stop();
}

void mathComputationTask() {
	threadSafeCout << "Starting mathematical computation..." << std::endl;
	
	double result = 0.0;
	for (int i = 1; i <= 1000; ++i) {
		result += 1.0 / (i * i);  // Computing partial sum of π²/6
		
		if (i % 250 == 0) {
			threadSafeCout << "Math progress: " << i << "/1000, current sum: " << result << std::endl;
		}
	}
	
	threadSafeCout << "Mathematical computation completed. Final result: " << result << std::endl;
}

void stringProcessingTask() {
	std::vector<std::string> words = {"hello", "world", "threading", "example", "test", "complete"};
	
	threadSafeCout << "Processing string operations..." << std::endl;
	
	for (const auto& word : words) {
		std::string upper = word;
		std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
		
		threadSafeCout << "Processed: " << word << " -> " << upper 
					  << " (length: " << word.length() << ")" << std::endl;
		
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	
	threadSafeCout << "String processing completed!" << std::endl;
}

void test_computation_tasks() {
	threadSafeCout << "\n=== Computation Tasks Test ===" << std::endl;
	
	Thread mathThread("MathComputation", mathComputationTask);
	Thread stringThread("StringProcessing", stringProcessingTask);

	threadSafeCout << "Starting computational tasks..." << std::endl;
	
	mathThread.start();
	stringThread.start();

	// Give them time to complete their computations
	std::this_thread::sleep_for(std::chrono::seconds(4));

	mathThread.stop();
	stringThread.stop();
}

void statusReportingTask() {
	for (int i = 0; i < 8; ++i) {
		threadSafeCout << "Status reporting task iteration " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

void quickFinishTask() {
	threadSafeCout << "Quick task starting..." << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	threadSafeCout << "Quick task completed!" << std::endl;
}

void longRunningTask() {
	for (int i = 0; i < 20; ++i) {
		threadSafeCout << "Long running task iteration " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}
}

void test_thread_status_methods() {
	threadSafeCout << "\n=== Thread Status Methods Test ===" << std::endl;
	
	Thread statusThread("StatusReporter", statusReportingTask);
	Thread quickThread("QuickFinisher", quickFinishTask);
	Thread longThread("LongRunner", longRunningTask);
	
	// Test initial state (not running)
	threadSafeCout << "\n--- Initial State Test ---" << std::endl;
	threadSafeCout << "Thread '" << statusThread.getName() << "' isRunning: " 
				  << (statusThread.isRunning() ? "true" : "false") << std::endl;
	threadSafeCout << "Thread '" << quickThread.getName() << "' isRunning: " 
				  << (quickThread.isRunning() ? "true" : "false") << std::endl;
	threadSafeCout << "Thread '" << longThread.getName() << "' isRunning: " 
				  << (longThread.isRunning() ? "true" : "false") << std::endl;
	
	// Test thread IDs before starting
	threadSafeCout << "\n--- Thread IDs Before Start ---" << std::endl;
	threadSafeCout << "Thread '" << statusThread.getName() << "' ID: " << statusThread.getThreadId() << std::endl;
	threadSafeCout << "Thread '" << quickThread.getName() << "' ID: " << quickThread.getThreadId() << std::endl;
	threadSafeCout << "Thread '" << longThread.getName() << "' ID: " << longThread.getThreadId() << std::endl;
	
	// Start threads and test running state
	threadSafeCout << "\n--- Starting Threads ---" << std::endl;
	statusThread.start();
	quickThread.start();
	longThread.start();
	
	// Test state immediately after start
	threadSafeCout << "\n--- State After Start ---" << std::endl;
	threadSafeCout << "Thread '" << statusThread.getName() << "' isRunning: " 
				  << (statusThread.isRunning() ? "true" : "false") << std::endl;
	threadSafeCout << "Thread '" << quickThread.getName() << "' isRunning: " 
				  << (quickThread.isRunning() ? "true" : "false") << std::endl;
	threadSafeCout << "Thread '" << longThread.getName() << "' isRunning: " 
				  << (longThread.isRunning() ? "true" : "false") << std::endl;
	
	// Test thread IDs after starting
	threadSafeCout << "\n--- Thread IDs After Start ---" << std::endl;
	threadSafeCout << "Thread '" << statusThread.getName() << "' ID: " << statusThread.getThreadId() << std::endl;
	threadSafeCout << "Thread '" << quickThread.getName() << "' ID: " << quickThread.getThreadId() << std::endl;
	threadSafeCout << "Thread '" << longThread.getName() << "' ID: " << longThread.getThreadId() << std::endl;
	
	// Monitor status changes over time
	threadSafeCout << "\n--- Monitoring Status Changes ---" << std::endl;
	for (int i = 0; i < 10; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		
		threadSafeCout << "Monitor " << i << " - StatusReporter: " 
					  << (statusThread.isRunning() ? "running" : "stopped")
					  << ", QuickFinisher: " 
					  << (quickThread.isRunning() ? "running" : "stopped")
					  << ", LongRunner: " 
					  << (longThread.isRunning() ? "running" : "stopped") << std::endl;
	}
	
	// Stop threads and test final state
	threadSafeCout << "\n--- Stopping Threads ---" << std::endl;
	statusThread.stop();
	quickThread.stop();
	longThread.stop();
	
	threadSafeCout << "\n--- Final State After Stop ---" << std::endl;
	threadSafeCout << "Thread '" << statusThread.getName() << "' isRunning: " 
				  << (statusThread.isRunning() ? "true" : "false") << std::endl;
	threadSafeCout << "Thread '" << quickThread.getName() << "' isRunning: " 
				  << (quickThread.isRunning() ? "true" : "false") << std::endl;
	threadSafeCout << "Thread '" << longThread.getName() << "' isRunning: " 
				  << (longThread.isRunning() ? "true" : "false") << std::endl;
}

void nameTestTask(int id) {
	auto task = [id]() {
		for (int i = 0; i < 5; ++i) {
			threadSafeCout << "Name test task " << id << " iteration " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(150));
		}
	};
	task();
}

void test_thread_names() {
	threadSafeCout << "\n=== Thread Names Test ===" << std::endl;
	
	// Test various naming patterns
	std::vector<Thread> namedThreads;
	namedThreads.reserve(6);
	
	namedThreads.emplace_back("Worker-1", []() { nameTestTask(1); });
	namedThreads.emplace_back("DataProcessor", []() { nameTestTask(2); });
	namedThreads.emplace_back("NetworkClient", []() { nameTestTask(3); });
	namedThreads.emplace_back("FileHandler_Main", []() { nameTestTask(4); });
	namedThreads.emplace_back("Background-Service-001", []() { nameTestTask(5); });
	namedThreads.emplace_back("TempThread", []() { nameTestTask(6); });
	
	// Display all thread names before starting
	threadSafeCout << "\n--- Thread Names Before Start ---" << std::endl;
	for (size_t i = 0; i < namedThreads.size(); ++i) {
		threadSafeCout << "Thread " << i << " name: '" << namedThreads[i].getName() 
					  << "', running: " << (namedThreads[i].isRunning() ? "true" : "false") << std::endl;
	}
	
	// Start all threads
	threadSafeCout << "\n--- Starting Named Threads ---" << std::endl;
	for (auto& thread : namedThreads) {
		thread.start();
		threadSafeCout << "Started thread: '" << thread.getName() << "' with ID: " << thread.getThreadId() << std::endl;
	}
	
	// Monitor while running
	threadSafeCout << "\n--- Monitoring Named Threads ---" << std::endl;
	for (int i = 0; i < 5; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		
		threadSafeCout << "Status check " << i << ":" << std::endl;
		for (const auto& thread : namedThreads) {
			threadSafeCout << "  '" << thread.getName() << "' (ID: " << thread.getThreadId() << "): " 
						  << (thread.isRunning() ? "running" : "stopped") << std::endl;
		}
	}
	
	// Stop all threads
	threadSafeCout << "\n--- Stopping Named Threads ---" << std::endl;
	for (auto& thread : namedThreads) {
		thread.stop();
		threadSafeCout << "Stopped thread: '" << thread.getName() << "'" << std::endl;
	}
}

void threadIdAnalysisTask(int duration) {
	auto task = [duration]() {
		threadSafeCout << "Thread ID analysis task running for " << duration << "ms" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(duration));
		threadSafeCout << "Thread ID analysis task completed" << std::endl;
	};
	task();
}

void test_thread_ids() {
	threadSafeCout << "\n=== Thread IDs Test ===" << std::endl;
	
	std::vector<Thread> idTestThreads;
	idTestThreads.reserve(5);
	
	// Create threads with different durations
	idTestThreads.emplace_back("ID-Test-1", []() { threadIdAnalysisTask(500); });
	idTestThreads.emplace_back("ID-Test-2", []() { threadIdAnalysisTask(800); });
	idTestThreads.emplace_back("ID-Test-3", []() { threadIdAnalysisTask(300); });
	idTestThreads.emplace_back("ID-Test-4", []() { threadIdAnalysisTask(1000); });
	idTestThreads.emplace_back("ID-Test-5", []() { threadIdAnalysisTask(200); });
	
	// Test IDs before starting (should be invalid/default)
	threadSafeCout << "\n--- Thread IDs Before Start ---" << std::endl;
	for (size_t i = 0; i < idTestThreads.size(); ++i) {
		threadSafeCout << "Thread '" << idTestThreads[i].getName() 
					  << "' ID before start: " << idTestThreads[i].getThreadId() << std::endl;
	}
	
	// Start threads and check IDs
	threadSafeCout << "\n--- Starting Threads and Checking IDs ---" << std::endl;
	for (auto& thread : idTestThreads) {
		thread.start();
		threadSafeCout << "Started '" << thread.getName() 
					  << "' - ID: " << thread.getThreadId() 
					  << ", Running: " << (thread.isRunning() ? "true" : "false") << std::endl;
		
		// Small delay between starts to see ID assignment
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	
	// Monitor thread completion by ID
	threadSafeCout << "\n--- Monitoring Thread Completion by ID ---" << std::endl;
	std::vector<bool> threadCompleted(idTestThreads.size(), false);
	
	for (int check = 0; check < 15; ++check) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		
		bool anyRunning = false;
		for (size_t i = 0; i < idTestThreads.size(); ++i) {
			bool isRunning = idTestThreads[i].isRunning();
			if (isRunning) {
				anyRunning = true;
			}
			
			if (!isRunning && !threadCompleted[i]) {
				threadSafeCout << "Thread '" << idTestThreads[i].getName() 
							  << "' (ID: " << idTestThreads[i].getThreadId() 
							  << ") completed naturally" << std::endl;
				threadCompleted[i] = true;
			}
		}
		
		if (!anyRunning) {
			threadSafeCout << "All threads completed naturally" << std::endl;
			break;
		}
	}
	
	// Stop any remaining threads
	threadSafeCout << "\n--- Stopping Remaining Threads ---" << std::endl;
	for (auto& thread : idTestThreads) {
		if (thread.isRunning()) {
			threadSafeCout << "Force stopping '" << thread.getName() 
						  << "' (ID: " << thread.getThreadId() << ")" << std::endl;
			thread.stop();
		}
	}
	
	// Final ID check
	threadSafeCout << "\n--- Final Thread ID Status ---" << std::endl;
	for (const auto& thread : idTestThreads) {
		threadSafeCout << "Thread '" << thread.getName() 
					  << "' final ID: " << thread.getThreadId() 
					  << ", running: " << (thread.isRunning() ? "true" : "false") << std::endl;
	}
}

void lifecycleTestTask() {
	for (int i = 0; i < 6; ++i) {
		threadSafeCout << "Lifecycle test iteration " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
}

void test_lifecycle_state_changes() {
	threadSafeCout << "\n=== Thread Lifecycle State Changes Test ===" << std::endl;
	
	Thread lifecycleThread("LifecycleTest", lifecycleTestTask);
	
	auto printStatus = [&](const std::string& stage) {
		threadSafeCout << stage << " - Name: '" << lifecycleThread.getName() 
					  << "', ID: " << lifecycleThread.getThreadId() 
					  << ", Running: " << (lifecycleThread.isRunning() ? "true" : "false") << std::endl;
	};
	
	// Stage 1: Initial state
	printStatus("STAGE 1 - Initial");
	
	// Stage 2: Start thread
	threadSafeCout << "\n--- Starting Thread ---" << std::endl;
	lifecycleThread.start();
	printStatus("STAGE 2 - After Start");
	
	// Stage 3: Monitor while running
	for (int i = 0; i < 8; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		threadSafeCout << "Monitor " << i << " - Running: " 
					  << (lifecycleThread.isRunning() ? "true" : "false") << std::endl;
	}
	
	// Stage 4: Stop thread
	threadSafeCout << "\n--- Stopping Thread ---" << std::endl;
	lifecycleThread.stop();
	printStatus("STAGE 4 - After Stop");
	
	// Stage 5: Restart thread (if supported)
	threadSafeCout << "\n--- Attempting Restart ---" << std::endl;
	lifecycleThread.start();
	printStatus("STAGE 5 - After Restart");
	
	// Let it run briefly
	std::this_thread::sleep_for(std::chrono::milliseconds(800));
	
	// Final stop
	lifecycleThread.stop();
	printStatus("STAGE 6 - Final Stop");
}

void concurrentStatusTask(int taskId) {
	auto task = [taskId]() {
		for (int i = 0; i < 5; ++i) {
			threadSafeCout << "Concurrent status task " << taskId << " iteration " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(100 + (taskId * 50)));
		}
	};
	task();
}

void test_concurrent_status_queries() {
	threadSafeCout << "\n=== Concurrent Status Queries Test ===" << std::endl;
	
	std::vector<Thread> statusThreads;
	statusThreads.reserve(4);
	
	// Create multiple threads
	for (int i = 1; i <= 4; ++i) {
		statusThreads.emplace_back("StatusQuery-" + std::to_string(i), 
								 [i]() { concurrentStatusTask(i); });
	}
	
	// Start all threads
	threadSafeCout << "\n--- Starting Multiple Threads ---" << std::endl;
	for (auto& thread : statusThreads) {
		thread.start();
	}
	
	// Continuously query status from multiple threads
	threadSafeCout << "\n--- Concurrent Status Monitoring ---" << std::endl;
	std::vector<std::thread> queryThreads;
	
	// Create status query threads
	for (int queryId = 1; queryId <= 2; ++queryId) {
		queryThreads.emplace_back([&statusThreads, queryId]() {
			for (int i = 0; i < 15; ++i) {
				threadSafeCout << "Query-" << queryId << " report " << i << ":" << std::endl;
				
				for (const auto& thread : statusThreads) {
					threadSafeCout << "  Query-" << queryId << " sees '" << thread.getName() 
								  << "' (ID: " << thread.getThreadId() << "): " 
								  << (thread.isRunning() ? "running" : "stopped") << std::endl;
				}
				
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
			}
		});
	}
	
	// Let monitoring run
	std::this_thread::sleep_for(std::chrono::seconds(3));
	
	// Stop worker threads
	threadSafeCout << "\n--- Stopping Worker Threads ---" << std::endl;
	for (auto& thread : statusThreads) {
		thread.stop();
	}
	
	// Wait for query threads to finish
	for (auto& queryThread : queryThreads) {
		queryThread.join();
	}
	
	threadSafeCout << "Concurrent status queries test completed" << std::endl;
}

// Update your main function to include the new tests:
int main() {
	threadSafeCout << "Starting comprehensive thread testing..." << std::endl;
	
	test_original();
	test_atomic_counter();
	test_producer_consumer();
	test_multiple_workers();
	test_thread_lifecycle();
	test_mixed_workloads();
	test_thread_synchronization();
	test_stress_output();

	test_user_controlled_execution();
	test_different_durations();
	test_parameterized_tasks();
	test_realistic_scenarios();
	test_computation_tasks();

	test_thread_status_methods();
	test_thread_names();
	test_thread_ids();
	test_lifecycle_state_changes();
	test_concurrent_status_queries();
	
	threadSafeCout << "\nAll thread tests completed!" << std::endl;
	threadSafeCout << "Final global counter value: " << globalCounter.load() << std::endl;
	
	return 0;
}