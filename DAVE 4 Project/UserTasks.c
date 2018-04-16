#include <DAVE.h>

#include <stdio.h>

#include "FuzzyController.h"
#include "UserTasks.h"
#include "Encoder.h"
#include "SharedData.h"
#include "FREERTOS/FreeRTOS.h"
#include "FREERTOS/task.h"


// File-scope task functions
static void controlTask(void);
static void lcdTask(void);

// Enumerators for all tasks, plus the number of existing tasks
enum taskenum{
	controlTaskEnum,
	lcdTaskEnum,
	NumberOfTasks
};

// Stack values assigned to each task
unsigned short task_stacks[NumberOfTasks]={
	configMINIMAL_STACK_SIZE,    // Control task
	configMINIMAL_STACK_SIZE+100 // LCD task
};

// Task priorities
UBaseType_t task_priorities[NumberOfTasks]={
	tskIDLE_PRIORITY+2,
	tskIDLE_PRIORITY+1
};

TaskHandle_t task_handlers[NumberOfTasks];

void startTasks(void){

	xTaskCreate((void*) controlTask, "Control Task", task_stacks[controlTaskEnum], NULL, task_priorities[controlTaskEnum], &task_handlers[controlTaskEnum]);
	xTaskCreate((void*) lcdTask, 	 "LCD Task", 	 task_stacks[lcdTaskEnum], 	   NULL, task_priorities[lcdTaskEnum],	   &task_handlers[lcdTaskEnum]);

}

// A task for the fuzzy controller
static void controlTask(void){
	static TickType_t prev_wake_time;

	// Control frequency in Hz
	static const float control_freq = 200.0F;

	// Motor parameters
	// Full rotation counts = counts/rotation * 4 encoder signal edges (quadrature encoder) * gearbox ratio
	static const int32_t full_rotation_counts = 5000;
	static const float max_rpm = 270.0F;

	// Encoder variables
	static uint32_t encoder_value=0, prev_encoder_value = 0;
	static int64_t encoder_value_diff = 0;

	// Motor speed measurement variables
	static float motor_speed = 0;
	static float prev_motor_speed = 0;

	// Motor position measurement variables
	static int32_t motor_position = 0;
	static float motor_position_normalized = 0.0F;

	// Controller inputs
	static float speed_error = 0;
	static float accel = 0;
	static float position_error = 0;

	// Fuzzy controller output variables
	static float fuzzy_out_pwm_increment = 0;
	static float fuzzy_out_speed_reference = 0;
	static float fuzzy_sum = 0;
	static float fuzzy_pwm = 0;

	// Current control parameters to follow
	struct control_params_struct current_control_params;

	// Initialize the fuzzy controller
	setupFuzzyController();

	// Turn on H-bridge
	DIGITAL_IO_SetOutputHigh(&INH_1);
	DIGITAL_IO_SetOutputHigh(&INH_2);

	// Start H-bridge PWM at 50% Duty Cycle (i.e. brake motor)
	PWM_CCU8_Start(&PWM_CCU8_0);
	PWM_CCU8_SetDutyCycleSymmetric(&PWM_CCU8_0,XMC_CCU8_SLICE_COMPARE_CHANNEL_1, 50*100);

	// Get current "wake" time to execute properly the very first task wait
	prev_wake_time=xTaskGetTickCount();

	while(1){
		DIGITAL_IO_SetOutputHigh(&TEST_PIN);

		current_control_params = getControlParams();

		encoder_value = getEncoderValue();

		// Handle overflow or underflow
		if((prev_encoder_value > ((UINT32_MAX/4)*3)) && (encoder_value < (UINT32_MAX/4))){
			// Overflow
			encoder_value_diff = ((int64_t) UINT32_MAX - prev_encoder_value + encoder_value + 1);
		} else if((prev_encoder_value < (UINT32_MAX/4)) && (encoder_value > ((UINT32_MAX/4)*3))){
			// Underflow
			encoder_value_diff = -((int64_t)UINT32_MAX - encoder_value + prev_encoder_value + 1);
		} else {
			// Normal difference
			encoder_value_diff = ((int64_t)encoder_value) - ((int64_t)prev_encoder_value);
		}

		// Compute motor speed in RPMs
		motor_speed = ((float)encoder_value_diff) * control_freq;
		motor_speed = (motor_speed / ((float)full_rotation_counts)) * 60.0F;

		// Compute motor position (in encoder counts)
		motor_position += encoder_value_diff;

		/*
		// Check if motor_position needs re-adjustment to fit between 0 and full_rotation counts (0deg to 360deg)
		// If so, adjust (normalized) position reference as well
		while(motor_position > full_rotation_counts){
			motor_position -= full_rotation_counts;
			current_control_params.position_ref -= 1.0F;
			setControlParams(current_control_params);
		}

		while(motor_position < 0){
			motor_position += full_rotation_counts;
			current_control_params.position_ref += 1.0F;
			setControlParams(current_control_params);
		}
		*/

		// If position control is activated, then use last cycle's speed reference
		if(current_control_params.control_position == true){
			current_control_params.speed_ref = fuzzy_out_speed_reference * max_rpm;
			// Force speed setting so that the user can read it back
			setControlParams(current_control_params);
		}

		// Compute normalized error
		speed_error = (current_control_params.speed_ref - motor_speed) / max_rpm;

		// Compute normalized acceleration
		accel = ((motor_speed/max_rpm) - (prev_motor_speed/max_rpm)) * control_freq;

		// Compute normalized position
		motor_position_normalized = (((float)motor_position) / ((float)full_rotation_counts));
		position_error = current_control_params.position_ref - motor_position_normalized;

		computeFuzzy(speed_error, accel, position_error, &fuzzy_out_pwm_increment, &fuzzy_out_speed_reference);

		fuzzy_sum += fuzzy_out_pwm_increment;

		if(fuzzy_sum > 1){
			fuzzy_sum = 1;
		} else if(fuzzy_sum < -1){
			fuzzy_sum = -1;
		}

		fuzzy_pwm = ((fuzzy_sum/2) + 0.5)*100;

		PWM_CCU8_SetDutyCycleSymmetric(&PWM_CCU8_0,XMC_CCU8_SLICE_COMPARE_CHANNEL_1, fuzzy_pwm*100);

		// Prepare motor speed for next iteration
		prev_motor_speed = motor_speed;

		// Prepare encoder value for next iteration
		prev_encoder_value = encoder_value;

		//Save motor data
		struct motor_data_struct motor_data = {motor_position_normalized, motor_speed, fuzzy_pwm};
		setMotorData(motor_data);

		DIGITAL_IO_SetOutputLow(&TEST_PIN);
		vTaskDelayUntil(&prev_wake_time, ((1.0F/control_freq) * 1000.0F)/portTICK_PERIOD_MS);
	}
}

/*
static void lcdTask(void){
	static TickType_t prev_wake_time;
	static struct motor_data_struct motor_data;

	prev_wake_time=xTaskGetTickCount();


	char buffer1[30], buffer2[30], buffer3[30];

	GUI_Clear();
	GUI_SetFont(&GUI_Font16B_1);  // Set the font-type
	GUI_SetColor(GUI_WHITE);

	while(1){

		motor_data = getMotorData();

		GUI_SetColor(GUI_BLACK);
		GUI_DispStringAt(buffer1, 18, 18);
		GUI_DispStringAt(buffer2, 18, 38);
		GUI_DispStringAt(buffer3, 18, 58);

		sprintf(buffer1,"Position: %"PRId32, (int32_t) motor_data.position);
		sprintf(buffer2,"Speed (RPM): %"PRId32, (int32_t) motor_data.speed);
		sprintf(buffer3,"PWM: %"PRId32,  (int32_t) motor_data.current_pwm);

		GUI_SetColor(GUI_WHITE);
		GUI_DispStringAt(buffer1, 18, 18);
		GUI_DispStringAt(buffer2, 18, 38);
		GUI_DispStringAt(buffer3, 18, 58);

		vTaskDelayUntil(&prev_wake_time, 500/portTICK_PERIOD_MS);
	}
}
*/

static void lcdTask(void){
	static TickType_t prev_wake_time;
	static struct motor_data_struct motor_data;

	prev_wake_time=xTaskGetTickCount();


	char buffer[30];

	GUI_Clear();
	GUI_SetFont(&GUI_Font16B_1);  // Set the font-type

	while(1){

		motor_data = getMotorData();



		GUI_SetColor(GUI_LIGHTCYAN);

		GUI_DispStringAt("Position:", 0, 18);
		GUI_DispStringAt("Speed:", 0, 38);
		GUI_DispStringAt("PWM:", 0, 58);

		GUI_SetColor(GUI_WHITE);

		sprintf(buffer, "%"PRId32".%"PRId32"    ", (int32_t) motor_data.position, ((int32_t)(motor_data.position*10)%10));
		GUI_DispStringAt(buffer, 60, 18);

		sprintf(buffer, "%"PRId32".%"PRId32"    ", (int32_t) motor_data.speed, ((int32_t)(motor_data.speed*10)%10));
		GUI_DispStringAt(buffer, 60, 38);

		sprintf(buffer, "%"PRId32".%"PRId32"    ", (int32_t) motor_data.current_pwm, ((int32_t)(motor_data.current_pwm*10)%10));
		GUI_DispStringAt(buffer, 60, 58);

		vTaskDelayUntil(&prev_wake_time, 500/portTICK_PERIOD_MS);
	}
}
