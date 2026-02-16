#pragma once

#include <string>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Config {
public:
    // Load configuration from JSON file
    static std::shared_ptr<Config> loadFromFile(const std::string& config_file);

    // Server configuration getters
    std::string getServerHost() const;
    unsigned short getServerPort() const;
    size_t getThreadPoolSize() const;
    long getRequestTimeoutMs() const;

    // Logging configuration
    std::string getLogLevel() const;
    std::string getLogFile() const;

    // Database configuration (Week 4)
    std::string getDatabaseConnectionString() const;
    size_t getDatabasePoolSize() const;

    // Redis configuration (Week 4)
    std::string getRedisConnectionString() const;
    size_t getRedisPoolSize() const;

    // ML Models configuration (Week 2)
    std::string getMLModelsPath() const;

    // Get raw JSON for custom access
    const json& getRawConfig() const { return config_; }

private:
    explicit Config(const json& config);
    json config_;
};
