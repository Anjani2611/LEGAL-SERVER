#pragma once

#include <memory>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/fmt.h>

class Logger {
public:
    // Factory method - creates logger with console + file output
    static std::shared_ptr<Logger> create(
        const std::string& name,
        const std::string& log_file = "",
        const std::string& level = "info"
    );

    // Logging methods (using fmt::runtime for dynamic format strings)
    template<typename... Args>
    void trace(const std::string& fmt, Args&&... args) {
        logger_->trace(fmt::runtime(fmt), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void debug(const std::string& fmt, Args&&... args) {
        logger_->debug(fmt::runtime(fmt), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info(const std::string& fmt, Args&&... args) {
        logger_->info(fmt::runtime(fmt), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warn(const std::string& fmt, Args&&... args) {
        logger_->warn(fmt::runtime(fmt), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void error(const std::string& fmt, Args&&... args) {
        logger_->error(fmt::runtime(fmt), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void critical(const std::string& fmt, Args&&... args) {
        logger_->critical(fmt::runtime(fmt), std::forward<Args>(args)...);
    }

    void setLevel(const std::string& level);
    void flush();

private:
    explicit Logger(std::shared_ptr<spdlog::logger> logger);
    std::shared_ptr<spdlog::logger> logger_;
};
