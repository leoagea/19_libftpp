#include <iostream>
#include <iomanip>
#include "../inc/libftpp.hpp"

// Helper function for better output formatting
void printSeparator(const std::string& testName) {
	std::cout << "\n" << std::string(50, '=') << std::endl;
	std::cout << "  " << testName << std::endl;
	std::cout << std::string(50, '=') << std::endl;
}

void runOriginalTests() {
	printSeparator("ORIGINAL TESTS");
	
	IVector2<int> vec1(3, 4);
	IVector2<int> vec2(1, 2);
	
	std::cout << "Vec1 : " << vec1.x << " / " << vec1.y << std::endl;
	std::cout << "Vec2 : " << vec2.x << " / " << vec2.y << std::endl;
	
	// Test operator overloads
	auto vecAdd = vec1 + vec2;
	std::cout << "vec1 + vec2 = (" << vecAdd.x << ", " << vecAdd.y << ")" << std::endl; 
	// Expected: vec1 + vec2 = (4, 6)

	auto vecSub = vec1 - vec2;
	std::cout << "vec1 - vec2 = (" << vecSub.x << ", " << vecSub.y << ")" << std::endl; 
	// Expected: vec1 - vec2 = (2, 2)

	auto vecMul = vec1 * vec2;
	std::cout << "vec1 * vec2 = (" << vecMul.x << ", " << vecMul.y << ")" << std::endl; 
	// Expected: vec1 * vec2 = (3, 8)

	auto vecDiv = vec1 / vec2;
	std::cout << "vec1 / vec2 = (" << vecDiv.x << ", " << vecDiv.y << ")" << std::endl; 
	// Expected: vec1 / vec2 = (3, 2)

	bool isEqual = vec1 == vec2;
	std::cout << "vec1 == vec2: " << (isEqual ? "true" : "false") << "" << std::endl; 
	// Expected: vec1 == vec2: false

	bool isNotEqual = vec1 != vec2;
	std::cout << "vec1 != vec2: " << (isNotEqual ? "true" : "false") << "" << std::endl; 
	// Expected: vec1 != vec2: true

	// Test additional methods
	float len = vec1.length();
	std::cout << "Length of vec1: " << len << "" << std::endl; 
	// Expected: Length of vec1: 5 (or sqrt(3*3 + 4*4))

	auto normVec = vec1.normalize();
	std::cout << "Normalized vec1 = (" << normVec.x << ", " << normVec.y << ")" << std::endl; 
	// Expected: Normalized vec1 = (0.6, 0.8)

	float dotProd = vec1.dot(vec2);
	std::cout << "Dot product of vec1 and vec2: " << dotProd << "" << std::endl; 
	// Expected: Dot product of vec1 and vec2: 11 (or 3*1 + 4*2)

	auto crossProd = vec1.cross();
	std::cout << "Cross product of vec1: (" << crossProd.x << ", " << crossProd.y << ")" << std::endl; 
	// Expected: Cross product of vec1: (some_value, some_value)
}

void runFloatingPointTests() {
	printSeparator("FLOATING POINT TESTS");
	
	IVector2<float> vecF1(3.5f, 4.2f);
	IVector2<float> vecF2(1.1f, 2.7f);
	
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "Float Vec1: (" << vecF1.x << ", " << vecF1.y << ")" << std::endl;
	std::cout << "Float Vec2: (" << vecF2.x << ", " << vecF2.y << ")" << std::endl;
	
	auto fAdd = vecF1 + vecF2;
	std::cout << "vecF1 + vecF2 = (" << fAdd.x << ", " << fAdd.y << ")" << std::endl;
	
	auto fSub = vecF1 - vecF2;
	std::cout << "vecF1 - vecF2 = (" << fSub.x << ", " << fSub.y << ")" << std::endl;
	
	float fLength = vecF1.length();
	std::cout << "Length of vecF1: " << fLength << std::endl;
	
	auto fNorm = vecF1.normalize();
	std::cout << "Normalized vecF1 = (" << fNorm.x << ", " << fNorm.y << ")" << std::endl;
	
	float fDot = vecF1.dot(vecF2);
	std::cout << "Dot product: " << fDot << std::endl;
	
	std::cout << std::defaultfloat; // Reset formatting
}

void runDoubleTests() {
	printSeparator("DOUBLE PRECISION TESTS");
	
	IVector2<double> vecD1(3.141592653589793, 2.718281828459045);
	IVector2<double> vecD2(1.414213562373095, 1.732050807568877);
	
	std::cout << std::fixed << std::setprecision(6);
	std::cout << "Double Vec1: (" << vecD1.x << ", " << vecD1.y << ")" << std::endl;
	std::cout << "Double Vec2: (" << vecD2.x << ", " << vecD2.y << ")" << std::endl;
	
	double dLength = vecD1.length();
	std::cout << "High precision length: " << dLength << std::endl;
	
	auto dNorm = vecD1.normalize();
	std::cout << "Normalized = (" << dNorm.x << ", " << dNorm.y << ")" << std::endl;
	
	double dDot = vecD1.dot(vecD2);
	std::cout << "High precision dot product: " << dDot << std::endl;
	
	std::cout << std::defaultfloat; // Reset formatting
}

void runZeroVectorTests() {
	printSeparator("ZERO VECTOR TESTS");
	
	IVector2<int> zero(0, 0);
	IVector2<int> nonZero(5, 3);
	
	std::cout << "Zero vector: (" << zero.x << ", " << zero.y << ")" << std::endl;
	std::cout << "Non-zero vector: (" << nonZero.x << ", " << nonZero.y << ")" << std::endl;
	
	auto zeroAdd = zero + nonZero;
	std::cout << "zero + nonZero = (" << zeroAdd.x << ", " << zeroAdd.y << ")" << std::endl;
	
	auto zeroMul = zero * nonZero;
	std::cout << "zero * nonZero = (" << zeroMul.x << ", " << zeroMul.y << ")" << std::endl;
	
	float zeroLength = zero.length();
	std::cout << "Length of zero vector: " << zeroLength << std::endl;
	
	float zeroDot = zero.dot(nonZero);
	std::cout << "Dot product with zero: " << zeroDot << std::endl;
	
	// Test normalization of zero vector (should handle gracefully)
	try {
		auto zeroNorm = zero.normalize();
		std::cout << "Normalized zero = (" << zeroNorm.x << ", " << zeroNorm.y << ")" << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Zero vector normalization error: " << e.what() << std::endl;
	}
}

void runUnitVectorTests() {
	printSeparator("UNIT VECTOR TESTS");
	
	IVector2<float> unitX(1.0f, 0.0f);
	IVector2<float> unitY(0.0f, 1.0f);
	
	std::cout << std::fixed << std::setprecision(3);
	
	std::cout << "Unit X: (" << unitX.x << ", " << unitX.y << ")" << std::endl;
	std::cout << "Unit Y: (" << unitY.x << ", " << unitY.y << ")" << std::endl;
	
	float unitXLength = unitX.length();
	float unitYLength = unitY.length();
	std::cout << "Length of unit X: " << unitXLength << std::endl;
	std::cout << "Length of unit Y: " << unitYLength << std::endl;
	
	float dotProduct = unitX.dot(unitY);
	std::cout << "Dot product of perpendicular units: " << dotProduct << std::endl;
	
	auto unitSum = unitX + unitY;
	float sumLength = unitSum.length();
	std::cout << "Length of (unitX + unitY): " << sumLength << std::endl;
	std::cout << "Should be sqrt(2) ≈ 1.414" << std::endl;
	
	std::cout << std::defaultfloat;
}

void runLargeNumberTests() {
	printSeparator("LARGE NUMBER TESTS");
	
	IVector2<long> large1(1000000, 2000000);
	IVector2<long> large2(500000, 750000);
	
	std::cout << "Large Vec1: (" << large1.x << ", " << large1.y << ")" << std::endl;
	std::cout << "Large Vec2: (" << large2.x << ", " << large2.y << ")" << std::endl;
	
	auto largeAdd = large1 + large2;
	std::cout << "Addition: (" << largeAdd.x << ", " << largeAdd.y << ")" << std::endl;
	
	auto largeMul = large1 * large2;
	std::cout << "Multiplication: (" << largeMul.x << ", " << largeMul.y << ")" << std::endl;
	
	float largeLength = large1.length();
	std::cout << "Length of large vector: " << largeLength << std::endl;
	
	long largeDot = large1.dot(large2);
	std::cout << "Dot product: " << largeDot << std::endl;
}

void runNegativeNumberTests() {
	printSeparator("NEGATIVE NUMBER TESTS");
	
	IVector2<int> pos(3, 4);
	IVector2<int> neg(-3, -4);
	IVector2<int> mixed(5, -2);
	
	std::cout << "Positive: (" << pos.x << ", " << pos.y << ")" << std::endl;
	std::cout << "Negative: (" << neg.x << ", " << neg.y << ")" << std::endl;
	std::cout << "Mixed: (" << mixed.x << ", " << mixed.y << ")" << std::endl;
	
	auto negAdd = pos + neg;
	std::cout << "pos + neg = (" << negAdd.x << ", " << negAdd.y << ")" << std::endl;
	
	auto negMul = pos * neg;
	std::cout << "pos * neg = (" << negMul.x << ", " << negMul.y << ")" << std::endl;
	
	float posLength = pos.length();
	float negLength = neg.length();
	std::cout << "Length of positive: " << posLength << std::endl;
	std::cout << "Length of negative: " << negLength << std::endl;
	std::cout << "They should be equal!" << std::endl;
	
	auto negNorm = neg.normalize();
	std::cout << "Normalized negative = (" << negNorm.x << ", " << negNorm.y << ")" << std::endl;
}

void runComparisonTests() {
	printSeparator("COMPARISON TESTS");
	
	IVector2<int> vec1(3, 4);
	IVector2<int> vec2(3, 4);  // Same as vec1
	IVector2<int> vec3(4, 3);  // Different
	IVector2<int> vec4(-3, -4); // Opposite of vec1
	
	std::cout << "vec1: (" << vec1.x << ", " << vec1.y << ")" << std::endl;
	std::cout << "vec2: (" << vec2.x << ", " << vec2.y << ")" << std::endl;
	std::cout << "vec3: (" << vec3.x << ", " << vec3.y << ")" << std::endl;
	std::cout << "vec4: (" << vec4.x << ", " << vec4.y << ")" << std::endl;
	
	std::cout << "vec1 == vec2: " << (vec1 == vec2 ? "true" : "false") << std::endl;
	std::cout << "vec1 == vec3: " << (vec1 == vec3 ? "true" : "false") << std::endl;
	std::cout << "vec1 == vec4: " << (vec1 == vec4 ? "true" : "false") << std::endl;
	
	std::cout << "vec1 != vec2: " << (vec1 != vec2 ? "true" : "false") << std::endl;
	std::cout << "vec1 != vec3: " << (vec1 != vec3 ? "true" : "false") << std::endl;
	std::cout << "vec1 != vec4: " << (vec1 != vec4 ? "true" : "false") << std::endl;
}

void runDivisionByZeroTests() {
	printSeparator("DIVISION BY ZERO TESTS");
	
	IVector2<int> numerator(10, 20);
	IVector2<int> zeroX(0, 5);
	IVector2<int> zeroY(5, 0);
	IVector2<int> zeroAll(0, 0);
	
	std::cout << "Numerator: (" << numerator.x << ", " << numerator.y << ")" << std::endl;
	
	try {
		auto divZeroX = numerator / zeroX;
		std::cout << "numerator / (0,5) = (" << divZeroX.x << ", " << divZeroX.y << ")" << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Division by zero X caught: " << e.what() << std::endl;
	}
	
	try {
		auto divZeroY = numerator / zeroY;
		std::cout << "numerator / (5,0) = (" << divZeroY.x << ", " << divZeroY.y << ")" << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Division by zero Y caught: " << e.what() << std::endl;
	}
	
	try {
		auto divZeroAll = numerator / zeroAll;
		std::cout << "numerator / (0,0) = (" << divZeroAll.x << ", " << divZeroAll.y << ")" << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Division by zero caught: " << e.what() << std::endl;
	}
}

void runPerpendicularnessTests() {
	printSeparator("PERPENDICULARITY TESTS");
	
	IVector2<float> vec1(3.0f, 4.0f);
	auto perp = vec1.cross(); // This should give perpendicular vector
	
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "Original: (" << vec1.x << ", " << vec1.y << ")" << std::endl;
	std::cout << "Perpendicular: (" << perp.x << ", " << perp.y << ")" << std::endl;
	
	float dotWithPerp = vec1.dot(perp);
	std::cout << "Dot product with perpendicular: " << dotWithPerp << std::endl;
	std::cout << "Should be 0 for perpendicular vectors!" << std::endl;
	
	float perpLength = perp.length();
	float origLength = vec1.length();
	std::cout << "Original length: " << origLength << std::endl;
	std::cout << "Perpendicular length: " << perpLength << std::endl;
	std::cout << "Should be equal!" << std::endl;
	
	std::cout << std::defaultfloat;
}

int main() {
	std::cout << "IVector2 Comprehensive Test Suite" << std::endl;
	std::cout << "=================================" << std::endl;
	
	// Run all test suites
	runOriginalTests();
	runFloatingPointTests();
	runDoubleTests();
	runZeroVectorTests();
	runUnitVectorTests();
	runLargeNumberTests();
	runNegativeNumberTests();
	runComparisonTests();
	runDivisionByZeroTests();
	runPerpendicularnessTests();
	
	printSeparator("ALL TESTS COMPLETED");
	
	return 0;
}