#include "servo.h"

#define SERVO_MOTOR1_PWM             (PWM4_MODULE2_CHA_C30)                          // ���������϶����Ӧ����
#define SERVO_MOTOR_FREQ            (100)                                           // ���������϶��Ƶ��  �����ע�ⷶΧ 50-300

// ------------------ ���ռ�ձȼ��㷽ʽ ------------------
// 
// �����Ӧ�� 0-180 ��Ƕȶ�Ӧ ��������� 0.5ms-2.5ms �ߵ�ƽ
// 
// ��ô��ͬƵ���µ�ռ�ձȼ��㷽ʽ����
// PWM_DUTY_MAX/(1000/freq)*(1+Angle/180) �� 50hz ʱ���� PWM_DUTY_MAX/(1000/50)*(1+Angle/180)
// 
// ��ô 100hz �� 90�ȵĴ�� ���ߵ�ƽʱ��1.5ms ��������Ϊ
// PWM_DUTY_MAX/(1000/100)*(1+90/180) = PWM_DUTY_MAX/10*1.5
// 
// ------------------ ���ռ�ձȼ��㷽ʽ ------------------
#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))//PWM_DUTY_MAX        (10000)

#if (SERVO_MOTOR_FREQ<50 || SERVO_MOTOR_FREQ>300)
    #error "SERVO_MOTOR_FREQ ERROE!"
#endif

//60~120
float servo_motor_duty = 86.0;                                                  // ��������Ƕ�
float servo_motor_dir = 1;                                                      // �������״̬

ServoStruct servoStr;

void Servo_Init()
{
		servoStr.thresholdLeft = 1833;// ���������϶�����Χ �Ƕ�
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