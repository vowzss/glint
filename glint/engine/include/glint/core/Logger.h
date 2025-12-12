#pragma once

#include <memory>
#include <string>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace glint::engine {

    class Logger {
        static std::shared_ptr<spdlog::logger> m_logger;

      public:
        static void init();

        template <typename... Args>
        inline static void trace(const char* sig, const char* fmt, Args&&... args) {
            m_logger->trace("[{}] > " + std::string(fmt), extractClassName(sig), std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline static void debug(const char* sig, const char* fmt, Args&&... args) {
            m_logger->debug("[{}] > " + std::string(fmt), extractClassName(sig), std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline static void info(const char* sig, const char* fmt, Args&&... args) {
            m_logger->info("[{}] > " + std::string(fmt), extractClassName(sig), std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline static void warn(const char* sig, const char* fmt, Args&&... args) {
            m_logger->warn("[{}] " + std::string(fmt), extractClassName(sig), std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline static void error(const char* sig, const char* fmt, Args&&... args) {
            m_logger->error("[{}] " + std::string(fmt), extractClassName(sig), std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline static void critical(const char* sig, const char* fmt, Args&&... args) {
            m_logger->critical("[{}] " + std::string(fmt), extractClassName(sig), std::forward<Args>(args)...);
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

    #define LOG_TRACE(...) glint::engine::Logger::trace(FUNC_SIG, __VA_ARGS__)
    #define LOG_DEBUG(...) glint::engine::Logger::debug(FUNC_SIG, __VA_ARGS__)
    #define LOG_INFO(...)  glint::engine::Logger::info(FUNC_SIG, __VA_ARGS__)
    #define LOG_WARN(...)  glint::engine::Logger::warn(FUNC_SIG, __VA_ARGS__)
    #define LOG_ERROR(...) glint::engine::Logger::error(FUNC_SIG, __VA_ARGS__)
    #define LOG_CRITICAL(...) glint::engine::Logger::critical(FUNC_SIG, __VA_ARGS__)
    // clang-format on

}