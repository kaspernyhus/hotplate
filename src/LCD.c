# include "LCD.h"

/************ Private *************/

void _send(LCD * const self, uint8_t value, uint8_t mode);
void _pulseEnable(LCD * const self);
void _write4bits(LCD * const self, uint8_t data);
void _command(LCD * const self, uint8_t value);
void _write(LCD * const self, uint8_t value);



/************ Public *************/

void LCD_init(  LCD * const self, 
                void (*set_rs)(uint8_t mode), 
                void (*set_data_pins)(uint8_t data), 
                void (*set_enable)(uint8_t mode),
                uint8_t cols, 
                uint8_t lines
              ) {
  self->set_rs = set_rs;
  self->set_enable = set_enable;
  self->set_data_pins = set_data_pins;

  self->_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS; // 4-bit mode
  self->_displayfunction |= LCD_2LINE;
  self->_numlines = lines;
  
  setRowOffsets(self, 0x00, 0x40, 0x00 + cols, 0x40 + cols);
  
  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40 ms after power rises above 2.7 V
  // before sending commands. Arduino can turn on way before 4.5 V so we'll wait 50
  _delay_us(50000); 
  // Now we pull RS low to begin commands
  self->set_rs(LOW);
  self->set_enable(LOW);
  

  // this is according to the Hitachi HD44780 datasheet
  // figure 24, pg 46

  // we start in 8bit mode, try to set 4 bit mode
  _write4bits(self, 0x03);
  _delay_us(4500); // wait min 4.1ms

  // second try
  _write4bits(self, 0x03);
  _delay_us(4500); // wait min 4.1ms
  
  // third go!
  _write4bits(self, 0x03); 
  _delay_us(150);

  // finally, set to 4-bit interface
  _write4bits(self, 0x02);


  // finally, set # lines, font size, etc.
  _command(self, LCD_FUNCTIONSET | self->_displayfunction);  

  // turn the display on with no cursor or blinking default
  self->_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
  display(self);

  // clear it off
  LCD_clear(self);

  // Initialize to default text direction (for romance languages)
  self->_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  _command(self, LCD_ENTRYMODESET | self->_displaymode);
}


void setRowOffsets(LCD * const self, int row0, int row1, int row2, int row3) {
  self->_row_offsets[0] = row0;
  self->_row_offsets[1] = row1;
  self->_row_offsets[2] = row2;
  self->_row_offsets[3] = row3;
}



/********** high level commands, for the user! ***********/


void LCD_sendStrXY(LCD * const self, char *string, int X, int Y) {
  setCursor(self,Y,X);
  LCD_print(self,string);
}


void LCD_print(LCD * const self, const char str[]) {
  int i = 0;
  while(str[i]!='\0') {
    _write(self, str[i]);
    i++;
  }
}

void LCD_clear(LCD * const self) {
  _command(self, LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  _delay_us(2000); // this command takes a long time!
}

void home(LCD * const self) {
  _command(self, LCD_RETURNHOME);  // set cursor position to zero
  //delayMicroseconds(2000);  // this command takes a long time!
  _delay_us(2000);
}

void setCursor(LCD * const self, uint8_t col, uint8_t row) {
  const size_t max_lines = sizeof(self->_row_offsets) / sizeof(*self->_row_offsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/ 0
  }
  if ( row >= self->_numlines ) {
    row = self->_numlines - 1;    // we count rows starting w/ 0
  }
  _command(self, LCD_SETDDRAMADDR | (col + self->_row_offsets[row]));
}

// Turn the display on/off (quickly)
void noDisplay(LCD * const self) {
  self->_displaycontrol &= ~LCD_DISPLAYON;
  _command(self, LCD_DISPLAYCONTROL | self->_displaycontrol);
}
void display(LCD * const self) {
  self->_displaycontrol |= LCD_DISPLAYON;
  _command(self, LCD_DISPLAYCONTROL | self->_displaycontrol);
}

// Turns the underline cursor on/off
void noCursor(LCD * const self) {
  self->_displaycontrol &= ~LCD_CURSORON;
  _command(self, LCD_DISPLAYCONTROL | self->_displaycontrol);
}
void cursor(LCD * const self) {
  self->_displaycontrol |= LCD_CURSORON;
  _command(self, LCD_DISPLAYCONTROL | self->_displaycontrol);
}

// Turn on and off the blinking cursor
void noBlink(LCD * const self) {
  self->_displaycontrol &= ~LCD_BLINKON;
  _command(self, LCD_DISPLAYCONTROL | self->_displaycontrol);
}
void blink(LCD * const self) {
  self->_displaycontrol |= LCD_BLINKON;
  _command(self, LCD_DISPLAYCONTROL | self->_displaycontrol);
}

// Write to CGRAM of new characters
void createChar(LCD * const self, uint8_t location, uint8_t charmap[]) {
   location &= 0x7;            // we only have 8 locations 0-7
   
   _command(self, LCD_SETCGRAMADDR | (location << 3));
   _delay_us(30);
   
   for (int i=0; i<8; i++) 
   {
      _write(self, charmap[i]);      // call the virtual write method
      _delay_us(40);
   }
}


/*********** mid level commands, for sending data/cmds ***********/

void _command(LCD * const self, uint8_t value) {
  _send(self, value, LOW);
}

void _write(LCD * const self, uint8_t value) {
  _send(self, value, HIGH);
}



/************ low level data pushing commands **********/

void _send(LCD * const self, uint8_t value, uint8_t mode) {
  self->set_rs(mode); // command/data
  _write4bits(self, value>>4);
  _write4bits(self, value);
}


void _write4bits(LCD * const self, uint8_t value) {
  self->set_data_pins(value);
  _pulseEnable(self);
}


void _pulseEnable(LCD * const self) {
  self->set_enable(LOW);
  _delay_us(1);
  self->set_enable(HIGH);
  _delay_us(1);
  self->set_enable(LOW);
  _delay_us(100);
}