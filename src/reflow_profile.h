#ifndef REFLOW_PROFILE_H_
#define REFLOW_PROFILE_H_
#include <stdio.h>
#include <avr/pgmspace.h>


#define SOAK_TEMP 135     // C
#define REFLOW_TEMP 183   // C
#define TOP_TEMP 210      // C
#define MAX_TEMP 230      // C
#define SOAK_TIME 100     // s
#define MAX_TIME_ABOVE_TL 45  // s
#define MAX_PEAK_TIME 30      // s


extern uint16_t temp_profile1[9];
const uint16_t sim_profile[270];
uint16_t getRampupTime(uint16_t initial_temp);


#endif /* REFLOW_PROFILE_H_ */


