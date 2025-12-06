#pragma once

#include <cstdint>

namespace glint::engine::core {

    enum class DeviceType : uint8_t {
        Keyboard,
        Gamepad,
    };

    struct InputDevice {
        DeviceType type;
        int index = -1;
    };

}