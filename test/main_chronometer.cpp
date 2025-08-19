#include <iostream>
#include <thread>
#include <iomanip>
#include "../libftpp.hpp"

void printDuration(const std::string& label, const Chronometer::duration_ms& duration) {
	std::cout << std::left << std::setw(25) << label << ": " 
			  << std::fixed << std::setprecision(2) << duration.count() << " ms" << std::endl;
}

void printStatus(const Chronometer& chrono) {
	std::cout << "\n--- Chronometer Status ---" << std::endl;
	std::cout << "Running: " << (chrono.isRunning() ? "Yes" : "No") << std::endl;
	std::cout << "Paused:  " << (chrono.isPaused() ? "Yes" : "No") << std::endl;
	printDuration("Elapsed Time", chrono.getElapsedTime());
	printDuration("Paused Time", chrono.getPausedTime());
	printDuration("Total Time", chrono.getTotalTime());
	std::cout << "-------------------------\n" << std::endl;
}

void test_basic_functionality() {
	std::cout << "=== Testing Basic Functionality ===" << std::endl;
	
	Chronometer chrono;
	
	std::cout << "Initial state:" << std::endl;
	printStatus(chrono);
	
	// Test start
	std::cout << "Starting chronometer..." << std::endl;
	chrono.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	printStatus(chrono);
	
	// Test pause
	std::cout << "Pausing chronometer..." << std::endl;
	chrono.pause();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	printStatus(chrono);
	
	// Test resume
	std::cout << "Resuming chronometer..." << std::endl;
	chrono.resume();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	printStatus(chrono);
	
	// Test stop
	std::cout << "Stopping chronometer..." << std::endl;
	chrono.stop();
	
	printStatus(chrono);
}

void test_lap_functionality() {
	std::cout << "\n=== Testing Lap Functionality ===" << std::endl;
	
	Chronometer chrono;
	chrono.start();
	
	std::cout << "Recording laps every 50ms..." << std::endl;
	
	for (int i = 1; i <= 5; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		chrono.lap();
		std::cout << "Lap " << i << " recorded" << std::endl;
	}
	
	std::cout << "\nLap Times:" << std::endl;
	const auto& lapTimes = chrono.getLapTimes();
	for (size_t i = 0; i < lapTimes.size(); ++i) {
		std::cout << "Lap " << (i + 1) << ": " 
				  << std::fixed << std::setprecision(2) 
				  << lapTimes[i].count() << " ms" << std::endl;
	}
	
	std::cout << "\nLap Statistics:" << std::endl;
	printDuration("Average Lap Time", chrono.getAverageLapTime());
	printDuration("Max Lap Time", chrono.getMaxLapTime());
	printDuration("Min Lap Time", chrono.getMinLapTime());
	printDuration("Last Lap Time", chrono.getLapTime());
	
	chrono.stop();
}

void test_split_functionality() {
	std::cout << "\n=== Testing Split Functionality ===" << std::endl;
	
	Chronometer chrono;
	chrono.start();
	
	std::cout << "Recording splits every 30ms..." << std::endl;
	
	for (int i = 1; i <= 5; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		chrono.split();
		std::cout << "Split " << i << " recorded" << std::endl;
	}
	
	std::cout << "\nSplit Times:" << std::endl;
	const auto& splitTimes = chrono.getSplitTimes();
	for (size_t i = 0; i < splitTimes.size(); ++i) {
		std::cout << "Split " << (i + 1) << ": " 
				  << std::fixed << std::setprecision(2) 
				  << splitTimes[i].count() << " ms" << std::endl;
	}
	
	printDuration("Last Split Time", chrono.getSplitTime());
	
	chrono.stop();
}

void test_complex_scenario() {
	std::cout << "\n=== Testing Complex Scenario ===" << std::endl;
	
	Chronometer chrono;
	
	std::cout << "Complex timing scenario:" << std::endl;
	std::cout << "1. Start -> 100ms -> Lap" << std::endl;
	chrono.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	chrono.lap();
	
	std::cout << "2. 50ms -> Pause -> 100ms (paused)" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	chrono.pause();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	std::cout << "3. Resume -> 80ms -> Split" << std::endl;
	chrono.resume();
	std::this_thread::sleep_for(std::chrono::milliseconds(80));
	chrono.split();
	
	std::cout << "4. 60ms -> Lap" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(60));
	chrono.lap();
	
	std::cout << "5. 40ms -> Stop" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(40));
	chrono.stop();
	
	printStatus(chrono);
	
	std::cout << "Final Results:" << std::endl;
	const auto& lapTimes = chrono.getLapTimes();
	const auto& splitTimes = chrono.getSplitTimes();
	
	std::cout << "\nLap Times (" << lapTimes.size() << " total):" << std::endl;
	for (size_t i = 0; i < lapTimes.size(); ++i) {
		std::cout << "  Lap " << (i + 1) << ": " 
				  << std::fixed << std::setprecision(2) 
				  << lapTimes[i].count() << " ms" << std::endl;
	}
	
	std::cout << "\nSplit Times (" << splitTimes.size() << " total):" << std::endl;
	for (size_t i = 0; i < splitTimes.size(); ++i) {
		std::cout << "  Split " << (i + 1) << ": " 
				  << std::fixed << std::setprecision(2) 
				  << splitTimes[i].count() << " ms" << std::endl;
	}
}

void test_edge_cases() {
	std::cout << "\n=== Testing Edge Cases ===" << std::endl;
	
	Chronometer chrono;
	
	std::cout << "Testing operations on stopped chronometer:" << std::endl;
	chrono.lap();    // Should do nothing
	chrono.split();  // Should do nothing
	chrono.pause();  // Should do nothing
	chrono.resume(); // Should do nothing
	
	printStatus(chrono);
	
	std::cout << "Testing multiple starts (should have no effect):" << std::endl;
	chrono.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	chrono.start(); // Should have no effect
	chrono.start(); // Should have no effect
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	
	printStatus(chrono);
	
	std::cout << "Testing lap/split operations when paused:" << std::endl;
	chrono.pause();
	chrono.lap();    // Should do nothing when paused
	chrono.split();  // Should do nothing when paused
	
	std::cout << "Lap count: " << chrono.getLapTimes().size() << std::endl;
	std::cout << "Split count: " << chrono.getSplitTimes().size() << std::endl;
	
	chrono.resume();
	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	chrono.lap();    // Should work now
	chrono.split();  // Should work now
	
	std::cout << "After resume - Lap count: " << chrono.getLapTimes().size() << std::endl;
	std::cout << "After resume - Split count: " << chrono.getSplitTimes().size() << std::endl;
	
	chrono.stop();
}

void test_empty_statistics() {
	std::cout << "\n=== Testing Empty Statistics ===" << std::endl;
	
	Chronometer chrono;
	
	std::cout << "Statistics when no laps recorded:" << std::endl;
	printDuration("Average Lap Time", chrono.getAverageLapTime());
	printDuration("Max Lap Time", chrono.getMaxLapTime());
	printDuration("Min Lap Time", chrono.getMinLapTime());
	printDuration("Current Lap Time", chrono.getLapTime());
	printDuration("Current Split Time", chrono.getSplitTime());
	
	std::cout << "Lap vector size: " << chrono.getLapTimes().size() << std::endl;
	std::cout << "Split vector size: " << chrono.getSplitTimes().size() << std::endl;
}

void test_thread_safety() {
	   std::cout << "\n=== Testing Thread Safety (Improved) ===" << std::endl;
	
	Chronometer chrono;
	chrono.start();
	
	std::atomic<int> lap_attempts{0}, split_attempts{0};
	std::atomic<int> pause_attempts{0}, resume_attempts{0};
	
	std::vector<std::thread> threads;
	
	// ✅ OPTIMIZED: Faster lap recording (more attempts during run windows)
	threads.emplace_back([&]() {
		for (int i = 0; i < 400; ++i) {  // More attempts
			std::this_thread::sleep_for(std::chrono::microseconds(250)); // Faster
			lap_attempts++;
			chrono.lap();
		}
	});
	
	// ✅ OPTIMIZED: Faster split recording
	threads.emplace_back([&]() {
		for (int i = 0; i < 400; ++i) {  // More attempts
			std::this_thread::sleep_for(std::chrono::microseconds(250)); // Faster
			split_attempts++;
			chrono.split();
		}
	});
	
	// ✅ OPTIMIZED: Much longer run periods, shorter pause periods
	threads.emplace_back([&]() {
		for (int i = 0; i < 10; ++i) {  // Fewer pause cycles
			std::this_thread::sleep_for(std::chrono::milliseconds(50)); // MUCH longer run period
			pause_attempts++;
			chrono.pause();
			std::this_thread::sleep_for(std::chrono::microseconds(500)); // Very short pause
			resume_attempts++;
			chrono.resume();
		}
	});
	
	for (auto& t : threads) {
		t.join();
	}
	
	chrono.stop();
	
	std::cout << "Lap attempts: " << lap_attempts.load() << ", Recorded: " << chrono.getLapTimes().size() << std::endl;
	std::cout << "Split attempts: " << split_attempts.load() << ", Recorded: " << chrono.getSplitTimes().size() << std::endl;
	std::cout << "Pause attempts: " << pause_attempts.load() << std::endl;
	std::cout << "Resume attempts: " << resume_attempts.load() << std::endl;
	
	double lap_success = (double)chrono.getLapTimes().size() / lap_attempts.load() * 100;
	double split_success = (double)chrono.getSplitTimes().size() / split_attempts.load() * 100;
	
	std::cout << "Lap success rate: " << std::fixed << std::setprecision(1) << lap_success << "%" << std::endl;
	std::cout << "Split success rate: " << std::fixed << std::setprecision(1) << split_success << "%" << std::endl;
}

void test_thread_safety_isolated() {
	std::cout << "\n=== Testing Thread Safety (Isolated) ===" << std::endl;
	
	// Test 1: No pause/resume interference
	{
		Chronometer chrono;
		chrono.start();
		
		std::atomic<int> lap_attempts{0};
		std::vector<std::thread> threads;
		
		for (int t = 0; t < 4; ++t) {
			threads.emplace_back([&]() {
				for (int i = 0; i < 25; ++i) {
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					lap_attempts++;
					chrono.lap();
				}
			});
		}
		
		for (auto& t : threads) {
			t.join();
		}
		
		chrono.stop();
		
		std::cout << "Test 1 (No pause/resume):" << std::endl;
		std::cout << "Attempts: " << lap_attempts.load() << ", Recorded: " << chrono.getLapTimes().size() << std::endl;
		double success = (double)chrono.getLapTimes().size() / lap_attempts.load() * 100;
		std::cout << "Success rate: " << std::fixed << std::setprecision(1) << success << "%" << std::endl;
	}
}

void test_thread_safety_no_pause() {
	std::cout << "\n=== Testing Thread Safety (No Pause/Resume) ===" << std::endl;
	
	Chronometer chrono;
	chrono.start();
	
	std::atomic<int> lap_attempts{0}, split_attempts{0};
	std::vector<std::thread> threads;
	
	// Multiple threads recording laps
	for (int t = 0; t < 3; ++t) {
		threads.emplace_back([&]() {
			for (int i = 0; i < 100; ++i) {
				std::this_thread::sleep_for(std::chrono::microseconds(100));
				lap_attempts++;
				chrono.lap();
			}
		});
	}
	
	// Multiple threads recording splits
	for (int t = 0; t < 3; ++t) {
		threads.emplace_back([&]() {
			for (int i = 0; i < 100; ++i) {
				std::this_thread::sleep_for(std::chrono::microseconds(150));
				split_attempts++;
				chrono.split();
			}
		});
	}
	
	for (auto& t : threads) {
		t.join();
	}
	
	chrono.stop();
	
	std::cout << "Lap attempts: " << lap_attempts.load() << ", Recorded: " << chrono.getLapTimes().size() << std::endl;
	std::cout << "Split attempts: " << split_attempts.load() << ", Recorded: " << chrono.getSplitTimes().size() << std::endl;
	
	double lap_success = (double)chrono.getLapTimes().size() / lap_attempts.load() * 100;
	double split_success = (double)chrono.getSplitTimes().size() / split_attempts.load() * 100;
	
	std::cout << "Lap success rate: " << std::fixed << std::setprecision(1) << lap_success << "%" << std::endl;
	std::cout << "Split success rate: " << std::fixed << std::setprecision(1) << split_success << "%" << std::endl;
}

void test_pause_resume_stress() {
	std::cout << "\n=== Testing Pause/Resume Stress ===" << std::endl;
	
	Chronometer chrono;
	chrono.start();
	
	std::atomic<int> state_changes{0};
	std::vector<std::thread> threads;
	
	// Multiple threads changing state
	for (int t = 0; t < 4; ++t) {
		threads.emplace_back([&]() {
			for (int i = 0; i < 50; ++i) {
				chrono.pause();
				state_changes++;
				std::this_thread::sleep_for(std::chrono::microseconds(100));
				chrono.resume();
				state_changes++;
				std::this_thread::sleep_for(std::chrono::microseconds(100));
			}
		});
	}
	
	for (auto& t : threads) {
		t.join();
	}
	
	chrono.stop();
	
	std::cout << "State changes: " << state_changes.load() << std::endl;
	std::cout << "Final state - Running: " << chrono.isRunning() << ", Paused: " << chrono.isPaused() << std::endl;
}

void interactive_test() {
	std::cout << "\n=== Interactive Test ===" << std::endl;
	std::cout << "Commands: start, pause, resume, lap, split, stop, status, quit" << std::endl;
	
	Chronometer chrono;
	std::string command;
	
	while (true) {
		std::cout << "\nEnter command: ";
		std::cin >> command;
		
		if (command == "start") {
			chrono.start();
			std::cout << "Chronometer started" << std::endl;
		} else if (command == "pause") {
			chrono.pause();
			std::cout << "Chronometer paused" << std::endl;
		} else if (command == "resume") {
			chrono.resume();
			std::cout << "Chronometer resumed" << std::endl;
		} else if (command == "lap") {
			chrono.lap();
			std::cout << "Lap recorded" << std::endl;
		} else if (command == "split") {
			chrono.split();
			std::cout << "Split recorded" << std::endl;
		} else if (command == "stop") {
			chrono.stop();
			std::cout << "Chronometer stopped" << std::endl;
		} else if (command == "status") {
			printStatus(chrono);
		} else if (command == "quit") {
			break;
		} else {
			std::cout << "Unknown command" << std::endl;
		}
	}
}

void test_operator_overload() {
	std::cout << "\n=== Testing Operator<< Overload ===" << std::endl;
	
	Chronometer chrono;
	
	// Test 1: Initial state (should be zero durations)
	std::cout << "\n--- Test 1: Initial State ---" << std::endl;
	std::cout << "Elapsed Time: " << chrono.getElapsedTime() << std::endl;
	std::cout << "Paused Time: " << chrono.getPausedTime() << std::endl;
	std::cout << "Total Time: " << chrono.getTotalTime() << std::endl;
	std::cout << "Lap Time: " << chrono.getLapTime() << std::endl;
	std::cout << "Split Time: " << chrono.getSplitTime() << std::endl;
	
	// Test 2: Running state with some elapsed time
	std::cout << "\n--- Test 2: Running State ---" << std::endl;
	chrono.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(150));
	
	std::cout << "After 150ms run:" << std::endl;
	std::cout << "  Elapsed Time: " << chrono.getElapsedTime() << std::endl;
	std::cout << "  Total Time: " << chrono.getTotalTime() << std::endl;
	
	// Test 3: With pause and resume
	std::cout << "\n--- Test 3: Pause/Resume Cycle ---" << std::endl;
	chrono.pause();
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Paused time
	
	std::cout << "After pause (100ms paused):" << std::endl;
	std::cout << "  Elapsed Time: " << chrono.getElapsedTime() << std::endl;
	std::cout << "  Paused Time: " << chrono.getPausedTime() << std::endl;
	std::cout << "  Total Time: " << chrono.getTotalTime() << std::endl;
	
	chrono.resume();
	std::this_thread::sleep_for(std::chrono::milliseconds(75));
	
	std::cout << "After resume (75ms more):" << std::endl;
	std::cout << "  Elapsed Time: " << chrono.getElapsedTime() << std::endl;
	std::cout << "  Paused Time: " << chrono.getPausedTime() << std::endl;
	std::cout << "  Total Time: " << chrono.getTotalTime() << std::endl;
	
	// Test 4: Lap functionality
	std::cout << "\n--- Test 4: Lap Times ---" << std::endl;
	chrono.lap(); // First lap
	std::this_thread::sleep_for(std::chrono::milliseconds(80));
	chrono.lap(); // Second lap
	std::this_thread::sleep_for(std::chrono::milliseconds(120));
	chrono.lap(); // Third lap
	
	const auto& lapTimes = chrono.getLapTimes();
	for (size_t i = 0; i < lapTimes.size(); ++i) {
		std::cout << "  Lap " << (i + 1) << ": " << lapTimes[i] << std::endl;
	}
	
	std::cout << "Last Lap Time: " << chrono.getLapTime() << std::endl;
	std::cout << "Average Lap Time: " << chrono.getAverageLapTime() << std::endl;
	std::cout << "Max Lap Time: " << chrono.getMaxLapTime() << std::endl;
	std::cout << "Min Lap Time: " << chrono.getMinLapTime() << std::endl;
	
	// Test 5: Split functionality
	std::cout << "\n--- Test 5: Split Times ---" << std::endl;
	chrono.split(); // First split
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	chrono.split(); // Second split
	std::this_thread::sleep_for(std::chrono::milliseconds(90));
	chrono.split(); // Third split
	
	const auto& splitTimes = chrono.getSplitTimes();
	for (size_t i = 0; i < splitTimes.size(); ++i) {
		std::cout << "  Split " << (i + 1) << ": " << splitTimes[i] << std::endl;
	}
	
	std::cout << "Last Split Time: " << chrono.getSplitTime() << std::endl;
	
	// Test 6: Final state
	std::cout << "\n--- Test 6: Final State ---" << std::endl;
	chrono.stop();
	
	std::cout << "Final Results:" << std::endl;
	std::cout << "  Total Elapsed: " << chrono.getElapsedTime() << std::endl;
	std::cout << "  Total Paused: " << chrono.getPausedTime() << std::endl;
	std::cout << "  Total Time: " << chrono.getTotalTime() << std::endl;
	std::cout << "  Lap Count: " << lapTimes.size() << std::endl;
	std::cout << "  Split Count: " << splitTimes.size() << std::endl;
	
	// Test 7: Precision formatting test
	std::cout << "\n--- Test 7: Precision Formatting ---" << std::endl;
	Chronometer precisionChrono;
	precisionChrono.start();
	
	// Very short times to test precision
	std::this_thread::sleep_for(std::chrono::microseconds(1500)); // 1.5ms
	precisionChrono.lap();
	
	std::this_thread::sleep_for(std::chrono::microseconds(2750)); // 2.75ms
	precisionChrono.split();
	
	std::this_thread::sleep_for(std::chrono::microseconds(500)); // 0.5ms
	precisionChrono.stop();
	
	std::cout << "Short duration test:" << std::endl;
	std::cout << "  First lap: " << precisionChrono.getLapTimes()[0] << std::endl;
	std::cout << "  First split: " << precisionChrono.getSplitTimes()[0] << std::endl;
	std::cout << "  Total elapsed: " << precisionChrono.getElapsedTime() << std::endl;
	
	// Test 8: Different duration types (if you want to test the template version)
	std::cout << "\n--- Test 8: Different Duration Types ---" << std::endl;
	auto microseconds_dur = std::chrono::microseconds(1500);
	auto seconds_dur = std::chrono::seconds(2);
	auto nanoseconds_dur = std::chrono::nanoseconds(500000);
	
	// Convert to your duration_ms type and print
	Chronometer::duration_ms micro_as_ms = std::chrono::duration_cast<Chronometer::duration_ms>(microseconds_dur);
	Chronometer::duration_ms sec_as_ms = std::chrono::duration_cast<Chronometer::duration_ms>(seconds_dur);
	Chronometer::duration_ms nano_as_ms = std::chrono::duration_cast<Chronometer::duration_ms>(nanoseconds_dur);
	
	std::cout << "1500 microseconds as ms: " << micro_as_ms << std::endl;
	std::cout << "2 seconds as ms: " << sec_as_ms << std::endl;
	std::cout << "500000 nanoseconds as ms: " << nano_as_ms << std::endl;
	
	std::cout << "\nOperator<< overload test completed!" << std::endl;
}

int main() {
	std::cout << "Chronometer Test Suite" << std::endl;
	std::cout << "======================" << std::endl;
	
	try {
		test_basic_functionality();
		test_lap_functionality();
		test_split_functionality();
		test_complex_scenario();
		test_edge_cases();
		test_empty_statistics();
		test_thread_safety();
		test_thread_safety_no_pause();
		test_pause_resume_stress();
		test_thread_safety_isolated();
		test_operator_overload();
		
		std::cout << "\n=== All Automated Tests Completed ===" << std::endl;
		
		// char choice;
		// std::cout << "\nWould you like to run interactive test? (y/n): ";
		// std::cin >> choice;
		
		// if (choice == 'y' || choice == 'Y') {
		//     interactive_test();
		// }
		
	} catch (const std::exception& e) {
		std::cerr << "Test failed with exception: " << e.what() << std::endl;
		return 1;
	}
	
	std::cout << "\nAll tests completed successfully!" << std::endl;
	return 0;
}
