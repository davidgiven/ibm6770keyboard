#include <USBComposite.h>

#define LED PC13

#if 0
class HIDGraphics : public HIDReporter
{
public:
protected:
public:
    HIDGraphics(USBHID& HID):
        HIDReporter(HID, &reportDescriptor, nullptr, 0)
    {
    }

    void begin()
    {
        buf.buffer = rxBuffer;
        buf.bufferSize = HID_BUFFER_SIZE(sizeof(rxBuffer), 0);
        buf.reportID = 0;
        HID.addOutputBuffer(&buf);
    }

    void end(void);

private:
    static constexpr uint8_t reportDescriptorBytes[] = {};
    static constexpr HIDReportDescriptor reportDescriptor = {
        reportDescriptorBytes, sizeof(reportDescriptorBytes)};

private:
    uint8_t rxBuffer[64];
    HIDBuffer_t buf;
};
#endif

static const uint8_t reportDescriptor[] = {
    HID_BOOT_KEYBOARD_REPORT_DESCRIPTOR(),

    /* See page 204 of https://usb.org/sites/default/files/hut1_2.pdf.
     * There's also a really useful example here:
     * https://usb.org/sites/default/files/hutrr29b_0.pdf */
    // clang-format off
    0x05, 0x14,						               // Usage Page (Auxiliary Display Page)
    0x09, 0x02,						               // Usage: Auxiliary Display
    0xA1, 0x01,						               // Collection (Application)
      0x09, 0x20,                        //   Usage (Display Attributes report)
      0xA1, 0x02,                        //   Collection (Logical)
        0x85, 0x40,                      //     Report ID: 0x40
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
        0x85, 0x41,                      //     Report ID: 0x41
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
        0x96, 0x00, 0x01,                //     Report size: 256
        0x92, 0x02, 0x01,               //     Output: Data, Var, Abs, Buf
      0xc0,
    0xc0
    // clang-format on
};

USBHID HID;
HIDKeyboard Keyboard(HID, 0);
USBCompositeSerial USBSerial;

HardwareSerial KeyboardSerial(USART2, PA2, PA2);

void setup()
{
    pinMode(LED, OUTPUT);
    digitalWrite(LED, 1);

    USBComposite.setProductId(0x6e01);
    USBComposite.setVendorId(0x1209);
    USBComposite.setManufacturerString("Cowlark Technologies");
    USBComposite.setProductString("IBM 6770 Keyboard Interface");

    HID.begin(USBSerial, reportDescriptor, sizeof(reportDescriptor));
    while (!USBComposite)
        ;
    Keyboard.begin();

    KeyboardSerial.begin(186453);
}

void loop()
{
    if (KeyboardSerial.available())
    {
        // KeyboardSerial.enableHalfDuplexRx();
        uint8_t b = KeyboardSerial.read();
        USBSerial.println(b, HEX);
    }
}
