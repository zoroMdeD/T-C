#include "stm32f4xx.h"
#include "stm32_ub_usb_cdc.h"
#include "stm32f4xx.h"
#include "stm32_ub_usb_cdc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "smart_cpu.h"
#include "command.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "delay.h"
#include "main.h"

//------------------------------------------------------------------ERIS-------------------------------------------------------------------
void CLR_CPU_Pins(void)
{
	GPIOH->ODR &= ~(GPIO_ODR_ODR_2 | GPIO_ODR_ODR_3 | GPIO_ODR_ODR_6 | GPIO_ODR_ODR_7
				  | GPIO_ODR_ODR_8 | GPIO_ODR_ODR_9 | GPIO_ODR_ODR_10 | GPIO_ODR_ODR_11);	//CPU_Jlink2;CPU_Jlink1;Smart24V;Smart5V;Smart3V3;SmartC54;CPU3M686;CPU12M
	GPIOC->ODR &= ~(GPIO_ODR_ODR_0 | GPIO_ODR_ODR_1 | GPIO_ODR_ODR_2);						//EM3_cntrl;EM2_cntrl;EM1_cntrl
	GPIOG->ODR &= ~GPIO_ODR_ODR_3;															//Rele_HART
}
//Функция выбора теста на плате Smart_Cpu
uint8_t Change_Test_Cpu(void)
{
	if(strcmp(buffer_command,"CPU_3V3") == 0)
	{
		CLR_CPU_Pins();
		GPIOH->ODR |= GPIO_ODR_ODR_8;

		return 1;
	}
	else if(strcmp(buffer_command,"CPU_5V") == 0)
	{
		CLR_CPU_Pins();
		GPIOH->ODR |= GPIO_ODR_ODR_7;

		return 2;
	}
	else if(strcmp(buffer_command,"CPU_24V") == 0)	//Проверяем наличие 24В, по факту 23.3В(падение на защитном диоде)
	{
		CLR_CPU_Pins();
		GPIOH->ODR |= GPIO_ODR_ODR_6;

		return 3;
	}
	else if(strcmp(buffer_command,"CPU_C54") == 0)	//Проверяем наличие 3.3В
	{
		CLR_CPU_Pins();
		GPIOH->ODR |= GPIO_ODR_ODR_9;

		return 4;
	}
	else if(strcmp(buffer_command,"CPU_SCOPE") == 0)
	{
		CLR_CPU_Pins();
		GPIOH->ODR |= GPIO_ODR_ODR_10;

		return 5;
	}
	else if(strcmp(buffer_command,"PROGRAMM_CPU") == 0)
	{
		CLR_CPU_Pins();
		GPIOH->ODR |= GPIO_ODR_ODR_3;

		return 6;
	}
	else if(strcmp(buffer_command,"SERIAL") == 0)	//Не понятно для чего!!!
	{
		CLR_CPU_Pins();
		GPIOG->ODR |= GPIO_ODR_ODR_2;

		return 7;
	}

	else if(strcmp(buffer_command,"MGT_TEST") == 0)
	{
		CLR_CPU_Pins();

		GPIOC->ODR |= GPIO_ODR_ODR_1;		//1-й магнит
		delay(2000);
		GPIOC->ODR &= ~GPIO_ODR_ODR_1;
		GPIOC->ODR |= GPIO_ODR_ODR_2;		//2-ой магнит
		delay(2000);
		GPIOC->ODR &= ~GPIO_ODR_ODR_2;
		GPIOC->ODR |= GPIO_ODR_ODR_0;		//3-й магнит
		delay(2000);
		GPIOC->ODR &= ~GPIO_ODR_ODR_0;

		return 8;
	}
	else if(strcmp(buffer_command,"CPU_SCOPE2") == 0)
	{
		CLR_CPU_Pins();
		GPIOH->ODR |= GPIO_ODR_ODR_11;

		return 9;
	}
	else if(strcmp(buffer_command,"HART") == 0)	//Не работает!!!
	{
		CLR_CPU_Pins();
		GPIOG->ODR |= GPIO_ODR_ODR_3;

		return 10;
	}

	else if(strcmp(buffer_command,"CPU_RUN") == 0)
	{
		//-----------CPU_TEST_ON------------------
		RELE_TO_BIZ_ON;
		BIZ_TO_CPU_ON;
		delay(20);
		if(((GPIOB->IDR & GPIO_Pin_4) != 0) && ((GPIOB->IDR & GPIO_Pin_10) != 0))
		{
			VCP_send_buffer_new("RELE_TO_BIZ_ON\n",15);
			VCP_send_buffer_new("BIZ_TO_CPU_ON\n",14);
		}
		else
			VCP_send_buffer_new("ERROR\n",6);
		//-----------END_CPU_TEST_ON--------------
		delay(250);
		//------------------SERIAL-----------------------
		CLR_CPU_Pins();
		GPIOG->ODR |= GPIO_ODR_ODR_2;
		VCP_send_buffer_new("SERIAL_ON\n",10);
		//----------------END_SERIAL---------------------
		delay(250);
		//---------------POWER_ON_CPU--------------------
		POWER_NI_ON;
		POWER_SUMM_NI_ON;
		delay(20);
		if(((GPIOF->IDR & GPIO_Pin_11) != 0) && ((GPIOF->IDR & GPIO_Pin_12) != 0))
		{
			VCP_send_buffer_new("POWER_NI_ON\n",12);
			VCP_send_buffer_new("POWER_SUMM_NI_ON\n",17);
		}
		else
			VCP_send_buffer_new("ERROR\n",6);

		FIND_LIGHT_LED();
		//------------END_POWER_ON_CPU--------------------
		return 11;
	}
	else
	{
		CLR_CPU_Pins();
		GPIOG->ODR &= ~GPIO_ODR_ODR_2;	//RS485

		return 0;
	}
}
