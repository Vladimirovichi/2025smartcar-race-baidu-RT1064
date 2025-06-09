#include "key.h"
#include "uart.h"

uint8 keynum;

void Key_Init(void)
{
		gpio_init(C27, GPI, 1, GPI_PULL_UP);//C27��ʼ��ΪGPIO���ܡ�����ģʽ���������
		gpio_init(B9, GPO, 1, GPO_PUSH_PULL);//B9��ʼ��ΪGPIO���ܡ����ģʽ���������
		keynum = 0;
}

void Key_Scan(void)
{
		if(gpio_get_level(C27) == 0)
		{
				system_delay_ms(10);
				if(gpio_get_level(C27) == 0)
				{
						if(keynum == 0)
						{
								USB_Edgeboard_TransmitKey(500);
								keynum++;
								gpio_toggle_level(B9);
						}		
				}
		}
}
