#include "SharedData.h"

#include "FREERTOS/FreeRTOS.h"
#include "FREERTOS/task.h"


// A file-scope variable for sharing motor data between tasks
struct motor_data_struct shared_motor_data = {0,0,50};

struct control_params_struct shared_control_params = {true, 0, 0};

void setMotorData(struct motor_data_struct new_motor_data){
	taskENTER_CRITICAL();
	shared_motor_data = new_motor_data;
	taskEXIT_CRITICAL();
}


struct motor_data_struct getMotorData(void){
	struct motor_data_struct result;
	taskENTER_CRITICAL();
	result = shared_motor_data;
	taskEXIT_CRITICAL();
	return result;
}

void setControlParams(struct control_params_struct new_control_params){
	taskENTER_CRITICAL();
	shared_control_params = new_control_params;
	taskEXIT_CRITICAL();
}

struct control_params_struct getControlParams(void){
	struct control_params_struct result;
	taskENTER_CRITICAL();
	result = shared_control_params;
	taskEXIT_CRITICAL();
	return result;
}
