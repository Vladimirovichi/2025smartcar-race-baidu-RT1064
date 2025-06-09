#ifndef __MOTOR_H_
#define __MOTOR_H_


#include "zf_driver_pwm.h"
#include "zf_driver_gpio.h"

typedef struct 
{
	float ReductionRatio ;					    //������ٱ�
	float EncoderLine ; 						//����������=��դ��16*4
	signed int EncoderValue;				    //������ʵʱ�ٶ�
	float DiameterWheel;						//����ֱ����mm
	bool CloseLoop;							    //����ģʽ
	uint16_t Counter;							//�̼߳�����
}MotorStruct;

void motor_init();
void Set_Pwm(int pwm);//PWM_DUTY_MAX        (10000)
void MOTOR_ControlLoop(float speed);
void MOTOR_Timer(void);
void Set_Pwm(int pwm);

extern MotorStruct motorStr;

#endif