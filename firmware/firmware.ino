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

class IBM6770Screen : public HIDLCD<480, 32>
{
public:
    IBM6770Screen(USBHID& HID, KeyboardInterface& keyboardInterface):
        HIDLCD(HID),
        _keyboardInterface(keyboardInterface)
    {
        memset(_shadow, 0xff, sizeof(_shadow));
    }

    void sync()
    {
        /* Look for changed data and send it to the screen --- but only one
         * packet at a time to avoid blocking the main loop too long (as this
         * will cause keypresses to be delayed). */

        uint16_t sendAddress = 0xffff;
        uint16_t currentAddress = 0xffff;
        uint8_t sendBuffer[12];
        int sendCount = 0;

        Bounds& dirtyArea = getDirtyArea();
        int y1 = dirtyArea.y1 & ~7;
        int y2 = (dirtyArea.y2 + 1) & ~7;

        for (int y = y1; y < y2; y += 8)
        {
            dirtyArea.y1 = y;
            for (int x = dirtyArea.x1; x < dirtyArea.x2; x++)
            {
                if (sendCount == sizeof(sendBuffer))
                    goto exit;

                int line = y / 8;
                uint8_t* s = &_shadow[line][x];
                uint8_t b = getColumn(x, y);
                if (*s != b)
                {
                    /* This column has changed, so we need to send it. */

                    uint16_t address = 0x200 * line + x + 1;
                    if (currentAddress != address)
                    {
                        /* Packet in progress --- finish up the current packet
                         * and we'll make a new one next time round. */

                        if (sendCount != 0)
                            goto exit;

                        _keyboardInterface.cmdSetGpuAddress(address);
                        currentAddress = address;
                    }

                    *s = b;
                    sendBuffer[sendCount++] = b;
                    currentAddress++;
                }
            }
        }
        clearDirtyArea();

    exit:
        if (sendCount != 0)
            _keyboardInterface.cmdSendGpuData(sendCount, sendBuffer);
    }

private:
    KeyboardInterface& _keyboardInterface;
    uint8_t _shadow[4][480];
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

    KeyboardInterface.cmdPoll();
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
    if (Screen.isDirty() && !KeyboardInterface.gpuBusy())
        Screen.sync();
}
