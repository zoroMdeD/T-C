#include "main.h"

void TIM6_DAC_IRQHandler()
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		time--;
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }
}

int main(void)
{
	check = RX_EMPTY;
	check_button = 0;

	SystemInit();
	INIT_All();
	delay_init(1000);

	delay(2000);	//Задержка включения USB
	REBOOT_USB;

	UB_USB_CDC_Init();		//Инициализация USB-OTG-порта как CDC-устройство

	while(1)
	{
		if(UB_USB_CDC_GetStatus() == USB_CDC_CONNECTED)				//Проверка работы USB-подключения к ПК
		{
			check = UB_USB_CDC_ReceiveString(buffer_command);		//Проверка получения данных по USB
			if(check == RX_READY)
			{
				if(strcmp(buffer_command,"TEST_BIZ") == 0)			//Запустить тестирование платы Smart_Biz
				{
					VCP_send_buffer_new("TEST_BIZ_ON\n",12);
					POWER_NI_ON;
					delay(20);
					if((GPIOF->IDR & GPIO_Pin_11) != 0)
						VCP_send_buffer_new("POWER_NI_ON\n",12);
					else
						VCP_send_buffer_new("ERROR\n",6);
					pnt = 1;
					do
					{
						check = UB_USB_CDC_ReceiveString(buffer_command);
						if(check == RX_READY)
						{
							if(strcmp(buffer_command,"RELE_&_BIZ_ON") == 0)
							{
								RELE_TO_BIZ_ON;
								BIZ_TO_CPU_ON;
								delay(20);
								if(((GPIOB->IDR & GPIO_Pin_4) > 0) && ((GPIOB->IDR & GPIO_Pin_10) > 0))
								{
									VCP_send_buffer_new("RELE_TO_BIZ_ON\n",15);
									VCP_send_buffer_new("BIZ_TO_CPU_ON\n",14);
								}
								else
									VCP_send_buffer_new("ERROR\n",6);
							}
							else if(strcmp(buffer_command,"RELE_&_BIZ_OFF") == 0)
							{
								RELE_TO_BIZ_OFF;
								BIZ_TO_CPU_OFF;
								delay(20);
								if(((GPIOB->IDR & GPIO_Pin_4) < 1) && ((GPIOB->IDR & GPIO_Pin_10) < 1))
								{
									VCP_send_buffer_new("RELE_TO_BIZ_OFF\n",16);
									VCP_send_buffer_new("BIZ_TO_CPU_OFF\n",15);
								}
								else
									VCP_send_buffer_new("ERROR\n",6);
							}
							else
							{
								pnt = Change_Test_Biz();
								VCP_put_char_new(pnt);
							}
						}
					}
					while (pnt != 0);
					POWER_NI_OFF;
					RELE_TO_BIZ_OFF;
					BIZ_TO_CPU_OFF;
				}
				else if(strcmp(buffer_command,"TEST_RELE") == 0)	//Запустить тестирование платы Smart_Rele
				{
					VCP_send_buffer_new("TEST_RELE_ON\n",13);
					pnt = 1;
					do
					{
						check = UB_USB_CDC_ReceiveString(buffer_command);
						if(check == RX_READY)
						{
							if(strcmp(buffer_command,"POWER_RELE_ON") == 0)
							{
								POWER_NI_ON;
								POWER_SUMM_NI_ON;
								delay(20);
								if(((GPIOF->IDR & GPIO_Pin_11) > 0) && ((GPIOF->IDR & GPIO_Pin_12) > 0))
								{
									VCP_send_buffer_new("POWER_NI_ON\n",12);
									VCP_send_buffer_new("POWER_SUMM_NI_ON\n",17);
								}
								else
									VCP_send_buffer_new("ERROR\n",6);
							}
							else if(strcmp(buffer_command,"POWER_RELE_OFF") == 0)
							{
								POWER_NI_OFF;
								POWER_SUMM_NI_OFF
								delay(20);
								if(((GPIOF->IDR & GPIO_Pin_11) < 1) && ((GPIOF->IDR & GPIO_Pin_12) < 1))
								{
									VCP_send_buffer_new("POWER_NI_OFF\n",13);
									VCP_send_buffer_new("POWER_SUMM_NI_OFF\n",18);
								}
								else
									VCP_send_buffer_new("ERROR\n",6);
							}
							else
							{
								pnt = Change_Test_Rele();
								VCP_put_char_new(pnt);
							}
						}
					}
					while (pnt != 0);
					POWER_NI_OFF;
					POWER_SUMM_NI_OFF
				}
				else if(strcmp(buffer_command,"TEST_LED") == 0)		//Запустить тестирование платы Smart_Led
				{
					VCP_send_buffer_new("TEST_LED_ON\n",12);
					pnt = 1;
					do
					{
						check = UB_USB_CDC_ReceiveString(buffer_command);
						if(check == RX_READY)
						{
							if(strcmp(buffer_command,"POWER_ON_LED") == 0)
							{
								POWER_NI_ON;
								POWER_SUMM_NI_ON;
								delay(20);
								if(((GPIOF->IDR & GPIO_Pin_11) > 0) && ((GPIOF->IDR & GPIO_Pin_12) > 0))
								{
									VCP_send_buffer_new("POWER_NI_ON\n",12);
									VCP_send_buffer_new("POWER_SUMM_NI_ON\n",17);
								}
								else
									VCP_send_buffer_new("ERROR\n",6);

								FIND_LIGHT_LED();
							}
							else if(strcmp(buffer_command,"RELE_&_BIZ_ON") == 0)
							{
								RELE_TO_BIZ_ON;
								BIZ_TO_CPU_ON;
								delay(20);
								if(((GPIOB->IDR & GPIO_Pin_4) > 0) && ((GPIOB->IDR & GPIO_Pin_10) > 0))
								{
									VCP_send_buffer_new("RELE_TO_BIZ_ON\n",15);
									VCP_send_buffer_new("BIZ_TO_CPU_ON\n",14);
								}
								else
									VCP_send_buffer_new("ERROR\n",6);
							}
							else if(strcmp(buffer_command,"BT_ON") == 0)
							{
								BlueTooth_ON;
								BT05_INIT_Bluetooth();
							}
							else if(strcmp(buffer_command,"BT_OFF") == 0)
							{
								BlueTooth_OFF;
								delay(20);
								if((GPIOE->IDR & GPIO_Pin_2) < 1)
								{
									VCP_send_buffer_new("BLUETOOTH_OFF\n",14);
								}
								else
									VCP_send_buffer_new("ERROR\n",6);
							}
							else
							{
								pnt = Change_Test_Led();
								VCP_put_char_new(pnt);
							}
						}
					}
					while (pnt != 0);
					POWER_NI_OFF;
					POWER_SUMM_NI_OFF;
					RELE_TO_BIZ_OFF;
					BIZ_TO_CPU_OFF;
					BlueTooth_OFF;
				}
				else if(strcmp(buffer_command,"TEST_CPU") == 0)		//Запустить тестирование платы Smart_Cpu
				{
					VCP_send_buffer_new("TEST_CPU_ON\n",12);
					pnt = 1;
					do
					{
						check = UB_USB_CDC_ReceiveString(buffer_command);
						if(check == RX_READY)
						{
							if(strcmp(buffer_command,"POWER_ON_CPU") == 0)
							{
								POWER_NI_ON;
								POWER_SUMM_NI_ON;
								delay(20);
								if(((GPIOF->IDR & GPIO_Pin_11) > 0) && ((GPIOF->IDR & GPIO_Pin_12) > 0))
								{
									VCP_send_buffer_new("POWER_NI_ON\n",12);
									VCP_send_buffer_new("POWER_SUMM_NI_ON\n",17);
								}
								else
									VCP_send_buffer_new("ERROR\n",6);
							}
							else if(strcmp(buffer_command,"POWER_OFF_CPU") == 0)
							{
								POWER_NI_OFF;
								POWER_SUMM_NI_OFF;
								delay(20);
								if(((GPIOF->IDR & GPIO_Pin_11) < 1) && ((GPIOF->IDR & GPIO_Pin_12) < 1))
								{
									VCP_send_buffer_new("POWER_NI_OFF\n",13);
									VCP_send_buffer_new("POWER_SUMM_NI_OFF\n",18);
								}
								else
									VCP_send_buffer_new("ERROR\n",6);
							}
							else if(strcmp(buffer_command,"RELE_&_BIZ_ON") == 0)
							{
								RELE_TO_BIZ_ON;
								BIZ_TO_CPU_ON;
								delay(20);
								if(((GPIOB->IDR & GPIO_Pin_4) > 0) && ((GPIOB->IDR & GPIO_Pin_10) > 0))
								{
									VCP_send_buffer_new("RELE_TO_BIZ_ON\n",15);
									VCP_send_buffer_new("BIZ_TO_CPU_ON\n",14);
								}
								else
									VCP_send_buffer_new("ERROR\n",6);
							}
							else if(strcmp(buffer_command,"RELE_&_BIZ_OFF") == 0)
							{
								RELE_TO_BIZ_OFF;
								BIZ_TO_CPU_OFF;
								delay(20);
								if(((GPIOB->IDR & GPIO_Pin_4) < 1) && ((GPIOB->IDR & GPIO_Pin_10) < 1))
								{
									VCP_send_buffer_new("RELE_TO_BIZ_OFF\n",16);
									VCP_send_buffer_new("BIZ_TO_CPU_OFF\n",15);
								}
								else
									VCP_send_buffer_new("ERROR\n",6);
							}
							else
							{
								pnt = Change_Test_Cpu();
								VCP_put_char_new(pnt);
							}
						}
					}
					while (pnt != 0);
					POWER_NI_OFF;
					POWER_SUMM_NI_OFF;
					RELE_TO_BIZ_OFF;
					BIZ_TO_CPU_OFF;
				}
				else if(strcmp(buffer_command,"VERSION") == 0)		//Запустить тестирование платы Smart_Cpu
				{
					VCP_send_buffer_new("Firmware version: T&C_v.1.0.2\n",30);
				}
				else
				{
					VCP_send_buffer_new("UNKNOWN_CMD\n",12);
				}
			}
		}
	}
}
