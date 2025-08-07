#include <iostream>
#include <string>
#include <vector>
#include "../inc/libftpp.hpp"

enum class State {
    Idle,
    Running,
    Paused,
    Stopped,
    Error,
    Maintenance
};

enum class GameState {
    Menu,
    Playing,
    GameOver,
    Loading
};

class TestCounter {
public:
    static int transitionCount;
    static int actionCount;
    
    static void incrementTransition() { transitionCount++; }
    static void incrementAction() { actionCount++; }
    static void reset() { transitionCount = 0; actionCount = 0; }
    static void printCounts() {
        std::cout << "Transitions: " << transitionCount << ", Actions: " << actionCount << std::endl;
    }
};

int TestCounter::transitionCount = 0;
int TestCounter::actionCount = 0;

void test_original() {
    std::cout << "\n=== Original Test ===\n";
    
    StateMachine<State> sm;

    sm.addState(State::Idle);
    sm.addState(State::Running);
    sm.addState(State::Paused);
    sm.addState(State::Stopped);

    sm.addAction(State::Idle, [] { std::cout << "System is idle." << std::endl; });
    sm.addAction(State::Running, [] { std::cout << "System is running." << std::endl; });
    sm.addAction(State::Paused, [] { std::cout << "System is paused." << std::endl; });
    // No addAction for State::Stopped, it will use the default empty lambda

    sm.addTransition(State::Idle, State::Running, [] { std::cout << "Transitioning from Idle to Running." << std::endl; });
    sm.addTransition(State::Running, State::Paused, [] { std::cout << "Transitioning from Running to Paused." << std::endl; });
    sm.addTransition(State::Paused, State::Running, [] { std::cout << "Transitioning from Paused to Running." << std::endl; });
    // No addTransition for State::Stopped

    sm.update();  // Should print: "System is idle."
    sm.transitionTo(State::Running);  // Should print: "Transitioning from Idle to Running."
    sm.update();  // Should print: "System is running."
    sm.transitionTo(State::Paused);  // Should print: "Transitioning from Running to Paused."
    sm.update();  // Should print: "System is paused."

    // Transitioning to and from the new State::Stopped
    try {
        sm.transitionTo(State::Stopped);  // Should not print any transition message, and throw an exception
    } catch (const std::invalid_argument& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;  // Handle state not found
    }
    
    try {
        sm.transitionTo(State::Stopped);  // Should not print anything, default empty lambda is executed
    } catch (const std::invalid_argument& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;  // Handle state not found
    }
    
    try {
        sm.transitionTo(State::Running);  // Should not print any transition message, and throw an exception
    } catch (const std::invalid_argument& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;  // Handle state not found
    }
}

void test_multiple_actions() {
    std::cout << "\n=== Multiple Actions Per State Test ===\n";
    
    StateMachine<State> sm;
    TestCounter::reset();
    
    sm.addState(State::Idle);
    sm.addState(State::Running);
    
    // Add multiple actions to the same state
    sm.addAction(State::Idle, []() {
        std::cout << "Action 1: Checking system status..." << std::endl;
    });
    sm.addAction(State::Idle, []() {
        std::cout << "Action 2: Updating display..." << std::endl;
    });
    sm.addAction(State::Idle, TestCounter::incrementAction);
    
    sm.addAction(State::Running, []() {
        std::cout << "Action 1: Processing data..." << std::endl;
    });
    sm.addAction(State::Running, []() {
        std::cout << "Action 2: Monitoring performance..." << std::endl;
    });
    sm.addAction(State::Running, TestCounter::incrementAction);
    
    std::cout << "Starting in Idle state:" << std::endl;
    sm.update();  // Should execute all Idle actions
    
    sm.addTransition(State::Idle, State::Running, TestCounter::incrementTransition);
    sm.transitionTo(State::Running);
    
    std::cout << "Now in Running state:" << std::endl;
    sm.update();  // Should execute all Running actions
    
    TestCounter::printCounts();
}

void test_multiple_transitions() {
    std::cout << "\n=== Multiple Transitions Test ===\n";
    
    StateMachine<State> sm;
    
    sm.addState(State::Idle);
    sm.addState(State::Running);
    sm.addState(State::Error);
    
    // Add multiple transition actions for the same state pair
    sm.addTransition(State::Idle, State::Running, []() {
        std::cout << "Transition action 1: Starting engines..." << std::endl;
    });
    sm.addTransition(State::Idle, State::Running, []() {
        std::cout << "Transition action 2: Enabling monitoring..." << std::endl;
    });
    sm.addTransition(State::Idle, State::Running, []() {
        std::cout << "Transition action 3: System ready!" << std::endl;
    });
    
    sm.addTransition(State::Running, State::Error, []() {
        std::cout << "Emergency shutdown initiated!" << std::endl;
    });
    
    std::cout << "Transitioning from Idle to Running:" << std::endl;
    sm.transitionTo(State::Running);  // Should execute all three transition actions
    
    std::cout << "\nTransitioning from Running to Error:" << std::endl;
    sm.transitionTo(State::Error);
}

void test_game_state_machine() {
    std::cout << "\n=== Game State Machine Test ===\n";
    
    StateMachine<GameState> game;
    
    // Add game states
    game.addState(GameState::Menu);
    game.addState(GameState::Loading);
    game.addState(GameState::Playing);
    game.addState(GameState::GameOver);
    
    // Add state actions
    game.addAction(GameState::Menu, []() {
        std::cout << "Displaying main menu..." << std::endl;
    });
    game.addAction(GameState::Loading, []() {
        std::cout << "Loading game assets..." << std::endl;
    });
    game.addAction(GameState::Playing, []() {
        std::cout << "Game running - player input active" << std::endl;
    });
    game.addAction(GameState::GameOver, []() {
        std::cout << "Showing final score and restart options" << std::endl;
    });
    
    // Add transitions
    game.addTransition(GameState::Menu, GameState::Loading, []() {
        std::cout << "Player selected 'Start Game'" << std::endl;
    });
    game.addTransition(GameState::Loading, GameState::Playing, []() {
        std::cout << "Assets loaded - starting gameplay" << std::endl;
    });
    game.addTransition(GameState::Playing, GameState::GameOver, []() {
        std::cout << "Player died or completed game" << std::endl;
    });
    game.addTransition(GameState::GameOver, GameState::Menu, []() {
        std::cout << "Returning to main menu" << std::endl;
    });
    game.addTransition(GameState::GameOver, GameState::Loading, []() {
        std::cout << "Player chose to restart" << std::endl;
    });
    
    // Simulate game flow
    std::cout << "Game starts:" << std::endl;
    game.update();  // Menu
    
    std::cout << "\nPlayer starts new game:" << std::endl;
    game.transitionTo(GameState::Loading);
    game.update();  // Loading
    
    std::cout << "\nGame loaded:" << std::endl;
    game.transitionTo(GameState::Playing);
    game.update();  // Playing
    
    std::cout << "\nGame ended:" << std::endl;
    game.transitionTo(GameState::GameOver);
    game.update();  // GameOver
    
    std::cout << "\nPlayer restarts:" << std::endl;
    game.transitionTo(GameState::Loading);
    game.update();  // Loading again
}

void test_complex_workflow() {
    std::cout << "\n=== Complex Workflow Test ===\n";
    
    StateMachine<State> system;
    
    // Add all states
    system.addState(State::Idle);
    system.addState(State::Running);
    system.addState(State::Paused);
    system.addState(State::Maintenance);
    system.addState(State::Error);
    system.addState(State::Stopped);
    
    // Add comprehensive actions
    system.addAction(State::Idle, []() { std::cout << "[IDLE] System waiting for commands" << std::endl; });
    system.addAction(State::Running, []() { std::cout << "[RUNNING] Processing tasks" << std::endl; });
    system.addAction(State::Paused, []() { std::cout << "[PAUSED] System suspended" << std::endl; });
    system.addAction(State::Maintenance, []() { std::cout << "[MAINTENANCE] Running diagnostics" << std::endl; });
    system.addAction(State::Error, []() { std::cout << "[ERROR] System fault detected" << std::endl; });
    system.addAction(State::Stopped, []() { std::cout << "[STOPPED] System shut down" << std::endl; });
    
    // Add complex transition network
    system.addTransition(State::Idle, State::Running, []() { std::cout << "→ Starting system" << std::endl; });
    system.addTransition(State::Running, State::Paused, []() { std::cout << "→ Pausing operations" << std::endl; });
    system.addTransition(State::Paused, State::Running, []() { std::cout << "→ Resuming operations" << std::endl; });
    system.addTransition(State::Running, State::Maintenance, []() { std::cout << "→ Entering maintenance mode" << std::endl; });
    system.addTransition(State::Maintenance, State::Idle, []() { std::cout << "→ Maintenance complete" << std::endl; });
    system.addTransition(State::Running, State::Error, []() { std::cout << "→ FAULT DETECTED!" << std::endl; });
    system.addTransition(State::Error, State::Maintenance, []() { std::cout << "→ Attempting repair" << std::endl; });
    system.addTransition(State::Idle, State::Stopped, []() { std::cout << "→ Shutting down system" << std::endl; });
    
    // Simulate complex workflow
    std::vector<State> workflow = {
        State::Running,      // Start
        State::Paused,       // Pause
        State::Running,      // Resume
        State::Maintenance,  // Maintenance
        State::Idle,         // Back to idle
        State::Running,      // Start again
        State::Error,        // Error occurs
        State::Maintenance,  // Repair
        State::Idle,         // Fixed
        State::Stopped       // Shutdown
    };
    
    system.update();  // Show initial state
    
    for (const auto& nextState : workflow) {
        std::cout << std::endl;
        system.transitionTo(nextState);
        system.update();
    }
}

void test_exception_handling() {
    std::cout << "\n=== Exception Handling Test ===\n";
    
    StateMachine<State> sm;
    
    sm.addState(State::Idle);
    sm.addState(State::Running);
    
    // Test 1: Duplicate state
    try {
        sm.addState(State::Idle);  // Should throw
        std::cout << "ERROR: Should have thrown exception" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "✓ Duplicate state exception: " << e.what() << std::endl;
    }
    
    // Test 2: Transition to non-existent state
    try {
        sm.transitionTo(State::Paused);  // State not added
        std::cout << "ERROR: Should have thrown exception" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "✓ Non-existent state exception: " << e.what() << std::endl;
    }
    
    // Test 3: Transition to same state
    try {
        sm.transitionTo(State::Idle);  // Already in Idle
        std::cout << "ERROR: Should have thrown exception" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "✓ Same state transition exception: " << e.what() << std::endl;
    }
    
    // Test 4: No transition defined
    sm.addState(State::Paused);
    try {
        sm.transitionTo(State::Paused);  // No transition from Idle to Paused
        std::cout << "ERROR: Should have thrown exception" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "✓ No transition defined exception: " << e.what() << std::endl;
    }
}

void test_empty_state_machine() {
    std::cout << "\n=== Empty State Machine Test ===\n";
    
    StateMachine<State> empty;
    
    try {
        empty.update();  // No states added
        std::cout << "Empty state machine update completed (no crash)" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Exception on empty update: " << e.what() << std::endl;
    }
    
    try {
        empty.transitionTo(State::Idle);  // No states exist
    } catch (const std::invalid_argument& e) {
        std::cout << "✓ Expected exception on empty machine: " << e.what() << std::endl;
    }
}

int main() {
    test_original();
    test_multiple_actions();
    test_multiple_transitions();
    test_game_state_machine();
    test_complex_workflow();
    test_exception_handling();
    test_empty_state_machine();
    
    std::cout << "\nAll state machine tests completed!" << std::endl;
    return 0;
}