#include <USBComposite.h>
#include <usb_hid.h>

#define LED_PIN PC13
#define SERIALRX_PIN PA10
#define SERIALTX_PIN PA9
#define RESET_PIN PB8

#include "keymap.h"

USBCompositeSerial USBSerial;

#include "keyboardinterface.h"
#include "hidlcd.h"

class IBM6770Screen : public HIDLCD
{
public:
    static constexpr int width = 480;
    static constexpr int height = 32;

public:
    IBM6770Screen(USBHID& HID, KeyboardInterface& keyboardInterface):
        HIDLCD(HID, width, height),
        _currentAddress(0xffff),
        _keyboardInterface(keyboardInterface)
    {
        memset(_frontBuffer, 0xff, sizeof(_frontBuffer));
        memset(_backBuffer, 0xff, sizeof(_backBuffer));
    }

    void setPixel(int x, int y, bool value) override
    {
        /* The buffer layout mirrors the device video memory layout, of
         * 4*480*8bits, each byte representing a column. This makes it efficient
         * to sync to the device. */

        uint8_t* ptr = &_frontBuffer[(y / 8) * width + x];

        uint8_t b = 1 << (y & 7);
        if (value)
            *ptr |= b;
        else
            *ptr &= ~b;
    }

    void sync()
    {
        /* Look for changed data and send it to the screen --- but only one
         * packet at a time to avoid blocking the main loop too long (as this
         * will cause keypresses to be delayed). */

        const uint8_t* fptr = _frontBuffer;
        uint8_t* bptr = _backBuffer;
        for (;;)
        {
            if (fptr == &_frontBuffer[sizeof(_frontBuffer)])
                return;
            if (*fptr != *bptr)
                break;
            fptr++;
            bptr++;
        }

        /* We found a difference. Determine the location and video memory
         * address. */

        int delta = fptr - _frontBuffer;
        int row = delta / width;
        int column = delta % width;
        uint16_t address = 0x200 * row + column + 1;

        if (address != _currentAddress)
            _keyboardInterface.cmdSetGpuAddress(address);

        uint8_t sendBuffer[12];
        int sendCount = 0;
        while ((column != width) && (*fptr != *bptr) &&
               (sendCount != sizeof(sendBuffer)))
        {
            uint8_t b = *fptr++;
            sendBuffer[sendCount++] = b;
            *bptr++ = b;
            address++;
            column++;
        }

        _keyboardInterface.cmdSendGpuData(sendCount, sendBuffer);
        _currentAddress = address;
    }

private:
    KeyboardInterface& _keyboardInterface;
    uint16_t _currentAddress;
    uint8_t _frontBuffer[width * height / 8];
    uint8_t _backBuffer[sizeof(_frontBuffer)];
};

KeyboardInterface KeyboardInterface(
    USART1, SERIALRX_PIN, SERIALTX_PIN, RESET_PIN);

USBHID HID;
HIDKeyboard USBKeyboard(HID);
IBM6770Screen Screen(HID, KeyboardInterface);
// IBM6770Screen Screen(HID);

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, 1);

    USBComposite.setProductId(0x6e01);
    USBComposite.setVendorId(0x1209);
    USBComposite.setManufacturerString("Cowlark Technologies");
    USBComposite.setProductString("IBM 6770 Keyboard Interface");

    HID.begin(USBSerial);
    while (!USBComposite)
        ;

    USBKeyboard.begin();

    Screen.begin();
    KeyboardInterface.begin();
    KeyboardInterface.initHardware();
    KeyboardInterface.cmdSoundControl('A'); /* keyclick off */
    KeyboardInterface.cmdSoundControl('4'); /* make click */
}

void loop()
{
    Screen.process();

    uint8_t flags = KeyboardInterface.cmdPoll();
    if (KeyboardInterface.keysAvailable())
    {
        uint8_t key = KeyboardInterface.cmdGetKey();
        uint8_t ascii = keyboardMap[key & 0x7f];
        if (ascii)
        {
            if (!(key & 0x80))
                USBKeyboard.press(ascii);
            else
                USBKeyboard.release(ascii);
        }
    }

    if (!(flags & 0x20))
        Screen.sync();
}
