#include "HAL_CAN.h"
#include "main.h"

#include "stm32f4xx_hal_can.h"

extern CAN_HandleTypeDef hcan1;

extern uint32_t TxMailbox;

CAN_TxHeaderTypeDef TxHeader;

int send_CAN_buffer(int16_t StdID, uint8_t * buffer){
	TxHeader.StdId = StdID;


	if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, buffer, &TxMailbox) != HAL_OK)
	{
	   Error_Handler ();
	   return -1;
	}
	return 0;
}
