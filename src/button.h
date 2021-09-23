#ifndef BTN_H_
#define BTN_H_


#include <stdint.h>


typedef struct {
  volatile uint8_t push_detected;
  volatile uint16_t debounce_ticks;
  uint8_t (*read_btn_pin)();
  void (*on_push_action)();
  uint8_t state;
  uint8_t max_states;
} Button;


void btn_init(Button * const self, void (*on_push_action)());
void btn_attatch_read(Button * const self, uint8_t (*read_btn_pin)());
void btn_tick(Button * const self);
void btn_set_debounce_ticks(Button * const self, uint16_t debounce_ticks);
void btn_push_detected(Button * const self);
void btn_set_max_states(Button * const self, uint8_t states_available);
uint8_t btn_state(Button * const self);


#endif /* BTN_H_ */