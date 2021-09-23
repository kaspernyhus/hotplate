#ifndef CIR_BUF_H_
#define CIR_BUF_H_

#include <stdint.h>


typedef struct {
    uint16_t *buffer;
    uint16_t head;
    uint16_t tail;
    uint16_t max;
    uint8_t full;
} cir_buf_t;

void cir_buf_init(cir_buf_t* const self, uint16_t* buffer, uint16_t size, uint16_t initial_value);
void cir_buf_reset(cir_buf_t* const self, uint16_t reset_value);
uint8_t cir_buf_set_head(cir_buf_t* const self, uint16_t position);
uint8_t cir_buf_set_tail(cir_buf_t* const self, uint16_t position);
uint8_t cir_buf_empty(cir_buf_t* const self);
uint8_t cir_buf_full(cir_buf_t* const self);
uint8_t cir_buf_add(cir_buf_t* const self, uint16_t data);
uint8_t cir_buf_get(cir_buf_t* const self, uint16_t* data);
uint8_t cir_buf_get_latest(cir_buf_t* const self, uint16_t* array, uint8_t len);


#endif /* CIR_BUF_H_ */