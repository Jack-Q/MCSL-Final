# Bluetooth & USB & IR Controller

## Communication Protocol

### Device --> Board

* Name

Report the name of the device, which will be displayed on LCD, limited to 11 characters.

```txt
  0x20 [char-00] [char-01] [char-02]
  0x21 [char-03] [char-04] [char-05]
  0x22 [char-06] [char-07] [char-08]
  0x23 [char-09] [char-10] 00

  Each package in 4 bytes only.
  The last byte is ignored.

```

* Connectivity & Type

Transimt the divice type and evice connectivity. The device will transimit this package
on a regular basis as a *heartbeat*.

```txt
  0xff [Type] 0x00 0xaa
        PC:      0x01
        Android: 0x02
```

* Message

Request to display a message on the LCD. Generally used as a status report or error feedback.
This may not be displayed owing to various reasons.

This string transimtted use a terminator '\0' as C language.

```txt
  0x70 [char-00] [char-01] [char-02]
  0x71 [char-03] [char-04] [char-05]
  0x72 [char-06] [char-07] [char-08]
  0x73 [char-09] [char-10] [char-11]
  0x74 [char-12] [char-13] [char-14]
  0x75 [char-15] 00 00

  The string is limited to 16 characters.
  00 is used as terminator
```

### Board --> Device

For simplicity, all package are 4 bytes.

* Mouse Action

Support a general mouse with 3 buttons (left, right, middle), and a scrollable
middle wheel (scroll up or down one unit each time), and a relative coordination
based positioning method (each movement is represented in one byte in each 
orientation/axis in 2D plane).

```txt
  0x55 [left|right|middle|scrollUp|scrollDown] dX dY
        0x01 Left                           dX: byte
        0x02 Right                          dY: byte
        0x04 Middle (Key press)
        0x08 Middle (Scroll up)
        0x10 Middle (Scroll down)
```

* Key Action

Since the keycode is reinterpreted by a Python script in host PC platform,
the action code here are taken directly from PyInput library to maximum the
compatibility.

```txt
  0xaa [Shift|Alt|Ctrl|Win] KeyCode[Python]
         0x01 Shift           0x00   No key
         0x02 Alt             other  Key Code defined in PyKeyboard
         0x04 Ctrl
         0x08 Win
```

* Special Action

The remote controller contians some functions that can be dirrectly mapped to
the funcitonality of computer such of power of control. These signal are
transimitted in this package. If the daemon script on host has no privilige to
perform the required action, it *may* send a error package to indicate this problem.

```txt
  0xdd [Comfirmation] [Delay] [Action]
       0x01 confirmation: a action may require additional confirmation on host PC
                    Delay: (in sec) postpont the actual execution of request script
                              Action Code

  Action Code:
    1 (0x01): shutdown

```

## Display

### Font Mapping

```txt
  [0] Device Icon            => LCD_FONT_PC LCD_FONT_PHONE
  [1] Mouse/Keyboard (left)  => LCD_FONT_MOUSE_L LCD_FONT_KEYBOARD_L
  [2] Mouse/Keyboard (right) => LCD_FONT_MOUSE_R LCD_FONT_KEYBOARD_R
  [3] IR                     => LCD_FONT_IR
  [4] Ctrl                   => LCD_FONT_CTRL
  [5] Alt                    => LCD_FONT_ALT
  [6] Shift                  => LCD_FONT_SHIFT
  [7] Win                    => LCD_FONT_WIN
```

### Display layout/pattern

* Welcome message

```txt
Row1: "Welcome to use  "
Row2: "                "
       ^              ^
       0             15
```

* Waiting for connection

```txt
Row1: "PLEASE CONNECT  "
Row2: " TO SOME DEVICE "
       ^              ^
       0             15
```

* Connected

```txt
Row1: [DT][  name   ] [MS][IR]
      ^   ^         ^ ^   ^
      0   1        11 13  15

Row2: [KEYS][key name][mm:ss]
      ^                ^     ^
      0               11    15

DT:       Device type icon
name:     Device name
IR:       Infared remote controller connection status
MS:       USB mouse connection status
KEYS:     Functional keys toggle status
key name: General key
mm:ss:    Time of connection
```

* Display message

Message will be displayed on the first row.
The second is the same as teh connection status.

```txt
Row1: "[   message    ]"
       ^              ^
       0             15
```

* Display option

Display a option as a simulation of dialog in GUI application.

```txt
Row1: "[    prompt    ]"
       ^              ^
       0             15
Row1: "? [O1] [O2] [O3]"
       ^              ^
       0             15

O1, O2, O3: option 1, option 2 and option 3
```