Reverse engineering the IBM 6770 keyboard
=========================================

What?
-----

This repository contains information on how the IBM 6770 and 6780 keyboards
work, plus how to convert them to work with USB. These keyboards are amazingly
cool proto-model-Ms dating from 1985, and have a 480x32 LCD screen on each one.

There's also some Arduino firmware for the keyboard controller and a very
simple client which will draw an image on the LCD.

- [Read all the juicy protocol details.](KeyboardProtocol.md)


How?
----

The `firmware` directory contains an Arduino sketch suitable for programming a
generic Blue Pill STM32F103 based microcontroller. This provides a HID Keyboard
and HID LCD interface which will talk to the keyboard microcontroller itself.

To use, you'll want to wire things up as following:

| Keyboard pin | Blue Pill pin | Meaning |
|:-------------|:--------------|:--------|
| 1            |               | -5V bias voltage for the LCD |
| 2            | 5V            | +5V power supply             |
| 3            |               | (unknown)                    |
| 4            |               | not connected                |
| 5            | GND           | 0V                           |
| 6            |               | not connected                |
| 7            | PA9, PA10     | data                         |
| 8            | PB8           | reset                        |

Note that the data line needs to be connected to both PA9 _and_ PA10. Also, if
you want to use the LCD, you'll need to source a -5V supply from somewhere. I
used a cheap Banggood negative voltage generator module. Almost no current is
required.

Once programmed and hooked up, plug in the Blue Pill. There'll be a short pause
as the keyboard is initialised and it'll make a short beep. If you have the LCD
working, you'll see it go black and then clear. It's now ready to use,
presenting itself as a keyboard.

To use the client, just run `make` in the `client` directory. Then do `kbdtool
<image>`. The image must be a 1bpp image which is 480x32.

(The client _should_ work on anything which conforms to the HID Auxiliary
Display bitmap protocol, but I haven't even heard of any other software or
hardware which uses this, so I've no idea if I've gotten the protocol right.)


Why?
----

I have one. And it's the nicest keyboard I've ever used.


Who?
----

Everything _except_ the `binaries` directory was designed, built and written by
me, David Given. You may contact me at dg@cowlark.com, or visit my website at
http://www.cowlark.com.  There may or may not be anything interesting there.


License
-------

Everything here _except_ the `binaries` directory is © 2023 David Given and is
licensed under the MIT open source license. Please see [LICENSE](LICENSE) for
the full text. The tl;dr is: you can do what you like with it provided you
don't claim you wrote it.

