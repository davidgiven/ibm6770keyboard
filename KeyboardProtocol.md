The keyboard protocol
=====================

The keyboard uses a half-duplex bidirectional single-wire serial link running
at 186453 baud 8E1 (yes, really). It's strictly controlled by the master. The
slave only replies after the master sends a byte, and the master has to wait
until this byte is received before sending the next byte.

At a higher level, commands are sent in packets. The first byte of each command
is always sent with the parity bit forced on (sending it as 8N1 works, as the
stop bit will be treated as the parity bit).

Example: the Poll command sends `22, 00, 00` and receives `00, nn, 00` where
`nn` is the response.The master sends `22`, and receives 00. Then it sends
`00`, and receives `nn`. Then it sends `00`, and receives `00`. The keyboard is
then ready for the next command.

So far I haven't seen any cases where the master sends a non-zero bytes and the
slave replies with a non-zero byte. If the slave replies with an `ff`, that
usually means a protocol error.

The pinout is as follows:

| Keyboard pin | Meaning |
|:-------------|:--------|
| 1            | -5V bias voltage for the LCD |
| 2            | +5V power supply             |
| 3            | (unknown)                    |
| 4            | not connected                |
| 5            | 0V                           |
| 6            | not connected                |
| 7            | data                         |
| 8            | reset                        |

Keyboard modes
--------------

When the keyboard starts up, it's in _demo mode_. In this state, typing on the
keyboard will cause the scancodes to be displayed on the screen. To get it into
_normal mode_, you need to send the `11 Reset` command.

The Poll command returns a flags byte describing the current keyboard state.

`80`
:   Key event buffer is empty.
`40`
:   Key event buffer is full and any additional keypresses will be lost.
`20`
:   GPU is busy --- don't send any graphics commands.
`10`
:   Sound is playing.
`08`
:   The keyboard is in demo mode.

The others are unknown (so far).

The keyboard has about 1kB of video memory. The main bitmap is divided into
four rows of 480 8-bit columns at addresses 0x001, 0x201, 0x401 and 0x601. Each
byte represents eight _vertical_ bits.

There's also a character overlay which is written to with `77`, which I haven't
investigated yet.

Commands
--------

(If the keyboard replies with anything useful, the byte marked with a `!` is
the one with the reply attached to it. No, it's not necessarily the last...)

### `11`

Reset.

Restarts the keyboard firmware in _normal mode_.

### `22!`

Poll.

Replies with the current keyboard status flags.

### `33!`

Fetches the next key from the event buffer. This has the scancode in the
bottom seven bytes. If this is a keyup, the top bit will be set. If the event
buffer is empty, this returns 0 --- which _is_ a valid scancode! (It's `\``.)

### `44 x`

Sets the cursor X position. Characters are six pixels wide.

### `55 f1 f2`

Controls the cursor.

For `f1`: the bottom four bits control which line the cursor is on --- 0, 1, 2
or 4. Add `40` if you want 8-pixel high characters; the default is 16.

For `f2`:

`10`
:   Cursor is flashing.
`40`
:   Cursor is an underscore only; the default is an underscore and an
overscore.

### `66 r x w`

Controls the overlay window.

`r`: right margin, in characters.

`x`: unknown --- maybe a command for scrolling?

`w`: width of the window.

### `77 x bytes...`

Draws a character on the overlay window.

`x` is the character position. This is then followed by either 6 bytes or 12
bytes, for 8-pixel and 16-pixel high characters respectively. See `55`.

### `88 nn`

Sound control.

`nn` can be:

`30`, `31`
:   Plays a low beep; long, short.

`32`, `33`
:   Plays a high beep: long, short.

`34`
:   Produces a click.

`40`
:   Turns keyclicks on.

`41`
:   Turns keyclicks off.

### `99 hi lo`

Sets the current video memory address.

Used for writing with `bb`.

### `aa`

Switches to _demo mode_.

### `bb n bytes...`

Writes to video memory.

Takes `n` bytes and writes them consecutively to video memory, incrementing
the write address each time. `n` can go up to 12 before the firmware
command buffer overflows and the keyboard crashes.

### `cc x`

Unknown.

### `dd hi1 lo1 hi2 lo2`

Clears video memory.

Writes `{hi2,lo2}` zero bytes at address `{hi1,lo1}`. `lo1` seems to have
to be 1 or odd things happen.

### `ee x`

Unknown.

### `ff`

Unknown.

