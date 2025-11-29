#include <GLFW/glfw3.h>
#include <spdlog/logger.h>

#include "glint/core/InputManager.h"
#include "glint/core/Logger.h"

namespace glint::engine::core {
    void InputManager::poll() {
        deltaX = 0.0;
        deltaY = 0.0;

        for (int code = 0, count = static_cast<int>(keys.size()); code < count; ++code) {
            if (!keys[code]) continue;
            dispatch(InputEvent{InputType::Key, InputAction::Held, code});
        }

        for (int code = 0, count = static_cast<int>(buttons.size()); code < count; ++code) {
            if (!buttons[code]) continue;
            dispatch(InputEvent{InputType::Button, InputAction::Held, code});
        }
    }

    void InputManager::dispatch(const InputEvent& e) {
        if (e.type == InputType::Unknown) return;

        auto& callbacks = listeners[e.code];
        size_t index = static_cast<size_t>(e.action);

        if (index < callbacks.size()) {
            for (const InputCallback& callback : callbacks[index]) {
                callback(e.code, e.action);
            }
        }

        switch (e.type) {
            case InputType::Cursor:
                deltaX += e.x - mouseX;
                deltaY += e.y - mouseY;

                mouseX = e.x;
                mouseY = e.y;
                break;
            case InputType::Button:
                buttons[e.code] = (e.action != InputAction::Released);
                break;
            case InputType::Key:
                keys[e.code] = (e.action != InputAction::Released);
                break;
            default:
                break;
        }
    }

    void InputManager::subscribe(int code, InputAction action, const InputCallback& callback) {
        LOG_TRACE("subscribe callback! code:[{}} action:[{}]", code, static_cast<int>(action));

        size_t index = static_cast<size_t>(action);
        listeners[code][index].push_back(callback);
    }

    void InputManager::subscribe(int code, const InputCallback& callback) {
        LOG_TRACE("subscribe callback! code:[{}} action:[All]", code);

        subscribe(code, InputAction::Unknown, callback);
    }

    void InputManager::unsubscribe(int code, InputAction action) {
        auto it = listeners.find(code);
        if (it == listeners.end()) return;

        it->second[static_cast<size_t>(action)].clear();

        if (std::all_of(it->second.begin(), it->second.end(), [](const auto& vec) { return vec.empty(); })) {
            listeners.erase(it);
        }
    }

}