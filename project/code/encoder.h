#ifndef __ENCODER_H_
#define __ENCODER_H_

#include "zf_common_typedef.h"
#include "zf_driver_encoder.h"
#include "zf_device_gnss.h"

/**
* @brief    电机相关
**/
//#define  PI					        3.141593f   //π
#define  MOTOR_CONTROL_CYCLE	    0.01f    	//电机控制周期T：10ms




void encoder_init();
void encoder_get();

#endif