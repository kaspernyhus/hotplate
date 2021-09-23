#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdio.h>
#include <stdint.h>
#include "LCD.h"
#include "SystemStates.h"

#define DEF_X 1
#define DEF_Y 6 


void display_init(LCD * const self);
void display_clear(LCD * const self);
void display_update_current_temp(LCD * const self, uint16_t temp);
void display_update(LCD * const self, uint8_t state, uint16_t elapsed, int16_t timer);
void display_results(LCD * const self, uint16_t elapsed, uint16_t max_temp, uint16_t time_above_tl, uint16_t time_at_peak);
void display_error(LCD * const self, uint8_t state, uint8_t errorState, uint16_t temp);


/* OLED */
char oled_buffer[20];



#endif /* DISPLAY_H_ */



