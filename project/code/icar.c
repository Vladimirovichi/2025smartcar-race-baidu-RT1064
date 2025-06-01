#include "icar.h"
#include "servo.h"

IcarStruct icarStr;

/**
* @Description  : 智能车参数初始化
* @params       : 
* @Date         : 
* @author       : Rango
* @notes        : 
**/
void ICAR_Init(void)
{
	icarStr.Electricity = 0;				    //电量信息
	icarStr.Voltage = 0;						//电压
	icarStr.SpeedSet = 0.0f;				    //电机目标速度：m/s	
	icarStr.SpeedFeedback = 0.0f;		        //电机模型实测速度：m/s
	icarStr.SpeedMaxRecords = 0.0f;
  icarStr.ServoPwmSet = servoStr.thresholdMiddle;
}