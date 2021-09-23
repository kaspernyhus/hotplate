#ifndef LOGGING_H_
#define LOGGING_H_

#include <stdio.h>
#include <stdint.h>
#include "UART.h"
#include "SystemStates.h"

#ifndef F_CPU
#define F_CPU 16000000U
#endif

/* UART */
#define BAUD 9600
#define MYUBRR F_CPU/8/BAUD-1 //full dublex


#define OFF 0
#define HEX 1
#define TEXT 2
#define LOGGING OFF

#define RAW_DATA 0
#define TEMP_INSIDE 1
#define TEMP_OUTSIDE 2
#define TEMP_VELOCITY 3
#define STATE 4
#define SSR_STATE 5
#define OTHER 6
#define TEMP_INSIDE_2 7



char buffer[100];

void logging_init();
void log_data(uint8_t type, uint16_t data, uint16_t timestamp);

#endif /* LOGGING_H_ */