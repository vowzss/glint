#pragma once

#include <memory>
#include <string>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace glint::engine {

    struct Logger {
      private:
        static std::shared_ptr<spdlog::logger> m_logger;

      public:
        static void init();

        template <typename... Args>
        inline static void trace(const char* sig, const std::string& fmt, Args&&... args) {
            m_logger->trace("[{}] > " + fmt, extractClassName(sig), std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline static void debug(const char* sig, const std::string& fmt, Args&&... args) {
            m_logger->debug("[{}] > " + fmt, extractClassName(sig), std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline static void info(const char* sig, const std::string& fmt, Args&&... args) {
            m_logger->info("[{}] > " + fmt, extractClassName(sig), std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline static void warn(const char* sig, const std::string& fmt, Args&&... args) {
            m_logger->warn("[{}] " + fmt, extractClassName(sig), std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline static void error(const char* sig, const std::string& fmt, Args&&... args) {
            m_logger->error("[{}] " + fmt, extractClassName(sig), std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline static void critical(const char* sig, const std::string& fmt, Args&&... args) {
            m_logger->critical("[{}] " + fmt, extractClassName(sig), std::forward<Args>(args)...);
        }

      private:
        static std::string extractClassName(const char* sig);
    };

    // clang-format off
    #if defined(__GNUC__) || defined(__clang__)
        #define FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(_MSC_VER)
        #define FUNC_SIG __FUNCSIG__
    #else
        #define FUNC_SIG __func__
    #endif

    #define LOG_TRACE(fmt, ...) glint::engine::Logger::trace(FUNC_SIG, fmt, ##__VA_ARGS__)
    #define LOG_DEBUG(fmt, ...) glint::engine::Logger::debug(FUNC_SIG, fmt, ##__VA_ARGS__)
    #define LOG_INFO(fmt, ...) glint::engine::Logger::info(FUNC_SIG, fmt, ##__VA_ARGS__)
    #define LOG_WARN(fmt, ...) glint::engine::Logger::warn(FUNC_SIG, fmt, ##__VA_ARGS__)
    #define LOG_ERROR(fmt, ...) glint::engine::Logger::error(FUNC_SIG, fmt, ##__VA_ARGS__)
    #define LOG_CRITICAL(fmt, ...) glint::engine::Logger::critical(FUNC_SIG, fmt, ##__VA_ARGS__)
    // clang-format on

}