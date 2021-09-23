/*
    
    Kasper Nyhus Kaae

---------------------------------
Timer module for
ARDUINO NANO
---------------------------------

*/


#include <avr/io.h>
#include "timer.h"



// ------------------- TIMER0 ------------------- //

void timer0_8bit_Init(uint8_t prescaler, uint8_t mode, uint8_t OCRA_compareValue) {
  TIMSK0 |= (1<<TOIE0); // Timer/Counter0 Overflow Interrupt Enable
  switch(prescaler) {
    case 0:                                       // No prescaller
      TCCR0B &=~ (1<<CS02)|(1<<CS01)|(1<<CS00);
      TCCR0B |= (1<<CS00);
      break;
    case 1:                                       // 8
      TCCR0B &=~ (1<<CS02)|(1<<CS01)|(1<<CS00);
      TCCR0B |= (1<<CS01);
      break;
    case 2:                                       // 64
      TCCR0B &=~ (1<<CS02)|(1<<CS01)|(1<<CS00);
      TCCR0B |= (1<<CS01)|(1<<CS00);
      break;
    case 3:                                       // 256
      TCCR0B &=~ (1<<CS02)|(1<<CS01)|(1<<CS00);
      TCCR0B |= (1<<CS02);
      break;
    case 4:                                       // 1024
      TCCR0B &=~ (1<<CS02)|(1<<CS01)|(1<<CS00);
      TCCR0B |= (1<<CS02)|(1<<CS00);
      break;
    case 5:                                       // External clk cource falling
      TCCR0B &=~ (1<<CS02)|(1<<CS01)|(1<<CS00);
      TCCR0B |= (1<<CS02)|(1<<CS01);
      break;
    case 6:                                       // External clk cource rising
      TCCR0B |= (1<<CS02)|(1<<CS01)|(1<<CS00);
      break;
    default:
      break;
  }
  switch(mode) {
    case 0:                                       // Normal mode
      TCCR0B &=~ (1<<WGM02);
      TCCR0A &=~ (1<<WGM01)|(1<<WGM00);
      break;
    case 1:                                       // PWM, phase correct, TOP = 0xFF
      TCCR0B &=~ (1<<WGM02);
      TCCR0A &=~ (1<<WGM01)|(1<<WGM00);
      TCCR0A |= (1<<WGM00);
      break;
    case 2:                                       // CTC, TOP = OCRA
      TCCR0B &=~ (1<<WGM02);
      TCCR0A &=~ (1<<WGM01)|(1<<WGM00);
      TCCR0A |= (1<<WGM01);
      OCR0A = OCRA_compareValue;
      break;
    case 3:                                       // Fast PWM, TOP = 0xFF
      TCCR0B &=~ (1<<WGM02);
      TCCR0A &=~ (1<<WGM01)|(1<<WGM00);
      TCCR0A |= (1<<WGM01)|(1<<WGM00);
      break;
    case 4:                                       // PWM, phase correct, TOP = OCRA
      TCCR0B &=~ (1<<WGM02);
      TCCR0A &=~ (1<<WGM01)|(1<<WGM00);
      TCCR0B |= (1<<WGM02);
      OCR0A = OCRA_compareValue;
      break;
    case 5:                                       // Fast PWM, TOP = OCRA
      TCCR0B |= (1<<WGM02);
      TCCR0A |= (1<<WGM01)|(1<<WGM00);
      OCR0A = OCRA_compareValue;
      break;
    default:
      break;
  }
}

void timer0_pin_output_mode(uint8_t mode) {
  switch(mode) {
  case 0:
    TCCR0A &=~(COM0A1)|(COM0A0);
    break;
  case 1:
    TCCR0A &=~(COM0B1)|(COM0B0);
    break;
  case 2:
    TCCR0A &=~(COM0A1)|(COM0A0);
    TCCR0A |= (COM0A0);
    break;
  case 3:
    TCCR0A &=~(COM0A1)|(COM0A0);
    TCCR0A |= (COM0A1);
    break;
  default:
    break;
  }
}


void timer0_update_OCRA(uint8_t compareValue) {
  OCR0A = compareValue;
}


void timer0_update_OCRB(uint8_t compareValue) {
  OCR0B = compareValue;
}


void timer0_itr_config(uint8_t itr_on, uint8_t type) {
  switch(type) {
  case 0:                         // Overflow
    if(itr_on) TIMSK0 |= (1<<TOIE0); // Enable
    else TIMSK0 &=~ (1<<TOIE0);   // Disable
    break;
  case 1:                         // Compare Match A
    if(itr_on) TIMSK0 |= (1<<OCIE0A);
    else TIMSK0 &=~ (1<<OCIE0A);
    break;
  case 2:                         // Compare Match B
    if(itr_on) TIMSK0 |= (1<<OCIE0B);
    else TIMSK0 &=~ (1<<OCIE0B);
    break;
  default:
    break;
  }
}



// ------------------- TIMER1 ------------------- //

void timer1_16bit_Init(uint8_t prescaler, uint8_t mode, uint16_t OCRA_compareValue) {
  TIMSK0 |= (1<<TOIE0); // Timer/Counter0 Overflow Interrupt Enable
  TCNT1 = 0;            //force counter reset
  switch(prescaler) {
    case 0:                                       // No prescaller
      TCCR1B &=~ (1<<CS12)|(1<<CS11)|(1<<CS10);
      TCCR1B |= (1<<CS10);
      break;
    case 1:                                       // 8
      TCCR1B &=~ (1<<CS12)|(1<<CS11)|(1<<CS10);
      TCCR1B |= (1<<CS11);
      break;
    case 2:                                       // 64
      TCCR1B &=~ (1<<CS12)|(1<<CS11)|(1<<CS10);
      TCCR1B |= (1<<CS11)|(1<<CS10);
      break;
    case 3:                                       // 256
      TCCR1B &=~ (1<<CS12)|(1<<CS11)|(1<<CS10);
      TCCR1B |= (1<<CS12);
      break;
    case 4:                                       // 1024
      TCCR1B &=~ (1<<CS12)|(1<<CS11)|(1<<CS10);
      TCCR1B |= (1<<CS12)|(1<<CS10);
      break;
    case 5:                                       // External clk cource falling
      TCCR1B &=~ (1<<CS12)|(1<<CS11)|(1<<CS10);
      TCCR1B |= (1<<CS12)|(1<<CS11);
      break;
    case 6:                                       // External clk cource rising
      TCCR1B |= (1<<CS12)|(1<<CS11)|(1<<CS10);
      break;
    default:
      break;
  }
  switch(mode) {
    case 0:                                       // Normal mode
      TCCR1B &=~ (1<<WGM13)|(1<<WGM12);
      TCCR1A &=~ (1<<WGM11)|(1<<WGM10);
      break;
    case 1:                                       // PWM, phase correct 10-bit, TOP = 0x3FF
      TCCR1B &=~ (1<<WGM13)|(1<<WGM12);
      TCCR1A |= (1<<WGM11)|(1<<WGM10);
      break;
    case 2:                                       // CTC, TOP = OCRA
      TCCR1B &=~ (1<<WGM13);
      TCCR1A &=~ (1<<WGM11)|(1<<WGM10);
      TCCR1B |= (1<<WGM12);
      OCR1A = OCRA_compareValue;
      break;
    case 3:                                       // Fast PWM 10-bit, TOP = 0x3FF
      TCCR1B &=~ (1<<WGM13);
      TCCR1B |= (1<<WGM12);
      TCCR1A |= (1<<WGM11)|(1<<WGM10);
      break;
    case 4:                                       // PWM, phase correct, TOP = OCRA
      TCCR1B &=~ (1<<WGM12);
      TCCR1B |= (1<<WGM13);
      TCCR1A |= (1<<WGM11)|(1<<WGM10);
      OCR1A = OCRA_compareValue;
      break;
    case 5:                                       // Fast PWM, TOP = OCRA
      TCCR1B |= (1<<WGM13)|(1<<WGM12);
      TCCR1A |= (1<<WGM11)|(1<<WGM10);
      OCR1A = OCRA_compareValue;
      break;
    case 6:                                       // Fast PWM 10-bit, TOP = 0x3FF
      TCCR1B &=~ (1<<WGM13)|(1<<WGM11);
      TCCR1B |= (1<<WGM12);
      TCCR1A |= (1<<WGM10);
      break;
    case 7:                                       // PWM, phase correct, 8-bit, TOP = 0x00FF
      TCCR1B &=~ (1<<WGM13)|(1<<WGM12);
      TCCR1A &=~ (1<<WGM11);
      TCCR1A |= (1<<WGM10);
      OCR1A = OCRA_compareValue;
      break;
    default:
      break;
  }
}


void timer1_update_OCRA(uint16_t compareValue) {
  OCR1A = compareValue;
}


void timer1_update_OCRB(uint16_t compareValue) {
  OCR1B = compareValue;
}


void timer1_itr_config(uint8_t itr_on, uint8_t type) {
  switch(type) {
  case 0:                           // Overflow
    if(itr_on) TIMSK1 |= (1<<TOIE1);  // Enable
    else TIMSK1 &=~ (1<<TOIE1);     // Disable
    break;
  case 1:                           // Compare Match A
    if(itr_on) TIMSK1 |= (1<<OCIE1A);
    else TIMSK1 &=~ (1<<OCIE1A);
    break;
  case 2:                           // Compare Match B
    if(itr_on) TIMSK1 |= (1<<OCIE1B);
    else TIMSK1 &=~ (1<<OCIE1B);
    break;
  default:
    break;
  }
}

void timer1_pin_output_mode(uint8_t mode) {
  switch(mode) {
  case 0:
    TCCR1A &=~(COM1A1)|(COM1A0);
    break;
  case 1:
    TCCR1A &=~(COM1B1)|(COM1B0);
    break;
  case 2:
    TCCR1A &=~(COM1A1)|(COM1A0);
    TCCR1A |= (COM1A0);
    break;
  case 3:
    TCCR1A &=~(COM1A0);
    TCCR1A |= (1<<COM1A1);
    break;
  default:
    break;
  }
}

