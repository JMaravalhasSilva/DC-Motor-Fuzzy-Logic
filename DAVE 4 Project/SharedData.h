#ifndef SHAREDDATA_H_
#define SHAREDDATA_H_

#include <stdlib.h>
#include <stdbool.h>

// This header defines thread-safe functions to share data
// between tasks without poluting the global namespace

// A struct for storing motor data
struct motor_data_struct{
	float position;
	float speed;
	float current_pwm;
};

// A struct for controlling the motor.
// A boolean value chooses position control (true) or speed (false) control
// If position control is chosen, then the speed reference is ignored
struct control_params_struct{
	bool control_position;
	float position_ref;
	float speed_ref;
};

void setMotorData(struct motor_data_struct new_motor_data);

struct motor_data_struct getMotorData(void);

void setControlParams(struct control_params_struct new_control_params);

struct control_params_struct getControlParams(void);

#endif
