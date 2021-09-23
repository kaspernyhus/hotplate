#include "CircularBuffer.h"


/* Private */
void _increment_head(cir_buf_t* const self) {
  if(++(self->head) == self->max) self->head = 0;
  self->full = (self->head == self->tail);
}

void _increment_tail(cir_buf_t* const self) {
  if(++(self->tail) == self->max) self->tail = 0;
}

void _decrement_tail(cir_buf_t* const self) {
  if((self->tail)-- == 0) self->tail = self->max-1;
}



/* Public */
void cir_buf_init(cir_buf_t* const self, uint16_t* buffer, uint16_t size, uint16_t initial_value) {
  self->buffer = buffer;
  self->max = size;
  cir_buf_reset(self, initial_value);
}

void cir_buf_reset(cir_buf_t* const self, uint16_t reset_value) {
  self->head = 0;
  self->tail = 0;
  self->full = 0;
  for(int i=0; i<self->max; i++) {
    self->buffer[i] = reset_value;
  }
}

uint8_t cir_buf_set_head(cir_buf_t* const self, uint16_t position) {
  if(position < self->max) {
    self->head = position;
    return 1;
  }
  else return 0; // setting head pointer failed
}

uint8_t cir_buf_set_tail(cir_buf_t* const self, uint16_t position) {
  if(position < self->max) {
    self->tail = position;
    return 1;
  }
  else return 0; // setting tail pointer failed
}


uint8_t cir_buf_empty(cir_buf_t* const self) {
  return (!self->full && (self->head == self->tail));
}

uint8_t cir_buf_full(cir_buf_t* const self) {
  return self->full;
}


uint8_t cir_buf_add(cir_buf_t* const self, uint16_t data) {
  self->buffer[self->head] = data;
  _increment_head(self);
  return 1;
}

uint8_t cir_buf_get(cir_buf_t* const self, uint16_t* data) {
    if(!cir_buf_empty(self)) {
        *data = self->buffer[self->tail];
        _increment_tail(self);
        return 1;
    }
    return 0;
}

uint8_t cir_buf_get_latest(cir_buf_t* const self, uint16_t* array, uint8_t len) {
  if(len<=self->max) {
    self->tail = self->head;
    for(int i=0; i<len; i++) {
      _decrement_tail(self);
    }
    uint8_t error;
    for(int i=0; i<len; i++) {
      error = cir_buf_get(self,array+i);
      if(!error) return 0;
    }
    return 1;
  }
  return 0;
}