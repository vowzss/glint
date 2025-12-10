#pragma once

#include <array>
#include <functional>

#include "glint/utils/EnumUtils.h"

namespace glint::engine::core {

    enum class InputType : uint8_t {
        Undefined = 0,
        Cursor,
        Button,
        Key,
    };
    ENUM_TO_STRING(InputType, "Undefined", "Cursor", "Button", "Key")

    enum class InputAction : uint8_t {
        Undefined = 0,
        Pressed = 1 << 0,
        Held = 1 << 1,
        Released = 1 << 2,
    };
    ENUM_TO_STRING(InputAction, "Undefined", "Pressed", "Held", "Released")

    using InputCallback = std::function<void(int code, InputAction action)>;
    using CursorCallback = std::function<void(double x, double y, double deltaX, double deltaY)>;
    using CallbackHandle = uint32_t;

    struct InputEvent {
        InputType type;
        InputAction action;

        int code = -1;
        double x = 0.0, y = 0.0;

      public:
        // --- factories ---
        inline static InputEvent cursor(double x, double y) noexcept {
            return {InputType::Cursor, InputAction::Undefined, -1, x, y};
        }
        inline static InputEvent key(int key, InputAction action) noexcept {
            return {InputType::Key, action, key};
        }
        inline static InputEvent button(int button, InputAction action) noexcept {
            return {InputType::Button, action, button};
        }
    };

    struct InputListener {
        CallbackHandle handle;

        InputType type;
        InputAction mask;

        int code = -1;

        InputCallback keyCallback;
        CursorCallback cursorCallback;

      public:
        InputListener(CallbackHandle handle_, InputType type_, InputAction mask_, int code_, const InputCallback& cb)
            : handle(handle_), type(type_), mask(mask_), code(code_), keyCallback(cb) {
        }

        InputListener(CallbackHandle handle_, const CursorCallback& cb)
            : handle(handle_), type(InputType::Cursor), mask(InputAction::Undefined), code(-1), cursorCallback(cb) {
        }
    };

    class InputManager {
        double mouseX = 0.0, mouseY = 0.0;
        double deltaX = 0.0, deltaY = 0.0;

        std::array<uint8_t, 512> keysCurrent{};
        std::array<uint8_t, 512> keysPrevious{};

        std::array<uint8_t, 8> buttonsCurrent = {};
        std::array<uint8_t, 8> buttonsPrevious = {};

        std::unordered_map<CallbackHandle, InputListener> registry;
        std::vector<CallbackHandle> cursorListeners;
        std::unordered_map<int, std::array<std::vector<CallbackHandle>, 4>> codeListeners;

        CallbackHandle nextHandle = 1;

      public:
        InputManager() = default;
        ~InputManager() = default;

        void poll();
        void dispatch(const InputEvent& e);

        CallbackHandle subscribe(const CursorCallback& callback);
        CallbackHandle subscribe(InputType type, int code, InputAction action, const InputCallback& callback);

        void unsubscribe(CallbackHandle handle);

        inline bool isKeyDown(int code) const noexcept {
            if (code < 0 || code >= static_cast<int>(keysCurrent.size())) return false;
            return (keysCurrent[code] & (static_cast<uint8_t>(InputAction::Held) | static_cast<uint8_t>(InputAction::Pressed))) != 0;
        }

        inline bool isButtonDown(int code) const noexcept {
            if (code < 0 || code >= static_cast<int>(buttonsCurrent.size())) return false;
            return (buttonsCurrent[code] & (static_cast<uint8_t>(InputAction::Held) | static_cast<uint8_t>(InputAction::Pressed))) != 0;
        }
    };

}