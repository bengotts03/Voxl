//
// Created by Ben Gotts on 03/08/2025.
//

#ifndef INPUT_H
#define INPUT_H
#include <unordered_map>

#include "Window.h"

enum KeyCode {
    ALPHA_0 = 48,
    ALPHA_1 = 49,
    ALPHA_2 = 50,
    ALPHA_3 = 51,
    ALPHA_4 = 52,
    ALPHA_5 = 53,
    ALPHA_6 = 54,
    ALPHA_7 = 55,
    ALPHA_8 = 56,
    ALPHA_9 = 57,
    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,

    SPACE = 32,
    LEFT_SHIFT = 340,
    LEFT_CONTROL = 341,
    LEFT_ALT = 342,
    RIGHT_SHIFT = 344,
    RIGHT_CONTROL = 345,
    RIGHT_ALT = 346,
};

enum MouseButton {
    LEFT = 0,
    RIGHT = 1,
    MIDDLE = 2
};

enum PressState {
    RELEASED = 0,
    PRESSED = 1
};

class Input {
public:
    Input(Window* window);

    static void UpdateStates();

    static bool GetKeyDown(KeyCode key);
    static bool GetKeyHold(KeyCode key);
    static bool GetKeyReleased(KeyCode key);

    static bool GetMouseButtonDown(MouseButton button);
    static bool GetMouseButtonHold(MouseButton button);
    static bool GetMouseButtonUp(MouseButton button);
private:
    static Window* _window;

    static std::unordered_map<MouseButton, PressState> _currentMouseStates;
    static std::unordered_map<MouseButton, PressState> _oldMouseStates;

    static std::unordered_map<KeyCode, PressState> _currentKeyStates;
    static std::unordered_map<KeyCode, PressState> _oldKeyStates;
};

#endif //INPUT_H