//common.h
#ifndef COMMON_H
#define COMMON_H

#include "gd32e50x.h"

#include "serial.h"
#include "CASBOOT.h"

#define CLI()  __set_PRIMASK(1)  
#define SEI()  __set_PRIMASK(0) 

//main.c
#define LED_OFF  GPIO_SetBits(GPIOE , GPIO_Pin_5)
#define LED_ON GPIO_ResetBits(GPIOE,GPIO_Pin_5)
#define LED_FLASH 	GPIOB->ODR ^= GPIO_Pin_5;

//timer.c
void DelayUs(uint32_t t);
void DelayMs(uint32_t t);
void InitTick(void);
uint32_t GetCurrentTick(void);	   //获取当前定时值
uint32_t GetPassedTick(uint32_t t);  //计算过去时间
void SysDelayMs(uint32_t t);

						
#endif
