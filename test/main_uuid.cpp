#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <sstream>
#include "../libftpp.hpp"

void test_basic_functionality() {
	std::cout << "=== Testing Basic Functionality ===" << std::endl;
	
	// Test default constructor (null Uuid)
	Uuid nullUuid;
	std::cout << "Null Uuid: " << nullUuid << std::endl;
	std::cout << "Is null: " << (nullUuid.isNull() ? "true" : "false") << std::endl;
	std::cout << "Is valid: " << (nullUuid.isValid() ? "true" : "false") << std::endl;
	
	// Test static null factory
	Uuid staticNull = Uuid::null();
	std::cout << "Static null: " << staticNull << std::endl;
	std::cout << "Nulls equal: " << (nullUuid == staticNull ? "true" : "false") << std::endl;
	
	// Test V4 generation
	Uuid uuid4 = Uuid::generate();
	std::cout << "Generated V4: " << uuid4 << std::endl;
	std::cout << "Version: " << uuid4.version() << std::endl;
	std::cout << "Variant: " << uuid4.variant() << std::endl;
	std::cout << "Is valid: " << (uuid4.isValid() ? "true" : "false") << std::endl;
	
	// Test V1 generation
	Uuid uuid1 = Uuid::generateV1();
	std::cout << "Generated V1: " << uuid1 << std::endl;
	std::cout << "Version: " << uuid1.version() << std::endl;
	std::cout << "Variant: " << uuid1.variant() << std::endl;
	std::cout << "Is valid: " << (uuid1.isValid() ? "true" : "false") << std::endl;
	
	std::cout << std::endl;
}

void test_string_functionality() {
	std::cout << "=== Testing String Functionality ===" << std::endl;
	
	Uuid uuid = Uuid::generate();
	
	// Test toString variants
	std::string lowercase = uuid.toString();
	std::string uppercase = uuid.toString(true);
	std::string mixedcase = uuid.toString(false);
	
	std::cout << "Original:   " << uuid << std::endl;
	std::cout << "Lowercase:  " << lowercase << std::endl;
	std::cout << "Uppercase:  " << uppercase << std::endl;
	std::cout << "Mixed case: " << mixedcase << std::endl;
	
	// Test string parsing
	try {
		Uuid fromString(lowercase);
		std::cout << "Parsed Uuid: " << fromString << std::endl;
		std::cout << "Strings match: " << (uuid == fromString ? "true" : "false") << std::endl;
		
		// Test with uppercase
		Uuid fromUpper(uppercase);
		std::cout << "From upper:  " << fromUpper << std::endl;
		std::cout << "Upper match: " << (uuid == fromUpper ? "true" : "false") << std::endl;
		
	} catch (const std::exception& e) {
		std::cout << "Error parsing Uuid: " << e.what() << std::endl;
	}
	
	// Test invalid string
	try {
		Uuid invalid("invalid-uuid-string");
		std::cout << "ERROR: Should have thrown exception!" << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Correctly caught invalid Uuid: " << e.what() << std::endl;
	}
	
	std::cout << std::endl;
}

void test_comparison_operators() {
	std::cout << "=== Testing Comparison Operators ===" << std::endl;
	
	Uuid uuid1 = Uuid::generate();
	Uuid uuid2 = Uuid::generate();
	Uuid uuid1_copy = Uuid(uuid1.toString());
	
	std::cout << "Uuid 1: " << uuid1 << std::endl;
	std::cout << "Uuid 2: " << uuid2 << std::endl;
	std::cout << "Uuid 1 copy: " << uuid1_copy << std::endl;
	
	// Test equality
	std::cout << "uuid1 == uuid1_copy: " << (uuid1 == uuid1_copy ? "true" : "false") << std::endl;
	std::cout << "uuid1 == uuid2: " << (uuid1 == uuid2 ? "true" : "false") << std::endl;
	std::cout << "uuid1 != uuid2: " << (uuid1 != uuid2 ? "true" : "false") << std::endl;
	
	// Test ordering
	std::cout << "uuid1 < uuid2: " << (uuid1 < uuid2 ? "true" : "false") << std::endl;
	std::cout << "uuid1 > uuid2: " << (uuid1 > uuid2 ? "true" : "false") << std::endl;
	std::cout << "uuid1 <= uuid1_copy: " << (uuid1 <= uuid1_copy ? "true" : "false") << std::endl;
	std::cout << "uuid1 >= uuid1_copy: " << (uuid1 >= uuid1_copy ? "true" : "false") << std::endl;
	
	std::cout << std::endl;
}

void test_container_support() {
	std::cout << "=== Testing Container Support ===" << std::endl;
	
	// Test std::set (requires operator<)
	std::set<Uuid> uuidSet;
	for (int i = 0; i < 5; ++i) {
		uuidSet.insert(Uuid::generate());
	}
	
	std::cout << "Set contents (" << uuidSet.size() << " elements):" << std::endl;
	for (const auto& uuid : uuidSet) {
		std::cout << "  " << uuid << std::endl;
	}
	
	// Test std::unordered_set (requires hash function)
	std::unordered_set<Uuid> uuidHashSet;
	for (int i = 0; i < 5; ++i) {
		uuidHashSet.insert(Uuid::generate());
	}
	
	std::cout << "Hash set contents (" << uuidHashSet.size() << " elements):" << std::endl;
	for (const auto& uuid : uuidHashSet) {
		std::cout << "  " << uuid << " (hash: " << uuid.hash() << ")" << std::endl;
	}
	
	// Test std::unordered_map
	std::unordered_map<Uuid, std::string> uuidMap;
	uuidMap[Uuid::generate()] = "First Uuid";
	uuidMap[Uuid::generate()] = "Second Uuid";
	uuidMap[Uuid::generate()] = "Third Uuid";
	
	std::cout << "Map contents (" << uuidMap.size() << " elements):" << std::endl;
	for (const auto& pair : uuidMap) {
		std::cout << "  " << pair.first << " -> " << pair.second << std::endl;
	}
	
	std::cout << std::endl;
}

void test_uniqueness() {
	std::cout << "=== Testing Uuid Uniqueness ===" << std::endl;
	
	const int count = 1000;
	std::unordered_set<Uuid> generatedUuids;
	
	auto start = std::chrono::steady_clock::now();
	
	for (int i = 0; i < count; ++i) {
		generatedUuids.insert(Uuid::generate());
	}
	
	auto end = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	
	std::cout << "Generated " << count << " UUIDs in " 
			  << duration.count() << " microseconds" << std::endl;
	std::cout << "Unique UUIDs: " << generatedUuids.size() << std::endl;
	std::cout << "Duplicates: " << (count - generatedUuids.size()) << std::endl;
	std::cout << "Average time per Uuid: " 
			  << std::fixed << std::setprecision(3) 
			  << (double)duration.count() / count << " microseconds" << std::endl;
	
	// Test V1 uniqueness (should be sequential)
	std::cout << "\nV1 Uuid sequential test:" << std::endl;
	for (int i = 0; i < 5; ++i) {
		Uuid v1 = Uuid::generateV1();
		std::cout << "V1 #" << i << ": " << v1 << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	
	std::cout << std::endl;
}

void test_thread_safety() {
	std::cout << "=== Testing Thread Safety ===" << std::endl;
	
	const int numThreads = 4;
	const int uuidsPerThread = 250;
	std::vector<std::thread> threads;
	std::vector<std::vector<Uuid>> results(numThreads);
	
	auto start = std::chrono::steady_clock::now();
	
	// Launch threads
	for (int t = 0; t < numThreads; ++t) {
		threads.emplace_back([t, uuidsPerThread, &results]() {
			results[t].reserve(uuidsPerThread);
			for (int i = 0; i < uuidsPerThread; ++i) {
				results[t].push_back(Uuid::generate());
			}
		});
	}
	
	// Wait for all threads
	for (auto& thread : threads) {
		thread.join();
	}
	
	auto end = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	
	// Check for duplicates across all threads
	std::vector<Uuid> allUuids;
	int totalGenerated = 0;
	
	for (auto& threadResults : results) {
		totalGenerated += threadResults.size();
		for (auto& uuid : threadResults) {
			allUuids.push_back(std::move(uuid));
		}
	}
	
	std::cout << "Generated " << totalGenerated << " UUIDs across " 
			  << numThreads << " threads" << std::endl;
	std::cout << "Unique UUIDs: " << allUuids.size() << std::endl;
	std::cout << "Duplicates: " << (totalGenerated - allUuids.size()) << std::endl;
	std::cout << "Time: " << duration.count() << " microseconds" << std::endl;
	std::cout << "Average per Uuid: " 
			  << std::fixed << std::setprecision(3)
			  << (double)duration.count() / totalGenerated << " microseconds" << std::endl;
	
	std::sort(allUuids.begin(), allUuids.end());
	auto unique_end = std::unique(allUuids.begin(), allUuids.end());
	size_t unique_count = std::distance(allUuids.begin(), unique_end);
	
	std::cout << "Unique UUIDs: " << unique_count << std::endl;
	std::cout << "Duplicates: " << (totalGenerated - unique_count) << std::endl;
	std::cout << std::endl;
}

void test_edge_cases() {
	std::cout << "=== Testing Edge Cases ===" << std::endl;
	
	// Test move semantics
	Uuid original = Uuid::generate();
	std::cout << "Original: " << original << std::endl;
	
	Uuid moved = std::move(original);
	std::cout << "Moved: " << moved << std::endl;
	std::cout << "Original after move: " << original << std::endl;
	
	// Test copy semantics
	Uuid moved_again = std::move(moved);
	std::cout << "Copied: " << moved_again << std::endl;
	std::cout << "Copy == Moved: " << (moved_again == moved ? "true" : "false") << std::endl;
	
	// Test data access
	auto data = moved.data();
	std::cout << "Raw data (first 4 bytes): ";
	for (int i = 0; i < 4; ++i) {
		std::cout << std::hex << std::setw(2) << std::setfill('0') 
				  << static_cast<int>(data[i]) << " ";
	}
	std::cout << std::dec << std::endl;
	
	// Test setData
	std::array<uint8_t, 16> customData;
	customData.fill(0xFF);
	customData[6] = 0x4F;  // Version 4
	customData[8] = 0x8F;  // Variant
	
	Uuid custom;
	custom.setData(customData);
	std::cout << "Custom data Uuid: " << custom << std::endl;
	std::cout << "Custom is valid: " << (custom.isValid() ? "true" : "false") << std::endl;
	
	// Test invalid string formats
	std::vector<std::string> invalidStrings = {
		"invalid",
		"550e8400-e29b-41d4-a716-44665544000",  // Too short
		"550e8400-e29b-41d4-a716-4466554400000", // Too long
		"550e8400-e29b-41d4-a716-44665544000g",  // Invalid hex
		"550e8400e29b41d4a716446655440000",      // No dashes
	};
	
	for (const auto& invalid : invalidStrings) {
		try {
			Uuid test(invalid);
			std::cout << "ERROR: Should have failed for: " << invalid << std::endl;
		} catch (const std::exception& e) {
			std::cout << "Correctly rejected: " << invalid << std::endl;
		}
	}
	
	std::cout << std::endl;
}

void test_stream_operators() {
	std::cout << "=== Testing Stream Operators ===" << std::endl;
	
	Uuid uuid = Uuid::generate();
	
	// Test output operator
	std::cout << "Stream output: " << uuid << std::endl;
	
	// Test stringstream
	std::stringstream ss;
	ss << uuid;
	std::string streamedString = ss.str();
	std::cout << "Stringstream: " << streamedString << std::endl;
	
	// Test that streamed string can recreate Uuid
	try {
		Uuid recreated(streamedString);
		std::cout << "Recreated: " << recreated << std::endl;
		std::cout << "Match: " << (uuid == recreated ? "true" : "false") << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Error recreating: " << e.what() << std::endl;
	}
	
	std::cout << std::endl;
}

int main() {
	std::cout << "Uuid Class Comprehensive Test Suite" << std::endl;
	std::cout << "===================================" << std::endl;
	
	try {
		test_basic_functionality();
		test_string_functionality();
		test_comparison_operators();
		test_container_support();
		test_uniqueness();
		test_thread_safety();
		test_edge_cases();
		test_stream_operators();
		
		std::cout << "=== All Tests Completed Successfully ===" << std::endl;
		
	} catch (const std::exception& e) {
		std::cerr << "Test failed with exception: " << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}