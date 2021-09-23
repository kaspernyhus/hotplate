#include "DSP.h"


/* Filter class functions */
void filter_init(filter * const self, int16_t initial_value, uint16_t alpha) {
  self->EMA = initial_value;
  self->alpha = alpha;
}

int16_t apply_filter(filter * const self, int16_t new_value) {
  self->EMA = calc_ema(new_value, self->EMA, self->alpha);
  return self->EMA;
}




int16_t calc_ema(int16_t new_value, int16_t old_value, uint16_t alpha) {
  int64_t temp0 = 0;
  temp0 = (int64_t)new_value * (alpha) + (int64_t)old_value * (65536 - alpha);
  return ((temp0+32768)/65536);
}


int16_t calc_velocity(cir_buf_t* Buffer, int8_t offset) {
  uint16_t previous[5]; // [ (x-4h); (x-3h); (x-2h); (x-h); x ] //  x = sample; h = sampling time
  int16_t temp_velocity;
  cir_buf_get_latest(Buffer, previous, 5);
  /* f'(x) = -(-(x-4h)+8(x-3h)-8(x-h)+x) */
  temp_velocity = -(-previous[0]+8*previous[1]-8*previous[3]+previous[4]); 
  return temp_velocity+offset;
}