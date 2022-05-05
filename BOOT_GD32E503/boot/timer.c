//timer.c
#include"common.h"

//1ms定时器
volatile uint32_t g_SysTimer;
extern   uint32_t updateTimeOut;

void systick_config(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U)){
        /* capture error */
        while (1){
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}



//初始化
void InitTick(void)
{
	g_SysTimer=0;	 
	systick_config();	//在core_cm3.h文件中定义此函数
}

//系统定时器中断函数
void SysTick_Handler( void )
{
	g_SysTimer++;		
	
	if(updateTimeOut >0 && updateTimeOut < 60000)
		updateTimeOut++;
}
//获取当前定时值
uint32_t GetCurrentTick(void)
{
	uint32_t ret;
	CLI();
	ret=g_SysTimer;
	SEI();
	return ret;
}
//计算过去时间
uint32_t GetPassedTick(uint32_t t)
{
	uint32_t cur=GetCurrentTick();

	if(cur>=t)
		return (cur-t);

	return (cur+(0xffffffff-t));
}
//延时
void SysDelayMs(uint32_t t)
{
	uint32_t tick=GetCurrentTick();
	while(GetPassedTick(tick)<t);
}
//////////////////////////////////////////

//粗略延时毫秒函数(系统时钟：72MHz) 
void DelayMs(uint32_t t)
{
	static uint32_t MsDelayCnt;
	while(t--)
	for(MsDelayCnt=0;MsDelayCnt<10280;MsDelayCnt++);
}

//粗略延时微秒函数(系统时钟:72MHz)
void DelayUs(uint32_t t)
{
	static uint32_t UsDelayCnt;
	while(t--)
	for(UsDelayCnt=0;UsDelayCnt<10;UsDelayCnt++);
}

