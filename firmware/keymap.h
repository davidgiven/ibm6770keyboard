#include "USBHID.h"
#pragma once

static const uint8_t keyboardMap[] = {
    // The scancode layout is as follows:
    //
    // 40 48  00 01 09 11 19 18 20 21 29 31 39 38 28 50      51 71 79 61
    // 41 42  4a   02 0a 12 1a 1b 23 22 2a 32 3a 3b 2b       52 72 7a 62
    // 43 4b  4e    04 0c 14 1c 1d 25 24 2c 34 3c 3d 3e 56   54 74 7c 64
    // 44 4c  5f   07 06 0e 16 1e 1f 27 26 2e 36 3f 5e       46 76 7e fe
    // 4d 45  65       67            47         4f           57 77 7f 6e

    /* 00 */ HID_KEY_Grave,
    /* 01 */ HID_KEY_1,
    /* 02 */ HID_KEY_Q,
    /* 03 */ 0,
    /* 04 */ HID_KEY_A,
    /* 05 */ 0,
    /* 06 */ HID_KEY_Z,
    /* 07 */ HID_KEY_NonUSBackslash,
    /* 08 */ 0,
    /* 09 */ HID_KEY_2,
    /* 0a */ HID_KEY_W,
    /* 0b */ 0,
    /* 0c */ HID_KEY_S,
    /* 0d */ 0,
    /* 0e */ HID_KEY_X,
    /* 0f */ 0,
    /* 10 */ 0,
    /* 11 */ HID_KEY_3,
    /* 12 */ HID_KEY_E,
    /* 13 */ 0,
    /* 14 */ HID_KEY_D,
    /* 15 */ 0,
    /* 16 */ HID_KEY_C,
    /* 17 */ 0,
    /* 18 */ HID_KEY_5,
    /* 19 */ HID_KEY_4,
    /* 1a */ HID_KEY_R,
    /* 1b */ HID_KEY_T,
    /* 1c */ HID_KEY_F,
    /* 1d */ HID_KEY_G,
    /* 1e */ HID_KEY_V,
    /* 1f */ HID_KEY_B,
    /* 20 */ HID_KEY_6,
    /* 21 */ HID_KEY_7,
    /* 22 */ HID_KEY_U,
    /* 23 */ HID_KEY_Y,
    /* 24 */ HID_KEY_J,
    /* 25 */ HID_KEY_H,
    /* 26 */ HID_KEY_M,
    /* 27 */ HID_KEY_N,
    /* 28 */ HID_KEY_Equals,
    /* 29 */ HID_KEY_8,
    /* 2a */ HID_KEY_I,
    /* 2b */ HID_KEY_RightBracket,
    /* 2c */ HID_KEY_K,
    /* 2d */ 0,
    /* 2e */ HID_KEY_Comma,
    /* 2f */ 0,
    /* 30 */ 0,
    /* 31 */ HID_KEY_9,
    /* 32 */ HID_KEY_O,
    /* 33 */ 0,
    /* 34 */ HID_KEY_L,
    /* 35 */ 0,
    /* 36 */ HID_KEY_Period,
    /* 37 */ 0,
    /* 38 */ HID_KEY_Minus,
    /* 39 */ HID_KEY_0,
    /* 3a */ HID_KEY_P,
    /* 3b */ HID_KEY_LeftBracket,
    /* 3c */ HID_KEY_Semicolon,
    /* 3d */ HID_KEY_Quote,
    /* 3e */ HID_KEY_Backslash,
    /* 3f */ HID_KEY_Slash,
    /* 40 */ HID_KEY_F1,
    /* 41 */ HID_KEY_F2,
    /* 42 */ HID_KEY_F6,
    /* 43 */ HID_KEY_F3,
    /* 44 */ HID_KEY_F4,
    /* 45 */ HID_KEY_F9,
    /* 46 */ HID_KP_1,
    /* 47 */ HID_KEY_Space,
    /* 48 */ HID_KEY_Escape,
    /* 49 */ 0,
    /* 4a */ HID_KEY_Tab,
    /* 4b */ HID_KEY_F7,
    /* 4c */ HID_KEY_F8,
    /* 4d */ HID_KEY_F5,
    /* 4e */ HID_KEY_LeftControl,
    /* 4f */ HID_KEY_RightAlt,
    /* 50 */ HID_KEY_Delete,
    /* 51 */ HID_KP_NumLock,
    /* 52 */ HID_KP_7,
    /* 53 */ 0,
    /* 54 */ HID_KP_4,
    /* 55 */ 0,
    /* 56 */ HID_KEY_Return,
    /* 57 */ HID_KP_0,
    /* 58 */ 0,
    /* 59 */ 0,
    /* 5a */ 0,
    /* 5b */ 0,
    /* 5c */ 0,
    /* 5d */ 0,
    /* 5e */ HID_KEY_RightShift,
    /* 5f */ HID_KEY_LeftShift,
    /* 60 */ 0,
    /* 61 */ HID_KP_Subtract,
    /* 62 */ 0,
    /* 63 */ 0,
    /* 64 */ 0,
    /* 65 */ HID_KEY_LeftAlt,
    /* 66 */ 0,
    /* 67 */ HID_KEY_LeftGUI,
    /* 68 */ 0,
    /* 69 */ 0,
    /* 6a */ 0,
    /* 6b */ 0,
    /* 6c */ 0,
    /* 6d */ 0,
    /* 6e */ HID_KP_Enter,
    /* 6f */ 0,
    /* 70 */ 0,
    /* 71 */ HID_KP_Divide,
    /* 72 */ HID_KP_8,
    /* 73 */ 0,
    /* 74 */ HID_KP_5,
    /* 75 */ 0,
    /* 76 */ HID_KP_2,
    /* 77 */ 0,
    /* 78 */ 0,
    /* 79 */ HID_KP_Multiply,
    /* 7a */ HID_KP_9,
    /* 7b */ 0,
    /* 7c */ HID_KP_6,
    /* 7d */ 0,
    /* 7e */ HID_KP_3,
    /* 7f */ HID_KP_Point,
};
