#include "logging.h"


void logging_init() {
  UART0_Init(MYUBRR);
  UART0_puts("UART initialized\r\n");
}

void log_data(uint8_t type, uint16_t data, uint16_t timestamp) {
  if(LOGGING == TEXT) {
    switch(type) {
    case RAW_DATA:
      UART0_puts("RAW_DATA");
      break;
    case TEMP_INSIDE:
      UART0_puts("TEMP_INSIDE");
      break;
    case TEMP_INSIDE_2:
      UART0_puts("TEMP_INSIDE_2");
      break;
    case TEMP_OUTSIDE:
      UART0_puts("TEMP_OUTSIDE");
      break;
    case TEMP_VELOCITY:
      UART0_puts("TEMP_VELOCITY");
      break;
    case STATE:
      UART0_puts("STATE");
      break;
    case SSR_STATE:
      UART0_puts("SSR_STATE");
      break;
    case OTHER:
      UART0_puts("OTHER");
      break;
    default:
      break;
    }
    sprintf(buffer,",%d,%d\n",timestamp,data);
    UART0_puts(buffer);
  }
  else if (LOGGING == HEX) {
    UART0_sendChar(0xFF);
    UART0_sendChar(type);
    UART0_sendChar(data<<8);
    UART0_sendChar(data&0xFF);
    UART0_sendChar(0xFA);
  }
}