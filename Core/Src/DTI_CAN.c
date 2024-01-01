#include "DTI_CAN.h"
#include "HAL_CAN.h"

#include <stdio.h>
#include <string.h>
//#include "esp_err.h"
//#include "esp_log.h"


int parse_CAN_message(DTI_CAN* can_struct, uint16_t ID, uint8_t* message){
    uint8_t PID = ID>>8;
    int ret_val = 0;
    switch(PID){
        case(0x20):
        can_struct->ERPM = message[0]<<24|message[1]<<16|message[2]<<8|message[3];
        can_struct->duty_C = (message[4]<<8|message[5])/10;
        can_struct->ip_voltage = (message[6]<<8|message[7]);

        break;
        case(0x21):
        can_struct->ac_curr = (message[0]<<8|message[1])/10;
        can_struct->dc_curr = (message[2]<<8|message[3])/10;
        

        break;
        case(0x22):
        can_struct->uc_temp = (message[0]<<8|message[1])/10;
        can_struct->motor_temp = (message[2]<<8|message[3])/10;
        can_struct->fault_code = message[4];

        break;
        case(0x23):
        can_struct->foc_id = (message[0]<<24|message[1]<<16|message[2]<<8|message[3])/100;
        can_struct->foc_iq = (message[4]<<24|message[5]<<16|message[6]<<8|message[7])/100;

        break;
        case(0x24):
        can_struct->throttle = message[0];
        can_struct->brake = message[1];
        can_struct->digital_io = message[2];
        can_struct->drive_en = message[3];
        can_struct->limit_var_0 = message[4];
        can_struct->limit_var_1 = message[5];

        break;

        default:
        ret_val =  1;
        break;

    }
    return ret_val;
}

int send_drive_en(DTI_CAN can_s){
    uint16_t StId = 0x0C<<5|can_s.node_id;
    uint8_t buffer[8];
    memset(&buffer[1], 0xFF, 7);
    buffer[0] = 1;

    return send_CAN_buffer(StId, buffer);
}


int set_dti_erpm(DTI_CAN can_s,  int32_t erpm){
    uint16_t StId = 0x03<<5|can_s.node_id;
    uint8_t buffer[8];
    memset(buffer, 0xFF, 8);
    buffer[0] = (uint8_t)erpm;
    buffer[1] = (uint8_t)(erpm>>8);
    buffer[2] = (uint8_t)(erpm>>16);
    buffer[3] = (uint8_t)(erpm>>24);

    return send_CAN_buffer(StId, buffer);
}
