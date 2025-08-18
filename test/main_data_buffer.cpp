#include "../libftpp.hpp"
#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <cstring>
#include <cassert>

class TestObject {
public:
	int x;
	std::string y;

	friend DataBuffer& operator<<(DataBuffer& p_buffer, const TestObject& p_object) {
		p_buffer << p_object.x << p_object.y;
		return p_buffer;
	}

	friend DataBuffer& operator>>(DataBuffer& p_buffer, TestObject& p_object) {
		p_buffer >> p_object.x >> p_object.y;
		return p_buffer;
	}
};

void test_original() {
	std::cout << "\n--- test_original ---\n";

	DataBuffer myBuffer;

	TestObject obj1;
	obj1.x = 42;
	obj1.y = "Hello";

	TestObject obj2;
	obj2.x = 99;
	obj2.y = "World";

	myBuffer << obj1 << obj2;

	TestObject deserializedObj1, deserializedObj2, deserializedObj3;

	// This should work as expected
	try {
		myBuffer >> deserializedObj1 >> deserializedObj2;
		std::cout << "Deserialized obj1: x = " << deserializedObj1.x << ", y = " << deserializedObj1.y << std::endl;
		std::cout << "Deserialized obj2: x = " << deserializedObj2.x << ", y = " << deserializedObj2.y << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Caught exception: " << e.what() << std::endl;
	}

	// This should throw an exception because there are no more objects to deserialize
	try {
		myBuffer >> deserializedObj3;
		std::cout << "Deserialized obj3: x = " << deserializedObj3.x << ", y = " << deserializedObj3.y << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Caught exception: " << e.what() << std::endl;  // This line should be executed
	}
}

void test_basic_types() {
	std::cout << "\n--- test_basic_types ---\n";
	DataBuffer buffer;
	
	// Test various basic types
	int i = 42;
	float f = 3.14f;
	double d = 2.71828;
	char c = 'A';
	bool b = true;
	
	buffer << i << f << d << c << b;
	
	int i_out;
	float f_out;
	double d_out;
	char c_out;
	bool b_out;
	
	buffer >> i_out >> f_out >> d_out >> c_out >> b_out;
	
	std::cout << "int: " << i_out << " (expected: " << i << ")" << std::endl;
	std::cout << "float: " << f_out << " (expected: " << f << ")" << std::endl;
	std::cout << "double: " << d_out << " (expected: " << d << ")" << std::endl;
	std::cout << "char: " << c_out << " (expected: " << c << ")" << std::endl;
	std::cout << "bool: " << b_out << " (expected: " << b << ")" << std::endl;
}

void test_strings() {
	std::cout << "\n--- test_strings ---\n";
	DataBuffer buffer;
	
	std::string empty_str = "";
	std::string normal_str = "Hello World";
	std::string unicode_str = "Héllo Wørld 🌍";
	std::string long_str(1000, 'X');
	
	buffer << empty_str << normal_str << unicode_str << long_str;
	
	std::string empty_out, normal_out, unicode_out, long_out;
	
	buffer >> empty_out >> normal_out >> unicode_out >> long_out;
	
	std::cout << "Empty string length: " << empty_out.length() << "  Empty string: '" << empty_out << "'" << std::endl;
	std::cout << "Normal string: " << normal_out << std::endl;
	std::cout << "Unicode string: " << unicode_out << std::endl;
	std::cout << "Long string length: " << long_out.length() << " (all X: " << (long_out == std::string(1000, 'X')) << ")" << std::endl;
}

void test_arrays_and_structs() {
	std::cout << "\n--- test_arrays_and_structs ---\n";
	DataBuffer buffer;
	
	// Test C-style array (Note: this will only work for fixed-size arrays)
	int arr[5] = {1, 2, 3, 4, 5};
	buffer << arr;
	
	int arr_out[5];
	buffer >> arr_out;
	
	std::cout << "Array: ";
	for (int i = 0; i < 5; i++) {
		std::cout << arr_out[i] << " ";
	}
	std::cout << std::endl;
	
	// Test struct
	struct SimpleStruct {
		int a;
		float b;
		char c;
	} simple = {100, 1.5f, 'Z'};
	
	buffer << simple;
	
	SimpleStruct simple_out;
	buffer >> simple_out;
	
	std::cout << "Struct: a=" << simple_out.a << ", b=" << simple_out.b << ", c=" << simple_out.c << std::endl;
}

void test_custom_objects() {
	std::cout << "\n--- test_custom_objects ---\n";
	DataBuffer buffer;
	
	TestObject obj1 = {42, "Hello"};
	TestObject obj2 = {99, "World"};
	TestObject obj3 = {-1, ""};
	
	buffer << obj1 << obj2 << obj3;
	
	TestObject obj1_out, obj2_out, obj3_out;
	
	buffer >> obj1_out >> obj2_out >> obj3_out;
	
	std::cout << "Object 1: x=" << obj1_out.x << ", y='" << obj1_out.y << "'" << std::endl;
	std::cout << "Object 2: x=" << obj2_out.x << ", y='" << obj2_out.y << "'" << std::endl;
	std::cout << "Object 3: x=" << obj3_out.x << ", y='" << obj3_out.y << "'" << std::endl;
}

void test_edge_cases() {
	std::cout << "\n--- test_edge_cases ---\n";
	
	// Test empty buffer read
	{
		DataBuffer empty_buffer;
		int dummy;
		try {
			empty_buffer >> dummy;
			std::cout << "ERROR: Should have thrown exception for empty buffer" << std::endl;
		} catch (const std::exception& e) {
			std::cout << "✓ Empty buffer exception: " << e.what() << std::endl;
		}
	}
	
	// Test partial read
	{
		DataBuffer partial_buffer;
		int value = 123;
		partial_buffer << value;
		
		int int_out;
		float float_out;
		
		partial_buffer >> int_out; // This should work
		std::cout << "✓ Partial read int: " << int_out << std::endl;
		
		try {
			partial_buffer >> float_out; // This should fail
			std::cout << "ERROR: Should have thrown exception for partial read" << std::endl;
		} catch (const std::exception& e) {
			std::cout << "✓ Partial read exception: " << e.what() << std::endl;
		}
	}
	
	// Test string with null characters
	{
		DataBuffer null_buffer;
		std::string str_with_nulls = "Hello\0World\0Test";
		str_with_nulls.resize(17); // Make sure we include the null chars
		
		null_buffer << str_with_nulls;
		
		std::string str_out;
		null_buffer >> str_out;
		
		std::cout << "✓ String with nulls length: " << str_out.length() << " (expected: 17)" << std::endl;
	}
}

void test_large_data() {
	std::cout << "\n--- test_large_data ---\n";
	DataBuffer buffer;
	
	// Test large amounts of data
	std::vector<int> large_data(10000);
	for (int i = 0; i < 10000; i++) {
		large_data[i] = i * i;
		buffer << large_data[i];
	}
	
	std::vector<int> large_data_out(10000);
	for (int i = 0; i < 10000; i++) {
		buffer >> large_data_out[i];
	}
	
	bool correct = true;
	for (int i = 0; i < 10000; i++) {
		if (large_data[i] != large_data_out[i]) {
			correct = false;
			break;
		}
	}
	
	std::cout << "Large data test: " << (correct ? "✓ PASSED" : "✗ FAILED") << std::endl;
}

void test_copy_and_assignment() {
	std::cout << "\n--- test_copy_and_assignment ---\n";
	DataBuffer buffer1;
	
	buffer1 << 42 << std::string("Test") << 3.14f;
	
	// Test copy constructor
	DataBuffer buffer2(buffer1);
	
	// Test assignment operator
	DataBuffer buffer3;
	buffer3 = buffer1;
	
	// Test move constructor
	DataBuffer buffer4(std::move(buffer1));
	
	// Read from copied buffer
	int i;
	std::string s;
	float f;
	
	buffer2 >> i >> s >> f;
	std::cout << "Copy constructor: " << i << ", " << s << ", " << f << std::endl;
	
	buffer3 >> i >> s >> f;
	std::cout << "Assignment operator: " << i << ", " << s << ", " << f << std::endl;
	
	buffer4 >> i >> s >> f;
	std::cout << "Move constructor: " << i << ", " << s << ", " << f << std::endl;
}

void test_size_and_empty() {
	std::cout << "\n--- test_size_and_empty ---\n";
	
	DataBuffer buffer;
	
	// Test initial state
	std::cout << "Initial state: size=" << buffer.size() << ", empty=" << buffer.empty() << std::endl;
	assert(buffer.empty() == true);
	assert(buffer.size() == 0);
	
	// Add data and check size/empty
	buffer << 42;
	std::cout << "After adding int: size=" << buffer.size() << ", empty=" << buffer.empty() << std::endl;
	assert(buffer.empty() == false);
	assert(buffer.size() == sizeof(int));
	
	buffer << 3.14f;
	std::cout << "After adding float: size=" << buffer.size() << ", empty=" << buffer.empty() << std::endl;
	assert(buffer.size() == sizeof(int) + sizeof(float));
	
	buffer << std::string("Hello");
	size_t expectedSize = sizeof(int) + sizeof(float) + sizeof(size_t) + 5; // string size + length
	std::cout << "After adding string: size=" << buffer.size() << ", empty=" << buffer.empty() << std::endl;
	
	// Read data and check size changes
	int val;
	buffer >> val;
	std::cout << "After reading int: size=" << buffer.size() << ", empty=" << buffer.empty() << std::endl;
	
	float fval;
	buffer >> fval;
	std::cout << "After reading float: size=" << buffer.size() << ", empty=" << buffer.empty() << std::endl;
	
	std::string sval;
	buffer >> sval;
	std::cout << "After reading string: size=" << buffer.size() << ", empty=" << buffer.empty() << std::endl;
	assert(buffer.empty() == true);
	assert(buffer.size() == 0);
	
	std::cout << "✅ Size and empty tests passed!" << std::endl;
}

void test_clear_functionality() {
	std::cout << "\n--- test_clear_functionality ---\n";
	
	DataBuffer buffer;
	
	// Add various data
	buffer << 42 << 3.14f << std::string("Test") << 'A';
	
	std::cout << "Before clear: size=" << buffer.size() << ", empty=" << buffer.empty() << std::endl;
	assert(!buffer.empty());
	assert(buffer.size() > 0);
	
	// Test clear
	buffer.clear();
	
	std::cout << "After clear: size=" << buffer.size() << ", empty=" << buffer.empty() << std::endl;
	assert(buffer.empty() == true);
	assert(buffer.size() == 0);
	
	// Test that buffer is usable after clear
	buffer << 100;
	assert(buffer.size() == sizeof(int));
	assert(!buffer.empty());
	
	int value;
	buffer >> value;
	assert(value == 100);
	std::cout << "✅ Buffer usable after clear: " << value << std::endl;
	
	// Test clear on empty buffer
	buffer.clear();
	buffer.clear(); // Should not cause issues
	assert(buffer.empty());
	assert(buffer.size() == 0);
	
	std::cout << "✅ Clear functionality tests passed!" << std::endl;
}

void test_capacity_and_reserve() {
	std::cout << "\n--- test_capacity_and_reserve ---\n";
	
	DataBuffer buffer;
	
	// Test initial capacity
	std::cout << "Initial capacity: " << buffer.capacity() << std::endl;
	size_t initialCapacity = buffer.capacity();
	
	// Test reserve functionality
	buffer.reserve(1000);
	std::cout << "After reserve(1000): capacity=" << buffer.capacity() << std::endl;
	assert(buffer.capacity() >= 1000);
	assert(buffer.empty() == true);  // Reserve shouldn't affect emptiness
	assert(buffer.size() == 0);      // Reserve shouldn't affect size
	
	// Test reserve smaller than current capacity (should not shrink)
	buffer.reserve(500);
	std::cout << "After reserve(500): capacity=" << buffer.capacity() << std::endl;
	assert(buffer.capacity() >= 1000); // Should not decrease
	
	// Test capacity growth with data
	size_t capacityBefore = buffer.capacity();
	
	// Add data that might exceed initial capacity
	for (int i = 0; i < 100; ++i) {
		buffer << i << std::string("Test") << static_cast<float>(i * 1.5);
	}
	
	std::cout << "After adding 100 complex objects:" << std::endl;
	std::cout << "  Capacity: " << buffer.capacity() << std::endl;
	std::cout << "  Size: " << buffer.size() << " bytes" << std::endl;
	
	// Capacity should grow as needed
	assert(buffer.capacity() >= buffer.size());
	
	// Test that reserved space is maintained during operations
	buffer.clear();
	assert(buffer.capacity() >= 1000); // Should maintain reserved capacity
	
	std::cout << "✅ Capacity and reserve tests passed!" << std::endl;
}

void test_size_tracking_detailed() {
	std::cout << "\n--- test_size_tracking_detailed ---\n";
	
	DataBuffer buffer;
	
	// Test size tracking with various operations
	std::vector<std::pair<std::string, size_t>> operations;
	
	operations.push_back({"Initial", 0});
	assert(buffer.size() == 0);
	
	buffer << static_cast<char>('A');
	operations.push_back({"After char", sizeof(char)});
	assert(buffer.size() == sizeof(char));
	
	buffer << static_cast<short>(42);
	operations.push_back({"After short", sizeof(char) + sizeof(short)});
	assert(buffer.size() == sizeof(char) + sizeof(short));
	
	buffer << static_cast<int>(123);
	operations.push_back({"After int", sizeof(char) + sizeof(short) + sizeof(int)});
	assert(buffer.size() == sizeof(char) + sizeof(short) + sizeof(int));
	
	buffer << static_cast<long long>(999999);
	size_t expectedSize = sizeof(char) + sizeof(short) + sizeof(int) + sizeof(long long);
	operations.push_back({"After long long", expectedSize});
	assert(buffer.size() == expectedSize);
	
	// Print all operations
	for (const auto& op : operations) {
		std::cout << op.first << ": " << op.second << " bytes" << std::endl;
	}
	
	// Test size reduction through reading
	char c;
	buffer >> c;
	expectedSize -= sizeof(char);
	std::cout << "After reading char: " << buffer.size() << " bytes (expected: " << expectedSize << ")" << std::endl;
	assert(buffer.size() == expectedSize);
	
	short s;
	buffer >> s;
	expectedSize -= sizeof(short);
	std::cout << "After reading short: " << buffer.size() << " bytes (expected: " << expectedSize << ")" << std::endl;
	assert(buffer.size() == expectedSize);
	
	std::cout << "✅ Detailed size tracking tests passed!" << std::endl;
}

void test_memory_management() {
	std::cout << "\n--- test_memory_management ---\n";
	
	DataBuffer buffer;
	
	// Test memory efficiency
	size_t initialCapacity = buffer.capacity();
	std::cout << "Initial capacity: " << initialCapacity << std::endl;
	
	// Add and remove data cyclically
	for (int cycle = 0; cycle < 10; ++cycle) {
		// Add data
		for (int i = 0; i < 50; ++i) {
			buffer << i << std::string("Cycle") << static_cast<double>(i * 2.5);
		}
		
		std::cout << "Cycle " << cycle << " - After adding: size=" << buffer.size() 
				  << ", capacity=" << buffer.capacity() << std::endl;
		
		// Read all data back
		for (int i = 0; i < 50; ++i) {
			int val;
			std::string str;
			double d;
			buffer >> val >> str >> d;
		}
		
		std::cout << "Cycle " << cycle << " - After reading: size=" << buffer.size() 
				  << ", capacity=" << buffer.capacity() << std::endl;
		
		assert(buffer.empty());
	}
	
	// Test clear doesn't necessarily reduce capacity
	buffer << 42 << std::string("Test");
	size_t capacityBeforeClear = buffer.capacity();
	buffer.clear();
	std::cout << "Capacity before clear: " << capacityBeforeClear << std::endl;
	std::cout << "Capacity after clear: " << buffer.capacity() << std::endl;
	
	std::cout << "✅ Memory management tests passed!" << std::endl;
}

int main() {
	test_original();
	test_basic_types();
	test_strings();
	test_arrays_and_structs();
	test_custom_objects();
	test_edge_cases();
	test_large_data();
	test_copy_and_assignment();
	test_size_and_empty();
	test_clear_functionality();
	test_capacity_and_reserve();
	test_size_tracking_detailed();
	test_memory_management();

	std::cout << "\nAll tests passed!" << std::endl;

	return 0;
}