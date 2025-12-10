#include <spdlog/sinks/stdout_color_sinks.h>

#include "glint/core/Logger.h"

namespace glint::engine {

    std::shared_ptr<spdlog::logger> Logger::m_logger = nullptr;

    void Logger::init() {
        if (!m_logger) {
            auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

            m_logger = std::make_shared<spdlog::logger>("GLINT", sink);
            m_logger->set_level(spdlog::level::trace);
            m_logger->flush_on(spdlog::level::info);

            // Pattern: timestamp | colored level | message
            m_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
            spdlog::register_logger(m_logger);
        }
    }

    std::string Logger::extractClassName(const char* sig) {
        std::string s(sig);

        // remove everything after '('
        auto paren = s.find('(');
        if (paren != std::string::npos) s = s.substr(0, paren);

        // find last "::" (before function name)
        auto last = s.rfind("::");
        if (last == std::string::npos) return "<global>";

        // scan backward from 'last' to find start of class name
        size_t start = last;
        while (start > 0 && (std::isalnum(s[start - 1]) || s[start - 1] == '_')) {
            --start;
        }

        return s.substr(start, last - start);
    }

}