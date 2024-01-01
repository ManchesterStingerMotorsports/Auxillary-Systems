#ifndef _DTI_CAN_H_
#define _DTI_CAN_H_
#include <stdint.h>
//#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
class DTI_CAN {
    public:
    uint8_t node_id;
    int32_t ERPM; //Electrical RPM 
    float duty_C;
    int16_t ip_voltage;
    float ac_curr;
    float dc_curr;
    float uc_temp;
    float motor_temp;
    uint8_t fault_code;
    float foc_id;
    float foc_iq;
    int8_t throttle;
    int8_t brake;
    uint8_t digital_io;
    uint8_t drive_en;
    uint8_t limit_var_0;
    uint8_t limit_var_1;
    uint8_t CAN_map_ver; 

    DTI_CAN(uint8_t ID){
        node_id = ID;
    }
    

}   
#endif

/// @brief Struct with all the controller parameters
typedef struct _dti_can{
    uint8_t node_id; //1
    int32_t ERPM; //2 Electrical RPM
    float duty_C; //3
    int16_t ip_voltage; //4
    float ac_curr;//5
    float dc_curr;//6
    float uc_temp;//7
    float motor_temp;//8
    uint8_t fault_code;//9
    float foc_id;//10
    float foc_iq;//11
    int8_t throttle;//12
    int8_t brake;//13
    uint8_t digital_io;//14
    uint8_t drive_en;//15
    uint8_t limit_var_0;//16
    uint8_t limit_var_1;//17
    uint8_t CAN_map_ver; //18
    

} DTI_CAN;

#define DTI_CAN_INIT	{0, 0, 0.0, 0,		\
						0.0, 0.0, 0.0, 0.0,	\
						0, 0.0, 0.0, 0, 	\
						0, 0, 0, 0,			\
						0, 0}


/// @brief Parse input can message to 
/// @param can_struct Struct with all the input 
/// @param ID 
/// @param message 
/// @return 0 on success and -1 on failure
int parse_CAN_message(DTI_CAN* can_struct, uint16_t ID, uint8_t* message);

/// @brief Enable drive for the dti controller
/// @param  
/// @return 
int send_drive_en(DTI_CAN can_s);

/// @brief Set throttle
/// @param throttle 
/// @return 0 on success and -1 on failure
//int set_dti_throttle(int8_t throttle);

/// @brief Set desired AC current
/// @param current desired current
/// @return 
//int set_dti_current(int16_t current);

/// @brief Set erpm 
/// @param erpm 
/// @return 
int set_dti_erpm(DTI_CAN can_s,  int32_t erpm);

/// @brief Set digital output for your dti controller 
/// @param output desired output
/// @return 
//int set_dti_digital_o(uint8_t output);







#ifdef __cplusplus
}
#endif

#endif 
