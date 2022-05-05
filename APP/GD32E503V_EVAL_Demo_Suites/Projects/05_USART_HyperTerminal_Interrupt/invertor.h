#ifndef __INVERTOR_H
#define __INVERTOR_H

#include "gd32e50x.h"
#include "gd32e503v_eval.h"



#define AC_BUFFER_LEN     255
#define AC_MODBUS_TIMEOUT  500

#define AC_RS485_RECEIVE()		gpio_bit_reset(GPIOD, GPIO_PIN_0) //��������ģʽ
#define AC_RS485_SEND()  	 	gpio_bit_set(GPIOD, GPIO_PIN_0)

//ֻ���Ĵ���
#define AC_SOC                                0x100
#define AC_CHARGE_POWER                       0x10E
#define AC_DISCHARGE_POWER                    0x21B


#define DC_DC_TEMP					  0x220		//DC-DCɢ��Ƭ�¶�
#define DC_AC_TEMP					  0x221		//DC-ACɢ��Ƭ�¶�
#define TRANSFORMER_TEMP			  0x222		//��ѹ���¶�


extern uint8_t  AC_Modbus_Recv[AC_BUFFER_LEN];
extern volatile uint8_t AC_RecvOK;
extern volatile uint8_t AC_RecvLen;


typedef struct
{
    uint16_t soc; 					//���ʣ������ٷֱ�
    uint16_t voltage; 				//��ص�ѹ V  0.1��
    int16_t current; 				//��ص��� A 0.1��  ����0�ŵ磬С��0���
    uint16_t pv_voltage;			//̫���ܰ��ѹ V 0.1��
    uint16_t pv_current;			//̫���ܰ���� A 0.1��
    uint16_t pv_power;				//̫���ܰ幦�� W
    uint16_t load_charge_state;		//����״̬�ͳ��״̬ 0x0000�����أ�0x0001����䣬0x0002����ѹ��磬0x0004�����䣬0x0006��Li battery acitvate
    uint16_t charge_power;			//��繦�� W

} AC_Information_P01_t; 			//P01 ������������

extern AC_Information_P01_t AC_Info_P01;

typedef struct
{
    uint16_t ac_voltage; 			//����ѹ 0.1V
    uint16_t ac_current; 			//������ 0.1A
    uint16_t ac_frequency;			//���Ƶ�� 0.01Hz
    uint16_t load_current;			//���ص��� 0.1A

    uint16_t load_active_power;		//�����й����� W
    uint16_t load_apparent_power;	//�������ڹ��� VA

} AC_Information_P02_t; 			//P02 �����������

extern AC_Information_P02_t AC_Info_P02;

typedef struct
{
    uint16_t dc_dc_temp; 				//DC-DCɢ��Ƭ�¶� 0.1��
    uint16_t dc_ac_temp; 				//DC-ACɢ��Ƭ�¶� 0.1��
    uint16_t transformer_temp;			//��ѹ���¶� 0.1��

} AC_Information_Temp_t; 			//������¶�����

extern AC_Information_Temp_t AC_Info_Temp;


void AC_Handel_Modbus(void);
void AC_Diag(void);

void AC_UartCmd_Send(uint8_t *pbuf, uint8_t len);
void AC_Handle_Multiple(uint16_t addr, uint16_t num, uint8_t *pbuf);

#endif