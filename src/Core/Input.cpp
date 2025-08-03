//
// Created by Ben Gotts on 03/08/2025.
//

#include "Input.h"

Window* Input::_window;

std::unordered_map<MouseButton, PressState> Input::_currentMouseStates;
std::unordered_map<MouseButton, PressState> Input::_oldMouseStates;

std::unordered_map<KeyCode, PressState> Input::_currentKeyStates;
std::unordered_map<KeyCode, PressState> Input::_oldKeyStates;

Input::Input(Window* window) {
    _window = window;
}

void Input::UpdateStates() {
    _oldMouseStates = _currentMouseStates;
    _oldKeyStates = _currentKeyStates;
}

bool Input::GetKeyDown(KeyCode key) {
    auto newState = (PressState)glfwGetKey(_window->GetNativeWindow(), key);
    auto oldState = _oldKeyStates[key];

    _currentKeyStates[key] = newState;

    return newState == PRESSED && oldState == RELEASED;
}

bool Input::GetKeyHold(KeyCode key) {
    auto newState = (PressState)glfwGetKey(_window->GetNativeWindow(), key);
    auto oldState = _oldKeyStates[key];

    _currentKeyStates[key] = newState;

    return newState == PRESSED && oldState == PRESSED;
}

bool Input::GetKeyReleased(KeyCode key) {
    auto newState = (PressState)glfwGetKey(_window->GetNativeWindow(), key);
    auto oldState = _oldKeyStates[key];

    _currentKeyStates[key] = newState;

    return newState == RELEASED && oldState == PRESSED;
}

bool Input::GetMouseButtonDown(MouseButton button) {
    auto newState = (PressState)glfwGetMouseButton(_window->GetNativeWindow(), button);
    auto oldState = _oldMouseStates[button];

    _currentMouseStates[button] = newState;

    return newState == PRESSED && oldState == RELEASED;
}

bool Input::GetMouseButtonHold(MouseButton button) {
    auto newState = (PressState)glfwGetMouseButton(_window->GetNativeWindow(), button);
    auto oldState = _oldMouseStates[button];

    _currentMouseStates[button] = newState;

    return newState == PRESSED && oldState == PRESSED;
}

bool Input::GetMouseButtonUp(MouseButton button) {
    auto newState = (PressState)glfwGetMouseButton(_window->GetNativeWindow(), button);
    auto oldState = _oldMouseStates[button];

    _currentMouseStates[button] = newState;

    return newState == RELEASED && oldState == PRESSED;
}
