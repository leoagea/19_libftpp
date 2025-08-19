#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <sstream>
#include "../libftpp.hpp"

// Original test - kept as is
void printNumbers(const std::string& p_prefix) {
	threadSafeCout.setPrefix(p_prefix);
	for (int i = 1; i <= 5; ++i) {
		threadSafeCout << "Number: " << i << std::endl;
	}
}

// Test 1: Multi-line output with prefix consistency
void testMultiLineOutput(int threadId) {
	threadSafeCout.setPrefix("[ML-" + std::to_string(threadId) + "] ");
	threadSafeCout << "Starting multi-line test..." << std::endl;
	threadSafeCout << "Line 1 of output\nLine 2 of output\nLine 3 of output" << std::endl;
	threadSafeCout << "Test completed for thread " << threadId << std::endl;
}

// Test 2: Mixed data types output
void testMixedDataTypes(int threadId) {
	threadSafeCout.setPrefix("[MIX-" + std::to_string(threadId) + "] ");
	
	int intVal = 42 + threadId;
	float floatVal = 3.14f * threadId;
	char charVal = 'A' + threadId;
	bool boolVal = (threadId % 2 == 0);
	
	threadSafeCout << "Thread " << threadId << " data:" << std::endl;
	threadSafeCout << "Int: " << intVal << ", Float: " << floatVal << std::endl;
	threadSafeCout << "Char: " << charVal << ", Bool: " << boolVal << std::endl;
}

// Test 3: Rapid output stress test
void testRapidOutput(int threadId) {
	threadSafeCout.setPrefix("[RAPID-" + std::to_string(threadId) + "] ");
	for (int i = 0; i < 10; ++i) {
		threadSafeCout << "Rapid message " << i << " from thread " << threadId << std::endl;
		// Small delay to create interleaving
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

// Test 4: Prompt functionality test (interactive - commented out for automated testing)
void testPromptFunctionality(int threadId) {
	threadSafeCout.setPrefix("[PROMPT-" + std::to_string(threadId) + "] ");
	threadSafeCout << "Thread " << threadId << " would prompt for input here" << std::endl;
	
	// Uncomment for interactive testing:
	
	// int userInput;
	// threadSafeCout.prompt("Enter a number for thread " + std::to_string(threadId) + ":", userInput);
	// threadSafeCout << "You entered: " << userInput << std::endl;
}

// Test 5: Long text output
void testLongTextOutput(int threadId) {
	threadSafeCout.setPrefix("[LONG-" + std::to_string(threadId) + "] ");
	
	std::string longText = "This is a very long line of text that might wrap around "
						  "and we want to ensure that our thread-safe iostream handles "
						  "it correctly without breaking the prefix functionality. "
						  "Thread ID: " + std::to_string(threadId);
	
	threadSafeCout << longText << std::endl;
}

// Test 6: No prefix test
void testNoPrefix(int threadId) {
	// Intentionally don't set a prefix
	threadSafeCout << "Thread " << threadId << " output without prefix" << std::endl;
	threadSafeCout << "Another line without prefix from thread " << threadId << std::endl;
}

// Test 7: Changing prefix mid-execution
void testChangingPrefix(int threadId) {
	threadSafeCout.setPrefix("[CHANGE-" + std::to_string(threadId) + "-A] ");
	threadSafeCout << "First message with prefix A" << std::endl;
	
	threadSafeCout.setPrefix("[CHANGE-" + std::to_string(threadId) + "-B] ");
	threadSafeCout << "Second message with prefix B" << std::endl;
	
	threadSafeCout.setPrefix("[CHANGE-" + std::to_string(threadId) + "-C] ");
	threadSafeCout << "Third message with prefix C" << std::endl;
}

// Test 8: Empty and special character output
void testSpecialCharacters(int threadId) {
	threadSafeCout.setPrefix("[SPECIAL-" + std::to_string(threadId) + "] ");
	
	threadSafeCout << "Testing special chars: !@#$%^&*()_+-={}[]|\\:;\"'<>,.?/" << std::endl;
	threadSafeCout << "Testing empty string: " << "" << " (between colons)" << std::endl;
	threadSafeCout << "Testing tab:\tand newline characters" << std::endl;
}

// Test runner functions
void runOriginalTest() {
	std::cout << "\n=== ORIGINAL TEST ===\n" << std::endl;
	
	std::string prefix1 = "[Thread 1] ";
	std::string prefix2 = "[Thread 2] ";

	std::thread thread1(printNumbers, prefix1);
	std::thread thread2(printNumbers, prefix2);

	thread1.join();
	thread2.join();
}

void runMultiLineTest() {
	std::cout << "\n=== MULTI-LINE OUTPUT TEST ===\n" << std::endl;
	
	std::vector<std::thread> threads;
	for (int i = 1; i <= 3; ++i) {
		threads.emplace_back(testMultiLineOutput, i);
	}
	
	for (auto& t : threads) {
		t.join();
	}
}

void runMixedDataTypesTest() {
	std::cout << "\n=== MIXED DATA TYPES TEST ===\n" << std::endl;
	
	std::vector<std::thread> threads;
	for (int i = 1; i <= 2; ++i) {
		threads.emplace_back(testMixedDataTypes, i);
	}
	
	for (auto& t : threads) {
		t.join();
	}
}

void runRapidOutputTest() {
	std::cout << "\n=== RAPID OUTPUT STRESS TEST ===\n" << std::endl;
	
	std::vector<std::thread> threads;
	for (int i = 1; i <= 4; ++i) {
		threads.emplace_back(testRapidOutput, i);
	}
	
	for (auto& t : threads) {
		t.join();
	}
}

void runLongTextTest() {
	std::cout << "\n=== LONG TEXT OUTPUT TEST ===\n" << std::endl;
	
	std::vector<std::thread> threads;
	for (int i = 1; i <= 2; ++i) {
		threads.emplace_back(testLongTextOutput, i);
	}
	
	for (auto& t : threads) {
		t.join();
	}
}

void runNoPrefixTest() {
	std::cout << "\n=== NO PREFIX TEST ===\n" << std::endl;
	
	std::vector<std::thread> threads;
	for (int i = 1; i <= 2; ++i) {
		threads.emplace_back(testNoPrefix, i);
	}
	
	for (auto& t : threads) {
		t.join();
	}
}

void runChangingPrefixTest() {
	std::cout << "\n=== CHANGING PREFIX TEST ===\n" << std::endl;
	
	std::vector<std::thread> threads;
	for (int i = 1; i <= 2; ++i) {
		threads.emplace_back(testChangingPrefix, i);
	}
	
	for (auto& t : threads) {
		t.join();
	}
}

void runSpecialCharactersTest() {
	std::cout << "\n=== SPECIAL CHARACTERS TEST ===\n" << std::endl;
	
	std::vector<std::thread> threads;
	for (int i = 1; i <= 2; ++i) {
		threads.emplace_back(testSpecialCharacters, i);
	}
	
	for (auto& t : threads) {
		t.join();
	}
}

void runPromptTest() {
	std::cout << "\n=== PROMPT FUNCTIONALITY TEST ===\n" << std::endl;
	
	std::vector<std::thread> threads;
	for (int i = 1; i <= 2; ++i) {
		threads.emplace_back(testPromptFunctionality, i);
	}
	
	for (auto& t : threads) {
		t.join();
	}
}

int main() {
	std::cout << "Starting ThreadSafeIOStream comprehensive tests...\n" << std::endl;
	
	// Run all tests
	runOriginalTest();
	runMultiLineTest();
	runMixedDataTypesTest();
	runRapidOutputTest();
	runLongTextTest();
	runNoPrefixTest();
	runChangingPrefixTest();
	runSpecialCharactersTest();
	runPromptTest();
	
	std::cout << "\n=== ALL TESTS COMPLETED ===\n" << std::endl;
	
	return 0;
}