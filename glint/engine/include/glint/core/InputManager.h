#pragma once

#include <array>
#include <functional>

namespace glint::engine {

    namespace core {
        enum class InputType : uint8_t { Unknown, Cursor, Button, Key };
        enum class InputAction : uint8_t { Unknown, Pressed, Released, Held };

        struct InputEvent {
          public:
            InputType type;
            InputAction action;

            int code;
            double x, y;

          public:
            InputEvent() = delete;

            // Constructor for general input events
            InputEvent(InputType type_, InputAction action_, int code_ = -1, double x_ = 0.0, double y_ = 0.0)
                : type(type_), action(action_), code(code_), x(x_), y(y_) {}

            // --- factories ---
            inline static InputEvent cursor(double x, double y) { return InputEvent(InputType::Cursor, InputAction::Unknown, -1, x, y); }
            inline static InputEvent key(int key, InputAction action) { return InputEvent(InputType::Key, action, key); }
            inline static InputEvent button(int button, InputAction action) { return InputEvent(InputType::Button, action, button); }
        };

        struct InputManager {
          public:
            using InputCallback = std::function<void(int key, InputAction action)>;

          private:
            double mouseX = 0.0, mouseY = 0.0;
            double deltaX = 0.0, deltaY = 0.0;

            std::array<bool, 512> keys = {};
            std::array<bool, 8> buttons = {};

            std::unordered_map<int, std::array<std::vector<InputCallback>, 4>> listeners;

          public:
            InputManager() = default;
            ~InputManager() = default;

            void poll();
            void dispatch(const InputEvent& e);

            void subscribe(int code, InputAction action, const InputCallback& callback);
            void subscribe(int code, const InputCallback& callback);

            void unsubscribe(int code, InputAction state = InputAction::Unknown);

            inline bool isKeyDown(int code) const noexcept {
                if (code < 0 || code >= static_cast<int>(keys.size())) return false;
                return keys[code];
            }
            inline bool isButtonDown(int code) const noexcept {
                if (code < 0 || code >= static_cast<int>(buttons.size())) return false;
                return buttons[code];
            }
        };
    }
}