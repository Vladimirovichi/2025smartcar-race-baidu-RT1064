#include "servo.h"

#define SERVO_MOTOR1_PWM             (PWM4_MODULE2_CHA_C30)                          // 定义主板上舵机对应引脚
#define SERVO_MOTOR_FREQ            (100)                                           // 定义主板上舵机频率  请务必注意范围 50-300

// ------------------ 舵机占空比计算方式 ------------------
// 
// 舵机对应的 0-180 活动角度对应 控制脉冲的 0.5ms-2.5ms 高电平
// 
// 那么不同频率下的占空比计算方式就是
// PWM_DUTY_MAX/(1000/freq)*(1+Angle/180) 在 50hz 时就是 PWM_DUTY_MAX/(1000/50)*(1+Angle/180)
// 
// 那么 100hz 下 90度的打角 即高电平时间1.5ms 计算套用为
// PWM_DUTY_MAX/(1000/100)*(1+90/180) = PWM_DUTY_MAX/10*1.5
// 
// ------------------ 舵机占空比计算方式 ------------------
#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))//PWM_DUTY_MAX        (10000)

#if (SERVO_MOTOR_FREQ<50 || SERVO_MOTOR_FREQ>300)
    #error "SERVO_MOTOR_FREQ ERROE!"
#endif

//60~120
float servo_motor_duty = 86.0;                                                  // 舵机动作角度
float servo_motor_dir = 1;                                                      // 舵机动作状态

ServoStruct servoStr;

void Servo_Init()
{
		servoStr.thresholdLeft = 1833;// 定义主板上舵机活动范围 角度
		servoStr.thresholdRight = 1167;
		servoStr.thresholdMiddle = 1456;
		pwm_init(SERVO_MOTOR1_PWM, SERVO_MOTOR_FREQ, 0);
		pwm_set_duty(SERVO_MOTOR1_PWM, (uint32)servoStr.thresholdMiddle);
		
}
    
void Servo_Control(float angle)
{
		if(angle > servoStr.thresholdLeft)
		{
				angle = servoStr.thresholdLeft;
		}
		else if(angle < servoStr.thresholdRight)
		{
				angle = servoStr.thresholdRight;
		}
		
		pwm_set_duty(SERVO_MOTOR1_PWM, (uint32)angle);
}