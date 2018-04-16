#include "FuzzyController.h"
#include <stdlib.h>
#include <inttypes.h>

// A struct for storing the 3 parameters of a triangular membership function
struct mf_struct{
	float params[3];
};

// An enumerator for accessing the membership functions of the speed error fuzzy variable
enum speed_error_mf_names{
	speed_error_VN,
	speed_error_N,
	speed_error_Z,
	speed_error_P,
	speed_error_VP,
	speed_error_mf_count
};

// An enumerator for accessing the membership functions of the acceleration fuzzy variable
enum accel_mf_names{
	accel_N,
	accel_Z,
	accel_P,
	accel_mf_count
};

// An enumerator for accessing the membership functions of the position error fuzzy variable
enum position_error_mf_names{
	position_error_VN,
	position_error_N,
	position_error_Z,
	position_error_P,
	position_error_VP,
	position_error_mf_count
};

// An enumerator for accessing the membership functions of the pwm fuzzy variable
enum pwm_mf_names{
	pwm_BDecrease,
	pwm_MDecrease,
	pwm_SDecrease,
	pwm_NoChange,
	pwm_SIncrease,
	pwm_MIncrease,
	pwm_BIncrease,
	pwm_mf_count
};

// An enumerator for accessing the membership functions of the speed_ref fuzzy variable
enum speed_ref_mf_names{
	speed_ref_VN,
	speed_ref_N,
	speed_ref_Z,
	speed_ref_P,
	speed_ref_VP,
	speed_ref_mf_count
};


// A struct for storing fuzzy variables
struct fuzzy_variable{
	struct mf_struct* mf;
	float* mf_values;
	float range[2];
};

struct fuzzy_variable speed_error_fuzzy, accel_fuzzy, position_error_fuzzy, pwm_fuzzy, speed_ref_fuzzy;

// File-scope functions
void assignMembershipFunctionParams(struct mf_struct* mf, float p1, float p2, float p3);
void fuzzify(struct fuzzy_variable* fuzzy_var,float input, uint32_t number_of_mf);
float getTruthValue(struct mf_struct mf, float input);
void inference (void);
float fuzzyAnd(float a, float b);
float defuzzify(struct fuzzy_variable fuzzy_var, float samples, uint32_t number_of_mf);


void setupFuzzyController(void){
	uint8_t i;

	// Allocate memory for each fuzzy variable
	speed_error_fuzzy.mf = malloc(sizeof(struct mf_struct)*speed_error_mf_count);
	speed_error_fuzzy.mf_values = malloc(sizeof(float)*speed_error_mf_count);

	accel_fuzzy.mf = malloc(sizeof(struct mf_struct)*accel_mf_count);
	accel_fuzzy.mf_values = malloc(sizeof(float)*accel_mf_count);

	position_error_fuzzy.mf = malloc(sizeof(struct mf_struct)*position_error_mf_count);
	position_error_fuzzy.mf_values = malloc(sizeof(float)*position_error_mf_count);

	pwm_fuzzy.mf = malloc(sizeof(struct mf_struct)*pwm_mf_count);
	pwm_fuzzy.mf_values = malloc(sizeof(float)*pwm_mf_count);

	speed_ref_fuzzy.mf = malloc(sizeof(struct mf_struct)*speed_ref_mf_count);
	speed_ref_fuzzy.mf_values = malloc(sizeof(float)*speed_ref_mf_count);

	// Initialize each triangular membership function parameters
	assignMembershipFunctionParams(&(speed_error_fuzzy.mf[speed_error_VN]), -3, -2, -1);
	assignMembershipFunctionParams(&(speed_error_fuzzy.mf[speed_error_N]),  -2, -1,  0);
	assignMembershipFunctionParams(&(speed_error_fuzzy.mf[speed_error_Z]),  -1,  0,  1);
	assignMembershipFunctionParams(&(speed_error_fuzzy.mf[speed_error_P]),   0,  1,  2);
	assignMembershipFunctionParams(&(speed_error_fuzzy.mf[speed_error_VP]),  1,  2,  3);

	// Reset all truth values for the speed error fuzzy variable
	for(i=0; i<speed_error_mf_count; i++){
		speed_error_fuzzy.mf_values[i] = 0.0F;
	}

	// Set the input range
	speed_error_fuzzy.range[0] = -2;
	speed_error_fuzzy.range[1] = 2;

	// Same as the above but for the acceleration fuzzy variable
	assignMembershipFunctionParams(&(accel_fuzzy.mf[accel_N]), -180, -120,    0);
	assignMembershipFunctionParams(&(accel_fuzzy.mf[accel_Z]), -120,    0,  120);
	assignMembershipFunctionParams(&(accel_fuzzy.mf[accel_P]),    0,  120,  180);

	for(i=0; i<accel_mf_count; i++){
		accel_fuzzy.mf_values[i] = 0.0F;
	}

	accel_fuzzy.range[0] = -120;
	accel_fuzzy.range[1] = 120;

	// Same as the above but for the position error fuzzy variable
	assignMembershipFunctionParams(&(position_error_fuzzy.mf[position_error_VN]), -1.5,   -1, -0.5);
	assignMembershipFunctionParams(&(position_error_fuzzy.mf[position_error_N]),    -1, -0.5,    0);
	assignMembershipFunctionParams(&(position_error_fuzzy.mf[position_error_Z]),  -0.5,    0,  0.5);
	assignMembershipFunctionParams(&(position_error_fuzzy.mf[position_error_P]),     0,  0.5,    1);
	assignMembershipFunctionParams(&(position_error_fuzzy.mf[position_error_VP]),  0.5,    1,  1.5);

	for(i=0; i<position_error_mf_count; i++){
		position_error_fuzzy.mf_values[i] = 0.0F;
	}

	position_error_fuzzy.range[0] = -1;
	position_error_fuzzy.range[1] = 1;

	// Same as the above but for the pwm fuzzy variable
	assignMembershipFunctionParams(&(pwm_fuzzy.mf[pwm_BDecrease]), -3,     -2,   -1);
	assignMembershipFunctionParams(&(pwm_fuzzy.mf[pwm_MDecrease]), -1,   -0.7, -0.4);
	assignMembershipFunctionParams(&(pwm_fuzzy.mf[pwm_SDecrease]), -0.7, -0.4,	  0);
	assignMembershipFunctionParams(&(pwm_fuzzy.mf[pwm_NoChange]),  -0.3,    0,  0.3);
	assignMembershipFunctionParams(&(pwm_fuzzy.mf[pwm_SIncrease]),    0,  0.4,  0.7);
	assignMembershipFunctionParams(&(pwm_fuzzy.mf[pwm_MIncrease]),  0.4,  0.7,    1);
	assignMembershipFunctionParams(&(pwm_fuzzy.mf[pwm_BIncrease]),    1,    2,    3);

	for(i=0; i<pwm_mf_count; i++){
		pwm_fuzzy.mf_values[i] = 0.0F;
	}

	pwm_fuzzy.range[0] = -2;
	pwm_fuzzy.range[1] = 2;

	// Same as the above but for the speed reference fuzzy variable
	assignMembershipFunctionParams(&(speed_ref_fuzzy.mf[speed_ref_VN]),  -2,   -1,   0);
	assignMembershipFunctionParams(&(speed_ref_fuzzy.mf[speed_ref_N]), -0.8, -0.8,   0);
	assignMembershipFunctionParams(&(speed_ref_fuzzy.mf[speed_ref_Z]), -0.2,    0, 0.2);
	assignMembershipFunctionParams(&(speed_ref_fuzzy.mf[speed_ref_P]),    0,  0.8, 0.8);
	assignMembershipFunctionParams(&(speed_ref_fuzzy.mf[speed_ref_VP]),   0,    1,   2);

	for(i=0; i<speed_ref_mf_count; i++){
		speed_ref_fuzzy.mf_values[i] = 0.0F;
	}

	speed_ref_fuzzy.range[0] = -2;
	speed_ref_fuzzy.range[1] = 2;
}

void computeFuzzy(float speed_error_val, float accel_val, float position_error_val, float* pwm_increment, float* speed_reference){

	fuzzify(&speed_error_fuzzy, speed_error_val, speed_error_mf_count);
	fuzzify(&accel_fuzzy, accel_val, accel_mf_count);
	fuzzify(&position_error_fuzzy, position_error_val, position_error_mf_count);

	inference();

	*pwm_increment = defuzzify(pwm_fuzzy, 180, pwm_mf_count);
	*speed_reference = defuzzify(speed_ref_fuzzy, 180, speed_ref_mf_count);
}

// Assigns the 3 parameters of a triangular membership function
void assignMembershipFunctionParams(struct mf_struct* mf, float p1, float p2, float p3){
	mf->params[0] = p1;
	mf->params[1] = p2;
	mf->params[2] = p3;
}

// Fuzzyfies an input of a given fuzzy variable. Takes as argument the number of mfs
void fuzzify(struct fuzzy_variable* fuzzy_var, float input, uint32_t number_of_mf){
	static uint32_t i;

	// Limit input
	if(input < fuzzy_var->range[0]){
		input = fuzzy_var->range[0];
	} else if (input > fuzzy_var->range[1]){
		input = fuzzy_var->range[1];
	}

	for(i=0; i<number_of_mf; i++){
		fuzzy_var->mf_values[i] = getTruthValue(fuzzy_var->mf[i],input);
	}

}

// Given a membership function, returns the truth value for a given input
float getTruthValue(struct mf_struct mf, float input){

	if(input <= mf.params[0])  return 0;
	if(input >= mf.params[2])  return 0;
	if(input == mf.params[1]) return 1;

	if((input > mf.params[0]) && (input < mf.params[1])){
		return ((input - mf.params[0]) / (mf.params[1] -  mf.params[0]));
	}

	return ((mf.params[2] - input) / (mf.params[2] - mf.params[1]));
}

void inference (void){
	uint8_t i;
	float and_result;

	// Reset output variables
	for(i=0; i<pwm_mf_count; i++){
		pwm_fuzzy.mf_values[i] = 0;
	}

	for(i=0; i<speed_ref_mf_count; i++){
		speed_ref_fuzzy.mf_values[i] = 0;
	}

	// 1) If Speed_Error is Z and Accel is Z then PWM is NoChange
	and_result = fuzzyAnd(speed_error_fuzzy.mf_values[speed_error_Z],accel_fuzzy.mf_values[accel_Z]);

	if(and_result > pwm_fuzzy.mf_values[pwm_NoChange]){
		pwm_fuzzy.mf_values[pwm_NoChange] = and_result;
	}

	// 2) If Speed_Error is Z and Accel is P then PWM is SDecrease
	and_result = fuzzyAnd(speed_error_fuzzy.mf_values[speed_error_Z],accel_fuzzy.mf_values[accel_P]);

	if(and_result > pwm_fuzzy.mf_values[pwm_SDecrease]){
			pwm_fuzzy.mf_values[pwm_SDecrease] = and_result;
	}

	// 3) If Speed_Error is Z and Accel is N then PWM is SIncrease
	and_result = fuzzyAnd(speed_error_fuzzy.mf_values[speed_error_Z],accel_fuzzy.mf_values[accel_N]);

	if(and_result > pwm_fuzzy.mf_values[pwm_SIncrease]){
			pwm_fuzzy.mf_values[pwm_SIncrease] = and_result;
	}

	// 4) If Speed_Error is P and Accel is Z then PWM is SIncrease
	and_result = fuzzyAnd(speed_error_fuzzy.mf_values[speed_error_P],accel_fuzzy.mf_values[accel_Z]);

	if(and_result > pwm_fuzzy.mf_values[pwm_SIncrease]){
			pwm_fuzzy.mf_values[pwm_SIncrease] = and_result;
	}

	// 5) If Speed_Error is P and Accel is P then PWM is NoChange
	and_result = fuzzyAnd(speed_error_fuzzy.mf_values[speed_error_P],accel_fuzzy.mf_values[accel_P]);

	if(and_result > pwm_fuzzy.mf_values[pwm_NoChange]){
			pwm_fuzzy.mf_values[pwm_NoChange] = and_result;
	}

	// 6) If Speed_Error is P and Accel is N then PWM is MIncrease
	and_result = fuzzyAnd(speed_error_fuzzy.mf_values[speed_error_P],accel_fuzzy.mf_values[accel_N]);

	if(and_result > pwm_fuzzy.mf_values[pwm_MIncrease]){
			pwm_fuzzy.mf_values[pwm_MIncrease] = and_result;
	}

	// 7) If Speed_Error is VP and Accel is Z then PWM is MIncrease
	and_result = fuzzyAnd(speed_error_fuzzy.mf_values[speed_error_VP],accel_fuzzy.mf_values[accel_Z]);

	if(and_result > pwm_fuzzy.mf_values[pwm_MIncrease]){
			pwm_fuzzy.mf_values[pwm_MIncrease] = and_result;
	}

	// 8) If Speed_Error is VP and Accel is P then PWM is SIncrease
	and_result = fuzzyAnd(speed_error_fuzzy.mf_values[speed_error_VP],accel_fuzzy.mf_values[accel_P]);

	if(and_result > pwm_fuzzy.mf_values[pwm_SIncrease]){
			pwm_fuzzy.mf_values[pwm_SIncrease] = and_result;
	}

	// 9) If Speed_Error is VP and Accel is N then PWM is BIncrease
	and_result = fuzzyAnd(speed_error_fuzzy.mf_values[speed_error_VP],accel_fuzzy.mf_values[accel_N]);

	if(and_result > pwm_fuzzy.mf_values[pwm_BIncrease]){
			pwm_fuzzy.mf_values[pwm_BIncrease] = and_result;
	}

	// 10) If Speed_Error is N and Accel is Z then PWM is SDecrease
	and_result = fuzzyAnd(speed_error_fuzzy.mf_values[speed_error_N],accel_fuzzy.mf_values[accel_Z]);

	if(and_result > pwm_fuzzy.mf_values[pwm_SDecrease]){
			pwm_fuzzy.mf_values[pwm_SDecrease] = and_result;
	}

	// 11) If Speed_Error is N and Accel is P then PWM is MDecrease
	and_result = fuzzyAnd(speed_error_fuzzy.mf_values[speed_error_N],accel_fuzzy.mf_values[accel_P]);

	if(and_result > pwm_fuzzy.mf_values[pwm_MDecrease]){
			pwm_fuzzy.mf_values[pwm_MDecrease] = and_result;
	}

	// 12) If Speed_Error is N and Accel is N then PWM is NoChange
	and_result = fuzzyAnd(speed_error_fuzzy.mf_values[speed_error_N],accel_fuzzy.mf_values[accel_N]);

	if(and_result > pwm_fuzzy.mf_values[pwm_NoChange]){
			pwm_fuzzy.mf_values[pwm_NoChange] = and_result;
	}

	// 13) If Speed_Error is VN and Accel is Z then PWM is MDecrease
	and_result = fuzzyAnd(speed_error_fuzzy.mf_values[speed_error_VN],accel_fuzzy.mf_values[accel_Z]);

	if(and_result > pwm_fuzzy.mf_values[pwm_MDecrease]){
			pwm_fuzzy.mf_values[pwm_MDecrease] = and_result;
	}

	// 14) If Speed_Error is VN and Accel is P then PWM is BDecrease
	and_result = fuzzyAnd(speed_error_fuzzy.mf_values[speed_error_VN],accel_fuzzy.mf_values[accel_P]);

	if(and_result > pwm_fuzzy.mf_values[pwm_BDecrease]){
			pwm_fuzzy.mf_values[pwm_BDecrease] = and_result;
	}

	// 15) If Speed_Error is VN and Accel is N then PWM is SDecrease
	and_result = fuzzyAnd(speed_error_fuzzy.mf_values[speed_error_VN],accel_fuzzy.mf_values[accel_N]);

	if(and_result > pwm_fuzzy.mf_values[pwm_SDecrease]){
			pwm_fuzzy.mf_values[pwm_SDecrease] = and_result;
	}

	// 16) If Position_Error is VN then Speed_Ref is VN
	if(position_error_fuzzy.mf_values[position_error_VN] > speed_ref_fuzzy.mf_values[speed_ref_VN]){
		speed_ref_fuzzy.mf_values[speed_ref_VN] = position_error_fuzzy.mf_values[position_error_VN];
	}

	// 17) If Position_Error is N then Speed_Ref is N
	if(position_error_fuzzy.mf_values[position_error_N] > speed_ref_fuzzy.mf_values[speed_ref_N]){
		speed_ref_fuzzy.mf_values[speed_ref_N] = position_error_fuzzy.mf_values[position_error_N];
	}

	// 18) If Position_Error is Z then Speed_Ref is Z
	if(position_error_fuzzy.mf_values[position_error_Z] > speed_ref_fuzzy.mf_values[speed_ref_Z]){
		speed_ref_fuzzy.mf_values[speed_ref_Z] = position_error_fuzzy.mf_values[position_error_Z];
	}

	// 19) If Position_Error is P then Speed_Ref is P
	if(position_error_fuzzy.mf_values[position_error_P] > speed_ref_fuzzy.mf_values[speed_ref_P]){
		speed_ref_fuzzy.mf_values[speed_ref_P] = position_error_fuzzy.mf_values[position_error_P];
	}

	// 20) If Position_Error is VP then Speed_Ref is VP
	if(position_error_fuzzy.mf_values[position_error_VP] > speed_ref_fuzzy.mf_values[speed_ref_VP]){
		speed_ref_fuzzy.mf_values[speed_ref_VP] = position_error_fuzzy.mf_values[position_error_VP];
	}

}

float fuzzyAnd(float a, float b){
	return (a<b) ? a : b;
}

float defuzzify(struct fuzzy_variable fuzzy_var, float samples, uint32_t number_of_mf){
	static uint32_t i;
	static float current_x;
	static float x_increment;
	static float top_sum;
	static float bottom_sum;
	static float truth_value;

	current_x = fuzzy_var.range[0];
	x_increment = (fuzzy_var.range[1] - fuzzy_var.range[0])/samples;

	top_sum = 0;
	bottom_sum = 0;
	truth_value = 0;

	while(current_x <= fuzzy_var.range[1]){
		for(i=0; i<number_of_mf; i++){
			truth_value = getTruthValue(fuzzy_var.mf[i], current_x);
			if(truth_value>fuzzy_var.mf_values[i]){
				truth_value = fuzzy_var.mf_values[i];
			}
			top_sum += current_x * truth_value;
			bottom_sum += truth_value;
		}
		current_x += x_increment;
	}
	return (top_sum/bottom_sum);
}
