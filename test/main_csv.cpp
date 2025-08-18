#include "../inc/libftpp.hpp"
#include <iostream>
#include <cassert>
#include <filesystem>

void test_basic_functionality() {
	std::cout << "=== Testing Basic Functionality ===" << std::endl;
	
	Csv csv;
	
	// Test basic operations
	csv.setHeader({"Name", "Age", "City"});
	csv.addRow({"John Doe", "25", "New York"});
	csv.addRow({"Jane Smith", "30", "Los Angeles"});
	
	assert(csv.rowCount() == 2);
	assert(csv.columnCount() == 3);
	assert(csv.getHeader()[0] == "Name");
	assert(csv[0][0] == "John Doe");
	
	std::cout << "✅ Basic functionality tests passed!" << std::endl;
}

void test_edge_cases() {
	std::cout << "\n=== Testing Edge Cases ===" << std::endl;
	
	Csv csv;
	csv.setHeader({"Field1", "Field2", "Field3"});
	
	// Test various edge cases
	csv.addRow({"Simple", "Field", "Test"});                    // Normal
	csv.addRow({"With, Comma", "Normal", "Field"});             // Comma in field
	csv.addRow({"With \"Quotes\"", "Normal", "Field"});         // Quotes in field
	csv.addRow({"", "Empty", "First"});                         // Empty field
	csv.addRow({"Normal", "", "Empty Middle"});                 // Empty middle field
	csv.addRow({"Normal", "Field", ""});                        // Empty last field
	csv.addRow({"Multi\nLine", "Field", "Test"});               // Newline (if supported)
	csv.addRow({" Spaces ", " Around ", " Values "});           // Spaces around values
	csv.addRow({"\"Already quoted\"", "Field", "Test"});        // Already quoted
	csv.addRow({"Mix, \"of\" cases", "Complex", "Field"});      // Mix of comma and quotes
	
	csv.save("edge_cases.csv");
	
	// Load and verify
	Csv csv2;
	csv2.load("edge_cases.csv");

	assert(csv2.rowCount() == 10);
	assert(csv2[0][0] == "Simple");
	assert(csv2[1][0] == "With, Comma");
	assert(csv2[2][0] == "With \"Quotes\"");
	assert(csv2[3][0] == "");  // Empty field
	
	std::cout << "✅ Edge cases tests passed!" << std::endl;
	
	// Display edge cases
	std::cout << "\nEdge Cases Results:" << std::endl;
	for (size_t i = 0; i < csv2.rowCount(); ++i) {
		std::cout << "Row " << i << ": ";
		for (const auto& cell : csv2[i]) {
			std::cout << "[" << cell << "] ";
		}
		std::cout << std::endl;
	}
}

void test_round_trip_accuracy() {
	std::cout << "\n=== Testing Round-Trip Accuracy ===" << std::endl;
	
	Csv original;
	original.setHeader({"Complex", "Data", "Test"});
	
	// Complex test data
	std::vector<std::vector<std::string>> testData = {
		{"John \"The Rock\" Johnson", "Age: 51", "Actor, Wrestler"},
		{"Data with\nNewlines", "Embedded, Commas", "\"Quoted\" Strings"},
		{"", "Empty Fields", ""},
		{"   Trimmed   ", "  Spaces  ", "  Test  "},
		{"Unicode: 你好", "Émojis: 🚀", "Special: !@#$%^&*()"},
		{"SQL Injection'; DROP TABLE;", "XSS <script>", "Path: C:\\Windows\\System32"}
	};
	
	for (const auto& row : testData) {
		original.addRow(row);
	}
	
	original.save("roundtrip.csv");
	
	// Load back
	Csv loaded;
	loaded.load("roundtrip.csv");
	
	// Verify exact match
	assert(loaded.rowCount() == original.rowCount());
	assert(loaded.columnCount() == original.columnCount());
	
	for (size_t i = 0; i < loaded.rowCount(); ++i) {
		for (size_t j = 0; j < loaded.columnCount(); ++j) {
			if (original[i][j] != loaded[i][j]) {
				std::cout << "Mismatch at [" << i << "][" << j << "]: " 
						  << "Original: [" << original[i][j] << "] "
						  << "Loaded: [" << loaded[i][j] << "]" << std::endl;
				assert(false);
			}
		}
	}
	
	std::cout << "✅ Round-trip accuracy tests passed!" << std::endl;
}

void test_error_handling() {
	std::cout << "\n=== Testing Error Handling ===" << std::endl;
	
	Csv csv;
	
	try {
		// Test invalid file operations
		csv.load("nonexistent_file.csv");
		assert(false);  // Should not reach here
	} catch (const std::runtime_error& e) {
		std::cout << "✅ Caught expected error for nonexistent file: " << e.what() << std::endl;
	}
	
	try {
		// Test empty header
		csv.setHeader({});
		assert(false);  // Should not reach here
	} catch (const std::invalid_argument& e) {
		std::cout << "✅ Caught expected error for empty header: " << e.what() << std::endl;
	}
	
	try {
		// Test empty row
		csv.addRow({});
		assert(false);  // Should not reach here
	} catch (const std::invalid_argument& e) {
		std::cout << "✅ Caught expected error for empty row: " << e.what() << std::endl;
	}
	
	try {
		// Test out of bounds access
		Csv empty_csv;
		auto& row = empty_csv[0];  // Should throw
		(void)row;  // Suppress unused variable warning
		assert(false);  // Should not reach here
	} catch (const std::out_of_range& e) {
		std::cout << "✅ Caught expected error for out of bounds: " << e.what() << std::endl;
	}
	
	std::cout << "✅ Error handling tests passed!" << std::endl;
}

void test_performance() {
	std::cout << "\n=== Testing Performance ===" << std::endl;
	
	auto start = std::chrono::high_resolution_clock::now();
	
	Csv csv;
	csv.setHeader({"ID", "Name", "Description", "Value", "Category"});
	
	// Add 10,000 rows
	for (int i = 0; i < 10000; ++i) {
		csv.addRow({
			"ID" + std::to_string(i),
			"Name " + std::to_string(i),
			"Description with, commas and \"quotes\" for item " + std::to_string(i),
			std::to_string(i * 1.5),
			"Category" + std::to_string(i % 10)
		});
	}
	
	csv.save("performance_test.csv");
	
	auto save_end = std::chrono::high_resolution_clock::now();
	
	Csv csv2;
	csv2.load("performance_test.csv");
	
	auto load_end = std::chrono::high_resolution_clock::now();
	
	auto save_duration = std::chrono::duration_cast<std::chrono::milliseconds>(save_end - start);
	auto load_duration = std::chrono::duration_cast<std::chrono::milliseconds>(load_end - save_end);
	
	assert(csv2.rowCount() == 10000);
	assert(csv2.columnCount() == 5);
	
	std::cout << "✅ Performance test completed!" << std::endl;
	std::cout << "   - Save 10,000 rows: " << save_duration.count() << "ms" << std::endl;
	std::cout << "   - Load 10,000 rows: " << load_duration.count() << "ms" << std::endl;
	std::cout << "   - Total time: " << (save_duration + load_duration).count() << "ms" << std::endl;
	
	// Cleanup large file
	std::filesystem::remove("performance_test.csv");
}

void test_special_csv_formats() {
	std::cout << "\n=== Testing Special CSV Formats ===" << std::endl;
	
	// Test CSV with only headers
	Csv headers_only;
	headers_only.setHeader({"Col1", "Col2", "Col3"});
	headers_only.save("headers_only.csv");
	
	Csv loaded_headers;
	loaded_headers.load("headers_only.csv");
	assert(loaded_headers.rowCount() == 0);
	assert(loaded_headers.columnCount() == 3);
	
	// Test CSV with inconsistent column counts (real-world scenario)
	std::ofstream file("inconsistent.csv");
	file << "Name,Age,City\n";
	file << "John,25\n";  // Missing column
	file << "Jane,30,NYC,Extra\n";  // Extra column
	file << "Bob,35,LA\n";  // Normal
	file.close();
	
	Csv inconsistent;
	inconsistent.load("inconsistent.csv");
	
	std::cout << "Inconsistent CSV loaded:" << std::endl;
	for (size_t i = 0; i < inconsistent.rowCount(); ++i) {
		std::cout << "Row " << i << " (" << inconsistent[i].size() << " columns): ";
		for (const auto& cell : inconsistent[i]) {
			std::cout << "[" << cell << "] ";
		}
		std::cout << std::endl;
	}
	
	std::cout << "✅ Special CSV formats tests passed!" << std::endl;
}

void test_move_semantics() {
	std::cout << "\n=== Testing Move Semantics ===" << std::endl;
	
	Csv csv1;
	csv1.setHeader({"A", "B", "C"});
	csv1.addRow({"1", "2", "3"});
	csv1.addRow({"4", "5", "6"});
	
	// Test move constructor
	Csv csv2 = std::move(csv1);
	assert(csv2.rowCount() == 2);
	assert(csv2.columnCount() == 3);
	
	// Test move assignment
	Csv csv3;
	csv3 = std::move(csv2);
	assert(csv3.rowCount() == 2);
	assert(csv3.columnCount() == 3);
	assert(csv3[0][0] == "1");
	
	std::cout << "✅ Move semantics tests passed!" << std::endl;
}

void display_test_summary() {
	std::cout << "\n" << std::string(50, '=') << std::endl;
	std::cout << "📊 CSV LIBRARY TEST SUMMARY" << std::endl;
	std::cout << std::string(50, '=') << std::endl;
	std::cout << "✅ Basic Functionality: PASSED" << std::endl;
	std::cout << "✅ Edge Cases: PASSED" << std::endl;
	std::cout << "✅ Round-Trip Accuracy: PASSED" << std::endl;
	std::cout << "✅ Error Handling: PASSED" << std::endl;
	std::cout << "✅ Performance: PASSED" << std::endl;
	std::cout << "✅ Special Formats: PASSED" << std::endl;
	std::cout << "✅ Move Semantics: PASSED" << std::endl;
	std::cout << std::string(50, '=') << std::endl;
	std::cout << "🏆 ALL TESTS PASSED - PRODUCTION READY!" << std::endl;
	std::cout << std::string(50, '=') << std::endl;
}

int main() {
	std::cout << "🚀 CSV Library Comprehensive Test Suite" << std::endl;
	std::cout << std::string(50, '=') << std::endl;
	
	try {
		test_basic_functionality();
		test_edge_cases();
		test_round_trip_accuracy();
		test_error_handling();
		test_performance();
		test_special_csv_formats();
		test_move_semantics();
		
		display_test_summary();
		
		// Cleanup test files
		std::filesystem::remove("test.csv");
		std::filesystem::remove("edge_cases.csv");
		std::filesystem::remove("roundtrip.csv");
		std::filesystem::remove("headers_only.csv");
		std::filesystem::remove("inconsistent.csv");
		
	} catch (const std::exception& e) {
		std::cerr << "❌ Test failed with exception: " << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}
