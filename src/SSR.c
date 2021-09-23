#include "SSR.h"

#define OFF 0
#define ON 1

/* Private function */
void _set_state(SSR_t * const self, uint8_t state_request) {
  self->state = state_request;
    if(self->state) {
      self->set_SSR(ON);
      self->on_time++;
    }
    else {
      self->set_SSR(OFF);
    }
}


/* Public */
void SSR_init(SSR_t * const self, void (*set_state)(uint8_t state)) {
  self->state = 0;
  self->limit_active = 0;
  self->on_time_limit = 0xFFFF;
  self->required_off_time = 0xFFFF;
  self->on_time = 0;
  self->off_time = 0;
  self->set_SSR = set_state;
}

uint8_t SSR_set(SSR_t * const self, uint8_t state_request) {
  if(self->limit_active) {                                      // limit active?
    if(self->on_time >= self->on_time_limit) {                  // ON limit reached?
      if(self->off_time >= self->required_off_time) {           // OFF time reached?
        self->on_time = 0;
        self->off_time = 0;
        _set_state(self,state_request);
      }
      else {                                                    // OFF time not reached
        _set_state(self,OFF);
        self->off_time++;
      }
    }
    else {                                                      // ON time limit nor reached
      _set_state(self,state_request);
    }
  }
  else {                                                        // limit inactive
    _set_state(self,state_request);
  }
  
  return self->state;
}


void SSR_set_limit(SSR_t * const self, uint8_t limit_state) {
  self->limit_active = limit_state;
}

void SSR_set_on_time_limit(SSR_t * const self, uint16_t limit) {
  self->limit_active = 1;
  self->on_time_limit = limit;
}

void SSR_set_off_time_required(SSR_t * const self, uint16_t off_time_required) {
  self->required_off_time = off_time_required;
}

uint16_t SSR_get_on_time(SSR_t * const self) {
  return self->on_time;
}

uint8_t SSR_get_state(SSR_t * const self) {
  return self->state;
}