#ifndef __PID_H_
#define __PID_H_

#include "zf_driver_pwm.h"

/******************************************************************************/
#define PID_VKP					1.9f					//PID�궨ֵ
#define PID_VKI					0.2f					//PID�궨ֵ
#define PID_VKD					2.8f					//PID�궨ֵ

/**
* @brief    PID���
**/
typedef struct 
{
	float vi_Ref;        	            //�ٶ�PID���ٶ��趨ֵ
	float vi_FeedBack;                  //�ٶ�PID���ٶȷ���ֵ
    float vi_PreError;	 	            //�ٶ�PID���ٶ����,vi_Ref - vi_FeedBack
	float vi_PreDerror; 	            //�ٶ�PID��ǰһ�Σ��ٶ����֮�d_error-PreDerror;
	float v_Kp;	 		 			    //����ϵ����Kp = Kp
	float v_Ki;		 				    //����ϵ����Ki = Kp * ( T / Ti )
	float v_Kd;		 				    //΢��ϵ����Kd = KP * Td * T
	float vl_PreU; 				        //PID���ֵ
}PIDStruct;

void PID_Init(void);
signed int PID_MoveCalculate(PIDStruct *pp);


#endif