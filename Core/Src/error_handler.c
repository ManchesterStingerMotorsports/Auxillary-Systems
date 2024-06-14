#include "error_handler.h"
#include "main.h"
#include "cmsis_os.h"
#include "string.h"
#include "HAL_CAN.h"

/*---------- DEFINES----------------*/
#define ECU_ID		0x14
#define ERROR_QUEUE_SIZE		10 // 5 Elements




/*---------- EXTERN VARIABLES----------------*/
//extern osMutexId_t oCANMutex; // Global CAN Mutex object

//extern osMessageQueueId_t oErrorMsgQueue; // Global Error message queue object

/*---------- VARIABLES----------------*/

uint8_t canErrorMessage[8]; // Error message being transmitted via CAN
uint8_t errorState;


osThreadId_t errorTaskHandle;
const osThreadAttr_t errorTask_attributes = {
  .name = "errorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};



/*---------- DEFINES----------------*/
void init_error_handler(void){
	errorState = 0;
	memset(canErrorMessage,0x00, 8);
	canErrorMessage[0] = 'E';
	canErrorMessage[0] = 'C';
	canErrorMessage[0] = 'U';
	canErrorMessage[0] = 'E';
	// Error frame starts with "ECUE" that stands for Electronic Control
	// Unit Error

	// CREATES Error Message Queue
	oErrorMsgQueue = osMessageQueueNew(ERROR_QUEUE_SIZE, sizeof(err_msg_t), NULL);
	if(oErrorMsgQueue == NULL) Error_Handler();

	// CREATES Error handler thread
	errorTaskHandle = osThreadNew(error_handler_task, NULL, &errorTask_attributes);
}

void error_handler_task(void *){


	while(1){
		uint32_t errorCount = osMessageQueueGetCount(oErrorMsgQueue);

		// All messages checked if they exist
		for(int msg = 0; msg < errorCount; msg ++){
			err_msg_t errMsg;
			osMessageQueueGet(oErrorMsgQueue, &errMsg, NULL, 1); // Won't need delays since it checks if there are messages
			if(errMsg.error_val == 0){
				clear_error(errMsg.component);
			}
			else {
				set_error(errMsg.component);
			}
		}

		if(errorState){
		// Add OS Timers later so it doesn't just send it
			if(osMutexAcquire(oCANMutex, osWaitForever) == osOK){
				memcpy(canErrorMessage + 4, &errorState, 4);
				send_CAN_buffer(ECU_ID, canErrorMessage);

				osMutexRelease(oCANMutex); // RELEASE THE MUTEX. IT BLOCKS ALL CAN IF YOU DON'T
				osDelay(1000); // If errors exist, they only get transmitted once every second
			}
		}
	} // While loop end


}

void set_error(enum fs_component_t err){
	errorState &= (1<<err);
}

uint32_t get_error(void){
	return errorState;
}


void clear_error(enum fs_component_t err){
	errorState |= ~(1<<err);
}
