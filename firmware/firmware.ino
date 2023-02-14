#include <USBComposite.h>

#define LED PC13



USBHID HID;
HIDKeyboard Keyboard(HID, 0);
USBCompositeSerial USBSerial;

void setup()
{
    pinMode(LED, OUTPUT);
    digitalWrite(LED, 1);

    USBComposite.setProductId(0);
    USBComposite.setVendorId(0x1209);
    USBComposite.setManufacturerString("Cowlark Technologies");
    USBComposite.setProductString("IBM 6770 Keyboard Interface");

    HID.begin(USBSerial, HID_BOOT_KEYBOARD);
    while (!USBComposite)
        ;
    Keyboard.begin();
}

void loop()
{
    USBSerial.println(Keyboard.getLEDs());
}
