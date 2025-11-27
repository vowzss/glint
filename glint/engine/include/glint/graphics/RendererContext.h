#pragma once

#include <vector>

#ifdef NDEBUG
    #define BUILD_DEBUG 0
#else
    #define BUILD_DEBUG 1
#endif

namespace glint::engine {
    namespace graphics {
        struct RendererContext {
          private:
            std::vector<const char*> extensions;

          public:
            RendererContext() = delete;
            RendererContext(const std::vector<const char*>& extensions_) : extensions(extensions_) {}

            inline const char* const* getExtensionValues() const { return extensions.data(); }
            inline uint32_t getExtensionCount() const { return static_cast<uint32_t>(extensions.size()); }
        };
    }
}