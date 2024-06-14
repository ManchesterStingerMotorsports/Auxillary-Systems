/*
 * apps_core.h
 *
 *  Created on: Jun 13, 2024
 *      Author: malhaar-k
 */

#ifndef INC_APPS_CORE_H_
#define INC_APPS_CORE_H_


// If someone new is reading this. We don't want magic numbers in our code.
#define ADC_TIMEOUT 		1000
//#define APPS_ONE_MUL 		0.00026862f
//#define APPS_TWO_MUL 		0.00029304f
#define MAX_ERPM 			65000
#define PEDAL_TRESH 		5.0 		//At accelerator values below this, a set point of 0 erpm will be given
#define ACCEL_SCALER		30			// Gain of the integrator
// We can make this configurable instead of a constant
#define PEAK_MAX_ANG_ACCEL 		9123	// Peak torque / Rotor inertia
#define CONT_MAX_ANG_ACCEL 		3808	// Continuous torque acceleration/ Rotor inertia
// The above needs to be verified. Might need to consider the inertia of the entire drivetrain.


// Write magic formulae here... We are doing this to make the code more readable
#define MAP_ERPM(x) 		(uint32_t)(x*MAX_ERPM)


enum APPS_State{
	APPS_OK = 0, // APPS Functioning is normal.
	APPS_EQUAL, //Both are shorted Critical error
	APPS_ONE_ZERO, // First value is zero
	APPS_TWO_ZERO, // Second value is zero
	APPS_BOTH_ZERO, // Both Zero. Critical error
} ;

typedef struct _apps{
	enum APPS_State state;
	float apps_val;
}APPS;


///////// FUNCTION PROTOTYPES
/*
 * @brief Changes ADC channel to Channel 0
 */
void ADC_Select_CH0(void);
/*
 * @brief Changes ADC channel to Channel 1
 */
void ADC_Select_CH1(void);
/*
 * @brief Reads the adc register of the current channel. Method: Polling
 */
HAL_StatusTypeDef poll_adc_raw(uint16_t*);
/*
 * @brief Checks APPS for valid inputs and differentiates the errors
 *
 * @return APPS state. Check APPS enum
 */
APPS APPS_read_verify(void);


void erpm_calc_set_point(void);
/*
 * @brief Final APPS task that sends erpm setpoint to the controller
 */
void apps_task(void*);
/*
 * @brief Initialises the APPS task
 */
void init_apps(void);


#endif /* INC_APPS_CORE_H_ */
