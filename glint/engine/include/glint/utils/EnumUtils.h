#pragma once
#include <string>

namespace glint::engine::utils {

    namespace enums {
        template <typename Enum>
        inline std::string toString(Enum value, std::initializer_list<const char*> names) {
            size_t index = static_cast<size_t>(value);
            if (index >= names.size()) return "<invalid>";
            return *(names.begin() + index);
        }
    }

    // clang-format off
    #define ENUM_TO_STRING(EnumType, ...) \
    namespace enums { \
        inline std::string toString(EnumType value) {                   \
            return glint::engine::utils::enums::toString(value, { __VA_ARGS__ }); \
        } \
    }
    // clang-format on

}
