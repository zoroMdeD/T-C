#ifndef _COMMAND_H
#define _COMMAND_H

uint8_t buf[APP_TX_BUF_SIZE];				//����� ��� �������� ������ �� ��
char buffer_command[40];					//����� ��� ������ ������ � ��
char buf_Rx[512];
uint8_t pnt;								//�������
USB_CDC_RXSTATUS_t check;					//���� ������� ���������� USB CDC

uint8_t check_button;						//���� ������� ������� ������ ������ ��������������� ������������
uint8_t time;								//�������� ���-�� ������� ������� �������� ���������� ����������

//---------------------------------------------------------ERIS----------------------------------------------------------
void clear_RXBuffer(void);
void clear_TXString(void);
void INIT_All(void);
void FAKE_BlueTooth(void);
void INIT_Power_NI_or_Power_Summ_NI(void);
void INIT_Reboot_USB(void);
void INIT_Comparator_Pins(void);
//-------------------------------------------------------Smart_Biz-------------------------------------------------------
void INIT_Fuse_Test_Pins(void);
void INIT_Diode_Test_Pins(void);
void INIT_Ringing_Pins(void);
//-----------------------------------------------------End_Smart_Biz-----------------------------------------------------
//-------------------------------------------------------Smart_Cpu-------------------------------------------------------
void INIT_CPU_Test_Pins(void);
void INIT_CPU_ETALON_TEST_Pins(void);
void INIT_InputEtalonLed_Pins(void);
void FIND_LIGHT_ETALON_LED(void);
//-----------------------------------------------------End_Smart_Cpu-----------------------------------------------------
//-------------------------------------------------------Smart_Rele------------------------------------------------------
void INIT_RELE_Test_Pins(void);
void Control_Rele_Pins_SET(void);
void Control_Rele_Pins_RESET(void);
//-----------------------------------------------------End_Smart_Rele----------------------------------------------------
//-------------------------------------------------------Smart_LED-------------------------------------------------------
void INIT_LED_Test_Pins(void);
void INIT_RELE_Test_Pins_IN(void);
void BT05_INIT_Bluetooth(void);
uint8_t BT05_Seach_Bluetooth(void);
void INIT_Bluetooth(void);
uint8_t Seach_Bluetooth(void);
void READ_BT(void);
void FIND_LIGHT_LED(void);
//-----------------------------------------------------End_Smart_Led-----------------------------------------------------
//-------------------------------------------------------Smart_OLED------------------------------------------------------
void FIND_LIGHT_OLED(void);
//-----------------------------------------------------End_Smart_OLed----------------------------------------------------
//-------------------------------------------------------Smart_WRele-----------------------------------------------------
void INIT_WRELE_Test_Pins(void);
//-----------------------------------------------------End_Smart_WRele---------------------------------------------------
void INIT_Usart(void);

void INIT_TIMER(void);
void RUN_TIMER(void);
void STOP_TIMER(void);

void USART_Rx(void);
void send_str(char * string);

void VCP_put_char_new( uint8_t buf);
void VCP_send_buffer_new(uint8_t * buf, int len);

#endif

