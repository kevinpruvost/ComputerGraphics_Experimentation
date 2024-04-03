#pragma once

#include <common/Config.h>
#include <common/Logger.h>
#include <common/Exception.h>
#include <common/DLL.h>
#include <common/Callback.h>

class InputSystem
{
public:
    COMMONFRAMEWORK_API InputSystem();
    ~InputSystem() = default;
    // Key inputs
    enum class Key
    {
        // Alphabets
        KeyboardA, KeyboardB, KeyboardC, KeyboardD, KeyboardE, KeyboardF,
        KeyboardG, KeyboardH, KeyboardI, KeyboardJ, KeyboardK, KeyboardL,
        KeyboardM, KeyboardN, KeyboardO, KeyboardP, KeyboardQ, KeyboardR,
        KeyboardS, KeyboardT, KeyboardU, KeyboardV, KeyboardW, KeyboardX,
        KeyboardY, KeyboardZ,

        // Numbers
        Num0, Num1, Num2, Num3, Num4, Num5,
        Num6, Num7, Num8, Num9,

        // Function keys
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

        // Modifier keys
        LShift, RShift, LCtrl, RCtrl, LAlt, RAlt,

        // Navigation keys
        Left, Right, Up, Down, Home, End, PageUp, PageDown,

        // Special keys
        Escape, Enter, Space, Tab, Backspace, Insert, Delete,
        CapsLock, PrintScreen, ScrollLock, Pause,
        LeftSuper, RightSuper, Menu,
        LeftBracket, RightBracket, Semicolon, Comma, Period, Quote,
        Slash, Backslash, Tilde, Equal, Hyphen,

        // Numpad keys
        NumpadDivide, NumpadMultiply, NumpadSubtract,
        NumpadAdd, NumpadEnter, NumpadDecimal,
        Numpad0, Numpad1, Numpad2, Numpad3, Numpad4,
        Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,

        // Count of keys
        Count
    };

    enum class KeyModifier
    {
        None     = 0x00,
        Shift    = 0x01,
        Ctrl     = 0x02,
        Alt      = 0x04,
        Count
    };

    COMMONFRAMEWORK_API void SetWhileKeyDownCallback   (const CallbackContainer<void, Key, KeyModifier>& callback);
    COMMONFRAMEWORK_API void SetOnKeyDownCallback      (const CallbackContainer<void, Key, KeyModifier> & callback);
    COMMONFRAMEWORK_API void SetOnKeyUpCallback        (const CallbackContainer<void, Key, KeyModifier> & callback);
    COMMONFRAMEWORK_API void SetOnKeyRepeatCallback    (const CallbackContainer<void, Key, KeyModifier> & callback);

    // Mouse inputs
    COMMONFRAMEWORK_API void SetOnMouseMoveCallback     (const CallbackContainer<void, int, int> & callback);
    COMMONFRAMEWORK_API void SetOnMouseWheelCallback    (const CallbackContainer<void, int> & callback);
    COMMONFRAMEWORK_API void SetOnMouseDownCallback     (const CallbackContainer<void, int, int> & callback);
    COMMONFRAMEWORK_API void SetWhileMouseDownCallback  (const CallbackContainer<void, int, int>& callback);
    COMMONFRAMEWORK_API void SetOnMouseUpCallback       (const CallbackContainer<void, int, int> & callback);
    COMMONFRAMEWORK_API void SetOnMouseDblClickCallback (const CallbackContainer<void, int, int> & callback);

protected:
    Callback<void, Key, KeyModifier> _callbackKeyDown;
    Callback<void, Key, KeyModifier> _callbackWhileKeyDown;
    Callback<void, Key, KeyModifier> _callbackKeyUp;
    Callback<void, Key, KeyModifier> _callbackKeyRepeat;
    Callback<void, int, int> _callbackMouseMove;
    Callback<void, int>      _callbackMouseWheel;
    Callback<void, int, int> _callbackMouseDown;
    Callback<void, int, int> _callbackMouseWhileDown;
    Callback<void, int, int> _callbackMouseUp;
    Callback<void, int, int> _callbackMouseDblClick;
};