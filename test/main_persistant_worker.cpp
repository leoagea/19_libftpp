#include "../inc/libftpp.hpp"
#include <iostream>
#include <atomic>
#include <chrono>
#include <random>

// Global counters for testing
std::atomic<int> task1Counter{0};
std::atomic<int> task2Counter{0};
std::atomic<int> monitoringCounter{0};
std::atomic<int> heartbeatCounter{0};

void test_original() {
	std::cout << "\n=== Original Test ===\n";
	
	PersistentWorker worker;

	auto task1 = []() {
		threadSafeCout << "Executing Task 1" << std::endl;
	};

	auto task2 = []() {
		threadSafeCout << "Executing Task 2" << std::endl;
	};

	worker.addTask("Task1", task1);
	worker.addTask("Task2", task2);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	worker.removeTask("Task1");

	std::this_thread::sleep_for(std::chrono::seconds(1));
	
	std::cout << "Original test completed\n";
}

void test_counter_tasks() {
	std::cout << "\n=== Counter Tasks Test ===\n";
	
	task1Counter.store(0);
	task2Counter.store(0);
	
	PersistentWorker worker;

	auto counterTask1 = []() {
		int count = task1Counter.fetch_add(1);
		threadSafeCout << "Counter Task 1: " << count << std::endl;
	};

	auto counterTask2 = []() {
		int count = task2Counter.fetch_add(1);
		threadSafeCout << "Counter Task 2: " << count << std::endl;
	};

	worker.addTask("CounterTask1", counterTask1);
	worker.addTask("CounterTask2", counterTask2);

	std::this_thread::sleep_for(std::chrono::seconds(2));

	std::cout << "Final counts - Task1: " << task1Counter.load() 
			  << ", Task2: " << task2Counter.load() << std::endl;
}

void test_dynamic_task_management() {
	std::cout << "\n=== Dynamic Task Management Test ===\n";
	
	PersistentWorker worker;

	auto quickTask = []() {
		threadSafeCout << "Quick task executing" << std::endl;
	};

	auto periodicTask = []() {
		threadSafeCout << "Periodic task running" << std::endl;
	};

	auto monitorTask = []() {
		threadSafeCout << "Monitoring system..." << std::endl;
	};

	// Start with one task
	std::cout << "Starting with quick task only...\n";
	worker.addTask("QuickTask", quickTask);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	// Add more tasks
	std::cout << "Adding periodic and monitor tasks...\n";
	worker.addTask("PeriodicTask", periodicTask);
	worker.addTask("MonitorTask", monitorTask);
	std::this_thread::sleep_for(std::chrono::seconds(1));

	// Remove one task
	std::cout << "Removing quick task...\n";
	worker.removeTask("QuickTask");
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	// Remove another task
	std::cout << "Removing periodic task...\n";
	worker.removeTask("PeriodicTask");
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	// Remove final task
	std::cout << "Removing monitor task...\n";
	worker.removeTask("MonitorTask");
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	std::cout << "Dynamic management test completed\n";
}

void test_monitoring_simulation() {
	std::cout << "\n=== System Monitoring Simulation ===\n";
	
	monitoringCounter.store(0);
	PersistentWorker worker;

	auto cpuMonitor = []() {
		int count = monitoringCounter.fetch_add(1);
		threadSafeCout << "CPU Monitor [" << count << "]: CPU usage 45%" << std::endl;
	};

	auto memoryMonitor = []() {
		threadSafeCout << "Memory Monitor: RAM 2.1GB/8GB used" << std::endl;
	};

	auto diskMonitor = []() {
		threadSafeCout << "Disk Monitor: SSD 156GB/512GB used" << std::endl;
	};

	auto networkMonitor = []() {
		threadSafeCout << "Network Monitor: 10Mbps up, 5Mbps down" << std::endl;
	};

	std::cout << "Starting system monitoring...\n";
	worker.addTask("CPUMonitor", cpuMonitor);
	worker.addTask("MemoryMonitor", memoryMonitor);
	worker.addTask("DiskMonitor", diskMonitor);
	worker.addTask("NetworkMonitor", networkMonitor);

	std::this_thread::sleep_for(std::chrono::seconds(2));

	std::cout << "Simulating network issue - removing network monitor...\n";
	worker.removeTask("NetworkMonitor");
	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::cout << "Total CPU monitoring cycles: " << monitoringCounter.load() << std::endl;
}

void test_heartbeat_service() {
	std::cout << "\n=== Heartbeat Service Test ===\n";
	
	heartbeatCounter.store(0);
	PersistentWorker worker;

	auto heartbeat = []() {
		int beat = heartbeatCounter.fetch_add(1);
		auto now = std::chrono::system_clock::now();
		auto time_t = std::chrono::system_clock::to_time_t(now);
		threadSafeCout << "Heartbeat [" << beat << "] at timestamp: " << time_t << std::endl;
	};

	auto healthCheck = []() {
		threadSafeCout << "Health Check: All systems operational" << std::endl;
	};

	auto logRotation = []() {
		threadSafeCout << "Log Rotation: Checking log file sizes..." << std::endl;
	};

	std::cout << "Starting heartbeat service...\n";
	worker.addTask("Heartbeat", heartbeat);
	worker.addTask("HealthCheck", healthCheck);
	worker.addTask("LogRotation", logRotation);

	std::this_thread::sleep_for(std::chrono::seconds(3));

	std::cout << "Total heartbeats: " << heartbeatCounter.load() << std::endl;
}

void test_task_replacement() {
	std::cout << "\n=== Task Replacement Test ===\n";
	
	PersistentWorker worker;

	auto version1Task = []() {
		threadSafeCout << "Running Task Version 1.0" << std::endl;
	};

	auto version2Task = []() {
		threadSafeCout << "Running Task Version 2.0 (Updated!)" << std::endl;
	};

	auto version3Task = []() {
		threadSafeCout << "Running Task Version 3.0 (Latest!)" << std::endl;
	};

	// Start with version 1
	std::cout << "Deploying version 1.0...\n";
	worker.addTask("MainTask", version1Task);
	std::this_thread::sleep_for(std::chrono::seconds(1));

	// Update to version 2 (replace)
	std::cout << "Updating to version 2.0...\n";
	worker.addTask("MainTask", version2Task);  // Should replace version 1
	std::this_thread::sleep_for(std::chrono::seconds(1));

	// Update to version 3 (replace)
	std::cout << "Updating to version 3.0...\n";
	worker.addTask("MainTask", version3Task);  // Should replace version 2
	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::cout << "Task replacement test completed\n";
}

void test_computational_tasks() {
	std::cout << "\n=== Computational Tasks Test ===\n";
	
	PersistentWorker worker;

	auto mathTask = []() {
		static int iteration = 0;
		double result = 0.0;
		for (int i = 1; i <= 1000; ++i) {
			result += 1.0 / (i * i);
		}
		threadSafeCout << "Math Task [" << iteration++ << "]: π²/6 ≈ " << result << std::endl;
	};

	auto primeTask = []() {
		static int number = 100;
		bool isPrime = true;
		for (int i = 2; i * i <= number; ++i) {
			if (number % i == 0) {
				isPrime = false;
				break;
			}
		}
		threadSafeCout << "Prime Check: " << number << " is " 
					  << (isPrime ? "prime" : "not prime") << std::endl;
		number++;
	};

	std::cout << "Starting computational tasks...\n";
	worker.addTask("MathComputation", mathTask);
	worker.addTask("PrimeChecker", primeTask);

	std::this_thread::sleep_for(std::chrono::seconds(2));

	std::cout << "Stopping math computation...\n";
	worker.removeTask("MathComputation");
	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::cout << "Computational tasks test completed\n";
}

void test_random_data_generation() {
	std::cout << "\n=== Random Data Generation Test ===\n";
	
	PersistentWorker worker;

	auto randomNumberGenerator = []() {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<> dis(1, 100);
		
		int randomNum = dis(gen);
		threadSafeCout << "Random Generator: " << randomNum << std::endl;
	};

	auto dataLogger = []() {
		static int logCount = 0;
		threadSafeCout << "Data Logger: Entry #" << ++logCount << " recorded" << std::endl;
	};

	auto statisticsCalculator = []() {
		static double sum = 0.0;
		static int count = 0;
		
		sum += (count % 10) * 2.5;  // Simulate some data
		count++;
		
		double average = sum / count;
		threadSafeCout << "Statistics: Average = " << average 
					  << " (samples: " << count << ")" << std::endl;
	};

	std::cout << "Starting data generation services...\n";
	worker.addTask("RandomGenerator", randomNumberGenerator);
	worker.addTask("DataLogger", dataLogger);
	worker.addTask("StatsCalculator", statisticsCalculator);

	std::this_thread::sleep_for(std::chrono::seconds(2));

	std::cout << "Stopping random generator...\n";
	worker.removeTask("RandomGenerator");
	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::cout << "Random data generation test completed\n";
}

void test_multiple_workers() {
	std::cout << "\n=== Multiple Workers Test ===\n";
	
	PersistentWorker worker1;
	PersistentWorker worker2;

	auto worker1Task = []() {
		threadSafeCout << "Worker 1 executing its task" << std::endl;
	};

	auto worker2Task = []() {
		threadSafeCout << "Worker 2 executing its task" << std::endl;
	};

	auto sharedTask = []() {
		threadSafeCout << "Shared task executing" << std::endl;
	};

	std::cout << "Starting two separate workers...\n";
	worker1.addTask("Worker1Task", worker1Task);
	worker2.addTask("Worker2Task", worker2Task);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::cout << "Adding shared task to both workers...\n";
	worker1.addTask("SharedTask", sharedTask);
	worker2.addTask("SharedTask", sharedTask);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::cout << "Removing tasks from worker1...\n";
	worker1.removeTask("Worker1Task");
	worker1.removeTask("SharedTask");

	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::cout << "Multiple workers test completed\n";
}

void test_stress_test() {
	std::cout << "\n=== Stress Test - Many Tasks ===\n";
	
	PersistentWorker worker;

	// Add many tasks quickly
	for (int i = 0; i < 10; ++i) {
		worker.addTask("Task" + std::to_string(i), [i]() {
			threadSafeCout << "Stress Task " << i << " executing" << std::endl;
		});
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));

	// Remove half the tasks
	std::cout << "Removing half the tasks...\n";
	for (int i = 0; i < 5; ++i) {
		worker.removeTask("Task" + std::to_string(i));
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));

	// Remove remaining tasks
	std::cout << "Removing remaining tasks...\n";
	for (int i = 5; i < 10; ++i) {
		worker.removeTask("Task" + std::to_string(i));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	std::cout << "Stress test completed\n";
}

void test_task_lifecycle() {
	std::cout << "\n=== Task Lifecycle Test ===\n";
	
	PersistentWorker worker;

	auto shortLivedTask = []() {
		static int count = 0;
		threadSafeCout << "Short-lived task: iteration " << ++count << std::endl;
	};

	auto longLivedTask = []() {
		static int count = 0;
		threadSafeCout << "Long-lived task: iteration " << ++count << std::endl;
	};

	std::cout << "Adding short-lived task...\n";
	worker.addTask("ShortTask", shortLivedTask);
	std::this_thread::sleep_for(std::chrono::milliseconds(300));

	std::cout << "Adding long-lived task...\n";
	worker.addTask("LongTask", longLivedTask);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	std::cout << "Removing short-lived task (long task continues)...\n";
	worker.removeTask("ShortTask");
	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::cout << "Adding short task back...\n";
	worker.addTask("ShortTask", shortLivedTask);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	std::cout << "Removing all tasks...\n";
	worker.removeTask("ShortTask");
	worker.removeTask("LongTask");
	std::this_thread::sleep_for(std::chrono::milliseconds(300));

	std::cout << "Task lifecycle test completed\n";
}

int main() {
	std::cout << "Starting comprehensive PersistentWorker testing...\n";
	
	test_original();
	test_counter_tasks();
	test_dynamic_task_management();
	test_monitoring_simulation();
	test_heartbeat_service();
	test_task_replacement();
	test_computational_tasks();
	test_random_data_generation();
	test_multiple_workers();
	test_stress_test();
	test_task_lifecycle();
	
	std::cout << "\nAll PersistentWorker tests completed!\n";
	return 0;
}