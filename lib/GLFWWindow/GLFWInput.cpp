#include "GLFWWindow.h"

InputSystem::Key GLFWWindow::__TranslateKey(int key)
{
    switch (key)
    {
    case GLFW_KEY_0: return InputSystem::Key::Numpad0;
    case GLFW_KEY_1: return InputSystem::Key::Numpad1;
    case GLFW_KEY_2: return InputSystem::Key::Numpad2;
    case GLFW_KEY_3: return InputSystem::Key::Numpad3;
    case GLFW_KEY_4: return InputSystem::Key::Numpad4;
    case GLFW_KEY_5: return InputSystem::Key::Numpad5;
    case GLFW_KEY_6: return InputSystem::Key::Numpad6;
    case GLFW_KEY_7: return InputSystem::Key::Numpad7;
    case GLFW_KEY_8: return InputSystem::Key::Numpad8;
    case GLFW_KEY_9: return InputSystem::Key::Numpad9;

    case GLFW_KEY_F1: return InputSystem::Key::F1;
    case GLFW_KEY_F2: return InputSystem::Key::F2;
    case GLFW_KEY_F3: return InputSystem::Key::F3;
    case GLFW_KEY_F4: return InputSystem::Key::F4;
    case GLFW_KEY_F5: return InputSystem::Key::F5;
    case GLFW_KEY_F6: return InputSystem::Key::F6;
    case GLFW_KEY_F7: return InputSystem::Key::F7;
    case GLFW_KEY_F8: return InputSystem::Key::F8;
    case GLFW_KEY_F9: return InputSystem::Key::F9;
    case GLFW_KEY_F10: return InputSystem::Key::F10;
    case GLFW_KEY_F11: return InputSystem::Key::F11;
    case GLFW_KEY_F12: return InputSystem::Key::F12;

    case GLFW_KEY_LEFT_SHIFT: return InputSystem::Key::LShift;
    case GLFW_KEY_RIGHT_SHIFT: return InputSystem::Key::RShift;
    case GLFW_KEY_LEFT_CONTROL: return InputSystem::Key::LCtrl;
    case GLFW_KEY_RIGHT_CONTROL: return InputSystem::Key::RCtrl;
    case GLFW_KEY_LEFT_ALT: return InputSystem::Key::LAlt;
    case GLFW_KEY_RIGHT_ALT: return InputSystem::Key::RAlt;

    case GLFW_KEY_LEFT: return InputSystem::Key::Left;
    case GLFW_KEY_RIGHT: return InputSystem::Key::Right;
    case GLFW_KEY_UP: return InputSystem::Key::Up;
    case GLFW_KEY_DOWN: return InputSystem::Key::Down;
    case GLFW_KEY_HOME: return InputSystem::Key::Home;
    case GLFW_KEY_END: return InputSystem::Key::End;
    case GLFW_KEY_PAGE_UP: return InputSystem::Key::PageUp;
    case GLFW_KEY_PAGE_DOWN: return InputSystem::Key::PageDown;

    case GLFW_KEY_ESCAPE: return InputSystem::Key::Escape;
    case GLFW_KEY_ENTER: return InputSystem::Key::Enter;
    case GLFW_KEY_SPACE: return InputSystem::Key::Space;
    case GLFW_KEY_TAB: return InputSystem::Key::Tab;
    case GLFW_KEY_BACKSPACE: return InputSystem::Key::Backspace;
    case GLFW_KEY_INSERT: return InputSystem::Key::Insert;
    case GLFW_KEY_DELETE: return InputSystem::Key::Delete;
    case GLFW_KEY_CAPS_LOCK: return InputSystem::Key::CapsLock;
    case GLFW_KEY_PRINT_SCREEN: return InputSystem::Key::PrintScreen;
    case GLFW_KEY_SCROLL_LOCK: return InputSystem::Key::ScrollLock;
    case GLFW_KEY_PAUSE: return InputSystem::Key::Pause;
    case GLFW_KEY_LEFT_SUPER: return InputSystem::Key::LeftSuper;
    case GLFW_KEY_RIGHT_SUPER: return InputSystem::Key::RightSuper;
    case GLFW_KEY_MENU: return InputSystem::Key::Menu;
    case GLFW_KEY_LEFT_BRACKET: return InputSystem::Key::LeftBracket;
    case GLFW_KEY_RIGHT_BRACKET: return InputSystem::Key::RightBracket;
    case GLFW_KEY_SEMICOLON: return InputSystem::Key::Semicolon;
    case GLFW_KEY_COMMA: return InputSystem::Key::Comma;
    case GLFW_KEY_PERIOD: return InputSystem::Key::Period;
    case GLFW_KEY_APOSTROPHE: return InputSystem::Key::Quote;
    case GLFW_KEY_SLASH: return InputSystem::Key::Slash;
    case GLFW_KEY_BACKSLASH: return InputSystem::Key::Backslash;
    case GLFW_KEY_GRAVE_ACCENT: return InputSystem::Key::Tilde;
    case GLFW_KEY_EQUAL: return InputSystem::Key::Equal;
    case GLFW_KEY_MINUS: return InputSystem::Key::Hyphen;

    case GLFW_KEY_KP_DIVIDE: return InputSystem::Key::NumpadDivide;
    case GLFW_KEY_KP_MULTIPLY: return InputSystem::Key::NumpadMultiply;
    case GLFW_KEY_KP_SUBTRACT: return InputSystem::Key::NumpadSubtract;
    case GLFW_KEY_KP_ADD: return InputSystem::Key::NumpadAdd;
    case GLFW_KEY_KP_ENTER: return InputSystem::Key::NumpadEnter;
    case GLFW_KEY_KP_DECIMAL: return InputSystem::Key::NumpadDecimal;

    case GLFW_KEY_A: return InputSystem::Key::KeyboardA;
    case GLFW_KEY_B: return InputSystem::Key::KeyboardB;
    case GLFW_KEY_C: return InputSystem::Key::KeyboardC;
    case GLFW_KEY_D: return InputSystem::Key::KeyboardD;
    case GLFW_KEY_E: return InputSystem::Key::KeyboardE;
    case GLFW_KEY_F: return InputSystem::Key::KeyboardF;
    case GLFW_KEY_G: return InputSystem::Key::KeyboardG;
    case GLFW_KEY_H: return InputSystem::Key::KeyboardH;
    case GLFW_KEY_I: return InputSystem::Key::KeyboardI;
    case GLFW_KEY_J: return InputSystem::Key::KeyboardJ;
    case GLFW_KEY_K: return InputSystem::Key::KeyboardK;
    case GLFW_KEY_L: return InputSystem::Key::KeyboardL;
    case GLFW_KEY_M: return InputSystem::Key::KeyboardM;
    case GLFW_KEY_N: return InputSystem::Key::KeyboardN;
    case GLFW_KEY_O: return InputSystem::Key::KeyboardO;
    case GLFW_KEY_P: return InputSystem::Key::KeyboardP;
    case GLFW_KEY_Q: return InputSystem::Key::KeyboardQ;
    case GLFW_KEY_R: return InputSystem::Key::KeyboardR;
    case GLFW_KEY_S: return InputSystem::Key::KeyboardS;
    case GLFW_KEY_T: return InputSystem::Key::KeyboardT;
    case GLFW_KEY_U: return InputSystem::Key::KeyboardU;
    case GLFW_KEY_V: return InputSystem::Key::KeyboardV;
    case GLFW_KEY_W: return InputSystem::Key::KeyboardW;
    case GLFW_KEY_X: return InputSystem::Key::KeyboardX;
    case GLFW_KEY_Y: return InputSystem::Key::KeyboardY;
    case GLFW_KEY_Z: return InputSystem::Key::KeyboardZ;


    default: return InputSystem::Key::Count;
    }
}

InputSystem::KeyModifier GLFWWindow::__TranslateKeyModifier(int keymod)
{
    InputSystem::KeyModifier mods = InputSystem::KeyModifier::None;
    if (keymod & GLFW_MOD_SHIFT)
        mods = (InputSystem::KeyModifier)((int)mods | (int)InputSystem::KeyModifier::Shift);
    if (keymod & GLFW_MOD_CONTROL)
        mods = (InputSystem::KeyModifier)((int)mods | (int)InputSystem::KeyModifier::Ctrl);
    if (keymod & GLFW_MOD_ALT)
        mods = (InputSystem::KeyModifier)((int)mods | (int)InputSystem::KeyModifier::Alt);
    return mods;
}