#include <spdlog/logger.h>

#include "glint/core/Logger.h"
#include "glint/core/managers/InputManager.h"

namespace glint::engine::core {

    void InputManager::poll() {
        deltaX = 0.0;
        deltaY = 0.0;

        for (int code = 0; code < static_cast<int>(keysCurrent.size()); ++code) {
            uint8_t curr = keysCurrent[code];
            uint8_t prev = keysPrevious[code];

            InputAction action = InputAction::Undefined;
            if (curr && !prev) {
                action = InputAction::Pressed;
            } else if (curr && prev) {
                action = InputAction::Held;
            } else if (!curr && prev) {
                action = InputAction::Released;
            }

            if (action == InputAction::Undefined) continue;

            dispatch(InputEvent::key(code, action));
            keysPrevious[code] = curr;
        }

        for (int code = 0; code < static_cast<int>(buttonsCurrent.size()); ++code) {
            uint8_t curr = buttonsCurrent[code];
            uint8_t prev = buttonsPrevious[code];

            InputAction action = InputAction::Undefined;
            if (curr && !prev) {
                action = InputAction::Pressed;
            } else if (curr && prev) {
                action = InputAction::Held;
            } else if (!curr && prev) {
                action = InputAction::Released;
            }

            if (action == InputAction::Undefined) continue;

            dispatch(InputEvent::button(code, action));
            buttonsPrevious[code] = curr;
        }
    }

    void InputManager::dispatch(const InputEvent& e) {
        switch (e.type) {
            case InputType::Cursor: {
                deltaX += e.x - mouseX;
                deltaY += e.y - mouseY;
                mouseX = e.x;
                mouseY = e.y;

                for (CallbackHandle handle : cursorListeners) {
                    auto it = registry.find(handle);
                    if (it == registry.end()) continue;
                    if (!it->second.cursorCallback) continue;

                    it->second.cursorCallback(mouseX, mouseY, deltaX, deltaY);
                    LOG_TRACE("Dispatched cursor callback! handle:[{}]", handle);
                }

                return;
            }
            case InputType::Key:
            case InputType::Button: {
                bool isKey = (e.type == InputType::Key);

                auto& state = isKey ? keysCurrent[e.code] : buttonsCurrent[e.code];
                state = (e.action == InputAction::Released) ? 0 : 1;

                const auto& listeners = codeListeners.find(e.code);
                if (listeners == codeListeners.end()) return;

                uint8_t bits = static_cast<uint8_t>(e.action);

                for (uint8_t i = 0; i < 4; ++i) {
                    if ((bits & (1 << i)) == 0) continue;

                    for (CallbackHandle handle : listeners->second[i]) {
                        const auto& it = registry.find(handle);
                        if (it == registry.end()) continue;

                        const InputListener& listener = it->second;

                        using T = std::underlying_type_t<InputAction>;
                        bool call = (static_cast<T>(listener.mask) & static_cast<T>(e.action)) != 0;

                        if (call) {
                            listener.keyCallback(e.code, e.action);
                            LOG_TRACE("Dispatched input callback! handle:[{}] code:[{}] action:[{}]", handle, e.code, enums::toString(e.action));
                        }
                    }
                }

                return;
            }
            default: {
                return;
            }
        }
    }

    CallbackHandle InputManager::subscribe(const CursorCallback& callback) {
        CallbackHandle handle = nextHandle++;

        InputListener listener(handle, callback);
        registry.emplace(handle, listener);

        cursorListeners.push_back(handle);

        LOG_TRACE("Subscribed cursor callback! handle:[{}]", handle);
        return handle;
    }

    CallbackHandle InputManager::subscribe(InputType type, int code, InputAction mask, const InputCallback& callback) {
        CallbackHandle handle = nextHandle++;

        InputListener listener(handle, type, mask, code, callback);
        registry.emplace(handle, listener);

        uint8_t bits = static_cast<uint8_t>(mask);
        auto& vecs = codeListeners[code];

        for (uint8_t i = 0; i < 4; ++i) {
            if ((bits & (1 << i)) == 0) continue;
            vecs[i].push_back(handle);
        }

        LOG_TRACE("Subscribed input callback! handle:[{}] code:[{}] action:[{}]", handle, code, enums::toString(mask));
        return handle;
    }

    void InputManager::unsubscribe(CallbackHandle handle) {
        auto it = registry.find(handle);
        if (it == registry.end()) return;

        const InputListener& listener = it->second;

        if (listener.type == InputType::Cursor) {
            cursorListeners.erase(std::remove(cursorListeners.begin(), cursorListeners.end(), handle), cursorListeners.end());
            registry.erase(it);
            LOG_TRACE("Unsubscribed cursor callback! handle:[{}] code:[{}] action:[{}]", handle, listener.code, enums::toString(listener.mask));
            return;
        }

        uint8_t bits = static_cast<uint8_t>(listener.mask);
        auto& listeners = codeListeners[listener.code];

        for (uint8_t i = 0; i < 4; ++i) {
            if ((bits & (1 << i)) == 0) continue;

            auto& handles = listeners[i];
            handles.erase(std::remove(handles.begin(), handles.end(), handle), handles.end());
        }

        registry.erase(it);
        LOG_TRACE("Unsubscribed input callback! handle:[{}] code:[{}] action:[{}]", handle, listener.code, enums::toString(listener.mask));
    }

}