#pragma once

#include <memory>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/fmt/fmt.h>

class Logger {
public:
    static std::shared_ptr<Logger> create(
        const std::string& name,
        const std::string& log_file = ""
    ) {
        return std::make_shared<Logger>(name, log_file);
    }

    explicit Logger(const std::string& name, const std::string& log_file = "");

    template<typename... Args>
    void info(fmt::format_string<Args...> format, Args&&... args) {
        spdlog::info(format, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warn(fmt::format_string<Args...> format, Args&&... args) {
        spdlog::warn(format, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void error(fmt::format_string<Args...> format, Args&&... args) {
        spdlog::error(format, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void debug(fmt::format_string<Args...> format, Args&&... args) {
        spdlog::debug(format, std::forward<Args>(args)...);
    }

    // Non-template versions for simple messages
    void info(const std::string& msg) { spdlog::info(msg); }
    void warn(const std::string& msg) { spdlog::warn(msg); }
    void error(const std::string& msg) { spdlog::error(msg); }
    void debug(const std::string& msg) { spdlog::debug(msg); }

private:
    std::shared_ptr<spdlog::logger> logger_;
};
