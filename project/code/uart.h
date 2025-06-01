#ifndef __UART_H_
#define __UART_H_


#include "zf_driver_uart.h"
#include "zf_driver_gpio.h"
#include "zf_common_fifo.h"
#include "zf_common_debug.h"
#include "zf_common_interrupt.h"

#define UART_INDEX              (DEBUG_UART_INDEX   )                           // 默认 UART_1
#define UART_BAUDRATE           (DEBUG_UART_BAUDRATE)                           // 默认 115200
#define UART_TX_PIN             (DEBUG_UART_TX_PIN  )                           // 默认 UART1_TX_A9
#define UART_RX_PIN             (DEBUG_UART_RX_PIN  )                           // 默认 UART1_RX_A10

#define UART_PRIORITY           (LPUART1_IRQn)                                  // 对应串口中断的中断编号 在 MIMXRT1064.h 头文件中查看 IRQn_Type 枚举体


#define USB_FRAME_HEAD				 0x42				 //USB通信序列帧头
#define USB_FRAME_LENMIN  			 4					 //USB通信序列字节最短长度
#define USB_FRAME_LENMAX			 30					 //USB通信序列字节最长长度

#define USB_ADDR_HEART               0x00                //监测软件心跳
#define USB_ADDR_CONTROL             0x01                //智能车控制
#define USB_ADDR_SPEEDMODE           0x02                //速控模式
#define USB_ADDR_SERVOTHRESHOLD      0x03                //舵机阈值
#define USB_ADDR_BUZZER              0x04                //蜂鸣器音效
#define USB_ADDR_LIGHT               0x05                //LED灯效
#define USB_ADDR_KEYINPUT            0x06                //按键输入
#define USB_ADDR_BATTERY             0x07                //电池信息

#define USB_ADDR_INSPECTOR           0x0A                //智能车自检软件连接心跳
#define USB_ADDR_SPEEDBACK           0x08                //车速信息反馈：m/s

typedef struct 
{
	bool receiveStart;									    //数据接收开始
	uint8_t receiveIndex;									//接收序列
	bool receiveFinished;									//数据队列接收并校验完成
	uint8_t receiveBuff[USB_FRAME_LENMAX];	                //USB接收队列：临时接收
	uint8_t receiveBuffFinished[USB_FRAME_LENMAX];	        //USB接收队列：校验成功
	uint16_t counter;										//计数器
    uint16_t counterDrop;                                   //掉线计数器    
    
    uint16_t counterSend;                                   //自检数据发送计数器
    bool connected;                                         //上位机通信连接状态
    bool inspectorEnable;                                   //智能汽车自检软件连接使能
}UsbStruct;

typedef union 
{
	uint8_t U8_Buff[2];
	uint16_t U16;
	int16_t S16;
}Bint16_Union;
	
typedef union 
{
	uint8_t U8_Buff[4];
	float Float;
    unsigned long U32;
}Bint32_Union;

extern UsbStruct usbStr;

void Uart_Init();
void Uart_Read();
void USB_Edgeboard_Timr(void);
void USB_Edgeboard_Handle(void);
void USB_Edgeboard_TransmitKey(uint16_t time);
void USB_Edgeboard_CarSpeed(void);

#endif