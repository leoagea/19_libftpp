#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <vector>
#include "../inc/libftpp.hpp"

// ✅ Console Sink Implementation
class ConsoleSink : public Log::Sink {
private:
    Log::LogLevel _level = Log::LogLevel::DEBUG;

public:
    ConsoleSink() = default;

    void write(const Log::Record& record) override {
        std::cout << formatRecord(record) << std::endl;
    }
    
    void flush() override {
        std::cout.flush();
    }

    void set_level(Log::LogLevel level) override {
        _level = level;
    }

private:
    std::string formatRecord(const Log::Record& record) {
        std::stringstream ss;
        
        // Timestamp
        auto time_t = std::chrono::system_clock::to_time_t(record.timestamp);
        auto tm = *std::localtime(&time_t);
        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        
        // Level
        ss << " [" << levelToString(record.level) << "]";
        
        // Thread ID
        ss << " [Thread:" << record.threadId << "]";
        
        // Logger name
        ss << " [" << record.loggerName << "]";
        
        // Source location
        ss << " (" << record.source.file << ":" << record.source.line << ")";
        
        // Message
        ss << " - " << record.message;
        
        return ss.str();
    }
    
    std::string levelToString(Log::LogLevel level) {
        switch (level) {
            case Log::LogLevel::DEBUG: return "DEBUG";
            case Log::LogLevel::TRACE: return "TRACE";
            case Log::LogLevel::INFO: return "INFO";
            case Log::LogLevel::WARNING: return "WARNING";
            case Log::LogLevel::ERROR: return "ERROR";
            case Log::LogLevel::CRITICAL: return "CRITICAL";
            default: return "UNKNOWN";
        }
    }
};

// ✅ File Sink Implementation
class FileSink : public Log::Sink {
private:
    std::ofstream file;
    Log::LogLevel _level = Log::LogLevel::DEBUG;
    
public:
    FileSink() = default;
    FileSink(const std::string& filename) : file(filename, std::ios::app) {
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open log file: " + filename);
        }
    }
    
    ~FileSink() {
        if (file.is_open()) {
            file.close();
        }
    }
    
    void write(const Log::Record& record) override {
        file << formatRecord(record) << std::endl;
    }
    
    void flush() override {
        file.flush();
    }

    void set_level(Log::LogLevel level) override {
        _level = level;
    }

private:
    std::string formatRecord(const Log::Record& record) {
        std::stringstream ss;
        
        auto time_t = std::chrono::system_clock::to_time_t(record.timestamp);
        auto tm = *std::localtime(&time_t);
        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        ss << " [" << levelToString(record.level) << "]";
        ss << " [" << record.loggerName << "]";
        ss << " - " << record.message;
        
        return ss.str();
    }
    
    std::string levelToString(Log::LogLevel level) {
        switch (level) {
            case Log::LogLevel::DEBUG: return "DEBUG";
            case Log::LogLevel::TRACE: return "TRACE";
            case Log::LogLevel::INFO: return "INFO";
            case Log::LogLevel::WARNING: return "WARNING";
            case Log::LogLevel::ERROR: return "ERROR";
            case Log::LogLevel::CRITICAL: return "CRITICAL";
            default: return "UNKNOWN";
        }
    }
};

// ✅ Test Functions
void test_basic_logging() {
    std::cout << "=== Testing Basic Logging ===" << std::endl;
    
    Log::Logger logger("BasicTest");
    auto consoleSink = std::make_shared<ConsoleSink>();
    logger.addSink(consoleSink.get());
    
    // Test all log levels
    logger.setLogLevel(Log::LogLevel::DEBUG);
    
    logger.log(Log::LogLevel::DEBUG, "This is a debug message", Log::Source{__FILE__, __LINE__, __func__});
    logger.log(Log::LogLevel::TRACE, "This is a trace message", Log::Source{__FILE__, __LINE__, __func__});
    logger.log(Log::LogLevel::INFO, "This is an info message", Log::Source{__FILE__, __LINE__, __func__});
    logger.log(Log::LogLevel::WARNING, "This is a warning message", Log::Source{__FILE__, __LINE__, __func__});
    logger.log(Log::LogLevel::ERROR, "This is an error message", Log::Source{__FILE__, __LINE__, __func__});
    logger.log(Log::LogLevel::CRITICAL, "This is a critical message", Log::Source{__FILE__, __LINE__, __func__});
    
    std::cout << std::endl;
}

void test_log_levels() {
    std::cout << "=== Testing Log Levels ===" << std::endl;
    
    Log::Logger logger("LevelTest");
    auto consoleSink = std::make_shared<ConsoleSink>();
    logger.addSink(consoleSink.get());
    
    // Test different log levels
    std::cout << "Setting log level to WARNING..." << std::endl;
    logger.setLogLevel(Log::LogLevel::WARNING);
    
    std::cout << "Should see WARNING and CRITICAL:" << std::endl;
    logger.log(Log::LogLevel::DEBUG, "Debug (should not appear)", Log::Source{__FILE__, __LINE__, __func__});
    logger.log(Log::LogLevel::INFO, "Info (should not appear)", Log::Source{__FILE__, __LINE__, __func__});
    logger.log(Log::LogLevel::WARNING, "Warning (should appear)", Log::Source{__FILE__, __LINE__, __func__});
    logger.log(Log::LogLevel::ERROR, "Error (should appear)", Log::Source{__FILE__, __LINE__, __func__});
    logger.log(Log::LogLevel::CRITICAL, "Critical (should appear)", Log::Source{__FILE__, __LINE__, __func__});
    
    std::cout << std::endl;
}

void test_macros() {
    std::cout << "=== Testing Logging Macros ===" << std::endl;
    
    Log::Logger logger("MacroTest");
    auto consoleSink = std::make_shared<ConsoleSink>();
    logger.addSink(consoleSink.get());
    logger.setLogLevel(Log::LogLevel::DEBUG);
    
    LOG_DEBUG(logger, "Debug message using macro");
    LOG_TRACE(logger, "Trace message using macro");
    LOG_INFO(logger, "Info message using macro");
    LOG_WARNING(logger, "Warning message using macro");
    LOG_ERROR(logger, "Error message using macro");
    LOG_CRITICAL(logger, "Critical message using macro");
    
    std::cout << std::endl;
}

void test_multiple_sinks() {
    std::cout << "=== Testing Multiple Sinks ===" << std::endl;
    
    Log::Logger logger("MultiSinkTest");
    auto consoleSink = std::make_shared<ConsoleSink>();
    
    try {
        auto fileSink = std::make_shared<FileSink>("file.log");
        
        logger.addSink(consoleSink.get());
        logger.addSink(fileSink.get());
        logger.setLogLevel(Log::LogLevel::INFO);
        
        LOG_INFO(logger, "This message should go to both console and file");
        LOG_WARNING(logger, "Warning to both sinks");
        LOG_ERROR(logger, "Error to both sinks");
        
        std::cout << "Check 'test.log' file for file output" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "File sink error: " << e.what() << std::endl;
    }
    
    std::cout << std::endl;
}

void test_sink_management() {
    std::cout << "=== Testing Sink Management ===" << std::endl;
    
    Log::Logger logger("SinkManagementTest");
    auto consoleSink1 = std::make_shared<ConsoleSink>();
    auto consoleSink2 = std::make_shared<ConsoleSink>();

    
    logger.setLogLevel(Log::LogLevel::INFO);
    
    std::cout << "Adding first sink..." << std::endl;
    logger.addSink(consoleSink1.get());
    LOG_INFO(logger, "Message with one sink");
    
    std::cout << "Adding second sink..." << std::endl;
    logger.addSink(consoleSink2.get());
    LOG_INFO(logger, "Message with two sinks (should appear twice)");
    
    std::cout << "Removing first sink..." << std::endl;
    logger.removeSink(consoleSink1.get());
    LOG_INFO(logger, "Message with one sink again");
    
    std::cout << "Clearing all sinks..." << std::endl;
    logger.clearSinks();
    LOG_INFO(logger, "Message with no sinks (should not appear)");
    
    std::cout << std::endl;
}

void test_thread_safety() {
    std::cout << "=== Testing Thread Safety ===" << std::endl;
    
    Log::Logger logger("ThreadTest");
    auto consoleSink = std::make_shared<ConsoleSink>();
    logger.addSink(consoleSink.get());
    logger.setLogLevel(Log::LogLevel::INFO);
    
    const int numThreads = 4;
    const int messagesPerThread = 5;
    std::vector<std::thread> threads;
    
    // Launch multiple threads
    for (int t = 0; t < numThreads; ++t) {
        threads.emplace_back([&logger, t, messagesPerThread]() {
            for (int i = 0; i < messagesPerThread; ++i) {
                std::string message = "Thread " + std::to_string(t) + " Message " + std::to_string(i);
                LOG_INFO(logger, message);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
    }
    
    // Wait for all threads
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "Thread safety test completed" << std::endl;
    std::cout << std::endl;
}

void test_performance() {
    std::cout << "=== Testing Performance ===" << std::endl;
    
    Log::Logger logger("PerformanceTest");
    auto consoleSink = std::make_shared<ConsoleSink>();
    logger.addSink(consoleSink.get());
    logger.setLogLevel(Log::LogLevel::INFO);
    
    const int numMessages = 1000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < numMessages; ++i) {
        logger.log(Log::LogLevel::INFO, "Performance test message " + std::to_string(i), 
                  Log::Source{__FILE__, __LINE__, __func__});
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Logged " << numMessages << " messages in " 
              << duration.count() << " microseconds" << std::endl;
    std::cout << "Average: " << std::fixed << std::setprecision(3)
              << (double)duration.count() / numMessages << " microseconds per message" << std::endl;
    
    std::cout << std::endl;
}

void test_edge_cases() {
    std::cout << "=== Testing Edge Cases ===" << std::endl;
    
    Log::Logger logger("EdgeCaseTest");
    auto consoleSink = std::make_shared<ConsoleSink>();
    logger.addSink(consoleSink.get());
    logger.setLogLevel(Log::LogLevel::DEBUG);
    
    // Test empty message
    LOG_INFO(logger, "");
    
    // Test very long message
    std::string longMessage(1000, 'A');
    LOG_INFO(logger, longMessage);
    
    // Test special characters
    LOG_INFO(logger, "Special chars: !@#$%^&*()_+{}|:<>?[]\\;',./");
    
    // Test unicode (if supported)
    LOG_INFO(logger, "Unicode: 你好世界 🌍");
    
    // Test null sink handling
    logger.addSink(nullptr);
    LOG_INFO(logger, "Message with null sink added (should still work)");
    
    std::cout << std::endl;
}

int main() {
    std::cout << "Logger Class Comprehensive Test Suite" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    try {
        test_basic_logging();
        test_log_levels();
        test_macros();
        test_multiple_sinks();
        test_sink_management();
        test_thread_safety();
        test_performance();
        test_edge_cases();
        
        std::cout << "=== All Tests Completed Successfully ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}