#include "lcd.h"
#include "stm32l4xx_ll_gpio.h"

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

void delay(){
	int i = 4000;
	while(i--);
}

void GPIO_init_pin(GPIO_TypeDef *GPIOx, uint32_t PinMask) {
  LL_GPIO_SetPinMode(GPIOx, PinMask, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinSpeed(GPIOx, PinMask, LL_GPIO_SPEED_HIGH);
  LL_GPIO_SetPinOutputType(GPIOx, PinMask, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(GPIOx, PinMask, LL_GPIO_PULL_DOWN);
  ResetPin(GPIOx, PinMask);
}


void LCD_send() {
	SetPin(LCD_E);    while (!IsSetPin(LCD_E));
	delay();
	ResetPin(LCD_E);  while (IsSetPin(LCD_E));
}
void LCD_port_set_read(){
	LL_GPIO_SetPinMode(LCD_DB0, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(LCD_DB1, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(LCD_DB2, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(LCD_DB3, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(LCD_DB4, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(LCD_DB5, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(LCD_DB6, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(LCD_DB7, LL_GPIO_MODE_INPUT);
}
void LCD_port_set_write(){
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
	if(cmd) ResetPin(LCD_RS); else SetPin(LCD_RS);
	SetPin(LCD_RW);
	SetPin(LCD_E);    while (!IsSetPin(LCD_E));
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
	ResetPin(LCD_E);  while (IsSetPin(LCD_E));
	return data;
}

int WriteToLCD(int input, int Cmd) {
	// write command to LCD or let LCD display character fetched from memory
	LCD_port_set_read();
	while(LCD_read(1) & 0x80);
	LCD_port_set_write();
	if(Cmd) ResetPin(LCD_RS); else SetPin(LCD_RS);
	ResetPin(LCD_RW);

	input & 1 ? SetPin(LCD_DB0) : ResetPin(LCD_DB0);input >>= 1;
	input & 1 ? SetPin(LCD_DB1) : ResetPin(LCD_DB1);input >>= 1;
	input & 1 ? SetPin(LCD_DB2) : ResetPin(LCD_DB2);input >>= 1;
	input & 1 ? SetPin(LCD_DB3) : ResetPin(LCD_DB3);input >>= 1;
	input & 1 ? SetPin(LCD_DB4) : ResetPin(LCD_DB4);input >>= 1;
	input & 1 ? SetPin(LCD_DB5) : ResetPin(LCD_DB5);input >>= 1;
	input & 1 ? SetPin(LCD_DB6) : ResetPin(LCD_DB6);input >>= 1;
	input & 1 ? SetPin(LCD_DB7) : ResetPin(LCD_DB7);input >>= 1;
	LCD_send();
	return 0;
}

void LCD_cursor_set(int pos){
	WriteToLCD(0x80 | pos, 1);
}
void LCD_clear(){
	WriteToLCD(0x01,1);
}
void init_display() {
  // LCD register
  WriteToLCD(56, 1);  // Function Setting
  WriteToLCD( 6, 1);  // Entering mode
  WriteToLCD(12, 1);  // Display on
  WriteToLCD( 1, 1);  // Clear Screen
  WriteToLCD( 2, 1);  // Move to top left
}

/* This function aims to create a font in CGR
 * location(input): CGR address of the font
 * fontArray(input): Content of the font, 8 elements indicate 8 rows
 */
void CreateFont(int location, int *fontArray){
	for(int i = 0; i < 8; i++){
		WriteToLCD(0x40 | (location << 3) | i, 1);
		WriteToLCD(fontArray[i], 0);
	}
}

int LCD_char_map(int ascii){
	return ascii >= ' ' && ascii <= '}' ? ascii - ' ' + 0x20 : 0x20;
}


/* Let LCD show string
 * Noted the length of the string will not exceed the number of columns on our LCD
 */
void WriteStrToLCD(char *str){
	WriteToLCD(0x06, 1); // enter mode
	while(*str){
		WriteToLCD(LCD_char_map(*str++), 0); // write character
	}
}


void init_Font(){
	CreateFont(3, LCD_FONT_IR);
	CreateFont(4, LCD_FONT_CTRL);
	CreateFont(5, LCD_FONT_ALT);
	CreateFont(6, LCD_FONT_SHIFT);
	CreateFont(7, LCD_FONT_WIN);
}

void initTest(){
	char* msg1 = "Welcome to use!     ";
	char* msg2 = "                    ";
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

void LCD_updateDisplay(){

}
