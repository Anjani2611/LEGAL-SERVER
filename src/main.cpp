#include <iostream>
#include <memory>
#include <csignal>
#include <ctime>
#include <nlohmann/json.hpp>

#include "server/HttpServer.h"
#include "server/Config.h"
#include "utils/Logger.h"

using json = nlohmann::json;

// Global pointers for graceful shutdown
std::unique_ptr<HttpServer> g_server;
std::shared_ptr<Logger> g_logger;

void signalHandler(int signal) {
    if (signal == SIGTERM || signal == SIGINT) {
        g_logger->warn("Received signal {}. Shutting down gracefully...", signal);
        if (g_server) {
            g_server->stop();
        }
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    try {
        // Load configuration file
        std::string config_file = "config/dev.json";
        if (argc > 1) {
            config_file = argv[1];
        }

        std::cout << "Loading configuration from: " << config_file << std::endl;
        auto config = Config::loadFromFile(config_file);

        // Initialize logger
        g_logger = Logger::create(
            "legal-server",
            config->getLogFile(),
            config->getLogLevel()
        );

        g_logger->info("========================================");
        g_logger->info("Legal Document Processing Server v1.0.0");
        g_logger->info("========================================");
        g_logger->info("Configuration file: {}", config_file);
        g_logger->info("Log level: {}", config->getLogLevel());

        // Setup signal handlers for graceful shutdown
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);

        // Create HTTP server
        g_server = std::make_unique<HttpServer>(
            config->getServerHost(),
            config->getServerPort(),
            config->getThreadPoolSize(),
            g_logger
        );

        // Set request handler (Week 1: Health + Echo endpoints)
        g_server->setRequestHandler(
            [&g_logger](const std::string& method,
                       const std::string& path,
                       const std::string& body,
                       std::string& response) {

                g_logger->info("Request: {} {}", method, path);

                // Health check endpoint
                if (path == "/health" && method == "GET") {
                    json health_response = {
                        {"status", "healthy"},
                        {"service", "legal-server"},
                        {"version", "1.0.0"},
                        {"timestamp", std::time(nullptr)},
                        {"week", 1}
                    };
                    response = health_response.dump(2);  // Pretty print
                    g_logger->info("Health check OK");
                    return;
                }

                // Echo endpoint for testing
                if (path == "/echo" && method == "POST") {
                    try {
                        auto request_json = json::parse(body);
                        json echo_response = {
                            {"received", request_json},
                            {"timestamp", std::time(nullptr)},
                            {"message", "Echo successful"}
                        };
                        response = echo_response.dump(2);
                        g_logger->info("Echo request processed");
                        return;
                    } catch (const json::exception& e) {
                        json error_response = {
                            {"error", "Invalid JSON"},
                            {"message", e.what()}
                        };
                        response = error_response.dump(2);
                        g_logger->error("Echo request failed: {}", e.what());
                        return;
                    }
                }

                // Default 404 response
                json not_found = {
                    {"error", "Not Found"},
                    {"path", path},
                    {"method", method},
                    {"available_endpoints", {
                        {"GET /health", "Health check"},
                        {"POST /echo", "Echo test"}
                    }}
                };
                response = not_found.dump(2);
                g_logger->warn("404 Not Found: {} {}", method, path);
            }
        );

        g_logger->info("Server starting on {}:{}",
                      config->getServerHost(),
                      config->getServerPort());
        g_logger->info("Press Ctrl+C to stop the server");
        g_logger->info("========================================");

        g_server->start();

        // Block main thread forever (signal handler will exit)
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    } catch (const std::exception& e) {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
