#include "../inc/libftpp.hpp"
#include <iostream>
#include <vector>
#include <cassert>

class TestObject {
public:
	TestObject() { std::cout << "TestObject default constructor" << std::endl; }
	TestObject(int value) { std::cout << "TestObject constructor with value [" << value << "]" << std::endl;}
	~TestObject() { std::cout << "TestObject destructor" << std::endl; }

	void sayHello() const { std::cout << "Hello from TestObject" << std::endl; }
};

void test_original() {
	std::cout << "\n--- test_original ---\n";
	 // Create a Pool for TestObject
	Pool<TestObject> myPool;

	// Resize the pool to pre-allocate 5 objects
	// Should output the 5 "TestObject constructor"
	myPool.resize(5);

	// Acquire an object from the pool
	Pool<TestObject>::Object obj1 = myPool.acquire(15);
	obj1->sayHello();  // Should output: "Hello from TestObject"

	{
		// Acquire another object in a different scope
		Pool<TestObject>::Object obj2 = myPool.acquire();
		obj2->sayHello();  // Should also output: "Hello from TestObject"
		// obj2 is released back to the pool when it goes out of scope
	}

	// Acquire another object; this should give us the object that obj2 pointed to
	Pool<TestObject>::Object obj3 = myPool.acquire();
	obj3->sayHello();  // Should output: "Hello from TestObject"

	// obj1 and obj3 are released back to the pool when the program ends
}

void test_basic_usage() {
	std::cout << "\n--- test_basic_usage ---\n";
	Pool<TestObject> pool;
	pool.resize(3);

	auto obj1 = pool.acquire();
	obj1->sayHello();

	auto obj2 = pool.acquire(42);
	obj2->sayHello();

	{
		auto obj3 = pool.acquire();
		obj3->sayHello();
	} // obj3 released

	auto obj4 = pool.acquire();
	obj4->sayHello();
}

void test_copy_and_move() {
	std::cout << "\n--- test_copy_and_move ---\n";
	Pool<TestObject> pool;
	auto obj1 = pool.acquire(7);

	// Copy constructor
	Pool<TestObject>::Object obj2(obj1);
	obj2->sayHello();

	// Move constructor
	Pool<TestObject>::Object obj3(std::move(obj1));
	obj3->sayHello();

	// Copy assignment
	Pool<TestObject>::Object obj4;
	obj4 = obj2;
	obj4->sayHello();

	// Move assignment
	Pool<TestObject>::Object obj5;
	obj5 = std::move(obj3);
	obj5->sayHello();
}

void test_multiple_release_and_reacquire() {
	std::cout << "\n--- test_multiple_release_and_reacquire ---\n";
	Pool<TestObject> pool;
	std::vector<Pool<TestObject>::Object> objects;

	// Acquire and release many times
	for (int i = 0; i < 10; ++i) {
		objects.push_back(pool.acquire(i));
	}
	objects.clear(); // All objects released

	// Reacquire
	for (int i = 0; i < 5; ++i) {
		auto obj = pool.acquire();
		obj->sayHello();
	}
}

void test_null_object() {
	std::cout << "\n--- test_null_object ---\n";
	Pool<TestObject>::Object nullObj;
	assert(nullObj.operator->() == nullptr);
}

void test_pool_move_assignment() {
	std::cout << "\n--- test_pool_move_assignment ---\n";
	Pool<TestObject> pool1;
	pool1.resize(2);
	auto obj1 = pool1.acquire();
	
	Pool<TestObject> pool2;
	pool2 = std::move(pool1); // Move assignment
	
	auto obj2 = pool2.acquire();
	obj2->sayHello();
}

void test_pool_move_constructor() {
	std::cout << "\n--- test_pool_move_constructor ---\n";
	Pool<TestObject> pool1;
	pool1.resize(2);
	auto obj1 = pool1.acquire();
	
	// Move constructor
	Pool<TestObject> pool2(std::move(pool1));
	
	auto obj2 = pool2.acquire();
	obj2->sayHello();
}

void test_multiple_pools() {
	std::cout << "\n--- test_multiple_pools ---\n";
	Pool<TestObject> pool1;
	Pool<TestObject> pool2;
	
	auto obj1 = pool1.acquire(100);
	auto obj2 = pool2.acquire(200);
	
	obj1->sayHello();
	obj2->sayHello();
}

int main() {
	test_original();
	test_basic_usage();
	test_copy_and_move();
	test_multiple_release_and_reacquire();
	test_null_object();
	test_pool_move_assignment();
	test_pool_move_constructor();
	test_multiple_pools();

	std::cout << "\nAll tests completed.\n";
	return 0;
}