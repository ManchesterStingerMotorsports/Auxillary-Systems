/*
 * apps.h
 *
 *  Created on: Nov 2, 2024
 *      Author: shakeel
 */

#ifndef INC_APPS_H_
#define INC_APPS_H_

// ===========================

char* APPS_read_verify(void);
void DifferenceTimerCallback(void *argument); // shutdown circuit when timer > 500 ms if APPS had a difference > 10%

uint16_t calculate_average(uint16_t* buffer, size_t length); // Calculates average value stored in ADC_buffer


#endif /* INC_APPS_H_ */
