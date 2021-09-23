#ifndef SYSTEM_STATES_H_
#define SYSTEM_STATES_H_

/* States */
typedef enum {
  STOPED,
  STARTUP,
  RAMPUP,
  SOAK,
  REFLOW_TEMP_RAMP,
  REFLOW,
  MAX,
  COOLDOWN,
  FINISH,
  DONE,
  ERROR
} reflowState_t;

#endif /* SYSTEM_STATES_H_ */