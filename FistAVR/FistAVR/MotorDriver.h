#ifndef _MOTOR_DRIVER_H_
#define _MOTOR_DRIVER_H_

#include <stdlib.h>

/**************************************************
 *		   Motor Driver API		  *
 **************************************************/

void Init_PWM();

void Set_Motor_Velocity( uint8_t motor_number, int16_t  velocity); // velocity range: -254 to +254.

#endif
