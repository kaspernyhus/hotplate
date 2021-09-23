#include "MAX6675.h"

#define LOW 0x00
#define HIGH 0x01


void MAX6675_init(MAX6675 * const self, void (*set_cs)(uint8_t state)) {
  self->set_cs_pin = set_cs;
}

uint16_t MAX6675_read(MAX6675 * const self) {
  uint16_t temp_reading=0;

  /* Initiate new conversion in MAX6675 */
  self->set_cs_pin(LOW);
  _delay_ms(3);
  self->set_cs_pin(HIGH);
  _delay_ms(10);

  /* Transfer data to MCU */
  self->set_cs_pin(LOW);
  temp_reading = SPI_masterReceive()<<8;
  temp_reading |= SPI_masterReceive()&0xFF;
  self->set_cs_pin(HIGH);

  /* Termocouple input disconnected */
  if(temp_reading&0x04) {
    return 0x0000;
  }

  return (temp_reading>>3)&0x0FFF; // return 12-bit
}