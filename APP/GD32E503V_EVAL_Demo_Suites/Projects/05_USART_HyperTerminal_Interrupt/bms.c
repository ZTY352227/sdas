#include "bms.h"
#include "uart.h"
#include "ht1621b.h"

uint8_t BMS_Modbus_Recv[BMS_BUFFER_LEN] = { 0x00 };
uint8_t BMS_Modbus_Send[BMS_BUFFER_LEN] = { 0x00 };
volatile uint8_t BMS_RecvOK  = 0;
volatile uint8_t BMS_RecvLen = 0; //���ջ��棬���ճ��ȣ�������ɱ�־

BMS_Information_Pack_t BMS_Info_Pack;

//������յ�������
void BMS_Handel_Modbus(void)
{
    uint16_t registerAddr, Num;
    uint16_t crc;
    uint8_t  functionCode;

    if (BMS_RecvOK == 1)           //���յ�����
    {
        registerAddr = (uint16_t)(BMS_Modbus_Send[2] << 8) + BMS_Modbus_Send[3];

        if (BMS_Modbus_Recv[0] != 0x02)               //У���ַ
        {
            goto end;
        }

        //CRCУ��
        crc = (uint16_t)(BMS_Modbus_Recv[BMS_RecvLen - 1] << 8) + BMS_Modbus_Recv[BMS_RecvLen - 2];
        if (crc != usMBCRC16(BMS_Modbus_Recv, BMS_RecvLen - 2))
        {
            goto end;
        }

        functionCode = BMS_Modbus_Recv[1];                //������


        switch (registerAddr)
        {
        case PACK_VOLTAGE:
			Num = (uint16_t)(BMS_Modbus_Send[4] << 8) + BMS_Modbus_Send[5];
			BMS_Handle_Multiple(PACK_VOLTAGE, Num, &BMS_Modbus_Recv[3]);
            break;

        default:
            break;
        }
end:
        BMS_RecvLen = 0;
        BMS_RecvOK = 0;
    }
}

void BMS_Diag(void)
{
    uint16_t crc;
    static uint16_t bms_register_addr = PACK_VOLTAGE; //��ʼֵ

    BMS_Modbus_Send[0] = 0x02; //AC�ӵ�ַ
    BMS_Modbus_Send[1] = 0x03; //ֻ��
    BMS_Modbus_Send[2] = bms_register_addr >> 8;
    BMS_Modbus_Send[3] = bms_register_addr;
    BMS_Modbus_Send[4] = 0x00;
    BMS_Modbus_Send[5] = 0x0A; 
    crc = usMBCRC16(BMS_Modbus_Send, 6);
    BMS_Modbus_Send[6] = crc;
    BMS_Modbus_Send[7] = crc >> 8;

    switch (bms_register_addr)
    {
    case PACK_VOLTAGE:
		BMS_Modbus_Send[4] = 0x00;
		BMS_Modbus_Send[5] = 0x0a; 
        BMS_UartCmd_Send(BMS_Modbus_Send, 8);
        break;

    default:
        break;
    }
}

void BMS_UartCmd_Send(uint8_t *pbuf, uint8_t len)
{
    uint8_t i;
	
    BMS_RS485_SEND();
    for (i = 0; i < len; i++)
    {
        while(usart5_flag_get(USART5, USART5_FLAG_TBE) == RESET);//�ȴ����ͽ���
        /* transmit data */
        usart_data_transmit(USART5, pbuf[i]);
    }
    while(usart5_flag_get(USART5, USART5_FLAG_TBE) == RESET);//�ȴ����ͽ���
	DelayMS(1);
    BMS_RS485_RECEIVE();
}


//�������Ĵ�����������
void BMS_Handle_Multiple(uint16_t addr, uint16_t num, uint8_t *pbuf)
{
	uint8_t   Len, i, *p;
	
	Len = 2;

	if(addr >= PACK_VOLTAGE && (addr+num-1) <= ERROR_STATE)
	{
		 p = (uint8_t *)&BMS_Info_Pack.pack_voltage + 2*(addr - PACK_VOLTAGE);
		
		for(i=0;i<num;i++)
		{
			*p = pbuf[2*i+1];
			*(p+1) = pbuf[2*i];
			
			p += 2;
		}
	}	
}


