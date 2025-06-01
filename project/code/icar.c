#include "icar.h"
#include "servo.h"

IcarStruct icarStr;

/**
* @Description  : ���ܳ�������ʼ��
* @params       : 
* @Date         : 
* @author       : Rango
* @notes        : 
**/
void ICAR_Init(void)
{
	icarStr.Electricity = 0;				    //������Ϣ
	icarStr.Voltage = 0;						//��ѹ
	icarStr.SpeedSet = 0.0f;				    //���Ŀ���ٶȣ�m/s	
	icarStr.SpeedFeedback = 0.0f;		        //���ģ��ʵ���ٶȣ�m/s
	icarStr.SpeedMaxRecords = 0.0f;
  icarStr.ServoPwmSet = servoStr.thresholdMiddle;
}