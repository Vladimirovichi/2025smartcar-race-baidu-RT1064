#ifndef __MOTOR_H_
#define __MOTOR_H_


#include "zf_driver_pwm.h"
#include "zf_driver_gpio.h"

typedef struct 
{
	float ReductionRatio ;					    //电机减速比
	float EncoderLine ; 						//编码器线数=光栅数16*4
	signed int EncoderValue;				    //编码器实时速度
	float DiameterWheel;						//轮子直径：mm
	bool CloseLoop;							    //开环模式
	uint16_t Counter;							//线程计数器
}MotorStruct;

void motor_init();
void Set_Pwm(int pwm);//PWM_DUTY_MAX        (10000)
void MOTOR_ControlLoop(float speed);
void MOTOR_Timer(void);
void Set_Pwm(int pwm);

extern MotorStruct motorStr;

#endif