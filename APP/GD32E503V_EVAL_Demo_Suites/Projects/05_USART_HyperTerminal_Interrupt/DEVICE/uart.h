#ifndef __UART_H
#define __UART_H			 
#include "gd32e50x.h"


void uart_init(void);
uint16_t usMBCRC16(uint8_t * pucFrame, uint16_t usLen);

#endif	
