#ifndef BSP_H_
#define BSP_H_

#include <stdint.h>
#include <avr/io.h>

/*
------------ PINS ------------
  name   | port/pin |  func
------------------------------
  D2     |   PD2    |  BTN
  D3     |   PD3    |  LCD D6
  D4     |   PD4    |  LCD D5
  D5     |   PD5    |  LCD D4
  D6     |   PD6    |  LCD D7
  D7     |   PD7    |  
  D8     |   PB0    |  LCD EN
  D9     |   PB1    |  LCD RS
  D10    |   PB2    |  MAX6675 CS
  D11    |   PB3    |  MOSI
  D12    |   PB4    |  MISO
  D13    |   PB5    |  SCK
  A0     |   PC0    |  SSR
  A1     |   PC1    |  FAN
  A2     |   PC2    |  debug

*/




/* LCD Display */
#define LCD_RS PB1
#define LCD_EN PB0
#define LCD_D4 PD5
#define LCD_D5 PD4
#define LCD_D6 PD3
#define LCD_D7 PD6
void configure_LCD_pins();
void LCD_set_rs(uint8_t mode);
void LCD_set_data_pins(uint8_t data);
void LCD_set_enable(uint8_t mode);

/* Debug */
#define DEBUG_PIN PC2
void configure_debug_pin();
void toggle_debug_pin();

/* Temperature sensor chip select pins */
#define CS_1 PB2
void configure_sensor_pins();
void max6675_set_cs1(uint8_t state);


/* SSR */
#define SSR_pin PC0
void set_heater(uint8_t state);
void configure_SSR_pin();

/* Button */
#define BTN_PIN PD2
void configure_btn_pin(); 
uint8_t read_btn_pin();

/* Fan */
#define FAN_PIN PC1
void configure_fan_pin();
void fan_ON();
void fan_OFF();

#endif /* BSP_H_ */