#include "../inc/libftpp.hpp"
#include <iostream>
#include <vector>
#include <cassert>

class TestClass : public Memento {
	friend class Memento;

public:
	int x;
	std::string y;

private:
	void _saveToSnapshot(Memento::Snapshot& snapshotToFill) const override {
		snapshotToFill << x << y;
	}

	void _loadFromSnapshot(Memento::Snapshot& snapshot) override {
		snapshot >> x >> y;
	}
};

class ComplexTestClass : public Memento {
	friend class Memento;

public:
	int id;
	std::string name;
	std::vector<int> numbers;
	float balance;
	bool active;

	ComplexTestClass(int i = 0, const std::string& n = "", float b = 0.0f, bool a = true) 
		: id(i), name(n), balance(b), active(a) {}

	void addNumber(int num) { numbers.push_back(num); }
	
	void print() const {
		std::cout << "ID: " << id << ", Name: " << name << ", Balance: " << balance 
				  << ", Active: " << active << ", Numbers: [";
		for (size_t i = 0; i < numbers.size(); ++i) {
			std::cout << numbers[i];
			if (i < numbers.size() - 1) std::cout << ", ";
		}
		std::cout << "]" << std::endl;
	}

private:
	void _saveToSnapshot(Memento::Snapshot& snapshotToFill) const override {
		snapshotToFill << id << name << balance << active;
		
		// Save vector size first, then elements
		size_t size = numbers.size();
		snapshotToFill << size;
		for (const auto& num : numbers) {
			snapshotToFill << num;
		}
	}

	void _loadFromSnapshot(Memento::Snapshot& snapshot) override {
		snapshot >> id >> name >> balance >> active;
		
		// Load vector size first, then elements
		size_t size;
		snapshot >> size;
		numbers.clear();
		numbers.reserve(size);
		for (size_t i = 0; i < size; ++i) {
			int num;
			snapshot >> num;
			numbers.push_back(num);
		}
	}
};

class CounterClass : public Memento {
	friend class Memento;

public:
	int counter;
	
	CounterClass() : counter(0) {}
	void increment() { counter++; }
	void decrement() { counter--; }

private:
	void _saveToSnapshot(Memento::Snapshot& snapshotToFill) const override {
		snapshotToFill << counter;
	}

	void _loadFromSnapshot(Memento::Snapshot& snapshot) override {
		snapshot >> counter;
	}
};

void test_original() {
	std::cout << "\n=== Original Test ===\n";
	
	TestClass myObject;
	myObject.x = 42;
	myObject.y = "Hello";

	// Save the current state
	Memento::Snapshot savedState = myObject.save();

	// Modify the object
	myObject.x = 100;
	myObject.y = "World";

	// Output the modified object
	// Expected Output: "Current state: x = 100, y = World"
	std::cout << "Current state: x = " << myObject.x << ", y = " << myObject.y << std::endl;

	// Restore the object to its saved state
	myObject.load(savedState);
	
	// Output the restored object
	// Expected Output: "Restored state: x = 42, y = Hello"
	std::cout << "Restored state: x = " << myObject.x << ", y = " << myObject.y << std::endl;
}

void test_multiple_snapshots() {
	std::cout << "\n=== Multiple Snapshots Test ===\n";
	
	TestClass obj;
	obj.x = 1;
	obj.y = "First";
	
	Memento::Snapshot snap1 = obj.save();
	std::cout << "Snapshot 1: x = " << obj.x << ", y = " << obj.y << std::endl;
	
	obj.x = 2;
	obj.y = "Second";
	Memento::Snapshot snap2 = obj.save();
	std::cout << "Snapshot 2: x = " << obj.x << ", y = " << obj.y << std::endl;
	
	obj.x = 3;
	obj.y = "Third";
	std::cout << "Current: x = " << obj.x << ", y = " << obj.y << std::endl;
	
	// Restore to snap1
	obj.load(snap1);
	std::cout << "Restored to snap1: x = " << obj.x << ", y = " << obj.y << std::endl;
	
	// Restore to snap2
	obj.load(snap2);
	std::cout << "Restored to snap2: x = " << obj.x << ", y = " << obj.y << std::endl;
}

void test_complex_object() {
	std::cout << "\n=== Complex Object Test ===\n";
	
	ComplexTestClass obj(123, "Alice", 1500.75f, true);
	obj.addNumber(10);
	obj.addNumber(20);
	obj.addNumber(30);
	
	std::cout << "Initial state: ";
	obj.print();
	
	Memento::Snapshot snapshot = obj.save();
	
	// Modify object
	obj.id = 999;
	obj.name = "Bob";
	obj.balance = -100.0f;
	obj.active = false;
	obj.addNumber(999);
	obj.addNumber(888);
	
	std::cout << "Modified state: ";
	obj.print();
	
	// Restore
	obj.load(snapshot);
	std::cout << "Restored state: ";
	obj.print();
}

void test_move_semantics() {
	std::cout << "\n=== Move Semantics Test ===\n";
	
	TestClass obj;
	obj.x = 42;
	obj.y = "Move Test";
	
	Memento::Snapshot snap1 = obj.save();
	
	// Test move constructor
	Memento::Snapshot snap2 = std::move(snap1);
	
	obj.x = 999;
	obj.y = "Changed";
	std::cout << "Before restore: x = " << obj.x << ", y = " << obj.y << std::endl;
	
	obj.load(snap2);
	std::cout << "After restore: x = " << obj.x << ", y = " << obj.y << std::endl;
}

void test_counter_workflow() {
	std::cout << "\n=== Counter Workflow Test ===\n";
	
	CounterClass counter;
	std::vector<Memento::Snapshot> history;
	
	// Create some history
	for (int i = 0; i < 5; ++i) {
		history.push_back(counter.save());
		counter.increment();
		std::cout << "Counter: " << counter.counter << std::endl;
	}
	
	std::cout << "Final counter: " << counter.counter << std::endl;
	
	// Undo operations
	std::cout << "\nUndoing operations:" << std::endl;
	for (int i = history.size() - 1; i >= 0; --i) {
		counter.load(history[i]);
		std::cout << "Undo to: " << counter.counter << std::endl;
	}
}

void test_empty_strings() {
	std::cout << "\n=== Empty Strings Test ===\n";
	
	TestClass obj;
	obj.x = 0;
	obj.y = "";  // Empty string
	
	Memento::Snapshot snapshot = obj.save();
	
	obj.x = 123;
	obj.y = "Not empty";
	
	std::cout << "Before restore: x = " << obj.x << ", y = '" << obj.y << "'" << std::endl;
	
	obj.load(snapshot);
	std::cout << "After restore: x = " << obj.x << ", y = '" << obj.y << "'" << std::endl;
	std::cout << "Empty string length: " << obj.y.length() << std::endl;
}

void test_large_data() {
	std::cout << "\n=== Large Data Test ===\n";
	
	ComplexTestClass obj(1, "LargeTest", 0.0f, true);
	
	// Add many numbers
	for (int i = 0; i < 1000; ++i) {
		obj.addNumber(i * i);
	}
	
	std::cout << "Large object with " << obj.numbers.size() << " numbers created" << std::endl;
	
	Memento::Snapshot snapshot = obj.save();
	
	// Clear and modify
	obj.numbers.clear();
	obj.id = 999;
	obj.name = "Cleared";
	
	std::cout << "After clearing: " << obj.numbers.size() << " numbers, ID: " << obj.id << std::endl;
	
	// Restore
	obj.load(snapshot);
	std::cout << "After restore: " << obj.numbers.size() << " numbers, ID: " << obj.id << std::endl;
	
	// Verify some data
	bool correct = true;
	for (size_t i = 0; i < std::min(size_t(10), obj.numbers.size()); ++i) {
		if (obj.numbers[i] != static_cast<int>(i * i)) {
			correct = false;
			break;
		}
	}
	std::cout << "Data integrity check: " << (correct ? "PASSED" : "FAILED") << std::endl;
}

int main() {
	test_original();
	test_multiple_snapshots();
	test_complex_object();
	test_move_semantics();
	test_counter_workflow();
	test_empty_strings();
	test_large_data();
	
	std::cout << "\nAll memento tests completed!" << std::endl;
	return 0;
}