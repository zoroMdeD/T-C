#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f4xx.h"
#include "system_stm32f4xx.h"
#include "stm32_ub_usb_cdc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_tim.h"

#include "delay.h"
#include "command.h"
#include "smart_biz.h"
#include "smart_cpu.h"
#include "smart_led.h"
#include "smart_oled.h"
#include "smart_rele.h"
#include "smart_wrele.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define POWER_NI_ON					GPIOF->ODR |= GPIO_ODR_ODR_11;
#define POWER_NI_OFF				GPIOF->ODR &= ~GPIO_ODR_ODR_11;

#define POWER_SUMM_NI_ON			GPIOF->ODR |= GPIO_ODR_ODR_12;
#define POWER_SUMM_NI_OFF			GPIOF->ODR &= ~GPIO_ODR_ODR_12;

//Команда убрана
//#define CPU_ETALON_TEST_ON			GPIOB->ODR |= (GPIO_ODR_ODR_4 | GPIO_ODR_ODR_10);
//#define CPU_ETALON_TEST_OFF			GPIOB->ODR &= ~(GPIO_ODR_ODR_4 | GPIO_ODR_ODR_10);

#define RELE_TO_BIZ_ON				GPIOB->ODR |= GPIO_ODR_ODR_4;
#define RELE_TO_BIZ_OFF				GPIOB->ODR &= ~GPIO_ODR_ODR_4;

#define BIZ_TO_CPU_ON				GPIOB->ODR |= GPIO_ODR_ODR_10;
#define BIZ_TO_CPU_OFF				GPIOB->ODR &= ~GPIO_ODR_ODR_10;

//Команда убрана
//#define POWER_SMU_ON				GPIOG->ODR |= GPIO_ODR_ODR_8;
//#define POWER_SMU_OFF				GPIOG->ODR &= ~GPIO_ODR_ODR_8;

//Команда убрана
//#define CONNECT_SMART_LED			GPIOF->ODR |= GPIO_ODR_ODR_13;
//#define DISCONNECT_SMART_LED		GPIOF->ODR &= ~GPIO_ODR_ODR_13;

//Команда убрана
//#define POWER_RESERVED_RELAY_ON		GPIOA->ODR |= GPIO_ODR_ODR_4;
//#define POWER_RESERVED_RELAY_OFF		GPIOA->ODR &= ~GPIO_ODR_ODR_4;

#define BlueTooth_ON					GPIOE->ODR |= GPIO_ODR_ODR_2;
#define BlueTooth_OFF					GPIOE->ODR &= ~GPIO_ODR_ODR_2;

#endif

