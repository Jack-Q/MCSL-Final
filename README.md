## Communication Protocol

### Device --> Board

* Name
* Type: PC / Mobile
  
* Connectivity

### Board --> Device

* action
  0xaa [Shift|Alt|Ctrl|Win] KeyCode[Python]  
         0x01 Shift           0x00   No key
         0x02 Alt             other  Key Code defined in PyKeyboard
         0x04 Ctrl
         0x08 Win

## Display

Font Mapping:
```
  [1] Device Icon            => LCD_FONT_PC LCD_FONT_PHONE
  [2] Mouse/Keyboard (left)  => LCD_FONT_MOUSE_L LCD_FONT_KEYBOARD_L
  [3] Mouse/Keyboard (right) => LCD_FONT_MOUSE_R LCD_FONT_KEYBOARD_R
  [4] IR                     => LCD_FONT_IR
  [5] Ctrl                   => LCD_FONT_CTRL
  [6] Alt                    => LCD_FONT_ALT
  [7] Shift                  => LCD_FONT_SHIFT
  [8] Win                    => LCD_FONT_WIN
```