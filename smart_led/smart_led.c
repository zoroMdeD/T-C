#include "stm32f4xx.h"
#include "stm32_ub_usb_cdc.h"
#include "stm32f4xx.h"
#include "stm32_ub_usb_cdc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "smart_led.h"
#include "command.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "delay.h"

//extern volatile char RX_BUF;

//------------------------------------------------------------------ERIS-------------------------------------------------------------------
void CLR_LED_Pins(void)
{
	GPIOH->ODR &= ~(GPIO_ODR_ODR_2 | GPIO_ODR_ODR_3);
}
//Функция выбора теста на плате Smart_Led
uint8_t Change_Test_Led(void)
{
	if(strcmp(buffer_command,"CPU_JLINK1") == 0)
	{
		CLR_LED_Pins();
		GPIOH->ODR |= (GPIO_ODR_ODR_2 | GPIO_ODR_ODR_3);

		return 1;
	}
	// Функция для клона HM-10(BT-05)
	else if(strcmp(buffer_command,"CHECK_BT") == 0)
	{
		uint8_t check = 0;

		CLR_LED_Pins();
		send_str("AT+INQ\r\n");
		do
		{
			check = BT05_Seach_Bluetooth();
		}
		while (check != 1);

		send_str("AT+RESET\r\n");
		delay(1000);
		send_str("AT+RESET\r\n");

		return 2;
	}
	/* Функция для HM-10
	else if(strcmp(buffer_command,"CHECK_BT") == 0)
	{
		uint8_t check = 0;

		CLR_LED_Pins();
		send_str("AT+DISC?\r\n");
		do
		{
			check = Seach_Bluetooth();
		}
		while (check != 1);

		delay(1000);

		return 2;
	}
	*/
	else
	{
		CLR_LED_Pins();

		return 0;
	}
}
