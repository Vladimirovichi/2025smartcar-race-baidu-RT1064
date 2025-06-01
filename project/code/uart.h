#ifndef __UART_H_
#define __UART_H_


#include "zf_driver_uart.h"
#include "zf_driver_gpio.h"
#include "zf_common_fifo.h"
#include "zf_common_debug.h"
#include "zf_common_interrupt.h"

#define UART_INDEX              (DEBUG_UART_INDEX   )                           // Ĭ�� UART_1
#define UART_BAUDRATE           (DEBUG_UART_BAUDRATE)                           // Ĭ�� 115200
#define UART_TX_PIN             (DEBUG_UART_TX_PIN  )                           // Ĭ�� UART1_TX_A9
#define UART_RX_PIN             (DEBUG_UART_RX_PIN  )                           // Ĭ�� UART1_RX_A10

#define UART_PRIORITY           (LPUART1_IRQn)                                  // ��Ӧ�����жϵ��жϱ�� �� MIMXRT1064.h ͷ�ļ��в鿴 IRQn_Type ö����


#define USB_FRAME_HEAD				 0x42				 //USBͨ������֡ͷ
#define USB_FRAME_LENMIN  			 4					 //USBͨ�������ֽ���̳���
#define USB_FRAME_LENMAX			 30					 //USBͨ�������ֽ������

#define USB_ADDR_HEART               0x00                //����������
#define USB_ADDR_CONTROL             0x01                //���ܳ�����
#define USB_ADDR_SPEEDMODE           0x02                //�ٿ�ģʽ
#define USB_ADDR_SERVOTHRESHOLD      0x03                //�����ֵ
#define USB_ADDR_BUZZER              0x04                //��������Ч
#define USB_ADDR_LIGHT               0x05                //LED��Ч
#define USB_ADDR_KEYINPUT            0x06                //��������
#define USB_ADDR_BATTERY             0x07                //�����Ϣ

#define USB_ADDR_INSPECTOR           0x0A                //���ܳ��Լ������������
#define USB_ADDR_SPEEDBACK           0x08                //������Ϣ������m/s

typedef struct 
{
	bool receiveStart;									    //���ݽ��տ�ʼ
	uint8_t receiveIndex;									//��������
	bool receiveFinished;									//���ݶ��н��ղ�У�����
	uint8_t receiveBuff[USB_FRAME_LENMAX];	                //USB���ն��У���ʱ����
	uint8_t receiveBuffFinished[USB_FRAME_LENMAX];	        //USB���ն��У�У��ɹ�
	uint16_t counter;										//������
    uint16_t counterDrop;                                   //���߼�����    
    
    uint16_t counterSend;                                   //�Լ����ݷ��ͼ�����
    bool connected;                                         //��λ��ͨ������״̬
    bool inspectorEnable;                                   //���������Լ��������ʹ��
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