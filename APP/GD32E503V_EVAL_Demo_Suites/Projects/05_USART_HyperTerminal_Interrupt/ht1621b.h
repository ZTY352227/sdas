#ifndef __HT1621B_H
#define __HT1621B_H

#include "gd32e50x.h"
#include "gd32e503v_eval.h"


#define BIAS   0x52           //0b1000 0101 0010  0x52 :1/3duty 4com          0X50: 1/2偏置
#define SYSDIS 0X00           //0b1000 0000 0000  关振系统荡器和LCD偏压发生器
#define SYSEN  0X02           //0b1000 0000 0010 打开系统 振荡器
#define LCDOFF 0X04           //0b1000 0000 0100  关LCD偏压
#define LCDON  0X06           //0b1000 0000 0110  打开LCD偏 压
#define XTAL   0x28           //0b1000 0010 1000 外部接时钟
#define RC256  0X30           //0b1000 0011 0000  内部时钟RC256K
#define TONEON  0X12           //0b1000 0001 0010  打开声音 输出
#define TONEOFF 0X10           //0b1000 0001 0000 关闭声音输出
#define WDTDIS  0X0A        //0b1000 0000 1010  禁止看门狗
#define TONE_4K 0x80        //4k声音输出频率
#define TONE_2K 0xC0

//HT1621控制位（液晶模块接口定义，根据自已的需要更改）

#define CLS_HT_WR()   		 gpio_bit_reset(GPIOB, GPIO_PIN_15)   	//拉低
#define SET_HT_WR()   		 gpio_bit_set(GPIOB, GPIO_PIN_15)   	//拉高
#define CLS_HT_CS()   		 gpio_bit_reset(GPIOB, GPIO_PIN_12)   	//拉低
#define SET_HT_CS()    		 gpio_bit_set(GPIOB, GPIO_PIN_12)  		//拉高
#define CLS_HT_DATA()  		 gpio_bit_reset(GPIOB, GPIO_PIN_14)   	//数据位低
#define SET_HT_DATA()   	 gpio_bit_set(GPIOB, GPIO_PIN_14)     	//数据位高
#define CLS_HT_LIGHT()  	 gpio_bit_reset(GPIOE, GPIO_PIN_7)   	//背光关
#define SET_HT_LIGHT()  	 gpio_bit_set(GPIOE, GPIO_PIN_7)     	//背光开





void ht1621b_init(void);
void ht_gpio_init(void);
void Delay(uint16_t us);
void DelayMS(uint16_t iMs);
void Ht1621Wr_Data(uint8_t Data,uint8_t cnt);
void Ht1621WrOneData(uint8_t Addr,uint8_t Data);
void Ht1621WrOneNum(uint8_t Addr,uint8_t num);
void Ht1621WrAllData(uint8_t data);
void Ht1621WrCmd(uint8_t Cmd);


#endif