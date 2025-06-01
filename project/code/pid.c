#include "pid.h"

PIDStruct pidStr;


/**
* @brief        PID参数初始化
* @param        
* @ref          
* @author       Leo
* @note         
**/
void PID_Init(void)          
{	
    pidStr.vi_Ref = 0 ;	      	 
    pidStr.vi_FeedBack = 0 ;		
    pidStr.vi_PreError = 0 ;	  	
    pidStr.vi_PreDerror = 0 ;	  	    
    pidStr.v_Kp = PID_VKP;       
    pidStr.v_Ki = PID_VKI; 
    pidStr.v_Kd = PID_VKD;                
    pidStr.vl_PreU = 0;		
}

signed int PID_MoveCalculate(PIDStruct *pp)                        
{    
    float  error,d_error,dd_error; 
			 
    error = pp->vi_Ref - pp->vi_FeedBack;
    d_error = error - pp->vi_PreError;
    dd_error = d_error - pp->vi_PreDerror;          
        
    pp->vi_PreError = error;		                     
    pp->vi_PreDerror = d_error;
	
		pp->vl_PreU += (pp -> v_Kp * d_error + pp -> v_Ki * error + pp->v_Kd*dd_error);
	
    if( pp->vl_PreU >= PWM_DUTY_MAX )
    {
        pp->vl_PreU = PWM_DUTY_MAX;
    }
    else if( pp->vl_PreU <= -PWM_DUTY_MAX )	
    {
        pp->vl_PreU = -PWM_DUTY_MAX;
    }
                                             
    return (pp->vl_PreU);
} 
