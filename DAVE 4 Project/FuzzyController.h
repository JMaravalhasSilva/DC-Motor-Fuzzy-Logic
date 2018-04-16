#ifndef FUZZYCONTROLLER_H_
#define FUZZYCONTROLLER_H_

// Initializes the fuzzy controller
void setupFuzzyController(void);

// Computes fuzzy controller outputs: normalized PWM increment (from -1 to 1) and normalized speed reference (from -1 to 1)
// Values are computed based on normalized: speed error, acceleration and position error
void computeFuzzy(float speed_error_val, float accel_val, float position_error_val, float* pwm_increment, float* speed_reference);


#endif
