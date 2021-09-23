#include "button.h"


/* Private */
void _inc_btn_state(Button * const self) {
  if(self->state < self->max_states) {
    self->state++;
  }
  else {
    self->state = 0;
  }
}



/* Public */
void btn_init(Button * const self, void (*on_push_action)()) {
  self->push_detected = 0;
  self->state = 0;
  self->max_states = 1;
  self->on_push_action = on_push_action;
}

void btn_attatch_read(Button * const self, uint8_t (*read_btn_pin)()) {
  self->read_btn_pin = read_btn_pin;
}

void btn_set_debounce_ticks(Button * const self, uint16_t debounce_ticks) {
  self->debounce_ticks = debounce_ticks;
}

void btn_push_detected(Button * const self) {
  self->push_detected = 1;
}

void btn_tick(Button * const self) {
  if(self->push_detected) {
    if(self->debounce_ticks == 0) {
      if(self->read_btn_pin()) {        // debounced push detected
        self->on_push_action();
        _inc_btn_state(self);
      }
      self->push_detected = 0;
    }
    else {
      self->debounce_ticks--;
    }
  }
}

uint8_t btn_state(Button * const self) {
  return self->state;
}

void btn_set_max_states(Button * const self, uint8_t states_available) {
  self->max_states = states_available;
}