#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include "../libftpp.hpp"

// Original test functions - kept as is
void testPush(ThreadSafeQueue<int>& p_queue, int p_value) {
	p_queue.push_back(p_value);
	std::cout << "Pushed value: " << p_value << std::endl;
}

void testPop(ThreadSafeQueue<int>& p_queue) {
	try {
		int value = p_queue.pop_front();
		std::cout << "Popped value: " << value << std::endl;
	} catch (const std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}
}

// Enhanced test functions using ThreadSafeIOStream
void enhancedTestPush(ThreadSafeQueue<int>& p_queue, int p_value, int threadId) {
	threadSafeCout.setPrefix("[PUSH-" + std::to_string(threadId) + "] ");
	
	p_queue.push_back(p_value);
	threadSafeCout << "Successfully pushed value: " << p_value << std::endl;
	
	// Small delay to create realistic threading conditions
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void enhancedTestPop(ThreadSafeQueue<int>& p_queue, int threadId) {
	threadSafeCout.setPrefix("[POP-" + std::to_string(threadId) + "] ");
	
	try {
		int value = p_queue.pop_front();
		threadSafeCout << "Successfully popped value: " << value << std::endl;
	} catch (const std::exception& e) {
		threadSafeCout << "Exception caught: " << e.what() << std::endl;
	}
}

// Test 1: Basic FIFO behavior verification
void testFIFOBehavior(ThreadSafeQueue<int>& p_queue, int threadId) {
	threadSafeCout.setPrefix("[FIFO-" + std::to_string(threadId) + "] ");
	
	threadSafeCout << "Starting FIFO test..." << std::endl;
	
	// Push sequence of values
	for (int i = 1; i <= 5; ++i) {
		int value = threadId * 100 + i;
		p_queue.push_back(value);
		threadSafeCout << "Pushed: " << value << std::endl;
	}
	
	threadSafeCout << "Finished pushing sequence for thread " << threadId << std::endl;
}

void testFIFOConsumer(ThreadSafeQueue<int>& p_queue, int threadId, int expectedCount) {
	threadSafeCout.setPrefix("[CONSUMER-" + std::to_string(threadId) + "] ");
	
	threadSafeCout << "Starting to consume " << expectedCount << " items..." << std::endl;
	
	for (int i = 0; i < expectedCount; ++i) {
		try {
			int value = p_queue.pop_front();
			threadSafeCout << "Consumed item " << (i + 1) << ": " << value << std::endl;
		} catch (const std::exception& e) {
			threadSafeCout << "Failed to consume item " << (i + 1) << ": " << e.what() << std::endl;
			break;
		}
	}
}

// Test 2: High-throughput stress test
void stressTestProducer(ThreadSafeQueue<int>& p_queue, int threadId, int itemCount) {
	threadSafeCout.setPrefix("[PRODUCER-" + std::to_string(threadId) + "] ");
	
	threadSafeCout << "Starting stress test - producing " << itemCount << " items..." << std::endl;
	
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 1000);
	
	for (int i = 0; i < itemCount; ++i) {
		int value = threadId * 10000 + i;
		p_queue.push_back(value);
		
		if (i % 50 == 0) {
			threadSafeCout << "Produced " << (i + 1) << "/" << itemCount << " items" << std::endl;
		}
		
		// Random small delay
		std::this_thread::sleep_for(std::chrono::microseconds(dis(gen) % 100));
	}
	
	threadSafeCout << "Completed producing " << itemCount << " items" << std::endl;
}

void stressTestConsumer(ThreadSafeQueue<int>& p_queue, int threadId) {
	threadSafeCout.setPrefix("[CONSUMER-" + std::to_string(threadId) + "] ");
	
	int consumedCount = 0;
	int consecutiveFailures = 0;
	auto startTime = std::chrono::steady_clock::now();
	auto timeout = std::chrono::seconds(3); // Shorter timeout
	
	threadSafeCout << "Starting stress test consumer..." << std::endl;
	
	while (true) {
		try {
			int value = p_queue.pop_front();
			consumedCount++;
			consecutiveFailures = 0; // Reset failure counter
			
			if (consumedCount % 25 == 0) { // More frequent reporting
				threadSafeCout << "Consumed " << consumedCount << " items so far" << std::endl;
			}
			
		} catch (const std::exception& e) {
			consecutiveFailures++;
			auto currentTime = std::chrono::steady_clock::now();
			
			// Exit if timeout OR too many consecutive failures
			if (currentTime - startTime > timeout || consecutiveFailures > 100) {
				threadSafeCout << "Stopping consumer. Consumed total: " << consumedCount << " items" << std::endl;
				break;
			}
			
			// Shorter sleep to be more responsive
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	}
	
	threadSafeCout << "Final consumed count: " << consumedCount << std::endl;
}

// Test 3: Mixed push_front and push_back operations
void testMixedPushOperations(ThreadSafeQueue<int>& p_queue, int threadId) {
	threadSafeCout.setPrefix("[MIXED-" + std::to_string(threadId) + "] ");
	
	threadSafeCout << "Testing mixed push operations..." << std::endl;
	
	for (int i = 0; i < 5; ++i) {
		if (i % 2 == 0) {
			int value = threadId * 1000 + i;
			p_queue.push_back(value);
			threadSafeCout << "push_back(" << value << ")" << std::endl;
		} else {
			int value = threadId * 1000 + i + 500;
			p_queue.push_front(value);
			threadSafeCout << "push_front(" << value << ")" << std::endl;
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

// Test 4: Empty queue handling
void testEmptyQueueOperations(ThreadSafeQueue<int>& p_queue, int threadId) {
	threadSafeCout.setPrefix("[EMPTY-" + std::to_string(threadId) + "] ");
	
	threadSafeCout << "Testing operations on empty queue..." << std::endl;
	
	// Try multiple pop operations on empty queue
	for (int i = 0; i < 3; ++i) {
		try {
			int value = p_queue.pop_front();
			threadSafeCout << "Unexpected success: popped " << value << std::endl;
		} catch (const std::exception& e) {
			threadSafeCout << "Expected exception " << (i + 1) << ": " << e.what() << std::endl;
		}
		
		try {
			int value = p_queue.pop_back();
			threadSafeCout << "Unexpected success: popped " << value << std::endl;
		} catch (const std::exception& e) {
			threadSafeCout << "Expected exception " << (i + 1) << ": " << e.what() << std::endl;
		}
	}
}

// Test runner functions
void runOriginalTest() {
	threadSafeCout.setPrefix("[ORIGINAL] ");
	threadSafeCout << "\n=== ORIGINAL TEST ===\n" << std::endl;
	
	ThreadSafeQueue<int> myQueue;

	std::thread thread1(testPush, std::ref(myQueue), 10);
	std::thread thread2(testPush, std::ref(myQueue), 20);
	std::thread thread3(testPop, std::ref(myQueue));
	std::thread thread4(testPop, std::ref(myQueue));
	std::thread thread5(testPop, std::ref(myQueue));

	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
	thread5.join();
}

void runEnhancedBasicTest() {
	threadSafeCout.setPrefix("[ENHANCED] ");
	threadSafeCout << "\n=== ENHANCED BASIC TEST ===\n" << std::endl;
	
	ThreadSafeQueue<int> queue;
	
	std::vector<std::thread> threads;
	
	// Create producer threads
	for (int i = 1; i <= 3; ++i) {
		threads.emplace_back(enhancedTestPush, std::ref(queue), i * 10, i);
	}
	
	// Create consumer threads
	for (int i = 1; i <= 4; ++i) {
		threads.emplace_back(enhancedTestPop, std::ref(queue), i);
	}
	
	// Wait for all threads
	for (auto& t : threads) {
		t.join();
	}
}

void runFIFOTest() {
	threadSafeCout.setPrefix("[FIFO-TEST] ");
	threadSafeCout << "\n=== FIFO BEHAVIOR TEST ===\n" << std::endl;
	
	ThreadSafeQueue<int> queue;
	
	std::vector<std::thread> threads;
	
	// Create producer threads
	for (int i = 1; i <= 2; ++i) {
		threads.emplace_back(testFIFOBehavior, std::ref(queue), i);
	}
	
	// Wait for producers to finish
	for (auto& t : threads) {
		t.join();
	}
	
	threads.clear();
	
	// Create consumer threads
	for (int i = 1; i <= 2; ++i) {
		threads.emplace_back(testFIFOConsumer, std::ref(queue), i, 5);
	}
	
	// Wait for consumers
	for (auto& t : threads) {
		t.join();
	}
}

void runStressTest() {
	threadSafeCout.setPrefix("[STRESS] ");
	threadSafeCout << "\n=== STRESS TEST ===\n" << std::endl;
	
	ThreadSafeQueue<int> queue;
	
	std::vector<std::thread> threads;
	
	// Create multiple producer threads
	for (int i = 1; i <= 3; ++i) {
		threads.emplace_back(stressTestProducer, std::ref(queue), i, 100);
	}
	
	// Create consumer threads
	for (int i = 1; i <= 2; ++i) {
		threads.emplace_back(stressTestConsumer, std::ref(queue), i);
	}
	
	// Wait for all threads
	for (auto& t : threads) {
		t.join();
	}
}

void runMixedOperationsTest() {
	threadSafeCout.setPrefix("[MIXED-OPS] ");
	threadSafeCout << "\n=== MIXED OPERATIONS TEST ===\n" << std::endl;
	
	ThreadSafeQueue<int> queue;
	
	std::vector<std::thread> threads;
	
	// Mixed push operations
	for (int i = 1; i <= 3; ++i) {
		threads.emplace_back(testMixedPushOperations, std::ref(queue), i);
	}
	
	// Some consumers
	for (int i = 1; i <= 4; ++i) {
		threads.emplace_back(enhancedTestPop, std::ref(queue), i + 10);
	}
	
	for (auto& t : threads) {
		t.join();
	}
}

void runEmptyQueueTest() {
	threadSafeCout.setPrefix("[EMPTY-QUEUE] ");
	threadSafeCout << "\n=== EMPTY QUEUE TEST ===\n" << std::endl;
	
	ThreadSafeQueue<int> queue;
	
	std::vector<std::thread> threads;
	
	// Test empty queue operations
	for (int i = 1; i <= 2; ++i) {
		threads.emplace_back(testEmptyQueueOperations, std::ref(queue), i);
	}
	
	for (auto& t : threads) {
		t.join();
	}
}

int main() {
	threadSafeCout.setPrefix("[MAIN] ");
	threadSafeCout << "Starting comprehensive ThreadSafeQueue tests...\n" << std::endl;
	
	// Run all tests
	runOriginalTest();
	runEnhancedBasicTest();
	runFIFOTest();
	runStressTest();
	runMixedOperationsTest();
	runEmptyQueueTest();

	threadSafeCout << "\n=== ALL THREAD SAFE QUEUE TESTS COMPLETED ===\n" << std::endl;
	
	return 0;
}
