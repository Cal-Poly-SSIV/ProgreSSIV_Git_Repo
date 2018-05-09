#ifndef _LOOP_H_
#define _LOOP_H_

enum state {
	  INITIALIZE_PERIPHERALS,
    INITIALIZE_CONTROLLERS,
    WAIT_FOR_ARM,
    LINK_COMMUNICATION,
    RUNNING_NOMINALLY,
    RUNNING_SIMULINK,
    INDICATE_AND_LOG_ERROR,
    WAIT_FOR_CLEAR,
};

typedef enum state State;

#endif
