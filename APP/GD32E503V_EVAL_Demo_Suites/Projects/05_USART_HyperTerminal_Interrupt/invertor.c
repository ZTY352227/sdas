#include "invertor.h"
#include "uart.h"
#include "ht1621b.h"


uint8_t AC_Modbus_Recv[AC_BUFFER_LEN] = { 0x00 };
uint8_t AC_Modbus_Send[AC_BUFFER_LEN] = { 0x00 };
volatile uint8_t AC_RecvOK  = 0;
volatile uint8_t AC_RecvLen = 0; //���ջ��棬���ճ��ȣ�������ɱ�־

AC_Information_P01_t AC_Info_P01;
AC_Information_P02_t AC_Info_P02;
AC_Information_Temp_t AC_Info_Temp;


//������յ�������
void AC_Handel_Modbus(void)
{
    uint16_t registerAddr, Num;
    uint16_t crc;
    uint8_t  functionCode;

    if (AC_RecvOK == 1)           //���յ�����
    {
        registerAddr = ((uint16_t)AC_Modbus_Send[2] << 8) + AC_Modbus_Send[3];

        if (AC_Modbus_Recv[0] != 0x01)               //У���ַ
        {
            goto end;
        }

        //CRCУ��
        crc = (uint16_t)(AC_Modbus_Recv[AC_RecvLen - 1] << 8) + AC_Modbus_Recv[AC_RecvLen - 2];
        if (crc != usMBCRC16(AC_Modbus_Recv, AC_RecvLen - 2))
        {
            goto end;
        }

        functionCode = AC_Modbus_Recv[1];                //������


        switch (registerAddr)
        {
        case AC_CHARGE_POWER:
            AC_Info_P01.charge_power = (uint16_t)(AC_Modbus_Recv[3] << 8) + AC_Modbus_Recv[4];
            break;
        case AC_DISCHARGE_POWER:
            AC_Info_P02.load_active_power = (uint16_t)(AC_Modbus_Recv[3] << 8) + AC_Modbus_Recv[4];
            break;
		case DC_DC_TEMP:
			Num = (uint16_t)(AC_Modbus_Send[4] << 8) + AC_Modbus_Send[5];
			AC_Handle_Multiple(DC_DC_TEMP, Num, &AC_Modbus_Recv[3]);
            break;

        default:
            break;
        }
end:
        AC_RecvLen = 0;
        AC_RecvOK = 0;
    }
}

void AC_Diag(void)
{
    uint16_t crc;
    static uint16_t ac_register_addr = AC_CHARGE_POWER; //��ʼֵ

    AC_Modbus_Send[0] = 0x01; //AC�ӵ�ַ
    AC_Modbus_Send[1] = 0x03; //ֻ��
    AC_Modbus_Send[2] = ac_register_addr >> 8;
    AC_Modbus_Send[3] = ac_register_addr;
    AC_Modbus_Send[4] = 0x00;
    AC_Modbus_Send[5] = 0x01; //ֻ��һ��
    crc = usMBCRC16(AC_Modbus_Send, 6);
    AC_Modbus_Send[6] = crc;
    AC_Modbus_Send[7] = crc >> 8;

    switch (ac_register_addr)
    {

    case AC_CHARGE_POWER:
        AC_UartCmd_Send(AC_Modbus_Send, 8);
        ac_register_addr = AC_DISCHARGE_POWER;
        break;

    case AC_DISCHARGE_POWER:
        AC_UartCmd_Send(AC_Modbus_Send, 8);
        ac_register_addr = DC_DC_TEMP;
        break;
	
	case DC_DC_TEMP:
		AC_Modbus_Send[4] = 0x00;
		AC_Modbus_Send[5] = 0x03; //��3���Ĵ������¶�
		crc = usMBCRC16(AC_Modbus_Send, 6);
		AC_Modbus_Send[6] = crc;
		AC_Modbus_Send[7] = crc >> 8;
		AC_UartCmd_Send(AC_Modbus_Send, 8);
		ac_register_addr = AC_CHARGE_POWER;

    default:
        break;

    }
}

void AC_UartCmd_Send(uint8_t *pbuf, uint8_t len)
{
    uint8_t i;
//	usart_interrupt_disable(USART1, USART_INT_RBNE);
	usart_receive_config(USART1, USART_RECEIVE_DISABLE);
    AC_RS485_SEND();
    for (i = 0; i < len; i++)
    {
        while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));//�ȴ����ͽ���
        /* transmit data */
        usart_data_transmit(USART1, pbuf[i]);
    }
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));//�ȴ����ͽ���
	
	DelayMS(1);
	AC_RS485_RECEIVE();
	usart_receive_config(USART1, ENABLE);
	usart_receive_config(USART1, USART_RECEIVE_ENABLE);
//	usart_enable(USART1);
//	usart_data_receive(USART1);
//	usart_interrupt_enable(USART1, USART_INT_RBNE);
}

//�������Ĵ�����������
void AC_Handle_Multiple(uint16_t addr, uint16_t num, uint8_t *pbuf)
{
	uint8_t   Len, i, *p;
	
	Len = 2;

	if(addr >= DC_DC_TEMP && (addr+num-1) <= TRANSFORMER_TEMP)
	{
		 p = (uint8_t *)&AC_Info_Temp.dc_dc_temp + 2*(addr - DC_DC_TEMP);
		
		for(i=0;i<num;i++)
		{
			*p = pbuf[2*i+1];
			*(p+1) = pbuf[2*i];
			
			p += 2;
		}
	}	
}
