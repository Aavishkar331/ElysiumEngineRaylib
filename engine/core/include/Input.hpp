#ifndef INPUT_HPP
#define INPUT_HPP

#include "raylib.h"

/**
 * @class Input
 * @brief Thin wrapper around Raylib's input functions.
 *
 * Raylib handles input state tracking internally, so this class
 * simply delegates to Raylib's built-in functions.
 * Key/mouse button parameters use int (Raylib key codes like KEY_A, MOUSE_BUTTON_LEFT).
 */
class Input {
public:
    // Raylib handles input state internally — nothing to do here.
    static void Update() {}

    // --- Keyboard ---

    static bool IsKeyDown(int key) {
        return ::IsKeyDown(key);
    }

    static bool IsKeyPressed(int key) {
        return ::IsKeyPressed(key);
    }

    static bool IsKeyReleased(int key) {
        return ::IsKeyReleased(key);
    }

    // --- Mouse ---

    static bool IsMouseButtonDown(int button) {
        return ::IsMouseButtonDown(button);
    }

    static bool IsMouseButtonPressed(int button) {
        return ::IsMouseButtonPressed(button);
    }

    static bool IsMouseButtonReleased(int button) {
        return ::IsMouseButtonReleased(button);
    }

    static Vector2 GetMousePosition() {
        return ::GetMousePosition();
    }

    static float GetMouseWheelDelta() {
        return ::GetMouseWheelMove();
    }
};

#endif
