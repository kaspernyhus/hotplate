#include "bsp.h"


/* LCD */
void configure_LCD_pins() {
  DDRB |= (1<<LCD_RS) | (1<<LCD_EN);
  PORTB |= (1<<LCD_RS) | (1<<LCD_EN);
  DDRD |= (1<<LCD_D4) | (1<<LCD_D5) | (1<<LCD_D6) | (1<<LCD_D7);
  PORTD &=~ (1<<LCD_D4) | (1<<LCD_D5) | (1<<LCD_D6) | (1<<LCD_D7);
}

void LCD_set_rs(uint8_t mode) {
  if(mode) PORTB |= (1<<LCD_RS);
  else  PORTB &=~ (1<<LCD_RS);
}

void LCD_set_data_pins(uint8_t data) {
  if(data&0x01) PORTD |= (1<<LCD_D4);
  else PORTD &=~ (1<<LCD_D4);
  if(data&0x02) PORTD |= (1<<LCD_D5);
  else PORTD &=~ (1<<LCD_D5);
  if(data&0x04) PORTD |= (1<<LCD_D6);
  else PORTD &=~ (1<<LCD_D6);
  if(data&0x08) PORTD |= (1<<LCD_D7);
  else PORTD &=~ (1<<LCD_D7);
}


void LCD_set_enable(uint8_t mode) {
  if(mode) PORTB |= (1<<LCD_EN);
  else  PORTB &=~ (1<<LCD_EN);
}

/* Temperature sensors */
void configure_sensor_pins() {
  DDRB  |= (1<<CS_1);  // CS pin OUTPUT
  PORTB |= (1<<CS_1);  // pull CS pins high
}
void max6675_set_cs1(uint8_t state) {
  if(state) PORTB |= (1<<CS_1);
  else PORTB &=~(1<<CS_1);
}


/* SSR */
void configure_SSR_pin() {
  DDRC |= (1<<SSR_pin);
  set_heater(0);
}
void set_heater(uint8_t state) {
  if(state) PORTC |= (1<<SSR_pin); // Turn on heater
  else PORTC &=~(1<<SSR_pin); // Turn off heater
}


/* Button */
void configure_btn_pin() {
  DDRD &=~(1<<BTN_PIN);
  PORTD |=(1<<BTN_PIN); // INPUT PULLUP
}
uint8_t read_btn_pin() {
  return !(PIND&(1<<BTN_PIN));
}


/* Debug */
void configure_debug_pin() {
  DDRC |= (1<<DEBUG_PIN);
  PORTC |=(1<<DEBUG_PIN);
}
void toggle_debug_pin() {
  PORTC ^= (1<<DEBUG_PIN);
}


/* Fan */
void configure_fan_pin() {
  DDRC |= (1<<FAN_PIN);
  PORTC &=~(1<<FAN_PIN);
}
void fan_ON() {
  PORTC |= (1<<FAN_PIN);
}
void fan_OFF() {
  PORTC &=~(1<<FAN_PIN);
}