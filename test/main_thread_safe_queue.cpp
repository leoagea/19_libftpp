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

void testSizeAndEmpty(ThreadSafeQueue<int>& p_queue, int threadId) {
	threadSafeCout.setPrefix("[SIZE-EMPTY-" + std::to_string(threadId) + "] ");
	
	threadSafeCout << "Testing size and empty methods..." << std::endl;
	
	// Test initial state
	threadSafeCout << "Initial size: " << p_queue.size() << ", empty: " << (p_queue.empty() ? "true" : "false") << std::endl;
	
	// Add some elements and check size/empty
	for (int i = 1; i <= 5; ++i) {
		int value = threadId * 100 + i;
		p_queue.push_back(value);
		threadSafeCout << "After push_back(" << value << "): size=" << p_queue.size() 
					  << ", empty=" << (p_queue.empty() ? "true" : "false") << std::endl;
		
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	
	// Remove some elements and check size/empty
	for (int i = 1; i <= 3; ++i) {
		try {
			int value = p_queue.pop_front();
			threadSafeCout << "After pop_front(" << value << "): size=" << p_queue.size() 
						  << ", empty=" << (p_queue.empty() ? "true" : "false") << std::endl;
		} catch (const std::exception& e) {
			threadSafeCout << "Pop failed: " << e.what() << std::endl;
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void testClearMethod(ThreadSafeQueue<int>& p_queue, int threadId) {
	threadSafeCout.setPrefix("[CLEAR-" + std::to_string(threadId) + "] ");
	
	threadSafeCout << "Testing clear method..." << std::endl;
	
	// Add elements first
	threadSafeCout << "Adding elements before clear test..." << std::endl;
	for (int i = 1; i <= 10; ++i) {
		int value = threadId * 1000 + i;
		p_queue.push_back(value);
	}
	
	threadSafeCout << "Before clear: size=" << p_queue.size() 
				  << ", empty=" << (p_queue.empty() ? "true" : "false") << std::endl;
	
	// Clear the queue
	p_queue.clear();
	
	threadSafeCout << "After clear: size=" << p_queue.size() 
				  << ", empty=" << (p_queue.empty() ? "true" : "false") << std::endl;
	
	// Verify queue is actually empty by trying to pop
	try {
		int value = p_queue.pop_front();
		threadSafeCout << "ERROR: Successfully popped " << value << " from cleared queue!" << std::endl;
	} catch (const std::exception& e) {
		threadSafeCout << "Confirmed empty after clear: " << e.what() << std::endl;
	}
	
	// Test that queue is still functional after clear
	threadSafeCout << "Testing functionality after clear..." << std::endl;
	p_queue.push_back(threadId * 2000);
	threadSafeCout << "After adding element post-clear: size=" << p_queue.size() 
				  << ", empty=" << (p_queue.empty() ? "true" : "false") << std::endl;
}

void concurrentSizeMonitor(ThreadSafeQueue<int>& p_queue, int threadId, int durationSeconds) {
	threadSafeCout.setPrefix("[MONITOR-" + std::to_string(threadId) + "] ");
	
	threadSafeCout << "Starting size monitoring for " << durationSeconds << " seconds..." << std::endl;
	
	auto startTime = std::chrono::steady_clock::now();
	auto endTime = startTime + std::chrono::seconds(durationSeconds);
	
	size_t lastSize = 0;
	int reportCount = 0;
	
	while (std::chrono::steady_clock::now() < endTime) {
		size_t currentSize = p_queue.size();
		bool isEmpty = p_queue.empty();
		
		if (currentSize != lastSize || reportCount % 50 == 0) {
			threadSafeCout << "Monitor report " << ++reportCount << ": size=" << currentSize 
						  << ", empty=" << (isEmpty ? "true" : "false") << std::endl;
			lastSize = currentSize;
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	
	threadSafeCout << "Monitoring completed. Final size: " << p_queue.size() << std::endl;
}

void concurrentClearTest(ThreadSafeQueue<int>& p_queue, int threadId) {
	threadSafeCout.setPrefix("[CLEAR-CONCURRENT-" + std::to_string(threadId) + "] ");
	
	threadSafeCout << "Starting concurrent clear test..." << std::endl;
	
	// Periodically clear the queue while other operations are happening
	for (int i = 0; i < 3; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		
		size_t sizeBefore = p_queue.size();
		p_queue.clear();
		size_t sizeAfter = p_queue.size();
		
		threadSafeCout << "Clear " << (i + 1) << ": size before=" << sizeBefore 
					  << ", size after=" << sizeAfter << std::endl;
	}
}

void sizePrecisionProducer(ThreadSafeQueue<int>& p_queue, int threadId, int itemCount) {
	threadSafeCout.setPrefix("[SIZE-PROD-" + std::to_string(threadId) + "] ");
	
	threadSafeCout << "Producing " << itemCount << " items for size precision test..." << std::endl;
	
	for (int i = 0; i < itemCount; ++i) {
		int value = threadId * 10000 + i;
		p_queue.push_back(value);
		
		if (i % 20 == 0) {
			threadSafeCout << "Produced " << (i + 1) << "/" << itemCount 
						  << ", current size: " << p_queue.size() << std::endl;
		}
		
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}
	
	threadSafeCout << "Finished producing. Final size: " << p_queue.size() << std::endl;
}

void sizePrecisionConsumer(ThreadSafeQueue<int>& p_queue, int threadId, int targetCount) {
	threadSafeCout.setPrefix("[SIZE-CONS-" + std::to_string(threadId) + "] ");
	
	threadSafeCout << "Consuming up to " << targetCount << " items for size precision test..." << std::endl;
	
	int consumedCount = 0;
	
	while (consumedCount < targetCount) {
		try {
			int value = p_queue.pop_front();
			consumedCount++;
			
			if (consumedCount % 20 == 0) {
				threadSafeCout << "Consumed " << consumedCount << "/" << targetCount 
							  << ", current size: " << p_queue.size() << std::endl;
			}
			
		} catch (const std::exception& e) {
			// Queue might be empty, wait a bit
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	}
	
	threadSafeCout << "Finished consuming " << consumedCount << " items. Final size: " << p_queue.size() << std::endl;
}

void testEmptyEdgeCases(ThreadSafeQueue<int>& p_queue, int threadId) {
	threadSafeCout.setPrefix("[EMPTY-EDGE-" + std::to_string(threadId) + "] ");
	
	threadSafeCout << "Testing empty queue edge cases..." << std::endl;
	
	// Test multiple clears
	p_queue.clear();
	p_queue.clear();
	p_queue.clear();
	threadSafeCout << "After multiple clears: size=" << p_queue.size() 
				  << ", empty=" << (p_queue.empty() ? "true" : "false") << std::endl;
	
	// Test size/empty after failed operations
	for (int i = 0; i < 3; ++i) {
		try {
			p_queue.pop_front();
		} catch (const std::exception&) {
			// Expected
		}
		
		try {
			p_queue.pop_back();
		} catch (const std::exception&) {
			// Expected
		}
		
		threadSafeCout << "After failed ops " << (i + 1) << ": size=" << p_queue.size() 
					  << ", empty=" << (p_queue.empty() ? "true" : "false") << std::endl;
	}
	
	// Test rapid add/remove cycles
	for (int cycle = 0; cycle < 5; ++cycle) {
		p_queue.push_back(threadId * 100 + cycle);
		threadSafeCout << "Cycle " << cycle << " after push: size=" << p_queue.size() << std::endl;
		
		try {
			int value = p_queue.pop_front();
			threadSafeCout << "Cycle " << cycle << " after pop(" << value << "): size=" << p_queue.size() << std::endl;
		} catch (const std::exception& e) {
			threadSafeCout << "Cycle " << cycle << " pop failed: " << e.what() << std::endl;
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

void runSizeEmptyTest() {
	threadSafeCout.setPrefix("[SIZE-EMPTY-TEST] ");
	threadSafeCout << "\n=== SIZE AND EMPTY METHODS TEST ===\n" << std::endl;
	
	ThreadSafeQueue<int> queue;
	
	std::vector<std::thread> threads;
	
	// Test size and empty with multiple threads
	for (int i = 1; i <= 3; ++i) {
		threads.emplace_back(testSizeAndEmpty, std::ref(queue), i);
	}
	
	for (auto& t : threads) {
		t.join();
	}
}

void runClearTest() {
	threadSafeCout.setPrefix("[CLEAR-TEST] ");
	threadSafeCout << "\n=== CLEAR METHOD TEST ===\n" << std::endl;
	
	ThreadSafeQueue<int> queue;
	
	std::vector<std::thread> threads;
	
	// Test clear functionality
	for (int i = 1; i <= 2; ++i) {
		threads.emplace_back(testClearMethod, std::ref(queue), i);
	}
	
	for (auto& t : threads) {
		t.join();
	}
}

void runConcurrentMonitoringTest() {
	threadSafeCout.setPrefix("[CONCURRENT-MONITOR] ");
	threadSafeCout << "\n=== CONCURRENT SIZE MONITORING TEST ===\n" << std::endl;
	
	ThreadSafeQueue<int> queue;
	
	std::vector<std::thread> threads;
	
	// Start monitoring thread
	threads.emplace_back(concurrentSizeMonitor, std::ref(queue), 1, 3);
	
	// Start producer threads
	for (int i = 1; i <= 2; ++i) {
		threads.emplace_back(stressTestProducer, std::ref(queue), i, 50);
	}
	
	// Start consumer threads
	for (int i = 1; i <= 2; ++i) {
		threads.emplace_back(stressTestConsumer, std::ref(queue), i);
	}
	
	// Start concurrent clear thread
	threads.emplace_back(concurrentClearTest, std::ref(queue), 99);
	
	for (auto& t : threads) {
		t.join();
	}
}

void runSizePrecisionTest() {
	threadSafeCout.setPrefix("[SIZE-PRECISION] ");
	threadSafeCout << "\n=== SIZE PRECISION TEST ===\n" << std::endl;
	
	ThreadSafeQueue<int> queue;
	
	std::vector<std::thread> threads;
	
	// Create producers
	for (int i = 1; i <= 3; ++i) {
		threads.emplace_back(sizePrecisionProducer, std::ref(queue), i, 80);
	}
	
	// Create consumers
	for (int i = 1; i <= 2; ++i) {
		threads.emplace_back(sizePrecisionConsumer, std::ref(queue), i, 60);
	}
	
	// Monitor size changes
	threads.emplace_back(concurrentSizeMonitor, std::ref(queue), 99, 4);
	
	for (auto& t : threads) {
		t.join();
	}
	
	threadSafeCout << "Final queue state: size=" << queue.size() 
				  << ", empty=" << (queue.empty() ? "true" : "false") << std::endl;
}

void runEmptyEdgeCasesTest() {
	threadSafeCout.setPrefix("[EMPTY-EDGE] ");
	threadSafeCout << "\n=== EMPTY QUEUE EDGE CASES TEST ===\n" << std::endl;
	
	ThreadSafeQueue<int> queue;
	
	std::vector<std::thread> threads;
	
	// Test edge cases
	for (int i = 1; i <= 2; ++i) {
		threads.emplace_back(testEmptyEdgeCases, std::ref(queue), i);
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
	runSizeEmptyTest();
	runClearTest();
	runConcurrentMonitoringTest();
	runSizePrecisionTest();
	runEmptyEdgeCasesTest();

	threadSafeCout << "\n=== ALL THREAD SAFE QUEUE TESTS COMPLETED ===\n" << std::endl;
	
	return 0;
}
