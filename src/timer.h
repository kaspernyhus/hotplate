/*
    
    Kasper Nyhus Kaae

---------------------------------
Timer module for
ARDUINO NANO
---------------------------------

*/

void timer0_8bit_Init(uint8_t prescaler, uint8_t mode, uint8_t OCRA_compareValue);
void timer0_update_OCRA(uint8_t compareValue);
void timer0_update_OCRB(uint8_t compareValue);
void timer0_itr_config(uint8_t itr_on, uint8_t type);
void timer0_pin_output_mode(uint8_t mode);

void timer1_16bit_Init(uint8_t prescaler, uint8_t mode, uint16_t OCRA_compareValue);
void timer1_update_OCRA(uint16_t compareValue);
void timer1_update_OCRB(uint16_t compareValue);
void timer1_itr_config(uint8_t itr_on, uint8_t type);
void timer1_pin_output_mode(uint8_t mode);



// -------------------------//
//    Prescaller options    //
// -------------------------//

#define NO_PRESCALER 0
#define PRESCALE_8 1
#define PRESCALE_64 2
#define PRESCALE_256 3
#define PRESCALE_1024 4
#define EXTERNAL_CLK_SOURCE_FALLING 5
#define EXTERNAL_CLK_SOURCE_RISING 6


// -------------------------//
// Waveform Generation Mode //
// -------------------------//

#define NORMAL_MODE 0
#define PWM_PHASE_CORRECT 1
#define CTC_MODE 2
#define FAST_PWM 3
#define PWM_PHASE_CORRECT_TOP_OCRA 4
#define FAST_PWM_TOP_OCRA 5
#define MAX_8 0xFF
#define MAX_16 0x3FF


// -------------------------//
//      Interrupt modes     //
// -------------------------//

#define DISABLE 0
#define ENABLE 1
#define OVERFLOW 0
#define COMPARE_MATCH_A 1
#define COMPARE_MATCH_B 2