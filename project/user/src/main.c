/*********************************************************************************************************************
* RT1064DVL6A Opensourec Library 即（RT1064DVL6A 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 RT1064DVL6A 开源库的一部分
* 
* RT1064DVL6A 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 8.32.4 or MDK 5.33
* 适用平台          RT1064DVL6A
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2022-09-21        SeekFree            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"

// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完

// 本例程是开源库移植用空工程

#define PIT_CH                  (PIT_CH0 )                                      // 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define PIT_PRIORITY            (PIT_IRQn)                                      // 对应周期中断的中断编号
int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  	// 不可删除
    //debug_init();                   // 调试端口初始化

    // 此处编写用户代码 例如外设初始化代码等
		wireless_uart_init();																									// 初始化无线转串口   
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIRELESS_UART);	// 设置逐飞助手使用无线转串口进行收发 
    seekfree_assistant_oscilloscope_struct oscilloscope_data;							// 初始化逐飞助手示波器的结构体
    oscilloscope_data.channel_num = 4;																		// 设置为4个通道，通道数量最大为8个
	
	
		motor_init();
		encoder_init();
		PID_Init();
		Servo_Init();
		Uart_Init();
		ICAR_Init();
		Key_Init();
		
		pit_ms_init(PIT_CH, 1);                                                  // 初始化 PIT_CH0 为周期中断 1ms 周期
    interrupt_set_priority(PIT_PRIORITY, 0);                                    // 设置 PIT1 对周期中断的中断优先级为 0
		
		// 此处编写用户代码 例如外设初始化代码等
    while(1)
    {
				Key_Scan();
				USB_Edgeboard_Handle();
        // 此处编写需要循环执行的代码
//				encoder_get();
//				MOTOR_ControlLoop(0.5f);		//闭环速控
//				Set_Pwm(1000);
				oscilloscope_data.data[0] = motorStr.EncoderValue;
				oscilloscope_data.data[1] = 0;
				oscilloscope_data.data[2] = 0;
				oscilloscope_data.data[3] = 0;
						
        seekfree_assistant_oscilloscope_send(&oscilloscope_data);// 通过无线转串口发送到虚拟示波器上			
				
//				system_delay_ms(10);
        // 此处编写需要循环执行的代码
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 的周期中断处理函数 这个函数将在 PIT 对应的定时器中断调用 详见 isr.c
// 参数说明     void
// 返回参数     void
// 使用示例     pit_handler();
//-------------------------------------------------------------------------------------------------------------------
void pit_handler (void)
{
    //gpio_toggle_level(B9);                                                              // 周期中断触发
		MOTOR_Timer();
		USB_Edgeboard_Timr();
}
