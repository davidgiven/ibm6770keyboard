#pragma once

enum
{
    /* Zero, does not correspond to any key. */
    HID_KEY_None = 0,

    /* Keycode definitions. */
    HID_KEY_A = 4,
    HID_KEY_B = 5,
    HID_KEY_C = 6,
    HID_KEY_D = 7,
    HID_KEY_E = 8,
    HID_KEY_F = 9,
    HID_KEY_G = 10,
    HID_KEY_H = 11,
    HID_KEY_I = 12,
    HID_KEY_J = 13,
    HID_KEY_K = 14,
    HID_KEY_L = 15,
    HID_KEY_M = 16,
    HID_KEY_N = 17,
    HID_KEY_O = 18,
    HID_KEY_P = 19,
    HID_KEY_Q = 20,
    HID_KEY_R = 21,
    HID_KEY_S = 22,
    HID_KEY_T = 23,
    HID_KEY_U = 24,
    HID_KEY_V = 25,
    HID_KEY_W = 26,
    HID_KEY_X = 27,
    HID_KEY_Y = 28,
    HID_KEY_Z = 29,
    HID_KEY_1 = 30,
    HID_KEY_2 = 31,
    HID_KEY_3 = 32,
    HID_KEY_4 = 33,
    HID_KEY_5 = 34,
    HID_KEY_6 = 35,
    HID_KEY_7 = 36,
    HID_KEY_8 = 37,
    HID_KEY_9 = 38,
    HID_KEY_0 = 39,
    HID_KEY_Return = 40,
    HID_KEY_Escape = 41,
    HID_KEY_Delete = 42,
    HID_KEY_Tab = 43,
    HID_KEY_Space = 44,
    HID_KEY_Minus = 45,
    HID_KEY_Equals = 46,
    HID_KEY_LeftBracket = 47,
    HID_KEY_RightBracket = 48,
    HID_KEY_Backslash = 49,
    HID_KEY_Semicolon = 51,
    HID_KEY_Quote = 52,
    HID_KEY_Grave = 53,
    HID_KEY_Comma = 54,
    HID_KEY_Period = 55,
    HID_KEY_Slash = 56,
    HID_KEY_CapsLock = 57,
    HID_KEY_F1 = 58,
    HID_KEY_F2 = 59,
    HID_KEY_F3 = 60,
    HID_KEY_F4 = 61,
    HID_KEY_F5 = 62,
    HID_KEY_F6 = 63,
    HID_KEY_F7 = 64,
    HID_KEY_F8 = 65,
    HID_KEY_F9 = 66,
    HID_KEY_F10 = 67,
    HID_KEY_F11 = 68,
    HID_KEY_F12 = 69,
    HID_KEY_PrintScreen = 70,
    HID_KEY_ScrollLock = 71,
    HID_KEY_Pause = 72,
    HID_KEY_Insert = 73,
    HID_KEY_Home = 74,
    HID_KEY_PageUp = 75,
    HID_KEY_DeleteForward = 76,
    HID_KEY_End = 77,
    HID_KEY_PageDown = 78,
    HID_KEY_Right = 79,
    HID_KEY_Left = 80,
    HID_KEY_Down = 81,
    HID_KEY_Up = 82,
    HID_KP_NumLock = 83,
    HID_KP_Divide = 84,
    HID_KP_Multiply = 85,
    HID_KP_Subtract = 86,
    HID_KP_Add = 87,
    HID_KP_Enter = 88,
    HID_KP_1 = 89,
    HID_KP_2 = 90,
    HID_KP_3 = 91,
    HID_KP_4 = 92,
    HID_KP_5 = 93,
    HID_KP_6 = 94,
    HID_KP_7 = 95,
    HID_KP_8 = 96,
    HID_KP_9 = 97,
    HID_KP_0 = 98,
    HID_KP_Point = 99,
    HID_KEY_NonUSBackslash = 100,
    HID_KP_Equals = 103,
    HID_KEY_F13 = 104,
    HID_KEY_F14 = 105,
    HID_KEY_F15 = 106,
    HID_KEY_F16 = 107,
    HID_KEY_F17 = 108,
    HID_KEY_F18 = 109,
    HID_KEY_F19 = 110,
    HID_KEY_F20 = 111,
    HID_KEY_F21 = 112,
    HID_KEY_F22 = 113,
    HID_KEY_F23 = 114,
    HID_KEY_F24 = 115,
    HID_KEY_Help = 117,
    HID_KEY_Menu = 118,
    HID_KEY_Mute = 127,
    HID_KEY_SysReq = 154,
    HID_KP_Clear = 216,
    HID_KP_Decimal = 220,
    HID_KEY_LeftControl = 224,
    HID_KEY_LeftShift = 225,
    HID_KEY_LeftAlt = 226,
    HID_KEY_LeftGUI = 227,
    HID_KEY_RightControl = 228,
    HID_KEY_RightShift = 229,
    HID_KEY_RightAlt = 230,
    HID_KEY_RightGUI = 231
};

#define HID_KEYMOD_LeftControl 0b00000001
#define HID_KEYMOD_LeftShift 0b00000010
#define HID_KEYMOD_LeftAlt 0b00000100
#define HID_KEYMOD_LeftGUI 0b00001000
#define HID_KEYMOD_RightControl 0b00010000
#define HID_KEYMOD_RightShift 0b00100000
#define HID_KEYMOD_RightAlt 0b01000000
#define HID_KEYMOD_RightGUI 0b10000000

class HIDRawKeyboard : public HIDReporter
{
public:
    KeyReport_t keyReport;

protected:
    uint8_t leds[HID_BUFFER_ALLOCATE_SIZE(1, 1)];
    HIDBuffer_t ledData;
    uint8_t reportID;
    uint8_t rollover;
    bool adjustForHostCapsLock = true;

public:
    HIDRawKeyboard(USBHID& HID,
        uint8_t _reportID = HID_KEYBOARD_REPORT_ID,
        uint8_t _rollover = HID_KEYBOARD_ROLLOVER):
        HIDReporter(HID,
            hidReportKeyboard,
            (uint8*)&keyReport,
            sizeof(KeyReport_t) + _rollover - HID_KEYBOARD_MAX_ROLLOVER,
            _reportID),
        ledData(leds,
            HID_BUFFER_SIZE(1, _reportID),
            _reportID,
            HID_BUFFER_MODE_NO_WAIT),
        reportID(_reportID),
        rollover(_rollover)
    {
    }

    void begin(void)
    {
        HID.addOutputBuffer(&ledData);
    }

    void end(void) {}

    void setAdjustForHostCapsLock(bool state)
    {
        adjustForHostCapsLock = state;
    }

    uint8 getLEDs(void)
    {
        return leds[reportID != 0 ? 1 : 0];
    }

private:
    uint8_t getModifiers(uint8_t k)
    {
        switch (k)
        {
            case HID_KEY_LeftShift:
                return HID_KEYMOD_LeftShift;
            case HID_KEY_LeftControl:
                return HID_KEYMOD_LeftControl;
            case HID_KEY_LeftAlt:
                return HID_KEYMOD_LeftAlt;
            case HID_KEY_LeftGUI:
                return HID_KEYMOD_LeftGUI;
            case HID_KEY_RightShift:
                return HID_KEYMOD_RightShift;
            case HID_KEY_RightAlt:
                return HID_KEYMOD_RightAlt;
            default:
                return 0;
        }
    }

public:
    size_t press(uint8_t k)
    {
        keyReport.modifiers |= getModifiers(k);

        for (unsigned i = 0; i < rollover; i++)
        {
            if (keyReport.keys[i] == k)
            {
                goto SEND;
            }
        }
        for (unsigned i = 0; i < rollover; i++)
        {
            if (keyReport.keys[i] == 0)
            {
                keyReport.keys[i] = k;
                goto SEND;
            }
        }
    

    SEND:
        sendReport();
        return 1;
    }

    size_t release(uint8_t k)
    {
        keyReport.modifiers &= ~getModifiers(k);

        for (unsigned i = 0; i < rollover; i++)
        {
            if (keyReport.keys[i] == k)
            {
                keyReport.keys[i] = 0;
                break;
            }
        }

        sendReport();
        return 1;
    }

    void releaseAll(void)
    {
        memset(keyReport.keys, 0, rollover);
        keyReport.modifiers = 0;

        sendReport();
    }
};
