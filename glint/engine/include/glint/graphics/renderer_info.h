#pragma once

#include <vector>

namespace glint::engine::graphics {
    struct renderer_info {
        std::vector<const char*> extensions;

      public:
        renderer_info() = delete;
        renderer_info(const std::vector<const char*>& exts) : extensions(exts) {}
    };
}