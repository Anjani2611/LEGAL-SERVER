#include "utils/Logger.h"
#include <vector>
#include <stdexcept>

std::shared_ptr<Logger> Logger::create(
    const std::string& name,
    const std::string& log_file,
    const std::string& level
) {
    try {
        std::vector<spdlog::sink_ptr> sinks;

        // Console sink (colored output for terminal)
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");
        sinks.push_back(console_sink);

        // File sink (rotating: 10MB per file, 5 files max)
        if (!log_file.empty()) {
            auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                log_file,
                1024 * 1024 * 10,  // 10MB
                5                   // 5 rotated files
            );
            file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [thread %t] %v");
            sinks.push_back(file_sink);
        }

        // Create synchronous logger (thread-safe, no async thread pool issues)
        auto logger = std::make_shared<spdlog::logger>(
            name,
            sinks.begin(),
            sinks.end()
        );

        // Set log level
        if (level == "trace") {
            logger->set_level(spdlog::level::trace);
        } else if (level == "debug") {
            logger->set_level(spdlog::level::debug);
        } else if (level == "info") {
            logger->set_level(spdlog::level::info);
        } else if (level == "warn") {
            logger->set_level(spdlog::level::warn);
        } else if (level == "error") {
            logger->set_level(spdlog::level::err);
        } else {
            logger->set_level(spdlog::level::info);
        }

        // Auto-flush on warnings and above
        logger->flush_on(spdlog::level::warn);
        spdlog::register_logger(logger);

        return std::shared_ptr<Logger>(new Logger(logger));

    } catch (const spdlog::spdlog_ex& ex) {
        throw std::runtime_error("Logger initialization failed: " + std::string(ex.what()));
    }
}

Logger::Logger(std::shared_ptr<spdlog::logger> logger)
    : logger_(logger) {}

void Logger::setLevel(const std::string& level) {
    if (level == "trace") logger_->set_level(spdlog::level::trace);
    else if (level == "debug") logger_->set_level(spdlog::level::debug);
    else if (level == "info") logger_->set_level(spdlog::level::info);
    else if (level == "warn") logger_->set_level(spdlog::level::warn);
    else if (level == "error") logger_->set_level(spdlog::level::err);
}

void Logger::flush() {
    logger_->flush();
}
