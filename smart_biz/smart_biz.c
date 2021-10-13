#include "stm32f4xx.h"
#include "stm32_ub_usb_cdc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "smart_biz.h"
#include "command.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "delay.h"

//------------------------------------------------------------------ERIS-------------------------------------------------------------------
void CLR_BIZ_Pins(void)
{
	GPIOD->ODR &= ~(GPIO_ODR_ODR_1 | GPIO_ODR_ODR_2 | GPIO_ODR_ODR_3 | GPIO_ODR_ODR_4 | GPIO_ODR_ODR_5 | GPIO_ODR_ODR_6 | GPIO_ODR_ODR_7
				  | GPIO_ODR_ODR_9 | GPIO_ODR_ODR_10 | GPIO_ODR_ODR_11 | GPIO_ODR_ODR_12 | GPIO_ODR_ODR_13 | GPIO_ODR_ODR_14);
	GPIOG->ODR &= ~(GPIO_ODR_ODR_9 | GPIO_ODR_ODR_10 | GPIO_ODR_ODR_11 | GPIO_ODR_ODR_12 | GPIO_ODR_ODR_13 | GPIO_ODR_ODR_14 | GPIO_ODR_ODR_15);
	GPIOB->ODR &= ~GPIO_ODR_ODR_3;
	GPIOF->ODR &= ~GPIO_ODR_ODR_12;
}
//Функция выбора теста на плате Smart_biz
uint8_t Change_Test_Biz(void)
{
	if(strcmp(buffer_command,"TEST_FU1-R7") == 0)
	{
		CLR_BIZ_Pins();
		GPIOF->ODR |= GPIO_ODR_ODR_12;
		GPIOD->ODR |= GPIO_ODR_ODR_9;

		return 1;
	}
	else if(strcmp(buffer_command,"TEST_FU2-R8") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_10;

		return 2;
	}
	else if(strcmp(buffer_command,"TEST_FU3-R9") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_11;

		return 3;
	}
	else if(strcmp(buffer_command,"TEST_FU4-R10") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_12;

		return 4;
	}
	else if(strcmp(buffer_command,"TEST_FU5-R11") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_13;

		return 5;
	}
	else if(strcmp(buffer_command,"TEST_FU6-R12") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_14;

		return 6;
	}
	else if(strcmp(buffer_command,"VD13_FORWARD_OPEN") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_1;

		return 7;
	}
	else if(strcmp(buffer_command,"VD13_BACKWARD_OPEN") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_1;
		GPIOD->ODR |= GPIO_ODR_ODR_2;

		return 8;
	}
	else if(strcmp(buffer_command,"VD14_FORWARD_OPEN") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_3;

		return 9;
	}
	else if(strcmp(buffer_command,"VD14_BACKWARD_OPEN") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_3;
		GPIOD->ODR |= GPIO_ODR_ODR_4;

		return 10;
	}
	else if(strcmp(buffer_command,"VD15_FORWARD_OPEN") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_5;

		return 11;
	}
	else if(strcmp(buffer_command,"VD15_BACKWARD_OPEN") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_5;
		GPIOD->ODR |= GPIO_ODR_ODR_6;

		return 12;
	}
	else if(strcmp(buffer_command,"VD16_FORWARD_OPEN") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_7;

		return 13;
	}
	else if(strcmp(buffer_command,"VD16_BACKWARD_OPEN") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_7;
		GPIOG->ODR |= GPIO_ODR_ODR_9;

		return 14;
	}
	else if(strcmp(buffer_command,"VD17_FORWARD_OPEN") == 0)
	{
		CLR_BIZ_Pins();
		GPIOG->ODR |= GPIO_ODR_ODR_10;

		return 15;
	}
	else if(strcmp(buffer_command,"VD17_BACKWARD_OPEN") == 0)
	{
		CLR_BIZ_Pins();
		GPIOG->ODR |= GPIO_ODR_ODR_10;
		GPIOG->ODR |= GPIO_ODR_ODR_11;

		return 16;
	}
	else if(strcmp(buffer_command,"VD13_FORWARD_CLOSE") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_1;

		return 17;
	}
	else if(strcmp(buffer_command,"VD13_BACKWARD_CLOSE") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_1;
		GPIOD->ODR |= GPIO_ODR_ODR_2;

		return 18;
	}
	else if(strcmp(buffer_command,"VD14_FORWARD_CLOSE") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_3;

		return 19;
	}
	else if(strcmp(buffer_command,"VD14_BACKWARD_CLOSE") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_3;
		GPIOD->ODR |= GPIO_ODR_ODR_4;

		return 20;
	}
	else if(strcmp(buffer_command,"VD15_FORWARD_CLOSE") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_5;

		return 21;
	}
	else if(strcmp(buffer_command,"VD15_BACKWARD_CLOSE") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_5;
		GPIOD->ODR |= GPIO_ODR_ODR_6;

		return 22;
	}
	else if(strcmp(buffer_command,"VD16_FORWARD_CLOSE") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_7;

		return 23;
	}
	else if(strcmp(buffer_command,"VD16_BACKWARD_CLOSE") == 0)
	{
		CLR_BIZ_Pins();
		GPIOD->ODR |= GPIO_ODR_ODR_7;
		GPIOG->ODR |= GPIO_ODR_ODR_9;

		return 24;
	}
	else if(strcmp(buffer_command,"VD17_FORWARD_CLOSE") == 0)
	{
		CLR_BIZ_Pins();
		GPIOG->ODR |= GPIO_ODR_ODR_10;

		return 25;
	}
	else if(strcmp(buffer_command,"VD17_BACKWARD_CLOSE") == 0)
	{
		CLR_BIZ_Pins();
		GPIOG->ODR |= GPIO_ODR_ODR_10;
		GPIOG->ODR |= GPIO_ODR_ODR_11;

		return 26;
	}
	else if(strcmp(buffer_command,"LINE1") == 0)
	{
		CLR_BIZ_Pins();
		GPIOG->ODR |= GPIO_ODR_ODR_12;

		return 27;
	}
	/* Тест убран
	else if(strcmp(buffer_command,"LINE2") == 0)
	{
		CLR_BIZ_Pins();
		GPIOG->ODR |= GPIO_ODR_ODR_13;

		return 28;
	}
	*/
	else if(strcmp(buffer_command,"LINE3") == 0)
	{
		CLR_BIZ_Pins();
		GPIOG->ODR |= GPIO_ODR_ODR_14;

		return 29;
	}
	else if(strcmp(buffer_command,"LINE4") == 0)
	{
		CLR_BIZ_Pins();
		GPIOG->ODR |= GPIO_ODR_ODR_15;

		return 30;
	}
	else if(strcmp(buffer_command,"LINE5") == 0)
	{
		CLR_BIZ_Pins();
		GPIOB->ODR |= GPIO_ODR_ODR_3;

		return 31;
	}
	/* Тест убран
	else if(strcmp(buffer_command,"LINE6") == 0)
	{
		CLR_BIZ_Pins();

		return 32;
	}
	*/
	else
	{
		CLR_BIZ_Pins();

		return 0;
	}
}
