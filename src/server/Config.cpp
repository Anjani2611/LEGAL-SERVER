#include "server/Config.h"
#include <fstream>
#include <stdexcept>

std::shared_ptr<Config> Config::loadFromFile(const std::string& config_file) {
    try {
        std::ifstream file(config_file);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open config file: " + config_file);
        }

        json config;
        file >> config;

        return std::shared_ptr<Config>(new Config(config));

    } catch (const json::exception& e) {
        throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
    }
}

Config::Config(const json& config) : config_(config) {}

std::string Config::getServerHost() const {
    return config_.value("/server/host"_json_pointer, "0.0.0.0");
}

unsigned short Config::getServerPort() const {
    return config_.value("/server/port"_json_pointer, 8080);
}

size_t Config::getThreadPoolSize() const {
    return config_.value("/server/thread_pool_size"_json_pointer, 8);
}

long Config::getRequestTimeoutMs() const {
    return config_.value("/server/request_timeout_ms"_json_pointer, 30000L);
}

std::string Config::getLogLevel() const {
    return config_.value("/logging/level"_json_pointer, "info");
}

std::string Config::getLogFile() const {
    return config_.value("/logging/file"_json_pointer, "");
}

std::string Config::getDatabaseConnectionString() const {
    return config_.value("/database/connection_string"_json_pointer, "");
}

size_t Config::getDatabasePoolSize() const {
    return config_.value("/database/pool_size"_json_pointer, 10);
}

std::string Config::getRedisConnectionString() const {
    return config_.value("/redis/uri"_json_pointer, "");
}

size_t Config::getRedisPoolSize() const {
    return config_.value("/redis/pool_size"_json_pointer, 10);
}

std::string Config::getMLModelsPath() const {
    return config_.value("/ml_models/models_path"_json_pointer, "./models");
}
