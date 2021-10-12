#include "stm32f4xx.h"
#include "stm32_ub_usb_cdc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "smart_wrele.h"
#include "command.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "delay.h"

//------------------------------------------------------------------ERIS-------------------------------------------------------------------
void CLR_WRELE_Pins(void)
{
	GPIOG->ODR &= ~(GPIO_ODR_ODR_5 | GPIO_ODR_ODR_7);	//Wrele_24V_5_6;Wrele_24V_5_6_2;
	GPIOG->ODR &= ~GPIO_ODR_ODR_6;	//Wrele_rs_AB
}
uint8_t Change_Test_WRele(void)
{
	if(strcmp(buffer_command,"24V_1") == 0)
	{
		CLR_WRELE_Pins();
		GPIOG->ODR |= GPIO_ODR_ODR_5;

		return 1;
	}
	else if(strcmp(buffer_command,"24V_2") == 0)
	{
		CLR_WRELE_Pins();
		GPIOG->ODR |= GPIO_ODR_ODR_7;

		return 2;
	}
	else if(strcmp(buffer_command,"RS485") == 0)
	{
		CLR_WRELE_Pins();
		GPIOG->ODR |= GPIO_ODR_ODR_6;

		return 3;
	}
	else
	{
		CLR_WRELE_Pins();

		return 0;
	}
}
