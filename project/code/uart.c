#include "uart.h"
#include "icar.h"
#include "motor.h"
#include "servo.h"

uint8 uart_get_data[64];                                                        // ���ڽ������ݻ�����
uint8 fifo_get_data[64];                                                        // fifo �������������

extern int16_t data1;  // �Ӿ�ʶ��X����
extern int16_t data2;  // �Ӿ�ʶ��Y����

uint8 get_data = 0;                                                             // �������ݱ���
uint32 fifo_data_count = 0;                                                     // fifo ���ݸ���

fifo_struct uart_data_fifo;
UsbStruct usbStr;

void Uart_Init()
{
		fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, uart_get_data, 64);              // ��ʼ�� fifo ���ػ�����

    uart_init(UART_INDEX, UART_BAUDRATE, UART_TX_PIN, UART_RX_PIN);             // ��ʼ��������ģ�������� �������������ģʽ
    uart_rx_interrupt(UART_INDEX, ZF_ENABLE);                                   // ���� UART_INDEX �Ľ����ж�
    interrupt_set_priority(UART_PRIORITY, 5);                                   // ���ö�Ӧ UART_INDEX ���ж����ȼ�Ϊ 0
		
//		uart_write_string(UART_INDEX, "UART Text.");                                // ���������Ϣ
//    uart_write_byte(UART_INDEX, '\r');                                          // ����س�
//    uart_write_byte(UART_INDEX, '\n');                                          // �������

		//USB���ݳ�ʼ��
    usbStr.counter = 0;
    usbStr.receiveFinished = false;
    usbStr.receiveStart = false;
    usbStr.receiveIndex = 0;
    usbStr.connected = false;
    usbStr.inspectorEnable = false;
}

void Uart_Read()//������
{
//		fifo_data_count = fifo_used(&uart_data_fifo);                           // �鿴 fifo �Ƿ�������
//		if(fifo_data_count != 0)                                                // ��ȡ��������
//		{
//				fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);    // �� fifo �����ݶ�������� fifo ���صĻ���
//				uart_write_string(UART_INDEX, "\r\nUART get data:");                // ���������Ϣ
//				uart_write_buffer(UART_INDEX, fifo_get_data, fifo_data_count);      // ����ȡ�������ݷ��ͳ�ȥ
//		}
		
//    fifo_data_count = fifo_used(&uart_data_fifo);  // �鿴 fifo �Ƿ�������

//    if (fifo_data_count >= 6)
//    {
//        // ��ȡǰ4�ֽڣ������ FIFO��
//        fifo_data_count = 6;
//        fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_ONLY);

//        // ���֡ͷ��֡β
//        if (fifo_get_data[0] == 0xAA && fifo_get_data[5] == 0x55)
//        {
//            data1 = ((uint16_t)fifo_get_data[1] << 8) | fifo_get_data[2];
//            data2 = ((uint16_t)fifo_get_data[3] << 8) | fifo_get_data[4];

//            // �����Ѷ�ȡ�� 4 �ֽڣ�����ⲿ�����ݣ�
//            fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);
//        }
//        else
//        {
//            // ���������Ч֡ͷ��������1�ֽڣ�������һ�ּ��
//            uint8_t discard;
//            fifo_data_count = 1;
//            fifo_read_buffer(&uart_data_fifo, &discard, &fifo_data_count, FIFO_READ_AND_CLEAN);
//        }
//    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     UART_INDEX �Ľ����жϴ����� ����������� UART_INDEX ��Ӧ���жϵ��� ��� isr.c
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     uart_rx_interrupt_handler();
//-------------------------------------------------------------------------------------------------------------------
void uart_rx_interrupt_handler (void)
{ 
		if(uart_query_byte(UART_INDEX, &get_data))// �������� ��ѯʽ �����ݻ᷵�� TRUE û�����ݻ᷵�� FALSE
		{
				if(get_data == USB_FRAME_HEAD && !usbStr.receiveStart)//���֡ͷ
        {
            usbStr.receiveStart = true;
            usbStr.receiveBuff[0] = get_data;
            usbStr.receiveBuff[2] = USB_FRAME_LENMIN;
            usbStr.receiveIndex = 1;
        }
				else if(usbStr.receiveIndex == 2)	//����֡����
        {
            usbStr.receiveBuff[usbStr.receiveIndex] = get_data;
            usbStr.receiveIndex++;
            
            if(get_data > USB_FRAME_LENMAX || get_data < USB_FRAME_LENMIN) //֡������
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
				
				 //����֡���
        if((usbStr.receiveIndex >= USB_FRAME_LENMAX || usbStr.receiveIndex >= usbStr.receiveBuff[2]) && usbStr.receiveIndex > USB_FRAME_LENMIN)
        {
            uint8_t check = 0;
            uint8_t length = USB_FRAME_LENMIN;
        
            length = usbStr.receiveBuff[2];
            for(int i=0;i<length-1;i++)
                check += usbStr.receiveBuff[i];
            
            if(check == usbStr.receiveBuff[length-1])//У��λ
            {
                memcpy(usbStr.receiveBuffFinished,usbStr.receiveBuff,USB_FRAME_LENMAX);	
                usbStr.receiveFinished = true;
                
                //���ܳ�����ָ�����⴦������ʵʱ�ԣ�
                if(USB_ADDR_CONTROL  == usbStr.receiveBuffFinished[1])
                {
                    Bint16_Union bint16_Union;
                    Bint32_Union bint32_Union;
                    for(int i=0;i<4;i++)
                        bint32_Union.U8_Buff[i] = usbStr.receiveBuffFinished[3+i];
                    
                    bint16_Union.U8_Buff[0] = usbStr.receiveBuffFinished[7];
                    bint16_Union.U8_Buff[1] = usbStr.receiveBuffFinished[8];
                    
                    Servo_Control((float)bint16_Union.U16);
                    icarStr.ServoPwmSet = bint16_Union.U16;         //����
                    icarStr.SpeedSet = bint32_Union.Float;          //�ٶ�				
                }
				
                if(!usbStr.connected)//��λ����������ͨ��
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
* @brief        �������߳̿�����
* @param        
* @ref          
* @author       Rango
* @note         
**/
void USB_Edgeboard_Timr(void)
{
    if(usbStr.connected)//USBͨ�ŵ��߼��
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
* @brief        USBͨ�Ŵ�����
* @param        
* @ref          
* @author       Rango
* @note         
**/
void USB_Edgeboard_Handle(void)
{
    if(usbStr.receiveFinished)																//���ճɹ�
    {
        usbStr.receiveFinished = false;
           
        if(usbStr.receiveBuffFinished[1] & 0x80)	//������
        {
            uint8_t Addr = (uint8_t)(usbStr.receiveBuffFinished[1] & 0x7F);
            switch(Addr)
            {
                case USB_ADDR_BATTERY :             //�����Ϣ
                    break;
                
                case USB_ADDR_SERVOTHRESHOLD :      //�����ֵ
                    break;
            }
        }
        else //д����
        {
            switch(usbStr.receiveBuffFinished[1])
            {

                case USB_ADDR_SPEEDMODE:        //�ٿ�ģʽ�л�
                    if(usbStr.receiveBuffFinished[3] == 1)    //����ģʽ
                        motorStr.CloseLoop = false;                    
                    else
                        motorStr.CloseLoop = true;
                    
                    icarStr.SpeedSet = 0;
                    break;
                
                    
                //-----------------------------[�Լ�������]-------------------------------------------
                case USB_ADDR_INSPECTOR :           //�Լ��������
                    usbStr.inspectorEnable = true;
                    break; 
            }      
            
        }
    }
		
		//-----------------------[�Լ�������ݷ���]-----------------------------
    if(usbStr.inspectorEnable && usbStr.connected && usbStr.counterSend > 150)//150ms
    {
        USB_Edgeboard_CarSpeed();               //���ͳ���
        usbStr.counterSend = 0; 
    }
}

/**
* @brief        USB���Ͱ����ź�
* @param        time: ����ʱ��
* @ref
* @author       Rango
* @note
**/
void USB_Edgeboard_TransmitKey(uint16_t time)
{
    uint8_t check = 0;
    uint8_t buff[8] = {0};
    Bint16_Union bint16_Union;
    
    buff[0] = 0x42; //֡ͷ
    buff[1] = USB_ADDR_KEYINPUT; //��ַ
    buff[2] = 0x06; //֡��

    bint16_Union.U16 = time;
    buff[3] = bint16_Union.U8_Buff[0];
    buff[4] = bint16_Union.U8_Buff[1];
    
    for(int i=0;i<5;i++)
        check += buff[i];

    buff[5] = check;

		uart_write_buffer(UART_INDEX, &buff[0], 8);
}

/**
* @brief        ���ͳ�����Ϣ
* @ref          
* @author       Leo
* @note         
**/
void USB_Edgeboard_CarSpeed(void)
{
    Bint32_Union bint32_Union;
    uint8_t check = 0;
    uint8_t buff[10];
    buff[0] = 0x42; //֡ͷ
    buff[1] = USB_ADDR_SPEEDBACK; //��ַ
    buff[2] = 0x08; //֡��
        
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