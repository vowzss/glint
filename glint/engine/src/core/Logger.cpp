#include "glint/core/Logger.h"

namespace glint::engine {

    namespace core {
        std::shared_ptr<spdlog::logger> Logger::logger = nullptr;

        void Logger::init() {
            if (!logger) {
                logger = spdlog::stdout_color_mt("GLINT");

                logger->set_level(spdlog::level::trace);
                logger->flush_on(spdlog::level::info);

                // pattern: timestamp | level | tag | message
                logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] | %v - %v");
            }
        }
    }
}