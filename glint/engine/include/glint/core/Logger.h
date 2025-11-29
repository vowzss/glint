#pragma once
#include <memory>
#include <string>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace glint::engine {

    namespace core {
        class Logger {
          private:
            static std::shared_ptr<spdlog::logger> logger;

          public:
            static void init();

            template <typename... Args>
            static void trace(const std::string& tag, const std::string& fmt, Args&&... args) {
                logger->trace("[{}] " + fmt, tag, std::forward<Args>(args)...);
            }

            template <typename... Args>
            static void debug(const std::string& tag, const std::string& fmt, Args&&... args) {
                logger->debug("[{}] " + fmt, tag, std::forward<Args>(args)...);
            }

            template <typename... Args>
            static void info(const std::string& tag, const std::string& fmt, Args&&... args) {
                logger->info("[{}] " + fmt, tag, std::forward<Args>(args)...);
            }

            template <typename... Args>
            static void warn(const std::string& tag, const std::string& fmt, Args&&... args) {
                logger->warn("[{}] " + fmt, tag, std::forward<Args>(args)...);
            }

            template <typename... Args>
            static void error(const std::string& tag, const std::string& fmt, Args&&... args) {
                logger->error("[{}] " + fmt, tag, std::forward<Args>(args)...);
            }

            template <typename... Args>
            static void critical(const std::string& tag, const std::string& fmt, Args&&... args) {
                logger->critical("[{}] " + fmt, tag, std::forward<Args>(args)...);
            }
        };

        // clang-format off
        #define LOG_TRACE(fmt, ...) Logger::trace(__func__, fmt, ##__VA_ARGS__)
        #define LOG_DEBUG(fmt, ...) Logger::debug(__func__, fmt, ##__VA_ARGS__)
        #define LOG_INFO(fmt, ...) Logger::info(__func__, fmt, ##__VA_ARGS__)
        #define LOG_WARN(fmt, ...) Logger::warn(__func__, fmt, ##__VA_ARGS__)
        #define LOG_ERROR(fmt, ...) Logger::error(__func__, fmt, ##__VA_ARGS__)
        #define LOG_CRITICAL(fmt, ...) Logger::critical(__func__, fmt, ##__VA_ARGS__)
        // clang-format on
    }
}
