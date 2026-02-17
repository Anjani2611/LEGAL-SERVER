#include <iostream>
#include <memory>
#include <csignal>
#include <ctime>
#include <nlohmann/json.hpp>

#include "server/HttpServer.h"
#include "server/Config.h"
#include "utils/Logger.h"
#include "handlers/RequestHandler.h"
#include "services/LegalService.h"
#include "services/MLInferenceService.h"

using json = nlohmann::json;

class DocumentRepository;
class RedisCache;

std::unique_ptr<HttpServer> g_server;
std::shared_ptr<Logger> g_logger;

void signalHandler(int signal) {
    if (signal == SIGTERM || signal == SIGINT) {
        if (g_logger) {
            g_logger->warn("Received signal {}. Shutting down gracefully...", signal);
        }
        if (g_server) {
            g_server->stop();
        }
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    try {
        std::string config_file = "config/dev.json";
        if (argc > 1) {
            config_file = argv[1];
        }

        std::cout << "Loading configuration from: " << config_file << std::endl;
        auto config = Config::loadFromFile(config_file);

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

        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);

        std::shared_ptr<DocumentRepository> db = nullptr;
        std::shared_ptr<RedisCache> cache = nullptr;

        auto ml_service = std::make_shared<MLInferenceService>(
            config->getMLModelsPath(),
            g_logger,
            cache
        );

        auto legal_service = std::make_shared<LegalService>(
            g_logger,
            db,
            cache,
            ml_service
        );

        auto request_handler = std::make_shared<RequestHandler>(
            g_logger,
            legal_service,
            cache
        );

        g_server = std::make_unique<HttpServer>(
            config->getServerHost(),
            config->getServerPort(),
            config->getThreadPoolSize(),
            g_logger
        );

        g_server->setRequestHandler(
            [request_handler](const std::string& method,
                              const std::string& path,
                              const std::string& body,
                              std::string& response) {
                request_handler->handleRequest(method, path, body, response);
            }
        );

        g_logger->info("Server starting on {}:{}",
                       config->getServerHost(),
                       config->getServerPort());
        g_logger->info("Press Ctrl+C to stop the server");
        g_logger->info("========================================");

        g_server->start();

        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    } catch (const std::exception& e) {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
