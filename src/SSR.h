#ifndef SSR_H_
#define SSR_H_

#include <stdint.h>

#define OFF 0
#define ON 1
#define MAX_SSR_ON_TICKS 2


typedef struct {
  uint8_t state;
  uint8_t limit_active;
  uint16_t on_time_limit;
  uint16_t required_off_time;
  uint16_t on_time;
  uint16_t off_time;
  void (*set_SSR)(uint8_t state);
} SSR_t;


void SSR_init(SSR_t * const self, void (*set_state)(uint8_t state));
uint8_t SSR_set(SSR_t * const self, uint8_t state_request);

void SSR_set_limit(SSR_t * const self, uint8_t limit_state);
void SSR_set_on_time_limit(SSR_t * const self, uint16_t limit);
void SSR_set_off_time_required(SSR_t * const self, uint16_t off_time_required);

uint8_t SSR_get_state(SSR_t * const self);
uint16_t SSR_get_on_time(SSR_t * const self);

#endif /* SSR_H_ */