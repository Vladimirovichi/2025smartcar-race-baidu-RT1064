#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "uart.h"
#include "icar.h"

#define  MOTOR_SPEED_MAX		    10.0f	 	//������ת��(m/s) (0.017,8.04)

#define PWM_CH2                 (PWM1_MODULE3_CHB_D1)
#define PWM_CH4                 (PWM2_MODULE3_CHB_D3)
int16 duty = 0;

MotorStruct motorStr;
extern PIDStruct pidStr;

/**
* @brief        �����ʼ��
* @param        
* @ref          
* @author       Rango
* @note         
**/
void motor_init()
{
		pwm_init(PWM_CH2, 17000, 0);// ��ʼ�� PWM ͨ�� Ƶ�� 17KHz ��ʼռ�ձ� 0%
    pwm_init(PWM_CH4, 17000, 0);// ��ʼ�� PWM ͨ�� Ƶ�� 17KHz ��ʼռ�ձ� 0%
		pwm_set_duty(PWM_CH2, 0);
		pwm_set_duty(PWM_CH4, 0);
		
		motorStr.EncoderLine = 512.0f; 							//����������=��դ��16*4				
    motorStr.ReductionRatio = 2.7f;							//������ٱ�								
    motorStr.EncoderValue = 0;
    motorStr.DiameterWheel = 0.064f;//68cm					//����ֱ��:m
    motorStr.CloseLoop = true;                              //Ĭ�ϱջ�ģʽ
}

/**
* @brief        ���PWM���
* @param        
* @ref          
* @author       Rango
* @note         
**/
void Set_Pwm(int pwm)//PWM_DUTY_MAX        (10000)  
{
    if(pwm >= 0)
    {				
        if(pwm > PWM_DUTY_MAX)
				{
						pwm = PWM_DUTY_MAX;
				}
        pwm_set_duty(PWM_CH2, pwm);
				pwm_set_duty(PWM_CH4, 0);
    }
    else if(pwm < 0)
    {
        if(pwm < -PWM_DUTY_MAX)
				{
						pwm = PWM_DUTY_MAX;
				}
				else
				{
						pwm = -pwm;
				}
				pwm_set_duty(PWM_CH2, 0);
				pwm_set_duty(PWM_CH4, pwm);
    }	
}

/**
* @brief        ����ջ��ٿ�
* @param        speed���ٶ�m/s
* @ref          
* @author       Rango
* @note         
**/
void MOTOR_ControlLoop(float speed)
{	
    if(speed > MOTOR_SPEED_MAX)
        speed = MOTOR_SPEED_MAX;
    else if(speed < -MOTOR_SPEED_MAX)
        speed = -MOTOR_SPEED_MAX;
    
    pidStr.vi_Ref = (float)(speed*MOTOR_CONTROL_CYCLE / motorStr.DiameterWheel / PI * motorStr.EncoderLine * motorStr.ReductionRatio);//û��4��Ƶ * 4.0f
    
    Set_Pwm(PID_MoveCalculate(&pidStr));
}

/**
* @brief        ��������߳�
* @param        
* @ref          
* @author       Rango
* @note         
**/
void MOTOR_Timer(void)
{
    motorStr.Counter++;
    if(motorStr.Counter >= 10)							    //�ٿ�:10ms
    {
        encoder_get();								//����������

        if(usbStr.connected)        //ͨ�����ӻ������Բſ����ջ�������+ʡ�磩
        {
            if(motorStr.CloseLoop)
            {
                MOTOR_ControlLoop(icarStr.SpeedSet);		//�ջ��ٿ�
            }
            else//�����ٷֱȿ���
            {
                if(icarStr.SpeedSet > 100)
                    icarStr.SpeedSet = 100;
                else if(icarStr.SpeedSet < -100)
                    icarStr.SpeedSet = -100;
                signed int speedRate = PWM_DUTY_MAX/100.f*icarStr.SpeedSet; //�������ٷֱ�%
                
                Set_Pwm(speedRate);		//�����ٿ�
            }
        }
        else
        {
            Set_Pwm(0);
        }
        motorStr.Counter = 0;
    }
}
