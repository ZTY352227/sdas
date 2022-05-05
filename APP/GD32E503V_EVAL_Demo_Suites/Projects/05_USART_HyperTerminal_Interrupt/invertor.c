#include "invertor.h"
#include "uart.h"
#include "ht1621b.h"


uint8_t AC_Modbus_Recv[AC_BUFFER_LEN] = { 0x00 };
uint8_t AC_Modbus_Send[AC_BUFFER_LEN] = { 0x00 };
volatile uint8_t AC_RecvOK  = 0;
volatile uint8_t AC_RecvLen = 0; //接收缓存，接收长度，接收完成标志

AC_Information_P01_t AC_Info_P01;
AC_Information_P02_t AC_Info_P02;
AC_Information_Temp_t AC_Info_Temp;


//处理接收到的数据
void AC_Handel_Modbus(void)
{
    uint16_t registerAddr, Num;
    uint16_t crc;
    uint8_t  functionCode;

    if (AC_RecvOK == 1)           //接收到数据
    {
        registerAddr = ((uint16_t)AC_Modbus_Send[2] << 8) + AC_Modbus_Send[3];

        if (AC_Modbus_Recv[0] != 0x01)               //校验地址
        {
            goto end;
        }

        //CRC校验
        crc = (uint16_t)(AC_Modbus_Recv[AC_RecvLen - 1] << 8) + AC_Modbus_Recv[AC_RecvLen - 2];
        if (crc != usMBCRC16(AC_Modbus_Recv, AC_RecvLen - 2))
        {
            goto end;
        }

        functionCode = AC_Modbus_Recv[1];                //功能码


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
    static uint16_t ac_register_addr = AC_CHARGE_POWER; //初始值

    AC_Modbus_Send[0] = 0x01; //AC从地址
    AC_Modbus_Send[1] = 0x03; //只读
    AC_Modbus_Send[2] = ac_register_addr >> 8;
    AC_Modbus_Send[3] = ac_register_addr;
    AC_Modbus_Send[4] = 0x00;
    AC_Modbus_Send[5] = 0x01; //只读一个
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
		AC_Modbus_Send[5] = 0x03; //读3个寄存器，温度
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
        while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));//等待发送结束
        /* transmit data */
        usart_data_transmit(USART1, pbuf[i]);
    }
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));//等待发送结束
	
	DelayMS(1);
	AC_RS485_RECEIVE();
	usart_receive_config(USART1, ENABLE);
	usart_receive_config(USART1, USART_RECEIVE_ENABLE);
//	usart_enable(USART1);
//	usart_data_receive(USART1);
//	usart_interrupt_enable(USART1, USART_INT_RBNE);
}

//处理多个寄存器返回数据
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
