#include "../inc/libftpp.hpp"
#include <iostream>
#include <string>
#include <vector>

enum class EventType {
	EVENT_ONE,
	EVENT_TWO,
	EVENT_THREE,
	USER_LOGIN,
	USER_LOGOUT,
	DATA_UPDATED,
	ERROR_OCCURRED
};

class TestCounter {
public:
	static int count;
	static void increment() { count++; }
	static void decrement() { count--; }
	static void reset() { count = 0; }
	static void print() { std::cout << "Counter: " << count << std::endl; }
};

int TestCounter::count = 0;

void test_original() {
	std::cout << "\n=== Original Test ===\n";
	
	Observer<EventType> observer;

	// Subscribe to EVENT_ONE
	observer.subscribe(EventType::EVENT_ONE, []() {
		std::cout << "Event One triggered" << std::endl;
	});

	// Subscribe first lambda to EVENT_TWO
	observer.subscribe(EventType::EVENT_TWO, []() {
		std::cout << "Event Two triggered (First subscriber)" << std::endl;
	});

	// Subscribe second lambda to EVENT_TWO
	observer.subscribe(EventType::EVENT_TWO, []() {
		std::cout << "Event Two triggered (Second subscriber)" << std::endl;
	});

	// Triggering EVENT_ONE
	std::cout << "Notify EVENT_ONE" << std::endl;
	observer.notify(EventType::EVENT_ONE);  // Output: "Event One triggered"

	// Triggering EVENT_TWO
	std::cout << "Notify EVENT_TWO" << std::endl;
	observer.notify(EventType::EVENT_TWO);  
	// Output: 
	// "Event Two triggered (First subscriber)"
	// "Event Two triggered (Second subscriber)"
	// The order may differ

	// Triggering EVENT_THREE (No subscriber)
	std::cout << "Notify EVENT_THREE" << std::endl;
	observer.notify(EventType::EVENT_THREE);  // Output: None, as there are no subscribers
}

void test_multiple_subscribers() {
	std::cout << "\n=== Multiple Subscribers Test ===\n";
	
	Observer<EventType> observer;
	TestCounter::reset();

	// Add multiple subscribers to the same event
	for (int i = 1; i <= 5; ++i) {
		observer.subscribe(EventType::DATA_UPDATED, [i]() {
			std::cout << "Subscriber " << i << " received DATA_UPDATED" << std::endl;
		});
	}

	std::cout << "Notifying DATA_UPDATED with 5 subscribers:" << std::endl;
	observer.notify(EventType::DATA_UPDATED);
}

void test_counter_operations() {
	std::cout << "\n=== Counter Operations Test ===\n";
	
	Observer<EventType> observer;
	TestCounter::reset();

	// Subscribe counter operations to different events
	observer.subscribe(EventType::USER_LOGIN, TestCounter::increment);
	observer.subscribe(EventType::USER_LOGIN, TestCounter::print);
	
	observer.subscribe(EventType::USER_LOGOUT, TestCounter::decrement);
	observer.subscribe(EventType::USER_LOGOUT, TestCounter::print);

	observer.subscribe(EventType::ERROR_OCCURRED, TestCounter::reset);
	observer.subscribe(EventType::ERROR_OCCURRED, []() {
		std::cout << "Error occurred - counter reset!" << std::endl;
	});

	std::cout << "Initial state:" << std::endl;
	TestCounter::print();

	std::cout << "\nSimulating 3 user logins:" << std::endl;
	for (int i = 0; i < 3; ++i) {
		std::cout << "Login " << (i+1) << ":" << std::endl;
		observer.notify(EventType::USER_LOGIN);
	}

	std::cout << "\nSimulating 1 user logout:" << std::endl;
	observer.notify(EventType::USER_LOGOUT);

	std::cout << "\nSimulating error:" << std::endl;
	observer.notify(EventType::ERROR_OCCURRED);
}

void test_string_events() {
	std::cout << "\n=== String Events Test ===\n";
	
	Observer<std::string> stringObserver;

	// Subscribe to string events
	stringObserver.subscribe("file_saved", []() {
		std::cout << "File has been saved successfully!" << std::endl;
	});

	stringObserver.subscribe("file_loaded", []() {
		std::cout << "File has been loaded!" << std::endl;
	});

	stringObserver.subscribe("network_error", []() {
		std::cout << "Network connection failed!" << std::endl;
	});

	// Add multiple handlers for the same string event
	stringObserver.subscribe("user_action", []() {
		std::cout << "Logging user action..." << std::endl;
	});

	stringObserver.subscribe("user_action", []() {
		std::cout << "Updating UI..." << std::endl;
	});

	stringObserver.subscribe("user_action", []() {
		std::cout << "Sending analytics..." << std::endl;
	});

	std::cout << "Triggering file operations:" << std::endl;
	stringObserver.notify("file_saved");
	stringObserver.notify("file_loaded");

	std::cout << "\nTriggering user action (multiple handlers):" << std::endl;
	stringObserver.notify("user_action");

	std::cout << "\nTriggering network error:" << std::endl;
	stringObserver.notify("network_error");

	std::cout << "\nTriggering unknown event:" << std::endl;
	stringObserver.notify("unknown_event");  // Should do nothing
}

void test_capture_variables() {
	std::cout << "\n=== Capture Variables Test ===\n";
	
	Observer<EventType> observer;
	
	std::string userName = "Alice";
	int userId = 12345;
	std::vector<std::string> log;

	// Capture variables by reference
	observer.subscribe(EventType::USER_LOGIN, [&]() {
		log.push_back("User " + userName + " (ID: " + std::to_string(userId) + ") logged in");
		std::cout << "Welcome " << userName << "!" << std::endl;
	});

	// Capture by value
	observer.subscribe(EventType::USER_LOGOUT, [userName, userId, &log]() {
		log.push_back("User " + userName + " (ID: " + std::to_string(userId) + ") logged out");
		std::cout << "Goodbye " << userName << "!" << std::endl;
	});

	std::cout << "Testing with Alice:" << std::endl;
	observer.notify(EventType::USER_LOGIN);
	observer.notify(EventType::USER_LOGOUT);

	// Change variables (only affects capture by reference)
	userName = "Bob";
	userId = 67890;

	std::cout << "\nTesting after changing to Bob:" << std::endl;
	observer.notify(EventType::USER_LOGIN);   // Will show Bob (captured by reference)
	observer.notify(EventType::USER_LOGOUT);  // Will show Alice (captured by value)

	std::cout << "\nEvent log:" << std::endl;
	for (const auto& entry : log) {
		std::cout << "- " << entry << std::endl;
	}
}

void test_move_semantics() {
	std::cout << "\n=== Move Semantics Test ===\n";
	
	Observer<EventType> observer1;
	
	observer1.subscribe(EventType::EVENT_ONE, []() {
		std::cout << "Observer1: EVENT_ONE triggered" << std::endl;
	});

	observer1.subscribe(EventType::EVENT_TWO, []() {
		std::cout << "Observer1: EVENT_TWO triggered" << std::endl;
	});

	std::cout << "Testing observer1:" << std::endl;
	observer1.notify(EventType::EVENT_ONE);
	observer1.notify(EventType::EVENT_TWO);

	// Move observer1 to observer2
	Observer<EventType> observer2 = std::move(observer1);

	std::cout << "\nAfter move, testing observer2:" << std::endl;
	observer2.notify(EventType::EVENT_ONE);
	observer2.notify(EventType::EVENT_TWO);

	std::cout << "\nTesting moved-from observer1 (should be empty):" << std::endl;
	observer1.notify(EventType::EVENT_ONE);  // Should do nothing
	observer1.notify(EventType::EVENT_TWO);  // Should do nothing
}

void test_integer_events() {
	std::cout << "\n=== Integer Events Test ===\n";
	
	Observer<int> intObserver;

	// Subscribe to different integer events
	for (int i = 1; i <= 5; ++i) {
		intObserver.subscribe(i, [i]() {
			std::cout << "Event " << i << " triggered!" << std::endl;
		});
	}

	// Add multiple subscribers to event 3
	intObserver.subscribe(3, []() {
		std::cout << "Special handler for event 3!" << std::endl;
	});

	std::cout << "Triggering events 1-5:" << std::endl;
	for (int i = 1; i <= 5; ++i) {
		std::cout << "Notifying event " << i << ":" << std::endl;
		intObserver.notify(i);
	}

	std::cout << "\nTriggering non-existent event 10:" << std::endl;
	intObserver.notify(10);  // Should do nothing
} 

int main() {
	test_original();
	test_multiple_subscribers();
	test_counter_operations();
	test_string_events();
	test_capture_variables();
	test_move_semantics();
	test_integer_events();

	std::cout << "\nAll observer tests completed!" << std::endl;
	return 0;
}