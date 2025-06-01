#ifndef __PID_H_
#define __PID_H_

#include "zf_driver_pwm.h"

/******************************************************************************/
#define PID_VKP					1.9f					//PID标定值
#define PID_VKI					0.2f					//PID标定值
#define PID_VKD					2.8f					//PID标定值

/**
* @brief    PID相关
**/
typedef struct 
{
	float vi_Ref;        	            //速度PID，速度设定值
	float vi_FeedBack;                  //速度PID，速度反馈值
    float vi_PreError;	 	            //速度PID，速度误差,vi_Ref - vi_FeedBack
	float vi_PreDerror; 	            //速度PID，前一次，速度误差之差，d_error-PreDerror;
	float v_Kp;	 		 			    //比例系数，Kp = Kp
	float v_Ki;		 				    //积分系数，Ki = Kp * ( T / Ti )
	float v_Kd;		 				    //微分系数，Kd = KP * Td * T
	float vl_PreU; 				        //PID输出值
}PIDStruct;

void PID_Init(void);
signed int PID_MoveCalculate(PIDStruct *pp);


#endif