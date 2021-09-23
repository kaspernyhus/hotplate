#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "SystemStates.h"
#include "UART.h"
#include "timer.h"
#include "external_interrupts.h"
#include "display.h"
#include "button.h"
#include "MAX6675.h"
#include "reflow_profile.h"
#include "DSP.h"
#include "SSR.h"
#include "logging.h"
#include "CircularBuffer.h"
#include "bsp.h"
#include "LCD.h"



/* LCD */
LCD myLCD;

/* Button */
Button btn1;

/* Function declarations */
void init();
void on_btn_push();

/* Timer */
volatile uint8_t sys_tick;
volatile uint16_t sys_tick_counter = 0;
uint16_t timestamp = 0;
volatile uint16_t PID_tick_counter = 0;
volatile uint8_t PID_tick;
volatile uint16_t sec_counter = 0;
volatile uint8_t sec_tick = 0;
volatile uint16_t update_display_counter = 0;
volatile uint8_t update_display = 0;
uint8_t run_once = 1;


/* Temperature sensors */
MAX6675 TempSensor1;
filter Filter_temp_data;
filter Filter_vel_data;

/* Buffers */
uint16_t temp_data[10];
cir_buf_t TempBuffer;

/* System states */
reflowState_t reflowState;
uint8_t errorState = 0;

/* SSR */
SSR_t SSR;


int main() {
  /* PID */
  int16_t PID_error = 0;
  uint16_t raw_reading = 0;
  uint16_t current_temp = 0;
  int16_t raw_temp_vel = 0;
  int16_t temp_vel = 0;

  /* Timers & flags */
  uint16_t elapsed_time = 0;
  uint16_t profile_timer = 0;
  uint16_t set_time = 0;
  uint8_t reflow_started = 0;
  uint16_t highest_temp = 0;
  uint16_t time_above_tl = 0;
  uint16_t time_at_peak = 0;

  init();
  _delay_ms(100);
  
  
  while(1) {
    if(PID_tick) {
      /* ------------------------------------- */
      /*         SENSOR READING / DSP          */
      /* ------------------------------------- */
      // raw_reading = pgm_read_word(sim_profile + elapsed_time)*4;
      
      /* Get a new temperature reading from the sensor */
      raw_reading = MAX6675_read(&TempSensor1);
      
      /* Smooth the reading with a filter */
      current_temp = apply_filter(&Filter_temp_data, raw_reading);
      
      /* Save the reading to a circular buffer */
      cir_buf_add(&TempBuffer, current_temp);
      
      /* Calculate the temperature velocity */
      raw_temp_vel = calc_velocity(&TempBuffer, -2);
      temp_vel = apply_filter(&Filter_vel_data, raw_temp_vel);


      /* ------------------------------------- */
      /*             DATA LOGGING              */
      /* ------------------------------------- */
      log_data(RAW_DATA,raw_reading,timestamp);
      log_data(TEMP_INSIDE,current_temp,timestamp);
      log_data(TEMP_VELOCITY,temp_vel,timestamp);
      log_data(SSR_STATE,SSR_get_state(&SSR),timestamp);
      if(raw_reading>highest_temp) highest_temp = raw_reading;

      /* ------------------------------------- */
      /*          TEMPERATURE CONTROL          */
      /* ------------------------------------- */
      if (raw_reading == 0x0000) {    // indicates a sensor faliure
        errorState = 2;
        reflowState = ERROR;
      }
      PID_error = temp_profile1[reflowState] - current_temp;   // how far off the set temperature. // '+' == too low // '-' == too high !
      if(current_temp>temp_profile1[MAX]){                     // If temperature above MAX
        errorState = 1;
        reflowState = ERROR;
      }
      
      switch(reflowState) {
        case STOPED:
          SSR_set(&SSR,OFF);
          if(current_temp > 200) fan_ON();
          else fan_OFF();
          reflow_started = 0;
          break;
        
        case STARTUP:
          fan_OFF();
          reflow_started = 1;
          elapsed_time = 0;
          profile_timer = 0;
          set_time = getRampupTime(current_temp); // rampup time based on current temperature
          log_data(OTHER,set_time,timestamp);
          reflowState = RAMPUP;
          log_data(STATE,RAMPUP,timestamp);
          break;
        
        case RAMPUP:
          if(temp_vel > 40) SSR_set(&SSR,OFF);                      // if temperature changing faster than +3.5°C/s
          else if(profile_timer <= set_time) SSR_set(&SSR,ON);      // rampup time not yet reached
          else if(current_temp >= (temp_profile1[SOAK]-40)) {       // 10°C from setpoint 
            SSR_set_on_time_limit(&SSR, 4);
            SSR_set_off_time_required(&SSR, 14);
            if(temp_vel < 8) SSR_set(&SSR,ON);
            else SSR_set(&SSR,OFF);
          }
          else if(current_temp >= (temp_profile1[SOAK]-140)) {       // 35°C from setpoint max 1°C/s
            SSR_set_on_time_limit(&SSR, 10);
            SSR_set_off_time_required(&SSR, 12);
            if(temp_vel < 24) SSR_set(&SSR,ON);
            else SSR_set(&SSR,OFF);
          }
          else if(temp_vel <= 32) {                                  // temperature changing slower than +2°C/s
            SSR_set_on_time_limit(&SSR, 16);
            SSR_set_off_time_required(&SSR, 10);
            SSR_set(&SSR,ON);
          }                  
          else SSR_set(&SSR,OFF);

          if(current_temp >= (temp_profile1[SOAK])) {                // temperature at SOAK point start
            set_time = SOAK_TIME;
            profile_timer = 0;
            reflowState = SOAK;
            log_data(STATE,SOAK,timestamp);
          }
          break;
        
        case SOAK:
          SSR_set_on_time_limit(&SSR, 3);
          SSR_set_off_time_required(&SSR, 16);
          if (PID_error < 0) {                                    // temperature above set_point
            if(PID_error > -8 && temp_vel < 0) SSR_set(&SSR,ON);  // temperature +2°C above setpoint AND moving down
            else SSR_set(&SSR,OFF);
          }
          else if (PID_error > 0) {                               // temperature below set_point
            if(temp_vel > 1) SSR_set(&SSR,OFF);                   // temperature rising more than 0.5°C/s
            else SSR_set(&SSR,ON);
          }
          else SSR_set(&SSR,OFF);
            
          if(profile_timer >= set_time) {                         // when max soak time reached, change to reflow phase
            SSR_set_limit(&SSR,OFF);
            set_time = 11;
            profile_timer = 0;
            reflowState = REFLOW_TEMP_RAMP;
            log_data(STATE,REFLOW_TEMP_RAMP,timestamp);
          }
          break;
        
        case REFLOW_TEMP_RAMP:
          if(profile_timer <= set_time) SSR_set(&SSR,ON);          // rampup time not yet reached
          else if (temp_vel < 20) SSR_set(&SSR,ON);                // temperature changin too slow
          else SSR_set(&SSR,OFF);
          
          if(current_temp >= temp_profile1[REFLOW_TEMP_RAMP]) {    // reflow/liquidous temperature reached
            set_time = MAX_TIME_ABOVE_TL;
            profile_timer = 0;
            SSR_set_on_time_limit(&SSR, 12);
            SSR_set_off_time_required(&SSR, 12);     
            reflowState = REFLOW;
            log_data(STATE,REFLOW,timestamp);
          }
          break;

        case REFLOW:
          if (PID_error < 0) {                                    // temperature above set_point
            if(PID_error > -8 && temp_vel < 0) {                  // temperature +2°C above setpoint AND moving down
              SSR_set_on_time_limit(&SSR, 6);
              SSR_set_off_time_required(&SSR, 12);
              SSR_set(&SSR,ON);  
            }
            else SSR_set(&SSR,OFF);
          }
          else if (PID_error > 0) {                               // temperature below set_point
            if(temp_vel > 20) {                                   // temperature rising more than X°C/s
              SSR_set(&SSR,OFF);                   
            }
            else {
              SSR_set_on_time_limit(&SSR, 6);
              SSR_set_off_time_required(&SSR, 14);
              SSR_set(&SSR,ON);
            }
          }
          else SSR_set(&SSR,OFF);
          
          if(profile_timer >= set_time) {                         // when max reflow time reached
            reflowState = COOLDOWN;
            log_data(STATE,COOLDOWN,timestamp);
          }
          break;

        case COOLDOWN:
          SSR_set(&SSR,OFF);
          fan_ON();
          if(current_temp<200) {
            reflowState = FINISH;
            log_data(STATE,FINISH,timestamp);
          }
          break;

        case FINISH:
          SSR_set(&SSR,OFF);
          if(current_temp > 200) fan_ON();
          else reflowState = DONE;
          reflow_started = 0;
          display_results(&myLCD, elapsed_time, highest_temp, time_above_tl, time_at_peak);
          break;
        
        case DONE:
          fan_OFF();
          break;

        case ERROR:
          SSR_set(&SSR,OFF);
          fan_ON();
          log_data(ERROR,errorState,timestamp);
          break;
        
        default:
          SSR_set(&SSR,OFF);
          break;
      } /* control switch case end */

      /* ------------------------------------- */
      /*            UPDATE DISPLAY             */
      /* ------------------------------------- */
      timestamp++; /* Sys tick time stamp variable */
      PID_tick = 0;
    } /* PID tick */

    /* Elapsed TIME in seconds */
    if(sec_tick) {
      sec_tick = 0;
      profile_timer++;
      if(reflow_started) {
        elapsed_time++;
      }
      if(reflowState == SOAK) temp_profile1[SOAK]++;                      // gradually increase soak temp
      if(current_temp>=temp_profile1[REFLOW_TEMP_RAMP]) time_above_tl++;  // log time above liquidation temperature
      if(current_temp>=temp_profile1[REFLOW]-8) time_at_peak++;           // log time around peak temperature 
    }

    if(sys_tick) {
      sys_tick = 0;
      btn_tick(&btn1);
    }

    if(update_display) {
      update_display = 0;
      if(reflowState<FINISH)  display_update(&myLCD, reflowState, elapsed_time, (int16_t)(set_time-profile_timer));
      display_update_current_temp(&myLCD,current_temp);
      if(reflowState==ERROR) display_error(&myLCD, reflowState, errorState, current_temp);
    }
  } /* while(1) */
}


void on_btn_push() {
  UART0_puts("---------------- BUTTON PUSHED\n");
  if(reflowState == STOPED) {
    reflowState = STARTUP;
  }
  else {
  reflowState = STOPED;
  display_clear(&myLCD);
  log_data(STATE,STOPED,timestamp);
  }
}



void init() {
  cli();

  /* Configure UART logging */
  logging_init();

  /* SPI init */
  configure_sensor_pins();
  SPI_master_init();
  
  /* Temperature sensors */
  MAX6675_init(&TempSensor1,&max6675_set_cs1);

  /* Configure timer */
  timer1_16bit_Init(PRESCALE_64,CTC_MODE,249);
  timer1_itr_config(ENABLE, COMPARE_MATCH_A);
  
  /* Configure display */
  configure_LCD_pins();
  LCD_init(&myLCD, &LCD_set_rs, &LCD_set_data_pins, &LCD_set_enable, 20, 4);
  display_init(&myLCD);
  
  // MAX6675_init(&TempSensor3,&max6675_set_cs3);
  filter_init(&Filter_temp_data, MAX6675_read(&TempSensor1), EMA_ALPHA(0.2));
  filter_init(&Filter_vel_data, 0, EMA_ALPHA(0.2));
  
  /* SSR heater controller */
  configure_SSR_pin();
  SSR_init(&SSR,&set_heater);
  SSR_set_limit(&SSR,OFF);

  /* Fan */
  configure_fan_pin();

  /* Button init */
  configure_btn_pin();
  itr0_Init();
  btn_init(&btn1,&on_btn_push);
  btn_attatch_read(&btn1,&read_btn_pin);

  /* DEBUG PIN */
  configure_debug_pin();

  /* Buffers */
  cir_buf_init(&TempBuffer, temp_data, 6, MAX6675_read(&TempSensor1)); // initialize buffer with an initial sensor value
  
  reflowState = STOPED;
  
  sei();
}


ISR(TIMER1_COMPA_vect) {  // @ 1ms 
  if(sys_tick_counter == 29) {
    sys_tick_counter = 0;
    sys_tick = 1;
  }
  else sys_tick_counter++;
  
  if(PID_tick_counter == 249) { // PID tick
    PID_tick_counter = 0;
    PID_tick = 1;
    //toggle_debug_pin();
  }
  else PID_tick_counter++;
  
  if(sec_counter == 999) { // 1 sec
    sec_counter = 0;
    sec_tick = 1;
  }
  else sec_counter++;

  if(update_display_counter == 198) {
    update_display_counter = 0;
    update_display = 1;
  }
  else update_display_counter++;
}


ISR(INT0_vect) {
  btn_push_detected(&btn1);
  btn_set_debounce_ticks(&btn1,10);
}