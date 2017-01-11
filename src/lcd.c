#include "global_state.h"
#include "lcd.h"
#include "stm32l4xx_ll_gpio.h"
#include "string.h"

extern CTRL_status_t global_status;

static int LCD_FONT_SPACE[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static int LCD_FONT_PC[] = {0x00, 0x1f, 0x11, 0x11, 0x1f, 0x04, 0x0e, 0x00};
static int LCD_FONT_PHONE[] = {0x1f, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1f, 0x1b};
static int LCD_FONT_MOUSE_L[] = {0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00};
static int LCD_FONT_KEYBOARD_L[] = {0x00, 0x00, 0x1f, 0x1a,
                                    0x1f, 0x15, 0x1f, 0x00};
static int LCD_FONT_MOUSE_R[] = {0x18, 0x04, 0x0e, 0x15,
                                 0x15, 0x11, 0x11, 0x0e};
static int LCD_FONT_KEYBOARD_R[] = {0x03, 0x04, 0x1f, 0x15,
                                    0x1f, 0x0b, 0x1f, 0x00};
static int LCD_FONT_IR[] = {0x1c, 0x08, 0x08, 0x1c, 0x07, 0x05, 0x06, 0x05};
static int LCD_FONT_CTRL[] = {0x1f, 0x19, 0x17, 0x17, 0x17, 0x17, 0x19, 0x1f};
static int LCD_FONT_ALT[] = {0x1f, 0x1b, 0x11, 0x15, 0x11, 0x15, 0x15, 0x1f};
static int LCD_FONT_SHIFT[] = {0x1f, 0x19, 0x17, 0x17, 0x19, 0x1d, 0x13, 0x1f};
static int LCD_FONT_WIN[] = {0x00, 0x00, 0x1b, 0x1b, 0x00, 0x1b, 0x1b, 0x00};

#define SetPin LL_GPIO_SetOutputPin
#define ResetPin LL_GPIO_ResetOutputPin
#define IsSetPin LL_GPIO_IsInputPinSet

#define LCD_RS GPIOB, LL_GPIO_PIN_3
#define LCD_RW GPIOB, LL_GPIO_PIN_5
#define LCD_E GPIOB, LL_GPIO_PIN_4
#define LCD_DB0 GPIOB, LL_GPIO_PIN_10
#define LCD_DB1 GPIOA, LL_GPIO_PIN_8
#define LCD_DB2 GPIOA, LL_GPIO_PIN_9
#define LCD_DB3 GPIOC, LL_GPIO_PIN_7
#define LCD_DB4 GPIOB, LL_GPIO_PIN_6
#define LCD_DB5 GPIOA, LL_GPIO_PIN_7
#define LCD_DB6 GPIOA, LL_GPIO_PIN_6
#define LCD_DB7 GPIOA, LL_GPIO_PIN_5

#define LCD_CUSTOME_BASE 0x08
#define LCD_CUSTOME_DeviceIcon (LCD_CUSTOME_BASE + 0)
#define LCD_CUSTOME_MouseKybd_L (LCD_CUSTOME_BASE + 1)
#define LCD_CUSTOME_MouseKybd_R (LCD_CUSTOME_BASE + 2)
#define LCD_CUSTOME_IR (LCD_CUSTOME_BASE + 3)
#define LCD_CUSTOME_Ctrl (LCD_CUSTOME_BASE + 4)
#define LCD_CUSTOME_Alt (LCD_CUSTOME_BASE + 5)
#define LCD_CUSTOME_Shift (LCD_CUSTOME_BASE + 6)
#define LCD_CUSTOME_Win (LCD_CUSTOME_BASE + 7)

void delay() {
  int i = 4000;
  while (i--)
    ;
}

void GPIO_init_pin(GPIO_TypeDef *GPIOx, uint32_t PinMask) {
  LL_GPIO_SetPinMode(GPIOx, PinMask, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinSpeed(GPIOx, PinMask, LL_GPIO_SPEED_HIGH);
  LL_GPIO_SetPinOutputType(GPIOx, PinMask, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(GPIOx, PinMask, LL_GPIO_PULL_DOWN);
  ResetPin(GPIOx, PinMask);
}

void LCD_send() {
  SetPin(LCD_E);
  while (!IsSetPin(LCD_E))
    ;
  delay();
  ResetPin(LCD_E);
  while (IsSetPin(LCD_E))
    ;
}
void LCD_port_set_read() {
  LL_GPIO_SetPinMode(LCD_DB0, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinMode(LCD_DB1, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinMode(LCD_DB2, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinMode(LCD_DB3, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinMode(LCD_DB4, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinMode(LCD_DB5, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinMode(LCD_DB6, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinMode(LCD_DB7, LL_GPIO_MODE_INPUT);
}
void LCD_port_set_write() {
  LL_GPIO_SetPinMode(LCD_DB0, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(LCD_DB1, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(LCD_DB2, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(LCD_DB3, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(LCD_DB4, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(LCD_DB5, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(LCD_DB6, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(LCD_DB7, LL_GPIO_MODE_OUTPUT);
}

int LCD_read(int cmd) {
  if (cmd)
    ResetPin(LCD_RS);
  else
    SetPin(LCD_RS);
  SetPin(LCD_RW);
  SetPin(LCD_E);
  while (!IsSetPin(LCD_E))
    ;
  delay();
  int data = 0;
  data = data << 1 | (IsSetPin(LCD_DB7) ? 1 : 0);
  data = data << 1 | (IsSetPin(LCD_DB6) ? 1 : 0);
  data = data << 1 | (IsSetPin(LCD_DB5) ? 1 : 0);
  data = data << 1 | (IsSetPin(LCD_DB4) ? 1 : 0);
  data = data << 1 | (IsSetPin(LCD_DB3) ? 1 : 0);
  data = data << 1 | (IsSetPin(LCD_DB2) ? 1 : 0);
  data = data << 1 | (IsSetPin(LCD_DB1) ? 1 : 0);
  data = data << 1 | (IsSetPin(LCD_DB0) ? 1 : 0);
  ResetPin(LCD_E);
  while (IsSetPin(LCD_E))
    ;
  return data;
}

int WriteToLCD(int input, int Cmd) {
  // write command to LCD or let LCD display character fetched from memory
  LCD_port_set_read();
  while (LCD_read(1) & 0x80)
    ;
  LCD_port_set_write();
  if (Cmd)
    ResetPin(LCD_RS);
  else
    SetPin(LCD_RS);
  ResetPin(LCD_RW);

  input & 1 ? SetPin(LCD_DB0) : ResetPin(LCD_DB0);
  input >>= 1;
  input & 1 ? SetPin(LCD_DB1) : ResetPin(LCD_DB1);
  input >>= 1;
  input & 1 ? SetPin(LCD_DB2) : ResetPin(LCD_DB2);
  input >>= 1;
  input & 1 ? SetPin(LCD_DB3) : ResetPin(LCD_DB3);
  input >>= 1;
  input & 1 ? SetPin(LCD_DB4) : ResetPin(LCD_DB4);
  input >>= 1;
  input & 1 ? SetPin(LCD_DB5) : ResetPin(LCD_DB5);
  input >>= 1;
  input & 1 ? SetPin(LCD_DB6) : ResetPin(LCD_DB6);
  input >>= 1;
  input & 1 ? SetPin(LCD_DB7) : ResetPin(LCD_DB7);
  input >>= 1;
  LCD_send();
  return 0;
}

void LCD_cursor_set(int pos) { WriteToLCD(0x80 | pos, 1); }
void LCD_clear() { WriteToLCD(0x01, 1); }
void init_display() {
  // LCD register
  WriteToLCD(56, 1);  // Function Setting
  WriteToLCD(6, 1);   // Entering mode
  WriteToLCD(12, 1);  // Display on
  WriteToLCD(1, 1);   // Clear Screen
  WriteToLCD(2, 1);   // Move to top left
}

/* This function aims to create a font in CGR
 * location(input): CGR address of the font
 * fontArray(input): Content of the font, 8 elements indicate 8 rows
 */
void CreateFont(int location, int *fontArray) {
  for (int i = 0; i < 8; i++) {
    WriteToLCD(0x40 | (location << 3) | i, 1);
    WriteToLCD(fontArray[i], 0);
  }
}

int LCD_char_map(int ascii) {
  if (ascii - LCD_CUSTOME_BASE < 8) return ascii;
  return ascii >= ' ' && ascii <= '}' ? ascii - ' ' + 0x20 : 0x20;
}

/* Let LCD show string
 * Noted the length of the string will not exceed the number of columns on our
 * LCD
 */
void WriteStrToLCD(char *str) {
  WriteToLCD(0x06, 1);  // enter mode
  for (int i = 0; i < 16; i++) {
    WriteToLCD(LCD_char_map(*str++), 0);  // write character
  }
}

void init_Font() {
  CreateFont(LCD_CUSTOME_DeviceIcon, LCD_FONT_SPACE);
  CreateFont(LCD_CUSTOME_MouseKybd_L, LCD_FONT_SPACE);
  CreateFont(LCD_CUSTOME_MouseKybd_R, LCD_FONT_SPACE);
  CreateFont(LCD_CUSTOME_IR, LCD_FONT_IR);
  CreateFont(LCD_CUSTOME_Ctrl, LCD_FONT_CTRL);
  CreateFont(LCD_CUSTOME_Alt, LCD_FONT_ALT);
  CreateFont(LCD_CUSTOME_Shift, LCD_FONT_SHIFT);
  CreateFont(LCD_CUSTOME_Win, LCD_FONT_WIN);
}

void initTest() {
  char *msg1 = "Welcome to use!     ";
  char *msg2 = "                    ";
  LCD_cursor_set(0);
  WriteStrToLCD(msg1);
  LCD_cursor_set(0x40);
  WriteStrToLCD(msg2);
}

void LCD_init() {
  // Setup GPIO
  RCC->AHB2ENR |=
      RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;
  GPIO_init_pin(LCD_RS);
  GPIO_init_pin(LCD_RW);
  GPIO_init_pin(LCD_E);
  GPIO_init_pin(LCD_DB0);
  GPIO_init_pin(LCD_DB1);
  GPIO_init_pin(LCD_DB2);
  GPIO_init_pin(LCD_DB3);
  GPIO_init_pin(LCD_DB4);
  GPIO_init_pin(LCD_DB5);
  GPIO_init_pin(LCD_DB6);
  GPIO_init_pin(LCD_DB7);
  // Reset display
  init_display();
  // load font
  init_Font();

  initTest();
}

void LCD_updateFont() {
  static CTRL_deviceType_t currentDeviceType = 0;
  static CTRL_usbType_t currentUsbType = 0;

  if (currentDeviceType != global_status.deviceType) {
    currentDeviceType = global_status.deviceType;
    switch (global_status.deviceType) {
      case CTRL_DEVICETYPE_NC:
        CreateFont(LCD_CUSTOME_DeviceIcon, LCD_FONT_SPACE);
        break;
      case CTRL_DEVICETYPE_PC:
        CreateFont(LCD_CUSTOME_DeviceIcon, LCD_FONT_PC);
        break;
      case CTRL_DEVICETYPE_PHONE:
        CreateFont(LCD_CUSTOME_DeviceIcon, LCD_FONT_PHONE);
        break;
    }
  }

  if (currentUsbType != global_status.usbDeviceType) {
    currentUsbType = global_status.usbDeviceType;
    switch (global_status.usbDeviceType) {
      case CTRL_USB_NC:
        CreateFont(LCD_CUSTOME_MouseKybd_L, LCD_FONT_SPACE);
        CreateFont(LCD_CUSTOME_MouseKybd_R, LCD_FONT_SPACE);
        break;
      case CTRL_USB_MOUSE:
        CreateFont(LCD_CUSTOME_MouseKybd_L, LCD_FONT_MOUSE_L);
        CreateFont(LCD_CUSTOME_MouseKybd_R, LCD_FONT_MOUSE_R);
        break;
      case CTRL_USB_KEYBOARD:
        CreateFont(LCD_CUSTOME_MouseKybd_L, LCD_FONT_KEYBOARD_L);
        CreateFont(LCD_CUSTOME_MouseKybd_R, LCD_FONT_KEYBOARD_R);
        break;
    }
  }
}

void LCD_updateDisplay() {
  static char buffer[32];
  memset(buffer, 0, sizeof(buffer));
  // draw first row
  if (global_status.deviceType == CTRL_DEVICETYPE_NC) {
    strcpy(buffer, "PLEASE CONNECT  ");
  } else if (global_status.showMessage) {
    strcpy(buffer, global_status.message);
  } else if (global_status.showOption) {
    strcpy(buffer, global_status.optionPrompt);
  } else {
    // Device type
    buffer[0] = LCD_CUSTOME_DeviceIcon;
    // Device name
    strcpy(buffer + 1, global_status.name);
    buffer[12] = ' ';
    buffer[13] = LCD_CUSTOME_MouseKybd_L;
    buffer[14] = LCD_CUSTOME_MouseKybd_R;
    buffer[15] = LCD_CUSTOME_IR;
  }
  LCD_cursor_set(0);
  WriteStrToLCD(buffer);

  // draw second row
  if (global_status.deviceType == CTRL_DEVICETYPE_NC) {
    strcpy(buffer, " TO SOME DEVICE ");
  } else if (global_status.showOption) {
    switch (global_status.optionType) {
      case CTRL_OPTION_OK:
        strcpy(buffer, "?    [ OK ]     ");
        break;
      case CTRL_OPTION_YES_NO:
        strcpy(buffer, "? [ YES ] [ NO ]");
        break;
      case CTRL_OPTION_SURE_CANC:
        strcpy(buffer, "? [SURE][CANCEL]");
        break;
    }
  } else {
    memset(buffer, 0, sizeof(buffer));

    char *pos = buffer;
    if (global_status.shiftKey) *pos++ = LCD_CUSTOME_Shift;
    if (global_status.ctrlKey) *pos++ = LCD_CUSTOME_Ctrl;
    if (global_status.altKey) *pos++ = LCD_CUSTOME_Alt;
    if (global_status.winKey) *pos++ = LCD_CUSTOME_Win;
    if (global_status.key.keyvalue > 0) strcpy(pos, global_status.key.keyshow);

    buffer[10] = ' ';
    buffer[11] = '0' + global_status.min / 10;
    buffer[12] = '0' + global_status.min % 10;
    buffer[13] = global_status.dot ? ':' : ' ';
    buffer[14] = '0' + global_status.sec / 10;
    buffer[15] = '0' + global_status.sec % 10;
  }
  LCD_cursor_set(0x40);
  WriteStrToLCD(buffer);
}
