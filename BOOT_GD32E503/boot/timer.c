//timer.c
#include"common.h"

//1ms��ʱ��
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



//��ʼ��
void InitTick(void)
{
	g_SysTimer=0;	 
	systick_config();	//��core_cm3.h�ļ��ж���˺���
}

//ϵͳ��ʱ���жϺ���
void SysTick_Handler( void )
{
	g_SysTimer++;		
	
	if(updateTimeOut >0 && updateTimeOut < 60000)
		updateTimeOut++;
}
//��ȡ��ǰ��ʱֵ
uint32_t GetCurrentTick(void)
{
	uint32_t ret;
	CLI();
	ret=g_SysTimer;
	SEI();
	return ret;
}
//�����ȥʱ��
uint32_t GetPassedTick(uint32_t t)
{
	uint32_t cur=GetCurrentTick();

	if(cur>=t)
		return (cur-t);

	return (cur+(0xffffffff-t));
}
//��ʱ
void SysDelayMs(uint32_t t)
{
	uint32_t tick=GetCurrentTick();
	while(GetPassedTick(tick)<t);
}
//////////////////////////////////////////

//������ʱ���뺯��(ϵͳʱ�ӣ�72MHz) 
void DelayMs(uint32_t t)
{
	static uint32_t MsDelayCnt;
	while(t--)
	for(MsDelayCnt=0;MsDelayCnt<10280;MsDelayCnt++);
}

//������ʱ΢�뺯��(ϵͳʱ��:72MHz)
void DelayUs(uint32_t t)
{
	static uint32_t UsDelayCnt;
	while(t--)
	for(UsDelayCnt=0;UsDelayCnt<10;UsDelayCnt++);
}

