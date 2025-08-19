# libftpp - Advanced C++ Library

A comprehensive C++ library implementing advanced concepts including data structures, design patterns, networking, threading, mathematics, time utilities, and more.

## 📚 Library Components

### 🔧 Data Structures

#### DataBuffer
A flexible binary data buffer for serialization and deserialization of various data types.

```cpp
#include "libftpp.hpp"

DataBuffer buffer;

// Serialize data
int value = 42;
std::string text = "Hello";
buffer << value << text;

// Deserialize data
int outValue;
std::string outText;
buffer >> outValue >> outText;
```

#### Pool
Object pool for efficient memory management and object reuse.

```cpp
#include "libftpp.hpp"

class MyObject {
public:
    MyObject(int val = 0) : value(val) {}
    void doSomething() { /* ... */ }
private:
    int value;
};

Pool<MyObject> pool;
pool.resize(10);

// Acquire object from pool
auto obj = pool.acquire(42);
obj->doSomething();
// Object automatically returns to pool when out of scope
```

#### UUID
Generate and manage Universally Unique Identifiers (v1 and v4).

```cpp
#include "libftpp.hpp"

// Generate random UUID (v4)
UUID uuid = UUID::generate();
std::cout << "Generated UUID: " << uuid << std::endl;

// Create from string
UUID fromString("550e8400-e29b-41d4-a716-446655440000");

// Null UUID
UUID nullUuid = UUID::null();
std::cout << "Is null: " << nullUuid.isNull() << std::endl;
```

### 🎨 Design Patterns

#### Singleton
Thread-safe singleton pattern implementation.

```cpp
#include "libftpp.hpp"

class DatabaseManager {
public:
    DatabaseManager(const std::string& connectionString) { /* ... */ }
    void connect() { /* ... */ }
};

// Create singleton instance
Singleton<DatabaseManager>::instantiate("localhost:5432");
auto* db = Singleton<DatabaseManager>::instance();
db->connect();
```

#### Memento
Save and restore object states for undo/redo functionality.

```cpp
#include "libftpp.hpp"

class GameState : public Memento {
    friend class Memento;
public:
    int level = 1;
    int score = 0;
    std::string playerName;

private:
    void _saveToSnapshot(Memento::Snapshot& snapshot) const override {
        snapshot << level << score << playerName;
    }
    
    void _loadFromSnapshot(Memento::Snapshot& snapshot) override {
        snapshot >> level >> score >> playerName;
    }
};

GameState game;
game.level = 5;
game.score = 1000;

// Save state
auto checkpoint = game.save();

// Modify state
game.level = 1;
game.score = 0;

// Restore state
game.load(checkpoint);
```

### 🌐 Network

#### Client/Server
TCP client-server implementation with message-based communication.

**Server:**
```cpp
#include "libftpp.hpp"

Server server;

// Define message handlers
server.defineAction(1, [&server](long long& clientID, const Message& msg) {
    int value;
    msg >> value;
    std::cout << "Received: " << value << " from client " << clientID << std::endl;
    
    // Send response
    Message response(2);
    response << (value * 2);
    server.sendTo(response, clientID);
});

server.start(8080);

while (true) {
    server.update();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}
```

**Client:**
```cpp
#include "libftpp.hpp"

Client client;

// Define response handler
client.defineAction(2, [](const Message& msg) {
    int doubled;
    msg >> doubled;
    std::cout << "Received doubled value: " << doubled << std::endl;
});

client.connect("localhost", 8080);

// Send message
Message msg(1);
msg << 42;
client.send(msg);

while (true) {
    client.update();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}
```

#### Message
Type-safe message system for network communication.

```cpp
#include "libftpp.hpp"

// Create message with type
Message msg(100);

// Serialize data
msg << 42 << std::string("Hello") << 3.14f;

// Access message type and buffer
std::cout << "Message type: " << msg.type() << std::endl;
```

### 🧵 Threading

#### Thread
Enhanced thread wrapper with naming and status tracking.

```cpp
#include "libftpp.hpp"

void workerTask() {
    for (int i = 0; i < 5; ++i) {
        threadSafeCout << "Working... " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

Thread worker("WorkerThread", workerTask);
worker.start();

std::cout << "Thread '" << worker.getName() << "' is running: " 
          << worker.isRunning() << std::endl;

worker.join();
```

#### ThreadSafeIOStream
Thread-safe console output with optional prefixes.

```cpp
#include "libftpp.hpp"

void threadFunction(int id) {
    threadSafeCout.setPrefix("[Thread-" + std::to_string(id) + "] ");
    threadSafeCout << "Hello from thread " << id << std::endl;
    threadSafeCout << "Multiple" << " " << "values" << " " << id << std::endl;
}

std::vector<std::thread> threads;
for (int i = 0; i < 3; ++i) {
    threads.emplace_back(threadFunction, i);
}

for (auto& t : threads) {
    t.join();
}
```

#### ThreadSafeQueue
Thread-safe queue implementation for producer-consumer patterns.

```cpp
#include "libftpp.hpp"

ThreadSafeQueue<int> queue;

// Producer thread
std::thread producer([&queue]() {
    for (int i = 0; i < 10; ++i) {
        queue.push(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
});

// Consumer thread
std::thread consumer([&queue]() {
    int value;
    while (queue.pop(value)) {
        std::cout << "Consumed: " << value << std::endl;
    }
});

producer.join();
queue.close();
consumer.join();
```

### 📊 Mathematics

#### IVector2/IVector3
Generic 2D and 3D vector templates for mathematical operations.

```cpp
#include "libftpp.hpp"

// 2D vectors
IVector2<float> pos(10.0f, 20.0f);
IVector2<float> velocity(1.0f, -2.0f);

IVector2<float> newPos = pos + velocity;
float distance = pos.distance(velocity);
float dot = pos.dot(velocity);

// 3D vectors
IVector3<double> point3d(1.0, 2.0, 3.0);
IVector3<double> direction(0.0, 1.0, 0.0);

auto cross = point3d.cross(direction);
double magnitude = point3d.magnitude();
```

#### Perlin Noise 2D
Generate coherent noise for procedural generation.

```cpp
#include "libftpp.hpp"

PerlinNoise2D noise;

// Generate noise map
for (int y = 0; y < 100; ++y) {
    for (int x = 0; x < 100; ++x) {
        double value = noise.noise(x * 0.1, y * 0.1);
        // Use value for terrain generation, textures, etc.
        std::cout << (value > 0.0 ? '#' : ' ');
    }
    std::cout << std::endl;
}
```

### ⏰ Time

#### Chronometer
High-precision timing with lap and split functionality.

```cpp
#include "libftpp.hpp"

Chronometer chrono;

chrono.start();
std::this_thread::sleep_for(std::chrono::milliseconds(100));
chrono.lap();  // Record lap time

std::this_thread::sleep_for(std::chrono::milliseconds(200));
chrono.pause();

std::this_thread::sleep_for(std::chrono::milliseconds(50));  // Not counted
chrono.resume();

chrono.stop();

// Get timing statistics
auto elapsed = chrono.getElapsedTime();
auto laps = chrono.getLapTimes();
auto stats = chrono.getStatistics();

std::cout << "Total time: " << elapsed.count() << "ms" << std::endl;
std::cout << "Average lap: " << stats.average.count() << "ms" << std::endl;
```

#### Timer
Countdown timer with callback support.

```cpp
#include "libftpp.hpp"

Timer timer;

timer.setCallback([]() {
    std::cout << "Timer expired!" << std::endl;
});

timer.start(std::chrono::seconds(5));

while (timer.isRunning()) {
    auto remaining = timer.getRemainingTime();
    std::cout << "Remaining: " << remaining.count() << "ms\r" << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
```

### 🛠️ Utilities

#### Logger
Flexible logging system with multiple sinks and log levels.

```cpp
#include "libftpp.hpp"

// Custom console sink
class ConsoleSink : public Log::Sink {
public:
    void write(const Log::Record& record) override {
        std::cout << "[" << record.loggerName << "] " << record.message << std::endl;
    }
    void flush() override { std::cout.flush(); }
    void set_level(Log::LogLevel level) override { /* ... */ }
};

Log::Logger logger("MyApp");
auto consoleSink = std::make_shared<ConsoleSink>();
logger.addSink(consoleSink.get());
logger.setLogLevel(Log::LogLevel::DEBUG);

// Use logging macros
LOG_INFO(logger, "Application started");
LOG_WARNING(logger, "This is a warning");
LOG_ERROR(logger, "An error occurred");
```

#### CSV
Read and write CSV files with automatic parsing.

```cpp
#include "libftpp.hpp"

Csv csvFile;

// Set headers and add data
csvFile.setHeader({"Name", "Age", "City"});
csvFile.addRow({"Alice", "25", "New York"});
csvFile.addRow({"Bob", "30", "London"});
csvFile.addRow({"Charlie", "35", "Tokyo"});

// Save to file
csvFile.save("people.csv");

// Load from file
Csv loadedCsv;
loadedCsv.load("people.csv");

// Access data
std::cout << "Rows: " << loadedCsv.rowCount() << std::endl;
std::cout << "Columns: " << loadedCsv.columnCount() << std::endl;

for (size_t i = 0; i < loadedCsv.rowCount(); ++i) {
    auto& row = loadedCsv[i];
    std::cout << row[0] << " is " << row[1] << " years old" << std::endl;
}
```

## 🚀 Getting Started

### Prerequisites
- C++17 compatible compiler
- Make

### Building
```bash
make
```

### Testing
Each component includes comprehensive test files in the `test` directory:

```bash
# Test specific components
c++ -std=c++17 -Iinc -Lbuild/lib test/main_data_buffer.cpp -lftpp
c++ -std=c++17 -Iinc -Lbuild/lib test/main_server.cpp -lftpp
c++ -std=c++17 -Iinc -Lbuild/lib test/main_client.cpp -lftpp
```

### Usage
```cpp
#include "libftpp.hpp"

int main() {
    // Use any component from the library
    return 0;
}
```

## 📁 Project Structure
```
libftpp/
├── inc/                    # Header files
│   ├── datastructures/     # Data structures
│   ├── design_patterns/    # Design patterns
│   ├── network/           # Networking components
│   ├── threading/         # Threading utilities
│   ├── mathematics/       # Math utilities
│   ├── time/             # Time utilities
│   └── utilities/        # General utilities
├── srcs/                  # Implementation files
├── test/                  # Test files
└── build/                # Build output
```

## 🎯 Key Features
- **Thread-Safe**: Most components are designed with thread safety in mind
- **Modern C++**: Uses C++17 features and best practices
- **Header-Only Templates**: Many components are template-based for flexibility
- **Comprehensive Testing**: Each component includes thorough test suites
- **Memory Efficient**: Smart memory management and object pooling
- **Cross-Platform**: Compatible with major operating systems

This library provides a solid foundation for building complex C++ applications with advanced features like networking, concurrent programming, data serialization, and mathematical computations.
