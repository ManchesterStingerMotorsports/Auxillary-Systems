#ifndef _HAL_CAN_H_
#define _HAL_CAN_H_

#include <stdarg.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#define LOG_STUFF(x)            printf(x)



/// @brief 
/// @param StdID 
/// @param buffer 
/// @return 
int send_CAN_buffer(int16_t StdID, uint8_t * buffer);



#ifdef __cplusplus
}
#endif


#endif
