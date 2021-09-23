#include "display.h"

/* Error codes */
char *errors[] = {  
                    " ",
                    "max temp reached",
                    "  sensor error"
};

/* Custom characters */
uint8_t degree[] = {
  0b00111,
  0b00101,
  0b00111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};


void update_elapsed(LCD * const self, uint16_t elapsed, uint8_t X, uint8_t Y);


void display_init(LCD * const self) {
	LCD_sendStrXY(self, "Reflow Hot Plate",0,2);
	LCD_sendStrXY(self, "STOPED",DEF_X,DEF_Y);
  createChar(self,0,degree);
}

void display_clear(LCD * const self) {
  LCD_clear(self);
  LCD_sendStrXY(self, "Reflow Hot Plate",0,2);
}

void update_elapsed(LCD * const self, uint16_t elapsed, uint8_t X, uint8_t Y) {
  sprintf(oled_buffer, "%02i:%02i", elapsed/60, elapsed%60 );
  LCD_sendStrXY(self,oled_buffer,Y,X);
}


void display_update_current_temp(LCD * const self, uint16_t temp) {
  sprintf(oled_buffer, "%3d.%02d C", temp/4,temp%4*25);
  LCD_sendStrXY(self,oled_buffer,3,0);
  setCursor(self,6,3);
  _write(self, (uint8_t)0); // custom degree char
}

void display_update(LCD * const self, uint8_t state, uint16_t elapsed, int16_t timer) {
  switch (state) {
    case STOPED:
      LCD_sendStrXY(self," STOPED   ",DEF_X,DEF_Y);
      break;
    case RAMPUP:
      LCD_sendStrXY(self," RAMPUP    ",DEF_X,DEF_Y);
      break;
    case SOAK:
      LCD_sendStrXY(self,"  SOAK    ",DEF_X,DEF_Y);
      break;
    case REFLOW_TEMP_RAMP:
      LCD_sendStrXY(self," REFLOW   ",DEF_X,DEF_Y);
      break;
    case REFLOW:
      LCD_sendStrXY(self," REFLOW   ",DEF_X,DEF_Y);
      break;
    case MAX:
      LCD_sendStrXY(self,"  MAX!    ",DEF_X,DEF_Y);
      break;
    case COOLDOWN:
      LCD_sendStrXY(self,"COOLDOWN ",DEF_X,DEF_Y);
      break;
    case FINISH:
      LCD_sendStrXY(self," FINISH   ",DEF_X,DEF_Y);
      break;
    case DONE:
      LCD_sendStrXY(self," FINISH   ",DEF_X,DEF_Y);
      break;
    case ERROR:
      LCD_sendStrXY(self," ERROR    ",DEF_X,DEF_Y);
    default:
      LCD_sendStrXY(self,"          ",DEF_X,DEF_Y);
      break;
  }
  update_elapsed(self,elapsed,15,3);

  if(timer>0 && state != STOPED) {
    uint16_t time = 0;
    time = timer;
    sprintf(oled_buffer, "%02i:%02i", time/60, time%60);
    LCD_sendStrXY(self,oled_buffer,1,15);
  }
  else LCD_sendStrXY(self,"     ",1,15);
}


void display_error(LCD * const self, uint8_t state, uint8_t errorState, uint16_t temp) {
    LCD_sendStrXY(self," ERROR    ",DEF_X,DEF_Y);
    LCD_sendStrXY(self,errors[errorState],2,2);
  }



void display_results(LCD * const self, uint16_t elapsed, uint16_t max_temp, uint16_t time_above_tl, uint16_t time_at_peak) {
  LCD_clear(self);
  LCD_sendStrXY(self,"FINISHED   ",0,2);
  update_elapsed(self,elapsed,13,0);
  
  sprintf(oled_buffer, "Time above TL: %2ds", time_above_tl);
  LCD_sendStrXY(self,oled_buffer,1,0);

  sprintf(oled_buffer, "Time at peak : %2ds", time_at_peak);
  LCD_sendStrXY(self,oled_buffer,2,0);

  sprintf(oled_buffer, "   %3d.%02d C", max_temp/4,max_temp%4*25);
  LCD_sendStrXY(self,oled_buffer,3,9);
  setCursor(self,18,3);
  _write(self, (uint8_t)0); // custom degree char
}

