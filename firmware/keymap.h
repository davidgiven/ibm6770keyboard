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

    /* 00 */ '`',
    /* 01 */ '1',
    /* 02 */ 'q',
    /* 03 */ 0,
    /* 04 */ 'a',
    /* 05 */ 0,
    /* 06 */ 'z',
    /* 07 */ '\\',
    /* 08 */ 0,
    /* 09 */ '2',
    /* 0a */ 'w',
    /* 0b */ 0,
    /* 0c */ 's',
    /* 0d */ 0,
    /* 0e */ 'x',
    /* 0f */ 0,
    /* 10 */ 0,
    /* 11 */ '3',
    /* 12 */ 'e',
    /* 13 */ 0,
    /* 14 */ 'd',
    /* 15 */ 0,
    /* 16 */ 'c',
    /* 17 */ 0,
    /* 18 */ '5',
    /* 19 */ '4',
    /* 1a */ 'r',
    /* 1b */ 't',
    /* 1c */ 'f',
    /* 1d */ 'g',
    /* 1e */ 'v',
    /* 1f */ 'b',
    /* 20 */ '6',
    /* 21 */ '7',
    /* 22 */ 'u',
    /* 23 */ 'y',
    /* 24 */ 'j',
    /* 25 */ 'h',
    /* 26 */ 'm',
    /* 27 */ 'n',
    /* 28 */ '=',
    /* 29 */ '8',
    /* 2a */ 'i',
    /* 2b */ ']',
    /* 2c */ 'k',
    /* 2d */ 0,
    /* 2e */ ',',
    /* 2f */ 0,
    /* 30 */ 0,
    /* 31 */ '9',
    /* 32 */ 'o',
    /* 33 */ 0,
    /* 34 */ 'l',
    /* 35 */ 0,
    /* 36 */ '.',
    /* 37 */ 0,
    /* 38 */ '-',
    /* 39 */ '0',
    /* 3a */ 'p',
    /* 3b */ '[',
    /* 3c */ ';',
    /* 3d */ '\'',
    /* 3e */ '#',
    /* 3f */ '/',
    /* 40 */ KEY_F1,
    /* 41 */ KEY_F2,
    /* 42 */ KEY_F6,
    /* 43 */ KEY_F3,
    /* 44 */ KEY_F4,
    /* 45 */ KEY_F9,
    /* 46 */ 0x88 + 0x59, /* kp 1 */
    /* 47 */ ' ',
    /* 48 */ KEY_ESC,
    /* 49 */ 0,
    /* 4a */ KEY_TAB,
    /* 4b */ KEY_F7,
    /* 4c */ KEY_F8,
    /* 4d */ KEY_F5,
    /* 4e */ KEY_CAPS_LOCK,
    /* 4f */ KEY_RIGHT_GUI,
    /* 50 */ KEY_BACKSPACE,
    /* 51 */ 0x88 + 0x53, /* numlock */
    /* 52 */ 0x88 + 0x54, /* kp 7 */
    /* 53 */ 0,
    /* 54 */ 0x88 + 0x5c, /* kp 4 */
    /* 55 */ 0,
    /* 56 */ KEY_RETURN,
    /* 57 */ 0x88 + 0x62, /* kp 0 */
    /* 58 */ 0,
    /* 59 */ 0,
    /* 5a */ 0,
    /* 5b */ 0,
    /* 5c */ 0,
    /* 5d */ 0,
    /* 5e */ KEY_RIGHT_SHIFT,
    /* 5f */ KEY_LEFT_SHIFT,
    /* 60 */ 0,
    /* 61 */ 0x88 + 0x56, /* kp minus */
    /* 62 */ 0,
    /* 63 */ 0,
    /* 64 */ 0,
    /* 65 */ KEY_LEFT_CTRL,
    /* 66 */ 0,
    /* 67 */ KEY_LEFT_ALT,
    /* 68 */ 0,
    /* 69 */ 0,
    /* 6a */ 0,
    /* 6b */ 0,
    /* 6c */ 0,
    /* 6d */ 0,
    /* 6e */ 0x88 + 0x58, /* kp enter */
    /* 6f */ 0,
    /* 70 */ 0,
    /* 71 */ 0x88 + 0x54, /* kp slash */
    /* 72 */ 0x88 + 0x60, /* kp 8 */
    /* 73 */ 0,
    /* 74 */ 0x88 + 0x5d, /* kp 5 */
    /* 75 */ 0,
    /* 76 */ 0x88 + 0x5a, /* kp 2 */
    /* 77 */ 0,
    /* 78 */ 0,
    /* 79 */ 0x88 + 0x55, /* kp asterisk */
    /* 7a */ 0x88 + 0x61, /* kp 9 */
    /* 7b */ 0,
    /* 7c */ 0x88 + 0x5e, /* kp 6 */
    /* 7d */ 0,
    /* 7e */ 0x88 + 0x5b, /* kp 3 */
    /* 7f */ 0x88 + 0x63, /* kp . */
};
