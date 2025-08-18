#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "../libftpp.hpp"

class MyClass {
public:
	MyClass(int value) : _value(value)
	{
		std::cout << "MyClass constructor, with value [" << value << "]" << std::endl;
	}
	
	~MyClass() {
		std::cout << "MyClass destructor called" << std::endl;
	}

	void printMessage() {
		std::cout << "Hello from MyClass" << std::endl;
	}
	
	int getValue() const { return _value; }
	void setValue(int value) { _value = value; }

private:
	int _value;
};

class ConfigManager {
public:
	ConfigManager(const std::string& configFile) : _configFile(configFile), _loaded(false) {
		std::cout << "ConfigManager created with config file: " << configFile << std::endl;
		loadConfig();
	}
	
	~ConfigManager() {
		std::cout << "ConfigManager destroyed" << std::endl;
	}
	
	void loadConfig() {
		_loaded = true;
		_settings["app_name"] = "TestApp";
		_settings["version"] = "1.0.0";
		_settings["debug"] = "true";
		std::cout << "Configuration loaded from " << _configFile << std::endl;
	}
	
	std::string getSetting(const std::string& key) const {
		auto it = _settings.find(key);
		return (it != _settings.end()) ? it->second : "NOT_FOUND";
	}
	
	void setSetting(const std::string& key, const std::string& value) {
		_settings[key] = value;
		std::cout << "Setting " << key << " = " << value << std::endl;
	}
	
	void printAllSettings() const {
		std::cout << "Current settings:" << std::endl;
		for (const auto& pair : _settings) {
			std::cout << "  " << pair.first << " = " << pair.second << std::endl;
		}
	}

private:
	std::string _configFile;
	bool _loaded;
	std::map<std::string, std::string> _settings;
};

class Logger {
public:
	Logger(const std::string& logLevel = "INFO") : _logLevel(logLevel), _messageCount(0) {
		std::cout << "Logger initialized with level: " << logLevel << std::endl;
	}
	
	~Logger() {
		std::cout << "Logger destroyed. Total messages logged: " << _messageCount << std::endl;
	}
	
	void log(const std::string& message) {
		_messageCount++;
		std::cout << "[" << _logLevel << "] " << message << " (msg #" << _messageCount << ")" << std::endl;
	}
	
	void setLogLevel(const std::string& level) {
		_logLevel = level;
		std::cout << "Log level changed to: " << level << std::endl;
	}
	
	int getMessageCount() const { return _messageCount; }

private:
	std::string _logLevel;
	int _messageCount;
};

void test_original() {
	std::cout << "=== Original Test ===\n";
	
	try
	{
		// This should throw an exception as instance is not yet created
		Singleton<MyClass>::instance();
	} catch (const std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}

	Singleton<MyClass>::instantiate(42); // Setting up the instance

	Singleton<MyClass>::instance()->printMessage(); // Output: "Hello from MyClass"

	try
	{
		// This should throw an exception as instance is already created
		Singleton<MyClass>::instantiate(100);
	} catch (const std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
	
	std::cout << "Original test completed (cleanup will happen automatically)" << std::endl;
}

void test_state_persistence() {
	std::cout << "\n=== State Persistence Test ===\n";
	
	// Note: Can't test this properly with unique_ptr approach since we can't destroy and recreate
	// We'll test with separate singleton types instead
	
	try {
		// This will fail if MyClass singleton already exists from previous test
		Singleton<MyClass>::instantiate(10);
	} catch (const std::exception& e) {
		std::cout << "Expected: MyClass singleton already exists from previous test" << std::endl;
	}
	
	auto* instance1 = Singleton<MyClass>::instance();
	std::cout << "Current value: " << instance1->getValue() << std::endl;
	
	instance1->setValue(99);
	std::cout << "Modified value: " << instance1->getValue() << std::endl;
	
	// Get another reference - should be the same instance
	auto* instance2 = Singleton<MyClass>::instance();
	std::cout << "Value from second reference: " << instance2->getValue() << std::endl;
	
	// Verify they're the same object
	std::cout << "Are instances the same object? " << (instance1 == instance2 ? "YES" : "NO") << std::endl;
}

void test_config_manager() {
	std::cout << "\n=== Config Manager Test ===\n";
	
	try {
		// Try to access before creation
		Singleton<ConfigManager>::instance();
	} catch (const std::exception& e) {
		std::cout << "Expected exception: " << e.what() << std::endl;
	}
	
	// Create config manager singleton
	Singleton<ConfigManager>::instantiate("config.ini");
	auto* config = Singleton<ConfigManager>::instance();
	
	// Test configuration access
	config->printAllSettings();
	
	std::cout << "App name: " << config->getSetting("app_name") << std::endl;
	std::cout << "Version: " << config->getSetting("version") << std::endl;
	std::cout << "Unknown setting: " << config->getSetting("unknown") << std::endl;
	
	// Modify settings
	config->setSetting("theme", "dark");
	config->setSetting("language", "en");
	
	config->printAllSettings();
}

void test_logger() {
	std::cout << "\n=== Logger Test ===\n";
	
	Singleton<Logger>::instantiate("DEBUG");
	auto* logger = Singleton<Logger>::instance();
	
	logger->log("Application started");
	logger->log("Loading configuration");
	logger->log("Initializing components");
	
	logger->setLogLevel("ERROR");
	logger->log("This is an error message");
	
	std::cout << "Total messages logged: " << logger->getMessageCount() << std::endl;
}

void test_multiple_singletons() {
	std::cout << "\n=== Multiple Singleton Types Test ===\n";
	
	// MyClass and ConfigManager already exist from previous tests
	// Only create Logger if it doesn't exist
	
	// Use existing singletons
	auto* config = Singleton<ConfigManager>::instance();
	auto* logger = Singleton<Logger>::instance();
	auto* myClass = Singleton<MyClass>::instance();
	
	logger->log("Multiple singletons test started");
	config->setSetting("test_mode", "true");
	myClass->printMessage();
	
	std::cout << "MyClass value: " << myClass->getValue() << std::endl;
	std::cout << "Config test_mode: " << config->getSetting("test_mode") << std::endl;
	logger->log("Multiple singletons test completed");
}

void test_exception_safety() {
	std::cout << "\n=== Exception Safety Test ===\n";
	
	// Test accessing existing instance
	try {
		auto* instance = Singleton<MyClass>::instance();
		std::cout << "Existing MyClass instance accessible: " << instance->getValue() << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Unexpected exception: " << e.what() << std::endl;
	}
	
	// Test multiple instantiation attempts on already created singleton
	try {
		Singleton<MyClass>::instantiate(999);
		std::cout << "Second instantiation: SHOULD NOT REACH HERE" << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Expected exception on duplicate instantiation: " << e.what() << std::endl;
	}
	
	// Test creating a new type of singleton
	try {
		// Using a simple int wrapper class
		class IntWrapper {
		public:
			IntWrapper(int val) : value(val) { 
				std::cout << "IntWrapper created with: " << val << std::endl; 
			}
			~IntWrapper() { 
				std::cout << "IntWrapper destroyed" << std::endl; 
			}
			int value;
		};
		
		Singleton<IntWrapper>::instantiate(42);
		std::cout << "New singleton type created successfully" << std::endl;
		std::cout << "IntWrapper value: " << Singleton<IntWrapper>::instance()->value << std::endl;
		
	} catch (const std::exception& e) {
		std::cout << "Unexpected exception: " << e.what() << std::endl;
	}
}

void test_access_patterns() {
	std::cout << "\n=== Access Patterns Test ===\n";
	
	// Test multiple ways of accessing the same singleton
	auto* config1 = Singleton<ConfigManager>::instance();
	auto* config2 = Singleton<ConfigManager>::instance();
	auto* config3 = Singleton<ConfigManager>::instance();
	
	std::cout << "All config pointers are the same: " 
			  << (config1 == config2 && config2 == config3 ? "YES" : "NO") << std::endl;
	
	// Test that modifications persist across different access points
	config1->setSetting("test1", "value1");
	std::cout << "Set via config1, read via config2: " << config2->getSetting("test1") << std::endl;
	
	config3->setSetting("test2", "value2");
	std::cout << "Set via config3, read via config1: " << config1->getSetting("test2") << std::endl;
}

int main() {
	test_original();
	test_state_persistence();
	test_config_manager();
	test_logger();
	test_multiple_singletons();
	test_exception_safety();
	test_access_patterns();
	
	std::cout << "\nAll singleton tests completed!" << std::endl;
	std::cout << "Note: Destructors will be called automatically when program ends" << std::endl;
	std::cout << "      due to std::unique_ptr automatic cleanup" << std::endl;
	return 0;
}