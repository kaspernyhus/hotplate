#ifndef MAX6675_H
#define MAX6675_H

#include <stdint.h>
#include <util/delay.h>
#include "SPI.h"
#include "UART.h"


/* MAX6675 temperature sensor */
typedef struct {
  void (*set_cs_pin)(uint8_t state);
} MAX6675;

void MAX6675_init(MAX6675 * const self, void (*set_cs)(uint8_t state));
uint16_t MAX6675_read();


#endif