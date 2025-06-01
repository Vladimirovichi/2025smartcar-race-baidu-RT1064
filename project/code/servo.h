#ifndef __SERVO_H_
#define __SERVO_H_


#include "zf_driver_pwm.h"
#include "zf_driver_gpio.h"

/**
* @brief    ������
**/
typedef struct
{
	uint16_t thresholdMiddle;                   //�����ֵPWM
    uint16_t thresholdLeft;                     //�������ת�����ֵPWM
    uint16_t thresholdRight;                    //�������ת�����ֵPWM
}ServoStruct;

extern ServoStruct servoStr;

void Servo_Init();
void Servo_Control(float angle);

#endif