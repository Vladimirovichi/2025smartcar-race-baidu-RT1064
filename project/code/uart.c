#include "uart.h"
#include "icar.h"
#include "motor.h"
#include "servo.h"

uint8 uart_get_data[64];                                                        // 串口接收数据缓冲区
uint8 fifo_get_data[64];                                                        // fifo 输出读出缓冲区

extern int16_t data1;  // 视觉识别X坐标
extern int16_t data2;  // 视觉识别Y坐标

uint8 get_data = 0;                                                             // 接收数据变量
uint32 fifo_data_count = 0;                                                     // fifo 数据个数

fifo_struct uart_data_fifo;
UsbStruct usbStr;

void Uart_Init()
{
		fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, uart_get_data, 64);              // 初始化 fifo 挂载缓冲区

    uart_init(UART_INDEX, UART_BAUDRATE, UART_TX_PIN, UART_RX_PIN);             // 初始化编码器模块与引脚 正交解码编码器模式
    uart_rx_interrupt(UART_INDEX, ZF_ENABLE);                                   // 开启 UART_INDEX 的接收中断
    interrupt_set_priority(UART_PRIORITY, 5);                                   // 设置对应 UART_INDEX 的中断优先级为 0
		
//		uart_write_string(UART_INDEX, "UART Text.");                                // 输出测试信息
//    uart_write_byte(UART_INDEX, '\r');                                          // 输出回车
//    uart_write_byte(UART_INDEX, '\n');                                          // 输出换行

		//USB数据初始化
    usbStr.counter = 0;
    usbStr.receiveFinished = false;
    usbStr.receiveStart = false;
    usbStr.receiveIndex = 0;
    usbStr.connected = false;
    usbStr.inspectorEnable = false;
}

void Uart_Read()//测试用
{
//		fifo_data_count = fifo_used(&uart_data_fifo);                           // 查看 fifo 是否有数据
//		if(fifo_data_count != 0)                                                // 读取到数据了
//		{
//				fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);    // 将 fifo 中数据读出并清空 fifo 挂载的缓冲
//				uart_write_string(UART_INDEX, "\r\nUART get data:");                // 输出测试信息
//				uart_write_buffer(UART_INDEX, fifo_get_data, fifo_data_count);      // 将读取到的数据发送出去
//		}
		
//    fifo_data_count = fifo_used(&uart_data_fifo);  // 查看 fifo 是否有数据

//    if (fifo_data_count >= 6)
//    {
//        // 读取前4字节（不清除 FIFO）
//        fifo_data_count = 6;
//        fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_ONLY);

//        // 检查帧头和帧尾
//        if (fifo_get_data[0] == 0xAA && fifo_get_data[5] == 0x55)
//        {
//            data1 = ((uint16_t)fifo_get_data[1] << 8) | fifo_get_data[2];
//            data2 = ((uint16_t)fifo_get_data[3] << 8) | fifo_get_data[4];

//            // 消费已读取的 4 字节（清除这部分数据）
//            fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);
//        }
//        else
//        {
//            // 如果不是有效帧头，丢弃第1字节，继续下一轮检测
//            uint8_t discard;
//            fifo_data_count = 1;
//            fifo_read_buffer(&uart_data_fifo, &discard, &fifo_data_count, FIFO_READ_AND_CLEAN);
//        }
//    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART_INDEX 的接收中断处理函数 这个函数将在 UART_INDEX 对应的中断调用 详见 isr.c
// 参数说明     void
// 返回参数     void
// 使用示例     uart_rx_interrupt_handler();
//-------------------------------------------------------------------------------------------------------------------
void uart_rx_interrupt_handler (void)
{ 
		if(uart_query_byte(UART_INDEX, &get_data))// 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE
		{
				if(get_data == USB_FRAME_HEAD && !usbStr.receiveStart)//监测帧头
        {
            usbStr.receiveStart = true;
            usbStr.receiveBuff[0] = get_data;
            usbStr.receiveBuff[2] = USB_FRAME_LENMIN;
            usbStr.receiveIndex = 1;
        }
				else if(usbStr.receiveIndex == 2)	//接收帧长度
        {
            usbStr.receiveBuff[usbStr.receiveIndex] = get_data;
            usbStr.receiveIndex++;
            
            if(get_data > USB_FRAME_LENMAX || get_data < USB_FRAME_LENMIN) //帧长错误
            {
                usbStr.receiveBuff[2] = USB_FRAME_LENMIN;
                usbStr.receiveIndex = 0;
                usbStr.receiveStart = false;
            }
        }
        else if(usbStr.receiveStart && usbStr.receiveIndex < USB_FRAME_LENMAX)
        {
            usbStr.receiveBuff[usbStr.receiveIndex] = get_data;
            usbStr.receiveIndex++;
        }
				
				 //接收帧完毕
        if((usbStr.receiveIndex >= USB_FRAME_LENMAX || usbStr.receiveIndex >= usbStr.receiveBuff[2]) && usbStr.receiveIndex > USB_FRAME_LENMIN)
        {
            uint8_t check = 0;
            uint8_t length = USB_FRAME_LENMIN;
        
            length = usbStr.receiveBuff[2];
            for(int i=0;i<length-1;i++)
                check += usbStr.receiveBuff[i];
            
            if(check == usbStr.receiveBuff[length-1])//校验位
            {
                memcpy(usbStr.receiveBuffFinished,usbStr.receiveBuff,USB_FRAME_LENMAX);	
                usbStr.receiveFinished = true;
                
                //智能车控制指令特殊处理（保障实时性）
                if(USB_ADDR_CONTROL  == usbStr.receiveBuffFinished[1])
                {
                    Bint16_Union bint16_Union;
                    Bint32_Union bint32_Union;
                    for(int i=0;i<4;i++)
                        bint32_Union.U8_Buff[i] = usbStr.receiveBuffFinished[3+i];
                    
                    bint16_Union.U8_Buff[0] = usbStr.receiveBuffFinished[7];
                    bint16_Union.U8_Buff[1] = usbStr.receiveBuffFinished[8];
                    
                    Servo_Control((float)bint16_Union.U16);
                    icarStr.ServoPwmSet = bint16_Union.U16;         //方向
                    icarStr.SpeedSet = bint32_Union.Float;          //速度				
                }
				
                if(!usbStr.connected)//上位机初次连接通信
                {
                    usbStr.connected = true;
                }
                
                usbStr.counterDrop = 0;
            }
            
            usbStr.receiveIndex = 0;
            usbStr.receiveStart = false;
        }
		}
}

/**
* @brief        监测软件线程控制器
* @param        
* @ref          
* @author       Rango
* @note         
**/
void USB_Edgeboard_Timr(void)
{
    if(usbStr.connected)//USB通信掉线检测
    {
        usbStr.counterDrop++;
        if(usbStr.counterDrop >3000)//3s
        {
            usbStr.connected = false;
            usbStr.inspectorEnable = false;
            icarStr.selfcheckEnable = false;
        }
        
        if(usbStr.inspectorEnable)
        {
            usbStr.counterSend++;
        }
    }
}

/**
* @brief        USB通信处理函数
* @param        
* @ref          
* @author       Rango
* @note         
**/
void USB_Edgeboard_Handle(void)
{
    if(usbStr.receiveFinished)																//接收成功
    {
        usbStr.receiveFinished = false;
           
        if(usbStr.receiveBuffFinished[1] & 0x80)	//读数据
        {
            uint8_t Addr = (uint8_t)(usbStr.receiveBuffFinished[1] & 0x7F);
            switch(Addr)
            {
                case USB_ADDR_BATTERY :             //电池信息
                    break;
                
                case USB_ADDR_SERVOTHRESHOLD :      //舵机阈值
                    break;
            }
        }
        else //写数据
        {
            switch(usbStr.receiveBuffFinished[1])
            {

                case USB_ADDR_SPEEDMODE:        //速控模式切换
                    if(usbStr.receiveBuffFinished[3] == 1)    //开环模式
                        motorStr.CloseLoop = false;                    
                    else
                        motorStr.CloseLoop = true;
                    
                    icarStr.SpeedSet = 0;
                    break;
                
                    
                //-----------------------------[自检软件相关]-------------------------------------------
                case USB_ADDR_INSPECTOR :           //自检软件心跳
                    usbStr.inspectorEnable = true;
                    break; 
            }      
            
        }
    }
		
		//-----------------------[自检软件数据发送]-----------------------------
    if(usbStr.inspectorEnable && usbStr.connected && usbStr.counterSend > 150)//150ms
    {
        USB_Edgeboard_CarSpeed();               //发送车速
        usbStr.counterSend = 0; 
    }
}

/**
* @brief        USB发送按键信号
* @param        time: 按键时长
* @ref
* @author       Rango
* @note
**/
void USB_Edgeboard_TransmitKey(uint16_t time)
{
    uint8_t check = 0;
    uint8_t buff[8] = {0};
    Bint16_Union bint16_Union;
    
    buff[0] = 0x42; //帧头
    buff[1] = USB_ADDR_KEYINPUT; //地址
    buff[2] = 0x06; //帧长

    bint16_Union.U16 = time;
    buff[3] = bint16_Union.U8_Buff[0];
    buff[4] = bint16_Union.U8_Buff[1];
    
    for(int i=0;i<5;i++)
        check += buff[i];

    buff[5] = check;

		uart_write_buffer(UART_INDEX, &buff[0], 8);
}

/**
* @brief        发送车速信息
* @ref          
* @author       Leo
* @note         
**/
void USB_Edgeboard_CarSpeed(void)
{
    Bint32_Union bint32_Union;
    uint8_t check = 0;
    uint8_t buff[10];
    buff[0] = 0x42; //帧头
    buff[1] = USB_ADDR_SPEEDBACK; //地址
    buff[2] = 0x08; //帧长
        
    bint32_Union.Float = icarStr.SpeedFeedback;
    buff[3] = bint32_Union.U8_Buff[0];
    buff[4] = bint32_Union.U8_Buff[1];
    buff[5] = bint32_Union.U8_Buff[2];
    buff[6] = bint32_Union.U8_Buff[3];

    for(int i=0;i<7;i++)
        check += buff[i];
    
    buff[7] = check;
    
    uart_write_buffer(UART_INDEX, &buff[0], 10);
}