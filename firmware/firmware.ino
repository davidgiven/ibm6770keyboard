#include <USBComposite.h>
#include <usb_hid.h>

#define LED_PIN PC13
#define SERIALRX_PIN PA10
#define SERIALTX_PIN PA9
#define RESET_PIN PB8

#include "keymap.h"

#include "keyboardinterface.h"
KeyboardInterface KeyboardInterface(
    USART1, SERIALRX_PIN, SERIALTX_PIN, RESET_PIN);

static constexpr uint8_t DISPLAY_ATTRIBUTES_REPORT_ID = 21;
static constexpr uint8_t BLIT_REPORT_ID = 22;

static const uint8_t graphicsReportDescriptorBytes[] = {
    /* See page 204 of https://usb.org/sites/default/files/hut1_2.pdf.
     * There's also a really useful example here:
     * https://usb.org/sites/default/files/hutrr29b_0.pdf */
    // clang-format off
    0x05, 0x14,						               // Usage Page (Auxiliary Display Page)
    0x09, 0x02,						               // Usage: Auxiliary Display
    0xA1, 0x01,						               // Collection (Application)
      0x09, 0x20,                        //   Usage (Display Attributes report)
      0xA1, 0x02,                        //   Collection (Logical)
        0x85, DISPLAY_ATTRIBUTES_REPORT_ID, //  Report ID
        0x09, 0x80,                      //     Usage: bitmap size X
        0x09, 0x81,                      //     Usage: bitmap size Y
        0x09, 0x83,                      //     Usage: bitmap depth
        0x15, 0x00,                      //     Logical minimum: 0
        0x26, LSB(0x7fff), MSB(0x7fff), //      Logical maximum: 0x7fff
        0x75, 0x10,                      //     Report size: 16
        0x95, 0x03,                      //     Report size: 3
        0xb1, 0x03,                      //     Feature: Cnst, Var, Abs
      0xc0,                              //   end
      0x09, 0x8a,                        // Usage: blit report
      0xA1, 0x02,                        //   Collection (Logical)
        0x85, BLIT_REPORT_ID,            //     Report ID
        0x09, 0x8b,                      //     Usage: blit rectange X1
        0x09, 0x8c,                      //     Usage: blit rectange Y1
        0x09, 0x8d,                      //     Usage: blit rectange Y2
        0x09, 0x8e,                      //     Usage: blit rectange Y2
        0x15, 0x00,                      //     Logical minimum: 0
        0x26, LSB(0x7fff), MSB(0x7fff),  //     Logical maximum: 0x7fff
        0x75, 0x10,                      //     Report size: 16
        0x95, 0x03,                      //     Report size: 4
        0x91, 0x02,                      //     Output: Data, Var, Abs
        0x09, 0x8f,                      //     Usage: blit data buffer
        0x75, 0x08,                      //     Report size: 8
        0x95, 50,                        //     Report size: 50
        0x92, 0x02, 0x01,               //     Output: Data, Var, Abs, Buf
      0xc0,
    0xc0
    // clang-format on
};

static const HIDReportDescriptor graphicsReportDescriptor = {
    graphicsReportDescriptorBytes, sizeof(graphicsReportDescriptorBytes)};

typedef struct
{
    uint16_t width;
    uint16_t height;
    uint16_t depth;
} __packed DisplayAttributesReport;

typedef struct
{
    uint16_t x1, y1;
    uint16_t x2, y2;
    uint8_t data[50];
} __packed BlitReport;

USBCompositeSerial USBSerial;

#if 0
template <int WIDTH, int HEIGHT>
class HIDGraphics : public HIDReporter
{
public:
    HIDGraphics(USBHID& HID):
        HIDReporter(HID, &graphicsReportDescriptor, nullptr, 0),
        attributesBuffer(attributesBufferBytes,
            sizeof(attributesBufferBytes),
            DISPLAY_ATTRIBUTES_REPORT_ID),
        blitBuffer(blitBufferBytes, sizeof(blitBufferBytes), BLIT_REPORT_ID)
    {
    }

    void begin()
    {
        HID.addFeatureBuffer(&attributesBuffer);
        HID.addOutputBuffer(&blitBuffer);

        displayAttributesReport.width = WIDTH - 1;
        displayAttributesReport.height = HEIGHT - 1;
        displayAttributesReport.depth = 1;
        usb_hid_set_feature(
            DISPLAY_ATTRIBUTES_REPORT_ID, (uint8_t*)&displayAttributesReport);

        memset(buffer, 0, sizeof(buffer));
    }

    void process()
    {
        if (usb_hid_get_data(HID_REPORT_TYPE_OUTPUT,
                BLIT_REPORT_ID,
                (uint8_t*)&blitReport,
                true))
        {
            uint8_t* p = &blitReport.data[0];
            int buffer;
            int count = 0;
            for (int y = blitReport.y1; y < blitReport.y2; y++)
            {
                for (int x = blitReport.x1; x < blitReport.x1; x++)
                {
                    if (p == &blitReport.data[sizeof(blitReport.data)])
                        break;

                    if (!count)
                    {
                        buffer = *p++;
                        count = 8;
                    }

                    setPixel(x, y, buffer & 0x80);
                    buffer <<= 1;
                    count--;
                }
            }
        }
    }

    void setPixel(int x, int y, bool value)
    {
        if ((x < 0) || (y < 0) || (x >= WIDTH) || (y >= HEIGHT))
            return;

        uint8_t* p = &buffer[y * STRIDE + x / 8];
        int b = 0x80 >> (x & ~7);
        *p = (*p & ~b) | (value ? b : 0);
    }

    bool getPixel(int x, int y)
    {
        if ((x < 0) || (y < 0) || (x >= WIDTH) || (y >= HEIGHT))
            return false;

        uint8_t p = buffer[y * STRIDE + x / 8];
        int b = 0x80 >> (x & ~7);
        return p & b;
    }

private:
    void sendReport(uint8_t reportId, const uint8_t* data, unsigned len)
    {
        while (usb_hid_tx(&reportId, 1) != 1)
            ;
        while (len)
        {
            unsigned delta = usb_hid_tx(data, len);
            len -= delta;
            data += delta;
        }
    }

private:
    uint8_t attributesBufferBytes[HID_BUFFER_SIZE(
        sizeof(DisplayAttributesReport), DISPLAY_ATTRIBUTES_REPORT_ID)];
    HIDBuffer_t attributesBuffer;
    DisplayAttributesReport displayAttributesReport;

    uint8_t
        blitBufferBytes[HID_BUFFER_SIZE(sizeof(BlitReport), BLIT_REPORT_ID)];
    HIDBuffer_t blitBuffer;
    BlitReport blitReport;

    static constexpr int STRIDE = (WIDTH + 1) / 8;
    uint8_t buffer[STRIDE * HEIGHT];
    uint8_t backBuffer[STRIDE * HEIGHT];
};

class IBM6770Screen : public HIDGraphics<480, 32>
{
public:
    IBM6770Screen(USBHID& HID): HIDGraphics(HID)
    {
        memset(shadow, 0xff, sizeof(shadow));

        digitalWrite(RESET_PIN, 1);
        delay(200);
        digitalWrite(RESET_PIN, 0);
        delay(200);
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

        for (int line = 0; line < 4; line++)
        {
            for (int x = 0; x < 480; x++)
            {
                if (sendCount == sizeof(sendBuffer))
                    break;

                int y = line * 8;
                uint8_t b =
                    (getPixel(x, y + 0) << 7) | (getPixel(x, y + 1) << 6) |
                    (getPixel(x, y + 2) << 5) | (getPixel(x, y + 3) << 4) |
                    (getPixel(x, y + 4) << 3) | (getPixel(x, y + 5) << 2) |
                    (getPixel(x, y + 6) << 1) | (getPixel(x, y + 7) << 1);

                uint8_t* s = &shadow[line][x];
                if (*s != b)
                {
                    /* This column has changed, so we need to send it. */

                    uint16_t address = 0x200 * line + x;
                    if (currentAddress != address)
                    {
                        /* Packet in progress --- finish up the current packet
                         * and we'll make a new one next time round. */

                        if (sendCount != 0)
                            goto exit;

                        cmdSetAddress(address);
                        currentAddress = address;
                    }

                    *s = b;
                    sendBuffer[sendCount++] = b;
                    currentAddress++;
                }
            }
        }

    exit:
        if (sendCount != 0)
            cmdSendData(sendCount, sendBuffer);
    }

    void cmdSetAddress(uint16_t address)
    {
        USBSerial.print("address=0x");
        USBSerial.println(address, HEX);
    }

    void cmdSendData(int count, const uint8_t* buffer)
    {
        USBSerial.print("bytes=");
        for (int i = 0; i < count; i++)
        {
            USBSerial.print(" 0x");
            USBSerial.print(buffer[i]);
        }
        USBSerial.println();
    }

private:
    uint8_t shadow[4][480];
};
#endif

USBHID HID;
HIDKeyboard USBKeyboard(HID);
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

    // Screen.begin();
    KeyboardInterface.begin();
    KeyboardInterface.initHardware();
    KeyboardInterface.cmdSoundControl('A'); /* keyclick off */
    KeyboardInterface.cmdSoundControl('4'); /* make click */
}

void loop()
{
    // Screen.process();
    // Screen.sync();

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
}
