#include "utils/Logger.h"
#include <iostream>

Logger::Logger(const std::string& name, const std::string& log_file) {
    try {
        std::vector<spdlog::sink_ptr> sinks;

        // Console sink with color support
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
        sinks.push_back(console_sink);

        // File sink with rotation (if log file provided)
        if (!log_file.empty()) {
            auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                log_file, 100 * 1024 * 1024, 10
            );
            file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
            sinks.push_back(file_sink);
        }

        // Create multi-sink logger
        logger_ = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
        spdlog::register_logger(logger_);
        spdlog::set_default_logger(logger_);
        spdlog::set_level(spdlog::level::info);

    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Logger initialization failed: " << ex.what() << std::endl;
    }
}
