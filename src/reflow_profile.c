#include "reflow_profile.h"

uint16_t temp_profile1[] = {0, 0, 0, SOAK_TEMP*4, REFLOW_TEMP*4, TOP_TEMP*4, MAX_TEMP*4}; // STOPED, STARTUP, RAMPUP, SOAK, REFLOW_RAMP, REFLOW, MAX, COOLDOWN, FINISH


const uint16_t sim_profile[] PROGMEM = {
  15, 17, 27, 37, 41, 26, 35, 37, 42, 45, 
  46, 59, 65, 58, 67, 71, 66, 66, 84, 78, 
  86, 85, 78, 84, 89, 92, 101, 98, 104, 117, 
  101, 123, 120, 114, 123, 132, 128, 123, 131, 
  139, 148, 150, 137, 149, 153, 139, 153, 150, 
  146, 134, 146, 150, 145, 140, 138, 142, 139, 
  138, 145, 134, 153, 152, 148, 149, 143, 143, 
  139, 147, 145, 144, 140, 141, 137, 138, 140, 
  141, 141, 134, 142, 141, 137, 151, 142, 141, 
  141, 146, 148, 152, 150, 149, 141, 146, 150, 
  148, 134, 138, 140, 149, 145, 142, 151, 137, 
  146, 142, 135, 151, 135, 151, 140, 148, 150, 
  138, 143, 137, 141, 141, 144, 145, 141, 149, 
  137, 148, 134, 137, 146, 141, 137, 152, 137, 
  139, 140, 142, 137, 145, 145, 134, 138, 152, 
  152, 143, 141, 140, 147, 152, 138, 137, 146, 
  142, 149, 152, 147, 135, 152, 140, 141, 142, 
  152, 140, 142, 141, 152, 147, 152, 158, 159, 
  152, 156, 164, 163, 178, 178, 170, 170, 189, 
  176, 183, 192, 191, 200, 194, 203, 206, 218, 
  212, 203, 220, 212, 206, 215, 215, 206, 221, 
  209, 211, 204, 203, 207, 221, 219, 213, 222, 
  219, 220, 204, 218, 203, 218, 207, 203, 208, 
  208, 216, 210, 204, 195, 206, 202, 194, 194, 
  186, 182, 173, 177, 173, 162, 163, 164, 169, 
  153, 163, 146, 156, 156, 145, 150, 147, 141, 
  126, 129, 128, 131, 113, 121, 114, 120, 111, 
  103, 101, 98, 93, 94, 91, 91, 89, 74, 84, 79, 
  79, 67, 70, 65, 64, 56, 50, 56, 39, 36, 40, 47, 
  42};


/* Heater ON time from start temperatur to soak temperature */
uint16_t rampup_times[2][5] = {
  {0, 200, 280, 320, 360}, // temperature
  {20, 15,  13,  8,   0}  // ON time
};

uint16_t getRampupTime(uint16_t initial_temp) {
  for(int i=0;i<5;i++) {
    if(rampup_times[0][i]<initial_temp && initial_temp<rampup_times[0][i+1]){ // check to find initial temperature interval -> return on time [s]
      return rampup_times[1][i];
    }
  }
  return 0;
}