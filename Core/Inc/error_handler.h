#ifndef _ERROR_HANDLER_H_
#define _ERROR_HANDLER_H_
#include "main.h"
#include "cmsis_os.h"

enum fs_error_t{
	NO_ERROR = 0,
	BMS_FAILURE = 1,
	CONTROLLER_FAILURE,
	APPS_FAILURE,
	BRAKE_FAILURE,
};

enum fs_component_t{
	NONE = 0,
	BMS = 1,
	CONTROLLER = 2,
	ACCEL  = 3,
	BRAKE = 4,
};

typedef struct _error_message{
	int error_val; // Not used yet. Will be useful for future diagnosis
	enum fs_component_t component; // To know where the error is coming from
}err_msg_t;


void init_error_handler(void);

void error_handler_task(void*);

void set_error(enum fs_component_t);

uint32_t get_error(void);

void clear_error(enum fs_component_t);


#endif
