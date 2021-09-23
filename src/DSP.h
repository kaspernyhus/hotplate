#ifndef DSP_H_
#define DSP_H_
#include <stdio.h>
#include "CircularBuffer.h"

#define EMA_ALPHA(x) ( (uint16_t)(x * 65535) )


int16_t calc_ema(int16_t new_value, int16_t old_value, uint16_t alpha);
int16_t calc_velocity(cir_buf_t* Buffer, int8_t offset);


/* Filter "class" */
typedef struct {
  int16_t EMA;
  uint16_t alpha;
} filter;

void filter_init(filter * const self, int16_t initial_value, uint16_t alpha);
int16_t apply_filter(filter * const self, int16_t new_value);



#endif /* DSP_H_ */