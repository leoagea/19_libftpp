#include <iostream>
#include <cassert>
#include <cmath>
#include "../inc/libftpp.hpp"

void test_original() {
	std::cout << "\n=== Original Tests ===" << std::endl;
	
	IVector3<int> vec1(3, 4, 1);
	IVector3<int> vec2(1, 2, 3);
	
	std::cout << "Vec1 : " << vec1.x << " / " << vec1.y << " / " << vec1.z << std::endl;
	std::cout << "Vec2 : " << vec2.x << " / " << vec2.y << " / " << vec2.z << std::endl;

	// Test operator overloads
	auto vecAdd = vec1 + vec2;
	std::cout << "vec1 + vec2 = (" << vecAdd.x << ", " << vecAdd.y << ", " << vecAdd.z << ")" << std::endl;
	// Expected: vec1 + vec2 = (4, 6, 4)

	auto vecSub = vec1 - vec2;
	std::cout << "vec1 - vec2 = (" << vecSub.x << ", " << vecSub.y << ", " << vecSub.z << ")" << std::endl;
	// Expected: vec1 - vec2 = (2, 2, -2)

	auto vecMul = vec1 * vec2;
	std::cout << "vec1 * vec2 = (" << vecMul.x << ", " << vecMul.y << ", " << vecMul.z << ")" << std::endl;
	// Expected: vec1 * vec2 = (3, 8, 3)

	auto vecDiv = vec1 / vec2;
	std::cout << "vec1 / vec2 = (" << vecDiv.x << ", " << vecDiv.y << ", " << vecDiv.z << ")" << std::endl;
	// Expected: vec1 / vec2 = (3, 2, 0)

	bool isEqual = vec1 == vec2;
	std::cout << "vec1 == vec2: " << (isEqual ? "true" : "false") << std::endl;
	// Expected: vec1 == vec2: false

	bool isNotEqual = vec1 != vec2;
	std::cout << "vec1 != vec2: " << (isNotEqual ? "true" : "false") << std::endl;
	// Expected: vec1 != vec2: true

	// Test additional methods
	float len = vec1.length();
	std::cout << "Length of vec1: " << len << std::endl;
	// Expected: Length of vec1: 5.099 (or sqrt(3*3 + 4*4 + 1*1))

	auto normVec = vec1.normalize();
	std::cout << "Normalized vec1 = (" << normVec.x << ", " << normVec.y << ", " << normVec.z << ")" << std::endl;
	// Expected: Normalized vec1 = (some_value, some_value, some_value)

	float dotProd = vec1.dot(vec2);
	std::cout << "Dot product of vec1 and vec2: " << dotProd << std::endl;
	// Expected: Dot product of vec1 and vec2: 14 (or 3*1 + 4*2 + 1*3)

	auto crossProd = vec1.cross(vec2);
	std::cout << "Cross product of vec1 and vec2: (" << crossProd.x << ", " << crossProd.y << ", " << crossProd.z << ")" << std::endl;
	// Expected: Cross product of vec1 and vec2: (some_value, some_value, some_value)
}

void test_constructors() {
	std::cout << "\n=== Constructor Tests ===" << std::endl;
	
	// Only three-parameter constructor available
	IVector3<double> vec1(1.5, 2.5, 3.5);
	std::cout << "Three value constructor: (" << vec1.x << ", " << vec1.y << ", " << vec1.z << ")" << std::endl;
	
	// Test move constructor with explicit construction
	IVector3<int> vec2 = IVector3<int>(10, 20, 30);
	std::cout << "Move constructor: (" << vec2.x << ", " << vec2.y << ", " << vec2.z << ")" << std::endl;
	
	// Test zero vector construction
	IVector3<float> zeroVec(0.0f, 0.0f, 0.0f);
	std::cout << "Zero vector: (" << zeroVec.x << ", " << zeroVec.y << ", " << zeroVec.z << ")" << std::endl;
	
	// Test unit vectors
	IVector3<float> unitX(1.0f, 0.0f, 0.0f);
	IVector3<float> unitY(0.0f, 1.0f, 0.0f);
	IVector3<float> unitZ(0.0f, 0.0f, 1.0f);
	std::cout << "Unit vectors - X: (" << unitX.x << ", " << unitX.y << ", " << unitX.z << ")" << std::endl;
	std::cout << "Unit vectors - Y: (" << unitY.x << ", " << unitY.y << ", " << unitY.z << ")" << std::endl;
	std::cout << "Unit vectors - Z: (" << unitZ.x << ", " << unitZ.y << ", " << unitZ.z << ")" << std::endl;
}

void test_move_assignment() {
	std::cout << "\n=== Move Assignment Tests ===" << std::endl;
	
	IVector3<int> vec1(1, 2, 3);
	std::cout << "Original vec1: (" << vec1.x << ", " << vec1.y << ", " << vec1.z << ")" << std::endl;
	
	// Test move assignment (only available assignment)
	vec1 = IVector3<int>(4, 5, 6);
	std::cout << "After move assignment: (" << vec1.x << ", " << vec1.y << ", " << vec1.z << ")" << std::endl;
	
	// Chain multiple move assignments
	vec1 = IVector3<int>(7, 8, 9);
	std::cout << "After second move assignment: (" << vec1.x << ", " << vec1.y << ", " << vec1.z << ")" << std::endl;
}

void test_mathematical_operations() {
	std::cout << "\n=== Mathematical Operations ===" << std::endl;
	
	IVector3<float> vec1(3.0f, 4.0f, 0.0f);
	IVector3<float> vec2(1.0f, 0.0f, 0.0f);
	IVector3<float> vec3(0.0f, 1.0f, 0.0f);
	
	std::cout << "vec1: (" << vec1.x << ", " << vec1.y << ", " << vec1.z << ")" << std::endl;
	std::cout << "vec2: (" << vec2.x << ", " << vec2.y << ", " << vec2.z << ")" << std::endl;
	std::cout << "vec3: (" << vec3.x << ", " << vec3.y << ", " << vec3.z << ")" << std::endl;
	
	// Basic arithmetic operations
	auto sum = vec1 + vec2;
	auto diff = vec1 - vec2;
	auto product = vec1 * vec2;
	
	std::cout << "vec1 + vec2: (" << sum.x << ", " << sum.y << ", " << sum.z << ")" << std::endl;
	std::cout << "vec1 - vec2: (" << diff.x << ", " << diff.y << ", " << diff.z << ")" << std::endl;
	std::cout << "vec1 * vec2: (" << product.x << ", " << product.y << ", " << product.z << ")" << std::endl;
	
	// Length tests
	float len1 = vec1.length();
	float len2 = vec2.length();
	std::cout << "Length of vec1: " << len1 << " (expected: 5.0)" << std::endl;
	std::cout << "Length of vec2: " << len2 << " (expected: 1.0)" << std::endl;
	
	// Normalization
	auto norm1 = vec1.normalize();
	float normLen = norm1.length();
	std::cout << "Normalized vec1: (" << norm1.x << ", " << norm1.y << ", " << norm1.z << ")" << std::endl;
	std::cout << "Length of normalized vec1: " << normLen << " (should be ~1.0)" << std::endl;
	
	// Dot product
	float dot12 = vec1.dot(vec2);
	float dot13 = vec1.dot(vec3);
	float dot23 = vec2.dot(vec3);
	std::cout << "vec1 · vec2: " << dot12 << " (expected: 3.0)" << std::endl;
	std::cout << "vec1 · vec3: " << dot13 << " (expected: 4.0)" << std::endl;
	std::cout << "vec2 · vec3: " << dot23 << " (expected: 0.0)" << std::endl;
	
	// Cross product
	auto cross23 = vec2.cross(vec3);
	auto cross32 = vec3.cross(vec2);
	std::cout << "vec2 × vec3: (" << cross23.x << ", " << cross23.y << ", " << cross23.z << ")" << std::endl;
	std::cout << "vec3 × vec2: (" << cross32.x << ", " << cross32.y << ", " << cross32.z << ")" << std::endl;
	std::cout << "Expected: (0, 0, 1) and (0, 0, -1)" << std::endl;
}

void test_different_types() {
	std::cout << "\n=== Different Type Tests ===" << std::endl;
	
	// Integer vectors
	IVector3<int> intVec(5, -3, 2);
	std::cout << "Int vector: (" << intVec.x << ", " << intVec.y << ", " << intVec.z << ")" << std::endl;
	std::cout << "Int vector length: " << intVec.length() << std::endl;
	
	// Double vectors
	IVector3<double> doubleVec(1.234, 5.678, -2.345);
	std::cout << "Double vector: (" << doubleVec.x << ", " << doubleVec.y << ", " << doubleVec.z << ")" << std::endl;
	std::cout << "Double vector length: " << doubleVec.length() << std::endl;
	
	// Long vectors
	IVector3<long> longVec(1000000L, -500000L, 750000L);
	std::cout << "Long vector: (" << longVec.x << ", " << longVec.y << ", " << longVec.z << ")" << std::endl;
	std::cout << "Long vector length: " << longVec.length() << std::endl;
}

void test_edge_cases() {
	std::cout << "\n=== Edge Case Tests ===" << std::endl;
	
	// Very small numbers
	IVector3<float> tiny(0.001f, 0.002f, 0.003f);
	std::cout << "Tiny vector: (" << tiny.x << ", " << tiny.y << ", " << tiny.z << ")" << std::endl;
	std::cout << "Tiny vector length: " << tiny.length() << std::endl;
	
	// Very large numbers
	IVector3<float> huge(1000000.0f, 2000000.0f, 3000000.0f);
	std::cout << "Huge vector: (" << huge.x << ", " << huge.y << ", " << huge.z << ")" << std::endl;
	std::cout << "Huge vector length: " << huge.length() << std::endl;
	
	// Negative values
	IVector3<int> negative(-10, -20, -30);
	std::cout << "Negative vector: (" << negative.x << ", " << negative.y << ", " << negative.z << ")" << std::endl;
	std::cout << "Negative vector length: " << negative.length() << std::endl;
	
	// Mixed positive/negative
	IVector3<float> mixed(-1.5f, 2.5f, -3.5f);
	std::cout << "Mixed vector: (" << mixed.x << ", " << mixed.y << ", " << mixed.z << ")" << std::endl;
	std::cout << "Mixed vector length: " << mixed.length() << std::endl;
}

void test_geometric_operations() {
	std::cout << "\n=== Geometric Operation Tests ===" << std::endl;
	
	// Triangle vertices
	IVector3<float> a(0.0f, 0.0f, 0.0f);
	IVector3<float> b(3.0f, 0.0f, 0.0f);
	IVector3<float> c(0.0f, 4.0f, 0.0f);
	
	std::cout << "Triangle vertices:" << std::endl;
	std::cout << "A: (" << a.x << ", " << a.y << ", " << a.z << ")" << std::endl;
	std::cout << "B: (" << b.x << ", " << b.y << ", " << b.z << ")" << std::endl;
	std::cout << "C: (" << c.x << ", " << c.y << ", " << c.z << ")" << std::endl;
	
	// Side vectors
	auto ab = b - a;
	auto ac = c - a;
	auto bc = c - b;
	
	std::cout << "Side AB: (" << ab.x << ", " << ab.y << ", " << ab.z << "), length: " << ab.length() << std::endl;
	std::cout << "Side AC: (" << ac.x << ", " << ac.y << ", " << ac.z << "), length: " << ac.length() << std::endl;
	std::cout << "Side BC: (" << bc.x << ", " << bc.y << ", " << bc.z << "), length: " << bc.length() << std::endl;
	
	// Normal vector (cross product)
	auto normal = ab.cross(ac);
	std::cout << "Triangle normal: (" << normal.x << ", " << normal.y << ", " << normal.z << ")" << std::endl;
	std::cout << "Normal length: " << normal.length() << " (should be area * 2 = 12)" << std::endl;
	
	// Angle between vectors (using dot product)
	float dotABAC = ab.dot(ac);
	float abLen = ab.length();
	float acLen = ac.length();
	float cosAngle = dotABAC / (abLen * acLen);
	std::cout << "Dot product AB·AC: " << dotABAC << std::endl;
	std::cout << "cos(angle) between AB and AC: " << cosAngle << " (should be 0 for 90°)" << std::endl;
}

void test_comparison_operators() {
	std::cout << "\n=== Comparison Operator Tests ===" << std::endl;
	
	IVector3<int> vec1(1, 2, 3);
	IVector3<int> vec2(1, 2, 3);
	IVector3<int> vec3(4, 5, 6);
	
	std::cout << "vec1: (" << vec1.x << ", " << vec1.y << ", " << vec1.z << ")" << std::endl;
	std::cout << "vec2: (" << vec2.x << ", " << vec2.y << ", " << vec2.z << ")" << std::endl;
	std::cout << "vec3: (" << vec3.x << ", " << vec3.y << ", " << vec3.z << ")" << std::endl;
	
	std::cout << "vec1 == vec2: " << (vec1 == vec2 ? "true" : "false") << " (expected: true)" << std::endl;
	std::cout << "vec1 == vec3: " << (vec1 == vec3 ? "true" : "false") << " (expected: false)" << std::endl;
	std::cout << "vec1 != vec2: " << (vec1 != vec2 ? "true" : "false") << " (expected: false)" << std::endl;
	std::cout << "vec1 != vec3: " << (vec1 != vec3 ? "true" : "false") << " (expected: true)" << std::endl;
}

void test_performance() {
	std::cout << "\n=== Performance Tests ===" << std::endl;
	
	const int iterations = 100000;
	std::cout << "Performing " << iterations << " vector operations..." << std::endl;
	
	IVector3<float> vec1(1.5f, 2.5f, 3.5f);
	IVector3<float> vec2(4.5f, 5.5f, 6.5f);
	IVector3<float> result(0.0f, 0.0f, 0.0f);  // Initialize with explicit constructor
	
	// Addition performance
	for (int i = 0; i < iterations; ++i) {
		result = vec1 + vec2;
	}
	std::cout << "Completed " << iterations << " additions" << std::endl;
	
	// Multiplication performance
	for (int i = 0; i < iterations; ++i) {
		result = vec1 * vec2;
	}
	std::cout << "Completed " << iterations << " multiplications" << std::endl;
	
	// Dot product performance
	float dotResult = 0.0f;
	for (int i = 0; i < iterations; ++i) {
		dotResult += vec1.dot(vec2);
	}
	std::cout << "Completed " << iterations << " dot products, sum: " << dotResult << std::endl;
	
	// Cross product performance
	for (int i = 0; i < iterations; ++i) {
		result = vec1.cross(vec2);
	}
	std::cout << "Completed " << iterations << " cross products" << std::endl;
	
	// Normalization performance
	for (int i = 0; i < iterations; ++i) {
		result = vec1.normalize();
	}
	std::cout << "Completed " << iterations << " normalizations" << std::endl;
}

void test_chaining_operations() {
	std::cout << "\n=== Operation Chaining Tests ===" << std::endl;
	
	IVector3<float> v1(1.0f, 2.0f, 3.0f);
	IVector3<float> v2(4.0f, 5.0f, 6.0f);
	IVector3<float> v3(7.0f, 8.0f, 9.0f);
	
	std::cout << "v1: (" << v1.x << ", " << v1.y << ", " << v1.z << ")" << std::endl;
	std::cout << "v2: (" << v2.x << ", " << v2.y << ", " << v2.z << ")" << std::endl;
	std::cout << "v3: (" << v3.x << ", " << v3.y << ", " << v3.z << ")" << std::endl;
	
	// Complex chained operations (only vector-to-vector operations)
	auto result1 = (v1 + v2) * v3;
	std::cout << "(v1 + v2) * v3: (" << result1.x << ", " << result1.y << ", " << result1.z << ")" << std::endl;
	
	// Normalized result of complex operation
	auto result3 = (v1 + v2 + v3).normalize();
	std::cout << "normalize(v1+v2+v3): (" << result3.x << ", " << result3.y << ", " << result3.z << ")" << std::endl;
	std::cout << "Length: " << result3.length() << " (should be ~1.0)" << std::endl;
}

void test_zero_vector_properties() {
	std::cout << "\n=== Zero Vector Properties ===" << std::endl;
	
	IVector3<float> zero(0.0f, 0.0f, 0.0f);
	IVector3<float> nonZero(3.0f, 4.0f, 5.0f);
	
	std::cout << "Zero vector: (" << zero.x << ", " << zero.y << ", " << zero.z << ")" << std::endl;
	std::cout << "Non-zero vector: (" << nonZero.x << ", " << nonZero.y << ", " << nonZero.z << ")" << std::endl;
	
	// Zero vector properties
	std::cout << "Zero vector length: " << zero.length() << " (should be 0)" << std::endl;
	
	// Adding zero vector
	auto addZero = nonZero + zero;
	std::cout << "nonZero + zero: (" << addZero.x << ", " << addZero.y << ", " << addZero.z << ")" << std::endl;
	std::cout << "Should equal nonZero: " << (addZero == nonZero ? "true" : "false") << std::endl;
	
	// Multiplying by zero vector
	auto mulZero = nonZero * zero;
	std::cout << "nonZero * zero: (" << mulZero.x << ", " << mulZero.y << ", " << mulZero.z << ")" << std::endl;
	std::cout << "Should be zero vector: " << (mulZero == zero ? "true" : "false") << std::endl;
	
	// Dot product with zero
	float dotZero = nonZero.dot(zero);
	std::cout << "nonZero · zero: " << dotZero << " (should be 0)" << std::endl;
	
	// Cross product with zero
	auto crossZero = nonZero.cross(zero);
	std::cout << "nonZero × zero: (" << crossZero.x << ", " << crossZero.y << ", " << crossZero.z << ")" << std::endl;
	std::cout << "Should be zero vector: " << (crossZero == zero ? "true" : "false") << std::endl;
}

int main() {
	std::cout << "Starting comprehensive IVector3 testing..." << std::endl;
	
	test_original();
	test_constructors();
	test_move_assignment();
	test_mathematical_operations();
	test_different_types();
	test_edge_cases();
	test_geometric_operations();
	test_comparison_operators();
	test_performance();
	test_chaining_operations();
	test_zero_vector_properties();
	
	std::cout << "\nAll IVector3 tests completed!" << std::endl;
	return 0;
}
