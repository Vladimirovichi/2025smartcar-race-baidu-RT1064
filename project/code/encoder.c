#include "encoder.h"
#include "filter.h"
#include "motor.h"
#include "pid.h"
#include "icar.h"

int16 encoder1=0;//�˲�ǰ
int16 RC_encoder1; // �˲�֮��encoder��ֵ
float speed = 0;//���ģ��ʵ���ٶȣ�m/s
struct RC_Para Encoder1_Para = {0, 0, 0.1}; // ��ͨ�˲�
RC_Filter_pt RC_Encoder1 = &Encoder1_Para;

extern MotorStruct motorStr;
extern PIDStruct pidStr;

/**
* @brief        ��������ʼ��
* @param        
* @ref          
* @author       Rango
* @note         
**/
void encoder_init()
{
		encoder_quad_init(QTIMER1_ENCODER1,QTIMER1_ENCODER1_CH1_C0,QTIMER1_ENCODER1_CH2_C1);
}

/**
* @brief        ������ת�ٲ���
* @param        
* @ref          
* @author       Rango
* @note         
**/
void encoder_get()
{
		encoder1 = encoder_get_count(QTIMER1_ENCODER1);
		//PID��������������
		motorStr.EncoderValue = RC_encoder1 = (int16_t)RCFilter(encoder1, RC_Encoder1); // ��ͨ�˲�
		pidStr.vi_FeedBack = motorStr.EncoderValue;
		
		icarStr.SpeedFeedback = (float)(motorStr.EncoderValue * PI * motorStr.DiameterWheel)/ MOTOR_CONTROL_CYCLE / motorStr.EncoderLine / motorStr.ReductionRatio; /// 4.0f   m/s
		encoder_clear_count(QTIMER1_ENCODER1);
}