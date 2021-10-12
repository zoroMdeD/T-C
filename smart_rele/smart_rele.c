#include "stm32f4xx.h"
#include "stm32_ub_usb_cdc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "smart_rele.h"
#include "command.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "delay.h"

//------------------------------------------------------------------ERIS-------------------------------------------------------------------
void CLR_RELE_Pins(void)
{
	GPIOH->ODR &= ~GPIO_ODR_ODR_4;	//REL_ISP;
	GPIOG->ODR &= ~(GPIO_ODR_ODR_5 | GPIO_ODR_ODR_7);	//Wrele_24V_5_6;Wrele_24V_5_6_2;
	GPIOF->ODR &= ~(GPIO_ODR_ODR_0 | GPIO_ODR_ODR_3 | GPIO_ODR_ODR_6);
}
//Функция выбора теста на плате Smart_Rele
uint8_t Change_Test_Rele(void)
{
	if(strcmp(buffer_command,"24V_1") == 0)
	{
		CLR_RELE_Pins();
		GPIOG->ODR |= GPIO_ODR_ODR_5;

		return 1;
	}
	/* Тест убран
	else if(strcmp(buffer_command,"24V_2") == 0)
	{
		CLR_RELE_Pins();
		GPIOG->ODR |= GPIO_ODR_ODR_7;

		return 2;
	}
	*/
	else if(strcmp(buffer_command,"SIGN") == 0)
	{
		CLR_RELE_Pins();
		GPIOH->ODR |= GPIO_ODR_ODR_4;

		return 3;
	}
	else if(strcmp(buffer_command,"GROUP_RELE2") == 0)
	{
		CLR_RELE_Pins();
		Control_Rele_Pins_SET();
		delay(250);
		if((GPIOF->IDR & GPIO_Pin_1) != 0)
		{
			if((GPIOF->IDR & GPIO_Pin_2) != 1)
			{
				VCP_send_buffer_new("ONE: [OK]\n",10);
			}
		}
		else
		{
			VCP_send_buffer_new("ONE: [NO]\n",10);
		}
		delay(50);
		if((GPIOF->IDR & GPIO_Pin_4) != 0)
		{
			if((GPIOF->IDR & GPIO_Pin_5) != 1)
			{
				VCP_send_buffer_new("TWO: [OK]\n",10);
			}
		}
		else
		{
			VCP_send_buffer_new("TWO: [NO]\n",10);
		}
		delay(50);
		if((GPIOF->IDR & GPIO_Pin_7) != 0)
		{
			if((GPIOF->IDR & GPIO_Pin_8) != 1)
			{
				VCP_send_buffer_new("THREE: [OK]\n",12);
			}
		}
		else
		{
			VCP_send_buffer_new("THREE: [NO]\n",12);
		}

		return 4;
	}
	else if(strcmp(buffer_command,"GROUP_RELE1") == 0)
	{
		CLR_RELE_Pins();
		Control_Rele_Pins_SET();
		delay(250);
		if((GPIOF->IDR & GPIO_Pin_2) != 0)
		{
			if((GPIOF->IDR & GPIO_Pin_1) != 1)
			{
				VCP_send_buffer_new("ONE: [OK]\n",10);
			}
		}
		else
		{
			VCP_send_buffer_new("ONE: [NO]\n",10);
		}
		delay(50);
		if((GPIOF->IDR & GPIO_Pin_5) != 0)
		{
			if((GPIOF->IDR & GPIO_Pin_4) != 1)
			{
				VCP_send_buffer_new("TWO: [OK]\n",10);
			}
		}
		else
		{
			VCP_send_buffer_new("TWO: [NO]\n",10);
		}
		delay(50);
		if((GPIOF->IDR & GPIO_Pin_8) != 0)
		{
			if((GPIOF->IDR & GPIO_Pin_7) != 1)
			{
				VCP_send_buffer_new("THREE: [OK]\n",12);

			}
		}
		else
		{
			VCP_send_buffer_new("THREE: [NO]\n",12);
		}

		return 5;
	}
	else if(strcmp(buffer_command,"BLOCK_RELE") == 0)
	{
		CLR_RELE_Pins();
		GPIOH->ODR |= GPIO_ODR_ODR_5;

		return 6;
	}
	else
	{
		CLR_RELE_Pins();
		GPIOH->ODR &= ~GPIO_ODR_ODR_5;

		return 0;
	}
}
