#include <iostream>
#include <memory>
#include <csignal>
#include <chrono>
#include <thread>

#include "utils/Logger.h"

// Global logger for signal handling
std::shared_ptr<Logger> g_logger;

void signalHandler(int signal) {
    if (signal == SIGTERM || signal == SIGINT) {
        if (g_logger) {
            g_logger->warn("Received signal {}. Shutting down gracefully...", signal);
        }
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    try {
        // Initialize logger
        g_logger = Logger::create("legal-server");

        g_logger->info("================================");
        g_logger->info("Legal Document Processing Server");
        g_logger->info("================================");
        g_logger->info("Version: 1.0.0");
        g_logger->info("Build: Release");
        g_logger->info("C++ Standard: C++20");
        g_logger->info("Compiler: g++ 11.4.0");

        // Setup signal handlers
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);

        g_logger->info("Logger initialized successfully!");
        g_logger->info("Project location: D:\\dev\\legal-server");
        g_logger->info("Signal handlers registered");

        // Demonstrate different log levels
        g_logger->debug("This is a debug message");
        g_logger->info("This is an info message");
        g_logger->warn("This is a warning message");
        g_logger->error("This is an error message");

        g_logger->info("Startup complete. Press Ctrl+C to exit.");

        // Keep running until interrupted
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    } catch (const std::exception& e) {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        if (g_logger) {
            g_logger->error("Startup failed: {}", e.what());
        }
        return 1;
    }

    return 0;
}
