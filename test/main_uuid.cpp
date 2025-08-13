#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <sstream>
#include "../inc/libftpp.hpp"

void test_basic_functionality() {
    std::cout << "=== Testing Basic Functionality ===" << std::endl;
    
    // Test default constructor (null UUID)
    UUID nullUuid;
    std::cout << "Null UUID: " << nullUuid << std::endl;
    std::cout << "Is null: " << (nullUuid.isNull() ? "true" : "false") << std::endl;
    std::cout << "Is valid: " << (nullUuid.isValid() ? "true" : "false") << std::endl;
    
    // Test static null factory
    UUID staticNull = UUID::null();
    std::cout << "Static null: " << staticNull << std::endl;
    std::cout << "Nulls equal: " << (nullUuid == staticNull ? "true" : "false") << std::endl;
    
    // Test V4 generation
    UUID uuid4 = UUID::generate();
    std::cout << "Generated V4: " << uuid4 << std::endl;
    std::cout << "Version: " << uuid4.version() << std::endl;
    std::cout << "Variant: " << uuid4.variant() << std::endl;
    std::cout << "Is valid: " << (uuid4.isValid() ? "true" : "false") << std::endl;
    
    // Test V1 generation
    UUID uuid1 = UUID::generateV1();
    std::cout << "Generated V1: " << uuid1 << std::endl;
    std::cout << "Version: " << uuid1.version() << std::endl;
    std::cout << "Variant: " << uuid1.variant() << std::endl;
    std::cout << "Is valid: " << (uuid1.isValid() ? "true" : "false") << std::endl;
    
    std::cout << std::endl;
}

void test_string_functionality() {
    std::cout << "=== Testing String Functionality ===" << std::endl;
    
    UUID uuid = UUID::generate();
    
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
        UUID fromString(lowercase);
        std::cout << "Parsed UUID: " << fromString << std::endl;
        std::cout << "Strings match: " << (uuid == fromString ? "true" : "false") << std::endl;
        
        // Test with uppercase
        UUID fromUpper(uppercase);
        std::cout << "From upper:  " << fromUpper << std::endl;
        std::cout << "Upper match: " << (uuid == fromUpper ? "true" : "false") << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Error parsing UUID: " << e.what() << std::endl;
    }
    
    // Test invalid string
    try {
        UUID invalid("invalid-uuid-string");
        std::cout << "ERROR: Should have thrown exception!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Correctly caught invalid UUID: " << e.what() << std::endl;
    }
    
    std::cout << std::endl;
}

void test_comparison_operators() {
    std::cout << "=== Testing Comparison Operators ===" << std::endl;
    
    UUID uuid1 = UUID::generate();
    UUID uuid2 = UUID::generate();
    UUID uuid1_copy = UUID(uuid1.toString());
    
    std::cout << "UUID 1: " << uuid1 << std::endl;
    std::cout << "UUID 2: " << uuid2 << std::endl;
    std::cout << "UUID 1 copy: " << uuid1_copy << std::endl;
    
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
    std::set<UUID> uuidSet;
    for (int i = 0; i < 5; ++i) {
        uuidSet.insert(UUID::generate());
    }
    
    std::cout << "Set contents (" << uuidSet.size() << " elements):" << std::endl;
    for (const auto& uuid : uuidSet) {
        std::cout << "  " << uuid << std::endl;
    }
    
    // Test std::unordered_set (requires hash function)
    std::unordered_set<UUID> uuidHashSet;
    for (int i = 0; i < 5; ++i) {
        uuidHashSet.insert(UUID::generate());
    }
    
    std::cout << "Hash set contents (" << uuidHashSet.size() << " elements):" << std::endl;
    for (const auto& uuid : uuidHashSet) {
        std::cout << "  " << uuid << " (hash: " << uuid.hash() << ")" << std::endl;
    }
    
    // Test std::unordered_map
    std::unordered_map<UUID, std::string> uuidMap;
    uuidMap[UUID::generate()] = "First UUID";
    uuidMap[UUID::generate()] = "Second UUID";
    uuidMap[UUID::generate()] = "Third UUID";
    
    std::cout << "Map contents (" << uuidMap.size() << " elements):" << std::endl;
    for (const auto& pair : uuidMap) {
        std::cout << "  " << pair.first << " -> " << pair.second << std::endl;
    }
    
    std::cout << std::endl;
}

void test_uniqueness() {
    std::cout << "=== Testing UUID Uniqueness ===" << std::endl;
    
    const int count = 1000;
    std::unordered_set<UUID> generatedUuids;
    
    auto start = std::chrono::steady_clock::now();
    
    for (int i = 0; i < count; ++i) {
        generatedUuids.insert(UUID::generate());
    }
    
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Generated " << count << " UUIDs in " 
              << duration.count() << " microseconds" << std::endl;
    std::cout << "Unique UUIDs: " << generatedUuids.size() << std::endl;
    std::cout << "Duplicates: " << (count - generatedUuids.size()) << std::endl;
    std::cout << "Average time per UUID: " 
              << std::fixed << std::setprecision(3) 
              << (double)duration.count() / count << " microseconds" << std::endl;
    
    // Test V1 uniqueness (should be sequential)
    std::cout << "\nV1 UUID sequential test:" << std::endl;
    for (int i = 0; i < 5; ++i) {
        UUID v1 = UUID::generateV1();
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
    std::vector<std::vector<UUID>> results(numThreads);
    
    auto start = std::chrono::steady_clock::now();
    
    // Launch threads
    for (int t = 0; t < numThreads; ++t) {
        threads.emplace_back([t, uuidsPerThread, &results]() {
            results[t].reserve(uuidsPerThread);
            for (int i = 0; i < uuidsPerThread; ++i) {
                results[t].push_back(UUID::generate());
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
    std::unordered_set<UUID> allUuids;
    int totalGenerated = 0;
    
    for (const auto& threadResults : results) {
        totalGenerated += threadResults.size();
        for (const auto& uuid : threadResults) {
            allUuids.insert(uuid);
        }
    }
    
    std::cout << "Generated " << totalGenerated << " UUIDs across " 
              << numThreads << " threads" << std::endl;
    std::cout << "Unique UUIDs: " << allUuids.size() << std::endl;
    std::cout << "Duplicates: " << (totalGenerated - allUuids.size()) << std::endl;
    std::cout << "Time: " << duration.count() << " microseconds" << std::endl;
    std::cout << "Average per UUID: " 
              << std::fixed << std::setprecision(3)
              << (double)duration.count() / totalGenerated << " microseconds" << std::endl;
    
    std::cout << std::endl;
}

void test_edge_cases() {
    std::cout << "=== Testing Edge Cases ===" << std::endl;
    
    // Test move semantics
    UUID original = UUID::generate();
    std::cout << "Original: " << original << std::endl;
    
    UUID moved = std::move(original);
    std::cout << "Moved: " << moved << std::endl;
    std::cout << "Original after move: " << original << std::endl;
    
    // Test copy semantics
    UUID copied = moved;
    std::cout << "Copied: " << copied << std::endl;
    std::cout << "Copy == Moved: " << (copied == moved ? "true" : "false") << std::endl;
    
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
    
    UUID custom;
    custom.setData(customData);
    std::cout << "Custom data UUID: " << custom << std::endl;
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
            UUID test(invalid);
            std::cout << "ERROR: Should have failed for: " << invalid << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Correctly rejected: " << invalid << std::endl;
        }
    }
    
    std::cout << std::endl;
}

void test_stream_operators() {
    std::cout << "=== Testing Stream Operators ===" << std::endl;
    
    UUID uuid = UUID::generate();
    
    // Test output operator
    std::cout << "Stream output: " << uuid << std::endl;
    
    // Test stringstream
    std::stringstream ss;
    ss << uuid;
    std::string streamedString = ss.str();
    std::cout << "Stringstream: " << streamedString << std::endl;
    
    // Test that streamed string can recreate UUID
    try {
        UUID recreated(streamedString);
        std::cout << "Recreated: " << recreated << std::endl;
        std::cout << "Match: " << (uuid == recreated ? "true" : "false") << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error recreating: " << e.what() << std::endl;
    }
    
    std::cout << std::endl;
}

int main() {
    std::cout << "UUID Class Comprehensive Test Suite" << std::endl;
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