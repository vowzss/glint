#pragma once

namespace glint::engine::core::math::matrix {
    // --- factories ---
    constexpr mat2 mat2::identity() {
        mat2 result{};
        for (size_t i = 0; i < 2; ++i) {
            result[i][i] = 1.0f;
        }
        return result;
    }
}