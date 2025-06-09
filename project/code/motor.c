#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "uart.h"
#include "icar.h"

#define  MOTOR_SPEED_MAX		    1.5f	 	//电机最大转速(m/s) (0.017,8.04)

#define PWM_CH2                 (PWM1_MODULE3_CHB_D1)
//#define PWM_CH4                 (PWM2_MODULE3_CHB_D3)
int16 duty = 0;

MotorStruct motorStr;
extern PIDStruct pidStr;

/**
* @brief        电机初始化
* @param        
* @ref          
* @author       Rango
* @note         
**/
void motor_init()
{
		pwm_init(PWM_CH2, 17000, 0);// 初始化 PWM 通道 频率 17KHz 初始占空比 0%
		gpio_init(D3, GPO, 1, GPO_PUSH_PULL);//B9初始化为GPIO功能、输出模式、推挽输出
		pwm_set_duty(PWM_CH2, 0);
		
		motorStr.EncoderLine = 512.0f; 							//编码器线数=光栅数16*4				
    motorStr.ReductionRatio = 2.7f;							//电机减速比								
    motorStr.EncoderValue = 0;
    motorStr.DiameterWheel = 0.064f;//68cm					//轮子直径:m
    motorStr.CloseLoop = true;                              //默认闭环模式
}

/**
* @brief        电机PWM输出
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
				gpio_set_level(D3, 0);
        pwm_set_duty(PWM_CH2, pwm);
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
				gpio_set_level(D3, 1);
				pwm_set_duty(PWM_CH2, pwm);
    }	
}

/**
* @brief        电机闭环速控
* @param        speed：速度m/s
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
    
    pidStr.vi_Ref = (float)(speed*MOTOR_CONTROL_CYCLE / motorStr.DiameterWheel / PI * motorStr.EncoderLine * motorStr.ReductionRatio);//没有4倍频 * 4.0f
    
		duty = PID_MoveCalculate(&pidStr);
    Set_Pwm(duty);
}

/**
* @brief        电机控制线程
* @param        
* @ref          
* @author       Rango
* @note         
**/
void MOTOR_Timer(void)
{
    motorStr.Counter++;
    if(motorStr.Counter >= 20)							    //速控:10ms
    {
        encoder_get();								//编码器采样

        if(usbStr.connected)        //通信连接或电机测试才开启闭环（保护+省电）
        {
            if(motorStr.CloseLoop)
            {
                MOTOR_ControlLoop(icarStr.SpeedSet);		//闭环速控
            }
            else//开环百分比控制
            {
                if(icarStr.SpeedSet > 100)
                    icarStr.SpeedSet = 100;
                else if(icarStr.SpeedSet < -100)
                    icarStr.SpeedSet = -100;
                signed int speedRate = PWM_DUTY_MAX/100.f*icarStr.SpeedSet; //开环：百分比%
                
                Set_Pwm(speedRate);		//开环速控
            }
        }
        else
        {
            Set_Pwm(0);
        }
        motorStr.Counter = 0;
    }
}
