/*********************************************************************************************************************
* RT1064DVL6A Opensourec Library ����RT1064DVL6A ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
* 
* ���ļ��� RT1064DVL6A ��Դ���һ����
* 
* RT1064DVL6A ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
* 
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
* 
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
* 
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
* 
* �ļ�����          main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 8.32.4 or MDK 5.33
* ����ƽ̨          RT1064DVL6A
* ��������          https://seekfree.taobao.com/
* 
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-21        SeekFree            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"

// ���µĹ��̻��߹����ƶ���λ�����ִ�����²���
// ��һ�� �ر��������д򿪵��ļ�
// �ڶ��� project->clean  �ȴ��·�����������

// �������ǿ�Դ����ֲ�ÿչ���

#define PIT_CH                  (PIT_CH0 )                                      // ʹ�õ������жϱ�� ����޸� ��Ҫͬ����Ӧ�޸������жϱ���� isr.c �еĵ���
#define PIT_PRIORITY            (PIT_IRQn)                                      // ��Ӧ�����жϵ��жϱ��
int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  	// ����ɾ��
    //debug_init();                   // ���Զ˿ڳ�ʼ��

    // �˴���д�û����� ���������ʼ�������
		wireless_uart_init();																									// ��ʼ������ת����   
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIRELESS_UART);	// �����������ʹ������ת���ڽ����շ� 
    seekfree_assistant_oscilloscope_struct oscilloscope_data;							// ��ʼ���������ʾ�����Ľṹ��
    oscilloscope_data.channel_num = 4;																		// ����Ϊ4��ͨ����ͨ���������Ϊ8��
	
	
		motor_init();
		encoder_init();
		PID_Init();
		Servo_Init();
		Uart_Init();
		ICAR_Init();
		Key_Init();
		
		pit_ms_init(PIT_CH, 1);                                                  // ��ʼ�� PIT_CH0 Ϊ�����ж� 1ms ����
    interrupt_set_priority(PIT_PRIORITY, 0);                                    // ���� PIT1 �������жϵ��ж����ȼ�Ϊ 0
		
		// �˴���д�û����� ���������ʼ�������
    while(1)
    {
				Key_Scan();
				USB_Edgeboard_Handle();
        // �˴���д��Ҫѭ��ִ�еĴ���
//				encoder_get();
//				MOTOR_ControlLoop(0.5f);		//�ջ��ٿ�
//				Set_Pwm(1000);
				oscilloscope_data.data[0] = motorStr.EncoderValue;
				oscilloscope_data.data[1] = 0;
				oscilloscope_data.data[2] = 0;
				oscilloscope_data.data[3] = 0;
						
        seekfree_assistant_oscilloscope_send(&oscilloscope_data);// ͨ������ת���ڷ��͵�����ʾ������			
				
//				system_delay_ms(10);
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     PIT �������жϴ����� ����������� PIT ��Ӧ�Ķ�ʱ���жϵ��� ��� isr.c
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     pit_handler();
//-------------------------------------------------------------------------------------------------------------------
void pit_handler (void)
{
    //gpio_toggle_level(B9);                                                              // �����жϴ���
		MOTOR_Timer();
		USB_Edgeboard_Timr();
}
