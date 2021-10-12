#include "stm32f4xx.h"
#include "stm32_ub_usb_cdc.h"
#include "stm32f4xx.h"
#include "stm32_ub_usb_cdc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "smart_oled.h"
#include "command.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "delay.h"

//------------------------------------------------------------------ERIS-------------------------------------------------------------------
void CLR_OLED_Pins(void)
{
	GPIOH->ODR &= ~GPIO_ODR_ODR_3;
}
//Функция выбора теста на плате Smart_Led
uint8_t Change_Test_OLed(void)
{
	if(strcmp(buffer_command,"CPU_JLINK1") == 0)
	{
		CLR_OLED_Pins();
		GPIOH->ODR |= GPIO_ODR_ODR_3;

		return 1;
	}
	else if(strcmp(buffer_command,"CHECK_BT") == 0)
	{
		uint8_t check = 0;

		CLR_OLED_Pins();
		send_str("AT+DISC?\r\n");
		do
		{
			check = Seach_Bluetooth();
		}
		while (check != 1);

		delay(1000);

		return 2;
	}
	else
	{
		CLR_OLED_Pins();

		return 0;
	}
}
