#include "stm32f4xx.h"
#include "stm32_ub_usb_cdc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "command.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "delay.h"

#define	YES	0x59								//Статус фотодиода (работает)
#define	NO	0x4E								//Статус фотодиода (не работает)

#define RX_BUF_SIZE 80							//Размер буфера приема USART
uint8_t RXi;									//Номер элемента массива принятых данных USART
/*volatile*/ char RXc;								//Буффер данных приема USART
/*volatile*/ char RX_BUF[RX_BUF_SIZE] = {'\0'};		//Массив данных принятых по USART

char mass[20][20];								//Массив для хранения разобранных адресов устройств bluetooth
const char string1[] = "AT+RNAME? ";			//Комманда запроса имени устройства bluetooth
char string2[20];								//Строка с адресом устройства
const char string3[] = "\r\n";					//Конец комманды передаваемой модулю bluetooth
char string_out[40];							//Массив хранения отправляемой комманды модулю bluetooth	26

uint8_t tail = 0;								//Флаг конца принимаемой посылки от bluetooth
uint8_t imass = 0;								//Счетчик элементов конурентного адеса устройства
uint8_t num_mass = 0;							//Номер элемента массива адресов bluetooth
uint8_t num_device = 0;							//Счетчик кол-ва найденных устройств в радиусе действия bluetooth

//Функция очистки буффера приема по USART
void clear_RXBuffer(void)
{
    for (RXi = 0; RXi < RX_BUF_SIZE; RXi++)
        RX_BUF[RXi] = '\0';
    RXi = 0;
}
//Функция очистки строки для передачи по USART
void clear_TXString(void)
{
    for (uint8_t i = 0; i < 40; i++)
    {
    	string_out[i] = '\0';
    	if(i < 20)
    	{
    		string2[i] = '\0';
    	}
    }
}
//------------------------------------------------------------------ERIS-------------------------------------------------------------------
//Функция инициализации всей переферии
void INIT_All(void)
{
	INIT_Power_NI_or_Power_Summ_NI();
	FAKE_BlueTooth();
	INIT_Reboot_USB();
	INIT_Comparator_Pins();
	INIT_Fuse_Test_Pins();
	INIT_Diode_Test_Pins();
	INIT_Ringing_Pins();
	INIT_CPU_Test_Pins();
	INIT_CPU_ETALON_TEST_Pins();
	INIT_InputEtalonLed_Pins();
	INIT_RELE_Test_Pins();
	INIT_LED_Test_Pins();
	//INIT_WRELE_Test_Pins();
	INIT_Usart();
	INIT_TIMER();
}
void FAKE_BlueTooth(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitTypeDef GPIO_E;

	GPIO_E.GPIO_Pin = GPIO_Pin_2;
	GPIO_E.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_E.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_E.GPIO_OType = GPIO_OType_PP;
	GPIO_E.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOE, &GPIO_E);

	GPIOE->ODR &= ~GPIO_ODR_ODR_2;
}
void INIT_Power_NI_or_Power_Summ_NI(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	GPIO_InitTypeDef GPIO_F;

	GPIO_F.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_F.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_F.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_F.GPIO_OType = GPIO_OType_PP;
	GPIO_F.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOF, &GPIO_F);

	GPIOF->ODR &= ~(GPIO_ODR_ODR_11 | GPIO_ODR_ODR_12);
}
//Функция инициализации перезапуска USB
void INIT_Reboot_USB(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_D;

	GPIO_D.GPIO_Pin = GPIO_Pin_0;
	GPIO_D.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_D.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_D.GPIO_OType = GPIO_OType_PP;
	GPIO_D.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOD, &GPIO_D);

	GPIOD->ODR &= ~GPIO_ODR_ODR_0;
}
void INIT_Comparator_Pins(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitTypeDef GPIO_E;

	GPIO_E.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11
			 	 	| GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;			//CMP1-9
	GPIO_E.GPIO_Mode = GPIO_Mode_IN;
	GPIO_E.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_E.GPIO_OType = GPIO_OType_PP;
	GPIO_E.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOE, &GPIO_E);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_B;

	GPIO_B.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;			//CMP10-13
	GPIO_B.GPIO_Mode = GPIO_Mode_IN;
	GPIO_B.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_B.GPIO_OType = GPIO_OType_PP;
	GPIO_B.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOB, &GPIO_B);
}
//-------------------------------------------------------Smart_Biz-------------------------------------------------------
void INIT_Fuse_Test_Pins(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_D;

	GPIO_D.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;	//FU1-6
	GPIO_D.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_D.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_D.GPIO_OType = GPIO_OType_PP;
	GPIO_D.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOD, &GPIO_D);

	GPIOD->ODR &= ~(GPIO_ODR_ODR_9 | GPIO_ODR_ODR_10 | GPIO_ODR_ODR_11 | GPIO_ODR_ODR_12 | GPIO_ODR_ODR_13 | GPIO_ODR_ODR_14);
}
void INIT_Diode_Test_Pins(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_D;

	GPIO_D.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;	//VD1-7
	GPIO_D.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_D.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_D.GPIO_OType = GPIO_OType_PP;
	GPIO_D.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOD, &GPIO_D);

	GPIOD->ODR &= ~(GPIO_ODR_ODR_1 | GPIO_ODR_ODR_2 | GPIO_ODR_ODR_3 | GPIO_ODR_ODR_4 | GPIO_ODR_ODR_5 | GPIO_ODR_ODR_6 | GPIO_ODR_ODR_7);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_InitTypeDef GPIO_G;

	GPIO_G.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;	//VD8-10
	GPIO_G.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_G.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_G.GPIO_OType = GPIO_OType_PP;
	GPIO_G.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOG, &GPIO_G);

	GPIOG->ODR &= ~(GPIO_ODR_ODR_9 | GPIO_ODR_ODR_10 | GPIO_ODR_ODR_11);
}
void INIT_Ringing_Pins(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_InitTypeDef GPIO_G;

	GPIO_G.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;	//LINE1-4
	GPIO_G.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_G.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_G.GPIO_OType = GPIO_OType_PP;
	GPIO_G.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOG, &GPIO_G);

	GPIOG->ODR &= ~(GPIO_ODR_ODR_12 | GPIO_ODR_ODR_13 | GPIO_ODR_ODR_14 | GPIO_ODR_ODR_15);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_B;

	GPIO_B.GPIO_Pin = GPIO_Pin_3;	//LINE5
	GPIO_B.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_B.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_B.GPIO_OType = GPIO_OType_PP;
	GPIO_B.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOB, &GPIO_B);

	GPIOB->ODR &= ~GPIO_ODR_ODR_3;
}
//-----------------------------------------------------End_Smart_Biz-----------------------------------------------------
//-------------------------------------------------------Smart_Cpu-------------------------------------------------------
void INIT_CPU_Test_Pins(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	GPIO_InitTypeDef GPIO_H;

	GPIO_H.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7
					| GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;	//CPU_Jlink2;CPU_Jlink1;Smart24V;Smart5V;Smart3V3;SmartC54;CPU3M686;CPU12M
	GPIO_H.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_H.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_H.GPIO_OType = GPIO_OType_PP;
	GPIO_H.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOH, &GPIO_H);

	GPIOH->ODR &= ~(GPIO_ODR_ODR_2 | GPIO_ODR_ODR_3 | GPIO_ODR_ODR_6 | GPIO_ODR_ODR_7
				  | GPIO_ODR_ODR_8 | GPIO_ODR_ODR_9 | GPIO_ODR_ODR_10 | GPIO_ODR_ODR_11);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_C;

	GPIO_C.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;	//EM3_cntrl;EM2_cntrl;EM1_cntrl
	GPIO_C.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_C.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_C.GPIO_OType = GPIO_OType_PP;
	GPIO_C.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOC, &GPIO_C);

	GPIOC->ODR &= ~(GPIO_ODR_ODR_0 | GPIO_ODR_ODR_1 | GPIO_ODR_ODR_2);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_B;

	GPIO_B.GPIO_Pin = GPIO_Pin_10;	//CPUT0LEDE
	GPIO_B.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_B.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_B.GPIO_OType = GPIO_OType_PP;
	GPIO_B.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOB, &GPIO_B);

	GPIOB->ODR &= ~GPIO_ODR_ODR_10;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_InitTypeDef GPIO_G;

	GPIO_G.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_3;	//RS485;Rele_HART
	GPIO_G.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_G.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_G.GPIO_OType = GPIO_OType_PP;
	GPIO_G.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOG, &GPIO_G);

	GPIOG->ODR &= ~(GPIO_ODR_ODR_2 | GPIO_ODR_ODR_3);
}
void INIT_CPU_ETALON_TEST_Pins(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_B;

	GPIO_B.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_10 ;	//CPU_E-T;CPUT_LEDE;
	GPIO_B.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_B.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_B.GPIO_OType = GPIO_OType_PP;
	GPIO_B.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOB, &GPIO_B);

	GPIOB->ODR &= ~(GPIO_ODR_ODR_4 | GPIO_ODR_ODR_10);
}
void INIT_InputEtalonLed_Pins(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
	GPIO_InitTypeDef GPIO_I;

	GPIO_I.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_I.GPIO_Mode = GPIO_Mode_IN;
	GPIO_I.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_I.GPIO_OType = GPIO_OType_PP;
	GPIO_I.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOI, &GPIO_I);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_C;

	GPIO_C.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_C.GPIO_Mode = GPIO_Mode_IN;
	GPIO_C.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_C.GPIO_OType = GPIO_OType_PP;
	GPIO_C.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOC, &GPIO_C);
}
/* Функция убрана
//Функция проверки светодиодов на плате smart_led(etalon)
void FIND_LIGHT_ETALON_LED(void)
{
	uint8_t status[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	uint8_t step = 1;

	for(uint8_t n = 0; n < 10; n++)
	{
		status[n] = NO;
	}

	RUN_TIMER();
	time = 15;		//Время работы теста: 15 секунд
	while(time)
	{
		if((GPIOC->IDR & GPIO_Pin_3) != 0)			//LED5
		{
			status[0] = YES;
		}
		if((GPIOI->IDR & GPIO_Pin_11) != 0)			//LED4
		{
			status[1] = YES;
		}
		if((GPIOI->IDR & GPIO_Pin_10) != 0)			//LED3
		{
			status[2] = YES;
		}
		if((GPIOI->IDR & GPIO_Pin_9) != 0)			//LED2
		{
			status[3] = YES;
		}
		if((GPIOI->IDR & GPIO_Pin_8) != 0)			//LED1
		{
			status[4] = YES;
		}
		if((GPIOC->IDR & GPIO_Pin_13) != 0)			//LED8
		{
			status[5] = YES;
		}
		if((GPIOC->IDR & GPIO_Pin_5) != 0)			//LED7
		{
			status[6] = YES;
		}
		if((GPIOC->IDR & GPIO_Pin_4) != 0)			//LED6
		{
			status[7] = YES;
		}
		if(step)
		{
			if((GPIOC->IDR & GPIO_Pin_14) != 0)		//LED9(ON)
			{
				status[8] = YES;
				step = 0;
			}
		}
		if(!step)
		{
			if((GPIOC->IDR & GPIO_Pin_14) == 0)		//LED9(OFF)
			{
				status[9] = YES;
				break;
			}
		}
	}
	STOP_TIMER();
	VCP_send_buffer_new("TEST_ETALON_LED: ",17);
	for(uint8_t n = 0; n < 10; n++)
	{
		VCP_send_buffer_new("[",1);
		VCP_put_char_new(status[n]);
		VCP_send_buffer_new("] ",2);
	}
}
*/
//-----------------------------------------------------End_Smart_Cpu-----------------------------------------------------
//-------------------------------------------------------Smart_Rele------------------------------------------------------
void INIT_RELE_Test_Pins(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	GPIO_InitTypeDef GPIO_H;

	GPIO_H.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;	//REL_ISP
	GPIO_H.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_H.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_H.GPIO_OType = GPIO_OType_PP;
	GPIO_H.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOH, &GPIO_H);

	GPIOH->ODR &= ~(GPIO_ODR_ODR_4 | GPIO_ODR_ODR_5);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_InitTypeDef GPIO_G;

	GPIO_G.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8;		//Wrele_24V_5_6;Wrele_24V_5_6_2;Wrele_28V
	GPIO_G.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_G.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_G.GPIO_OType = GPIO_OType_PP;
	GPIO_G.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOG, &GPIO_G);

	GPIOG->ODR &= ~(GPIO_ODR_ODR_5 | GPIO_ODR_ODR_7 | GPIO_ODR_ODR_8);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	GPIO_InitTypeDef GPIO_F;

	GPIO_F.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_6;
	GPIO_F.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_F.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_F.GPIO_OType = GPIO_OType_PP;
	GPIO_F.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOF, &GPIO_F);

	GPIOF->ODR &= ~(GPIO_ODR_ODR_0 | GPIO_ODR_ODR_3 | GPIO_ODR_ODR_6);

	GPIO_InitTypeDef GPIO_F_IN;

	GPIO_F_IN.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_F_IN.GPIO_Mode = GPIO_Mode_IN;
	GPIO_F_IN.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_F_IN.GPIO_OType = GPIO_OType_PP;
	GPIO_F_IN.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOF, &GPIO_F_IN);
}
void Control_Rele_Pins_SET(void)
{
	GPIOF->ODR |= (GPIO_ODR_ODR_0 | GPIO_ODR_ODR_3 |  GPIO_ODR_ODR_6);
}
void Control_Rele_Pins_RESET(void)
{
	GPIOF->ODR &= ~(GPIO_ODR_ODR_0 | GPIO_ODR_ODR_3 |  GPIO_ODR_ODR_6);
}
//-----------------------------------------------------End_Smart_Rele----------------------------------------------------
//-------------------------------------------------------Smart_LED-------------------------------------------------------
void INIT_LED_Test_Pins(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	GPIO_InitTypeDef GPIO_F;

	GPIO_F.GPIO_Pin = GPIO_Pin_13;	//SCPU to Etalon LED
	GPIO_F.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_F.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_F.GPIO_OType = GPIO_OType_PP;
	GPIO_F.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOF, &GPIO_F);

	GPIOF->ODR &= ~GPIO_ODR_ODR_13;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	GPIO_InitTypeDef GPIO_H;

	GPIO_H.GPIO_Pin = GPIO_Pin_3;	//CPU_Jlink1
	GPIO_H.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_H.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_H.GPIO_OType = GPIO_OType_PP;
	GPIO_H.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOH, &GPIO_H);

	GPIOH->ODR &= ~GPIO_ODR_ODR_3;
}
//Функция инициализации bluetooth BT-05
void BT05_INIT_Bluetooth(void)
{
	uint8_t success = 0;

	send_str("AT\r\n");
	//------------------Проверка_исправности_модуля---------------------
	RUN_TIMER();
		time = 5;
		clear_RXBuffer();
		do
		{
			if((USART6->SR & USART_SR_RXNE) == USART_SR_RXNE)
			{
				tail = 1;
				RXc = USART6->DR;
				RX_BUF[RXi] = RXc;
				if(RXi > 0)
				{
					if((RX_BUF[RXi-1] == 0x0D) && (RX_BUF[RXi] == 0x0A))
					{
						success = 1;
						break;
					}
				}
				RXi++;
			}
		}
		while(time);
	STOP_TIMER();
	//------------------------------------------------------------------
	if(success)
	{
		VCP_send_buffer_new("BLUETOOTH: SUCCESS\n",19);
		clear_RXBuffer();
		delay(1000);
		send_str("AT+ROLE\r\n");
		USART_Rx();
		if(strcmp(RX_BUF,"+ROLE=1\r\n") == 0)
			VCP_send_buffer_new("BLUETOOTH: INITIALIZED\n",23);
		else
			VCP_send_buffer_new("BLUETOOTH: NOT INITIALIZED\n",27);
		//USART_Rx();
		clear_RXBuffer();
	}
	else
	{
		VCP_send_buffer_new("BLUETOOTH: ERROR\n",17);
	}
}
//Функция поиска bluetooth устройств (BT-05)
//Возвращает флаг об окончании/завершении поиска устройств bluetooth
uint8_t BT05_Seach_Bluetooth(void)
{
	uint8_t cicle = 0;
	clear_RXBuffer();

	USART_Rx();

	if(strcmp(RX_BUF,"+INQE\r\n") == 0)
	{
		clear_RXBuffer();
		cicle = 1;
		tail = 0;
	}
	else if(tail > 0)
		if(strstr(RX_BUF, "+INQ:") != NULL)
		{
			VCP_send_buffer_new(RX_BUF, RXi);
		}

	return cicle;
}
/* Функция убрана
//Функция инициализации bluetooth
void INIT_Bluetooth(void)
{
	uint8_t success = 0;

	send_str("AT+RESET\r\n");
	//------------------Проверка_исправности_модуля---------------------
	RUN_TIMER();
		time = 5;
		clear_RXBuffer();
		do
		{
			if((USART6->SR & USART_SR_RXNE) == USART_SR_RXNE)
			{
				tail = 1;
				RXc = USART6->DR;
				RX_BUF[RXi] = RXc;
				if(RXi > 0)
				{
					if((RX_BUF[RXi-1] == 0x0D) && (RX_BUF[RXi] == 0x0A))
					{
						success = 1;
						break;
					}
				}
				RXi++;
			}
		}
		while(time);
	STOP_TIMER();
	//------------------------------------------------------------------
	if(success)
	{
		VCP_send_buffer_new("BLUETOOTH: SUCCESS\n",19);
		clear_RXBuffer();
		delay(1000);
		send_str("AT+MODE?\r\n");
		USART_Rx();
		if(strcmp(RX_BUF,"OK+Get:2\r\n") == 0)
			VCP_send_buffer_new("BLUETOOTH: INITIALIZED\n",23);
		else
			VCP_send_buffer_new("BLUETOOTH: NOT INITIALIZED\n",27);
		//USART_Rx();
		clear_RXBuffer();
	}
	else
	{
		VCP_send_buffer_new("BLUETOOTH: ERROR\n",17);
	}
}
*/
/* Функция убрана
//Функция поиска bluetooth устройств
//Возвращает флаг об окончании/завершении поиска устройств bluetooth
uint8_t Seach_Bluetooth(void)
{
	uint8_t cicle = 0;
	clear_RXBuffer();

	USART_Rx();

	if(strcmp(RX_BUF,"OK+DISCE\r\n") == 0)
	{
		clear_RXBuffer();
		cicle = 1;
		tail = 0;
	}
	else if(tail > 0)
		if(strstr(RX_BUF, "OK+NAME:") != NULL)
		{
			VCP_send_buffer_new(RX_BUF, RXi);
		}

	return cicle;
}
*/
/* Функция убрана
//Функция запроса имени устройства найденных устройств bluetooth
void READ_BT(void)
{
	num_mass = 0;
	int c = 0;
	for(uint8_t n = 0; n < num_device; n++)
	{
		c = strchr(mass[n], '|') - mass[n];
		strncpy(string2, mass[n], c);
		sprintf(string_out, "%s%s%s", string1, string2, string3);
		send_str(string_out);
		USART_Rx();
		VCP_send_buffer_new(RX_BUF, RXi);
		if(strcmp(RX_BUF,"FAIL\r\n") != 0)
		{
			USART_Rx();
			VCP_send_buffer_new(RX_BUF, RXi);
		}
		clear_RXBuffer();
		clear_TXString();
		delay(250);
	}
	num_device = 0;
}
*/
//Функция запроса имени устройства найденных устройств bluetooth (BT-05)
void READ_BT(void)
{
	num_mass = 0;
	int c = 0;
	for(uint8_t n = 0; n < num_device; n++)
	{
		c = strchr(mass[n], '|') - mass[n];
		strncpy(string2, mass[n], c);
		sprintf(string_out, "%s%s%s", string1, string2, string3);
		send_str(string_out);
		USART_Rx();
		VCP_send_buffer_new(RX_BUF, RXi);
		if(strcmp(RX_BUF,"ERROR\r\n") != 0)
		{
			USART_Rx();
			VCP_send_buffer_new(RX_BUF, RXi);
		}
		clear_RXBuffer();
		clear_TXString();
		delay(250);
	}
	num_device = 0;
}
//Функция проверки светодиодов на плате smart_led
void FIND_LIGHT_LED(void)
{
	GPIOH->ODR &= ~GPIO_ODR_ODR_3;

	uint8_t status[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	uint8_t step = 1;

	for(uint8_t n = 0; n < 10; n++)
	{
		status[n] = NO;
	}

	RUN_TIMER();
	time = 15;
	while(time)
	{
		if((GPIOE->IDR & GPIO_Pin_13) != 0)			//LED5
		{
			status[0] = YES;
		}
		if((GPIOE->IDR & GPIO_Pin_12) != 0)			//LED4
		{
			status[1] = YES;
		}
		if((GPIOE->IDR & GPIO_Pin_11) != 0)			//LED3
		{
			status[2] = YES;
		}
		if((GPIOE->IDR & GPIO_Pin_10) != 0)			//LED2
		{
			status[3] = YES;
		}
		if((GPIOE->IDR & GPIO_Pin_9) != 0)			//LED1
		{
			status[4] = YES;
		}
		if((GPIOB->IDR & GPIO_Pin_14) != 0)			//LED8
		{
			status[5] = YES;
		}
		if((GPIOB->IDR & GPIO_Pin_13) != 0)			//LED7
		{
			status[6] = YES;
		}
		if((GPIOE->IDR & GPIO_Pin_14) != 0)			//LED6
		{
			status[7] = YES;
		}
		if(step)
		{
			if((GPIOB->IDR & GPIO_Pin_15) != 0)		//LED9(ON)
			{
				status[8] = YES;
				step = 0;
			}
		}
		if(!step)
		{
			if((GPIOB->IDR & GPIO_Pin_15) == 0)		//LED9(OFF)
			{
				status[9] = YES;
			}
		}
	}
	STOP_TIMER();
	VCP_send_buffer_new("TEST_LED: ",10);
	for(uint8_t n = 0; n < 10; n++)
	{
		VCP_send_buffer_new("[",1);
		VCP_put_char_new(status[n]);
		VCP_send_buffer_new("] ",2);
	}
}
//-----------------------------------------------------End_Smart_Led-----------------------------------------------------
//-------------------------------------------------------Smart_Oled------------------------------------------------------
/* Функции убраны
//Функция проверки светодиодов на плате smart_Oled
void FIND_LIGHT_OLED(void)
{
	GPIOH->ODR &= ~GPIO_ODR_ODR_3;

	uint8_t status[10] = {0, 0, 0, 0, 0};
	uint8_t step = 1;

	for(uint8_t n = 0; n < 5; n++)
	{
		status[n] = NO;
	}

	RUN_TIMER();
	time = 15;
	while(time)
	{
		if((GPIOE->IDR & GPIO_Pin_15) != 0)			//OLED1
		{
			status[0] = YES;
		}
		if((GPIOB->IDR & GPIO_Pin_12) != 0)			//OLED2
		{
			status[1] = YES;
		}
		if((GPIOE->IDR & GPIO_Pin_7) != 0)			//OLED3
		{
			status[2] = YES;
		}
		if(step)
		{
			if((GPIOE->IDR & GPIO_Pin_8) != 0)		//OLED4(ON)
			{
				status[3] = YES;
				step = 0;
			}
		}
		if(!step)
		{
			if((GPIOE->IDR & GPIO_Pin_8) == 0)		//OLED4(OFF)
			{
				status[4] = YES;
			}
		}
	}
    STOP_TIMER();
	VCP_send_buffer_new("TEST_OLED: ",11);
	for(uint8_t n = 0; n < 5; n++)
	{
		VCP_send_buffer_new("[",1);
		VCP_put_char_new(status[n]);
		VCP_send_buffer_new("] ",2);
	}
}
//-----------------------------------------------------End_Smart_Oled----------------------------------------------------
//-------------------------------------------------------Smart_WRele-----------------------------------------------------
void INIT_WRELE_Test_Pins(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_InitTypeDef GPIO_G;

	GPIO_G.GPIO_Pin = GPIO_Pin_6;		//Wrele_rs_AB
	GPIO_G.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_G.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_G.GPIO_OType = GPIO_OType_PP;
	GPIO_G.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOG, &GPIO_G);

	GPIOG->ODR &= ~GPIO_ODR_ODR_6;
}
//-----------------------------------------------------End_Smart_WRele---------------------------------------------------
void INIT_BUTTON_START(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_B;

	GPIO_B.GPIO_Pin = GPIO_Pin_0;		//PUSK
	GPIO_B.GPIO_Mode = GPIO_Mode_IN;
	GPIO_B.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_B.GPIO_OType = GPIO_OType_PP;
	GPIO_B.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOB, &GPIO_B);
}
*/
void INIT_Usart(void)
{
	GPIO_InitTypeDef GPIO;
	USART_InitTypeDef USART;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	//USART6 TX (PC6)
	GPIO.GPIO_Pin = GPIO_Pin_6;
	GPIO.GPIO_Mode = GPIO_Mode_AF;
	GPIO.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO.GPIO_OType = GPIO_OType_PP;
	GPIO.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO);

	//USART6 TX (PC7)
	GPIO.GPIO_Pin = GPIO_Pin_7;
	GPIO.GPIO_Mode = GPIO_Mode_AF;
	GPIO.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO.GPIO_OType = GPIO_OType_PP;
	GPIO.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

	USART.USART_BaudRate = 9600;//38400;//921600;
	USART.USART_WordLength = USART_WordLength_8b;
	USART.USART_StopBits = USART_StopBits_1;
	USART.USART_Parity = USART_Parity_No;
	USART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART.USART_Mode =	USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART6, &USART);

	USART_Cmd(USART6, ENABLE);
}
//Функция инициализации таймера
void INIT_TIMER(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseInitTypeDef TIMER6;

	TIM_TimeBaseStructInit(&TIMER6);
	TIMER6.TIM_Prescaler = 42000 - 1;
	TIMER6.TIM_Period = 2000;
	TIM_TimeBaseInit(TIM6, &TIMER6);
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
}
//Функция запуска таймера
void RUN_TIMER(void)
{
    TIM_Cmd(TIM6, ENABLE);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
}
//Функция остановки таймера
void STOP_TIMER(void)
{
    TIM_Cmd(TIM6, DISABLE);
    NVIC_DisableIRQ(TIM6_DAC_IRQn);
}
//Функция передачи байта по USART
void USART_Tx(unsigned char Data)
{
	while(!(USART6->SR & USART_SR_TC));
	USART6->DR = Data;
}
//Функция приема ответа от модуля bluetooth
void USART_Rx(void)
{
	clear_RXBuffer();
	do
	{
		if((USART6->SR & USART_SR_RXNE) == USART_SR_RXNE)
		{
			tail = 1;
			RXc = USART6->DR;
			RX_BUF[RXi] = RXc;
			if(RXi > 0)
			{
				if((RX_BUF[RXi-1] == 0x0D) && (RX_BUF[RXi] == 0x0A))
				{
					break;
				}
			}
			RXi++;
		}
	}
	while(1);
}
//Функция отправки комманды модулю bluetooth
//Принимает комманду в виде строки типа: "AT+command/r/n"
void send_str(char * string)
{
	uint8_t i = 0;
	while(string[i])
	{
		USART_Tx(string[i]);
		i++;
	}
}
