//STM32F103C8T6串口驱动 serial.c

#include "common.h"

//boot通信口初始化　此处使用UART1
//void BootComInit(uint32_t ulWantedBaud)
//{
//	USART_InitTypeDef USART_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//		
//	/* Enable USART1 clock */
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );	

//	/* Configure USART1 Rx (PA10) as input floating */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init( GPIOA, &GPIO_InitStructure );
//	
//	/* Configure USART1 Tx (PA9) as alternate function push-pull */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

//	GPIO_Init( GPIOA, &GPIO_InitStructure );
//	
//	USART_InitStructure.USART_BaudRate = ulWantedBaud;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No ;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	
//	USART_Init( USART1, &USART_InitStructure );
//	
//	USART_Cmd( USART1, ENABLE );
//}

void BootComSendByte(uint8_t dt)
{
	//while (!(USART1->SR & USART_FLAG_TXE)); 
	//USART1->DR = dt;
	
	usart_data_transmit(USART0,(uint8_t)dt);
	while(usart_flag_get(USART0,USART_FLAG_TC) == RESET);	
}
void BootComSend (uint8_t *buf,uint8_t size) 
{
	uint8_t i;
	for(i=0;i<size;i++)
		BootComSendByte(buf[i]);
}

uint8_t  BootComRecv (uint8_t *buf,uint16_t size,uint32_t timeout) 
{
	uint16_t i;
	uint32_t timer=GetCurrentTick();
	
	for(i=0;i<size;i++)
	{
		//while (!(USART1->SR & USART_FLAG_RXNE))
		while(usart_flag_get(USART0,USART_FLAG_RBNE)==RESET)
		{
			if(GetPassedTick(timer) >= timeout)
				return 0;
		}
		buf[i]=(uint8_t )usart_data_receive(USART0);
	}

	return 1;
} 










