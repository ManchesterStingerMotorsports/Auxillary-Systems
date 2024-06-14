/*
 * apps_core.c
 *
 *  Created on: Jun 13, 2024
 *      Author: malhaar-k
 */

#include "main.h"
#include "apps_core.h"
#include "stm32f407xx.h"
#include "cmsis_os.h"
#include "DTI_CAN.h"
#include "error_handler.h"

/*---------- DEFINES----------------*/
#define SAMPLING_TIME			100 // Sampling time for integrator


/*----------EXTERN VARIABLES--------*/
extern ADC_HandleTypeDef hadc1;

//extern osMutexId_t oCANMutex; // CAN Mutex object

//extern osMessageQueueId_t oErrorMsgQueue; // Error message queue object

//extern DTI_CAN main_struct;



/*---------- VARIABLES--------------*/

osThreadId_t AccelTaskHandle;

const osThreadAttr_t AccelTask_attributes = {
  .name = "AccelTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/*----------- FUNCTIONS ------------*/
void ADC_Select_CH0 (void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	  sConfig.Channel = ADC_CHANNEL_0;
	  sConfig.Rank = 1;
	  //sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

void ADC_Select_CH1 (void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	  sConfig.Channel = ADC_CHANNEL_1;
	  sConfig.Rank = 2;
	  //sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

HAL_StatusTypeDef poll_adc_raw(uint16_t* address){
	HAL_StatusTypeDef ret_var = HAL_OK;
	HAL_ADC_Start(&hadc1);
	ret_var = HAL_ADC_PollForConversion(&hadc1, ADC_TIMEOUT);
	if(ret_var != HAL_OK){
		return ret_var;
	}
	*address = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
	return HAL_OK;
}

// We want to use polling instead of interrupt based operation so we can have
// an RTOS later
APPS APPS_read_verify(void)
{
	APPS ret_struct = {
			.apps_val = 0.0,
			.state = APPS_OK
	};

	uint16_t val_one =0; //Primary
	uint16_t val_two =0;

		//ADC1->CHSELR  = 0x01; // I would like to abstract this too. Not needed
	ADC_Select_CH0();
	poll_adc_raw(&val_one);

		//ADC1->CHSELR  = 0x02;
	ADC_Select_CH1();
	poll_adc_raw(&val_two);

		//ERROR CONDITION 1
	if(val_one == val_two) ret_struct.state = APPS_EQUAL;
		//ERROR CONDITION 2
	if(val_one == 0) ret_struct.state = APPS_ONE_ZERO;
		//ERROR CONDITION 3 and 4
	if(val_two == 0) ret_struct.state = (val_one == 0? APPS_BOTH_ZERO: APPS_TWO_ZERO);

	switch(ret_struct.state){
		case(APPS_OK):
		ret_struct.apps_val = (val_one*1.1*100)/4096;  	// AAAAAAAAAAAAAAA MAGIC NUMBERS
		ret_struct.apps_val -= 0.1;					// Formula to calculate based on only lowside offset resistor

		break;
		case(APPS_ONE_ZERO):
		ret_struct.apps_val = (val_one*1.2*100)/4096;
		ret_struct.apps_val -= 0.2;


		break;
		case(APPS_TWO_ZERO):
		ret_struct.apps_val = (val_one*1.1*100)/4096;
		ret_struct.apps_val -= 0.1;


		break;
		default:
			// Critical errors. We want to send to error handler.
			//Error_Handler();
		break;
		}


	return ret_struct;
}

void apps_task(void* arg){
	err_msg_t apps_err = {
			.component = ACCEL,
			.error_val = 0
	};

	int32_t erpm_set_point = 0;
	//Infinite loop
	while(1){
		APPS accel = APPS_read_verify();
		if(accel.state != APPS_OK){
			// Append error message to message queue
			apps_err.error_val = accel.state;
			osMessagePut(oErrorMsgQueue, &apps_err, osWaitForever);
			osMessageQueuePut(oErrorMsgQueue, &apps_err, 0, osWaitForever);
		}
		else{
			// Set rpm for the motorcontroller
			// Throttle to torque to erpm set point is an integrator essentially.
			//accel = apps*scaler
			// w_set += accel * 0.1   integral(a*dt)
			if(controllerData.ERPM < MAX_ERPM){
				erpm_set_point += accel.apps_val*ACCEL_SCALER*SAMPLING_TIME;

			}
			set_dti_erpm(controllerData, erpm_set_point); //Sends omega_set_point

		}

		osDelay(100);
	}
}


void init_apps(void){
	/* APPS INIT*/
	AccelTaskHandle = osThreadNew(apps_task, NULL, AccelTask_attributes);
}



