#pragma once

// --- factories ---
constexpr mat4 mat4::identity() {
    mat4 result{};
    for (size_t i = 0; i < 4; ++i) {
        result[i][i] = 1.0f;
    }
    return result;
}
