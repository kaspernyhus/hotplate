#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include <stddef.h>
#include <util/delay.h>

/*
 * LCD RS (Register Select) // 0: Instruction // 1: Data
 * LCD Enable // Starts data read/write
 * LCD R/W (Read/~Write) // ground
 
 *  Four high order bidirectional tristate data bus
    pins. Used for data transfer and receive between
    the MPU and the HD44780U. DB7 can be used
    as a busy flag

 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 
 
 * LCD VSS pin to ground
 * LCD VCC pin to 5V


Display data RAM (DDRAM) stores display data represented in 8-bit character codes

*/


typedef struct {
  void (*set_rs)(uint8_t mode);
  void (*set_data_pins)(uint8_t data);
  void (*set_enable)(uint8_t mode);
  
  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;
  
  uint8_t _numlines;
  uint8_t _row_offsets[4];
} LCD;

void LCD_init(  LCD * const self, 
                void (*set_rs)(uint8_t mode), 
                void (*set_data_pins)(uint8_t data), 
                void (*set_enable)(uint8_t mode),
                uint8_t cols, 
                uint8_t lines
              );
void noBlink(LCD * const self);
void blink(LCD * const self);
void noCursor(LCD * const self);
void cursor(LCD * const self);
void noDisplay(LCD * const self);
void display(LCD * const self);
void setCursor(LCD * const self, uint8_t col, uint8_t row);
void home(LCD * const self);
void LCD_clear(LCD * const self);
void setRowOffsets(LCD * const self, int row0, int row1, int row2, int row3);
void LCD_print(LCD * const self, const char str[]);
void LCD_sendStrXY(LCD * const self, char *string, int X, int Y);
void createChar(LCD * const self, uint8_t location, uint8_t charmap[]);
void _write(LCD * const self, uint8_t value);

#define LOW 0
#define HIGH 1

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00



#endif /* LCD_H_ */