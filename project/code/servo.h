#ifndef __SERVO_H_
#define __SERVO_H_


#include "zf_driver_pwm.h"
#include "zf_driver_gpio.h"

/**
* @brief    舵机相关
**/
typedef struct
{
	uint16_t thresholdMiddle;                   //舵机中值PWM
    uint16_t thresholdLeft;                     //舵机左向转角最大值PWM
    uint16_t thresholdRight;                    //舵机右向转角最大值PWM
}ServoStruct;

extern ServoStruct servoStr;

void Servo_Init();
void Servo_Control(float angle);

#endif