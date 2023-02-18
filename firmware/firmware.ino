#include <USBComposite.h>
#include <usb_hid.h>

#define LED PC13

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
        memset(backBuffer, 0xff, sizeof(backBuffer));
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

USBHID HID;
HIDKeyboard Keyboard(HID, 0);
HIDGraphics<480, 32> Graphics(HID);

HardwareSerial KeyboardSerial(USART2, PA2, PA2);

void setup()
{
    pinMode(LED, OUTPUT);
    digitalWrite(LED, 1);

    USBComposite.setProductId(0x6e01);
    USBComposite.setVendorId(0x1209);
    USBComposite.setManufacturerString("Cowlark Technologies");
    USBComposite.setProductString("IBM 6770 Keyboard Interface");

    HID.begin(USBSerial);
    while (!USBComposite)
        ;

    Keyboard.begin();
    Graphics.begin();

    KeyboardSerial.begin(186453);
}

extern "C"
{
    void logs(const char* s)
    {
        USBSerial.println(s);
    }
    void logp(void* p)
    {
        USBSerial.print("ptr=0x");
        USBSerial.println((unsigned)p, HEX);
    }
}

void loop()
{
    Graphics.process();

    if (KeyboardSerial.available())
    {
        // KeyboardSerial.enableHalfDuplexRx();
        uint8_t b = KeyboardSerial.read();
        // USBSerial.println(b, HEX);
    }
}
