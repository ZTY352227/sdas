
//main.c
/**********************************

**********************************/

#include "common.h"
#include "gd32e503v_eval.h"
#include "gd32e50x_rcu.h"
#include "gd32e50x_usart.h"
#include "gd32e50x_fmc.h"
#include "gd32e50x_bkp.h"
//#include <stdlib.h>
//#include <stdio.h>
//#include "core_cmFunc.h"


#define  AC_LED2  gpio_bit_set(GPIOC, GPIO_PIN_12)
#define  AC_LED2_OFF  gpio_bit_reset(GPIOC, GPIO_PIN_12)
#define  TOGGLE_AC_LED2  gpio_bit_write(GPIOC, GPIO_PIN_12, 1-gpio_output_bit_get(GPIOC, GPIO_PIN_12))

#define  PD_LED1  gpio_bit_set(GPIOC, GPIO_PIN_11)
#define  PD_LED1_OFF  gpio_bit_reset(GPIOC, GPIO_PIN_11)
#define  TOGGLE_PD_LED1  gpio_bit_write(GPIOC, GPIO_PIN_11, 1-gpio_output_bit_get(GPIOC, GPIO_PIN_11))

#define BOOT_VERSION       10     //BOOT�汾  V1.0
#define BOOT_FLASH_SIZE    (256 - 16 - 8 - 8)     //оƬFLASH��С��KBΪ��λ  16KΪBOOT�ռ� 8KΪ�����Ϣ���� 8KΪϵͳ������Ϣ�ռ�
#define DEVICE_ID          0x04    //

#define MAIN_USER_FLASH_BEGIN 0x08004000  //ǰ16k����boot����ʹ��
#define MAIN_USER_FLASH_END	  0x08038000




#define PAGE_SIZE 64           //64  256 
#define FLASH_PAGE_SIZE 8192    //2048   8192

uint8_t g_WriteEnable=0;
uint8_t g_Buffer[PAGE_SIZE*2];
uint32_t g_Address;

void RCC_Configuration(void)
{
    //HCLK=72M  PCLK1(APB1ʱ��)=36M  PCLK2(APB2ʱ��)=72M
    //������main����ǰִ�е�SystemInit�������ú�

    //GPIOA GPIOBʱ������
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);

    //��ֹJTAG,SWJ
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable , ENABLE);
}

////���Ź�����
//void IwdgOpen(void)
//{
//	//����д��ؼĴ���
//	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
//	//����IWDGʱ��: 40KHz(LSI) / 256 = 156Hz
//	IWDG_SetPrescaler(IWDG_Prescaler_256);
//	//�������¼���ֵΪ 156 �ò�������ȡֵ��ΧΪ 0�C0x0FFF
//	IWDG_SetReload(156);
//	//IWDG��������λ
//	IWDG_ReloadCounter();
//	//ʹ��IWDG (the LSI oscillator will be enabled by hardware)
//	IWDG_Enable();
//}

typedef void (*RESET_FUNCTION )(void);


#define app_address    0x08004000

typedef  void (*pFunction)(void);
pFunction jump_To_application;

uint32_t jump_address = 0;

void ExecApp(void)
{
    uint32_t jump_addr=*((__IO uint32_t *)(app_address+4));
    RESET_FUNCTION Reset=(RESET_FUNCTION)jump_addr;

    AC_LED2_OFF;
    PD_LED1_OFF;
    //dma_channel_disable(DMA0, DMA_CH4);
    jump_address = *(__IO uint32_t*)(app_address + 4);               //�û��������ڶ����ִ洢ΪAPP������ʼ��ַ��APP����λ����ָ�룩
    jump_To_application = (pFunction) jump_address;
    __set_MSP(*(__IO uint32_t*)app_address);                         //��ʼ��APP�����ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
    jump_To_application();                                           //����PCָ��ΪAPP����ĸ�λ������ַ
    while(1);
}

//���ݰ�У��
uint8_t CheckSum(uint8_t *buf,uint16_t len)
{
    uint16_t i,ret=0;
    for(i=0; i<len; i++)
        ret+=buf[i];
    return ret;
}

//���õ�ַ
void SetPageAddr(void)
{
    uint8_t i;

    if(BootComRecv(g_Buffer,5,1000)==0)
    {
        BootComSendByte(1);//���ճ�ʱ
        return ;
    }

    if(CheckSum(g_Buffer,4)!=g_Buffer[4])
    {
        BootComSendByte(2);//У�����
        return ;
    }

    for(i=0; i<4; i++)
    {
        g_Address<<=8;		//��ȡAPPд����ʼ��ַ
        g_Address+=g_Buffer[i];
    }

    if((g_Address < app_address) || (g_Address >= MAIN_USER_FLASH_END))
    {
        BootComSendByte(3);//��ַ����
        return ;
    }

    g_Address=app_address; //Ĭ��ֵ, ��ʼ��ֱַ�Ӹ�ֵ
    BootComSendByte(0);
}

//д��һ�����ݰ�
void WritePage(void)
{
    uint16_t i;
    uint32_t *p;

    if(BootComRecv(g_Buffer,PAGE_SIZE+1,1000)==0)
    {
        BootComSendByte(1);//���ճ�ʱ
        return ;
    }

    if(CheckSum(g_Buffer,PAGE_SIZE)!=g_Buffer[PAGE_SIZE])
    {
        BootComSendByte(2);//У�����
        return ;
    }

    if(g_WriteEnable==0xAA)
    {
        if(g_Address % FLASH_PAGE_SIZE == 0) //�������ҳ
            fmc_page_erase(g_Address);
        p=(uint32_t *)g_Buffer;
        for (i = 0; i < PAGE_SIZE; i+=4,p++)
            fmc_word_program(g_Address+i, *p);
    }
    else
    {
        BootComSendByte(3);//û�����ӣ�������д
        return ;
    }

    g_Address+=PAGE_SIZE;
    BootComSendByte(0);//�ɹ����
}

//��һ������
void ReadPage(void)
{
    uint16_t i;
    uint8_t *p;
    p=(uint8_t *)(g_Address);

    for(i=0; i<PAGE_SIZE; i++)
    {
        g_Buffer[i]=p[i];
        BootComSendByte(g_Buffer[i]);
    }

    BootComSendByte(CheckSum(g_Buffer,PAGE_SIZE));
    g_Address+=PAGE_SIZE;
}

//����:0xAA + 'BOOT' + 0x34
uint8_t Connect(uint32_t wait_time)
{
    if(BootComRecv(g_Buffer,5,wait_time)==0)
        return 1;

    if((g_Buffer[0]=='B')&&(g_Buffer[1]=='O')&&(g_Buffer[2]=='O')&&(g_Buffer[3]=='T')&&(g_Buffer[4]==0x34))
    {
        g_Buffer[0]=BOOT_CONNECT;
        g_Buffer[1]=BOOT_VERSION;
        g_Buffer[2]=0x08;//оƬ����ʵ��ַ 0x08000000
        g_Buffer[3]=0x00;
        g_Buffer[4]=0x00;
        g_Buffer[5]=0x00;
        g_Buffer[6]=BOOT_FLASH_SIZE;//����FLASH�ռ��С
        g_Buffer[7]=DEVICE_ID;//����ID
        g_Buffer[8]=0; //Э��汾Ϊv0
        g_Buffer[9]=CheckSum(g_Buffer+1,8);
        BootComSend(g_Buffer,10);

        g_WriteEnable=0xAA;
        fmc_unlock();
        return 0;
    }

    return 1;
}


uint32_t updateTimeOut = 0;

int main(void)
{
    uint8_t tmp;
    uint16_t bkp;

    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x0);
    SEI();

    /* ������Դʹ�� */
//    rcu_periph_clock_enable(RCU_GPIOB);
//    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
//    gpio_bit_reset(GPIOB, GPIO_PIN_12);

    //����ָʾ��
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11|GPIO_PIN_12);
    AC_LED2_OFF;
    PD_LED1_OFF;

    RCC_Configuration();

    InitTick();//��ʱʱ��

    BKP_int();
    /* configure EVAL_COM0 */
    gd_eval_com_init(EVAL_COM0);//ͨ�Ŵ���

    bkp = bkp_read_data(BKP_DATA_0);

    //�û�����Ϊ��
    //if(((*(__IO uint32_t*)app_address) & 0x2FFE0000 ) == 0x20000000 && bkp == 0x1234)
    if(bkp == 0x1234)
    {
        while(1)
        {
            tmp=BootComRecv(g_Buffer,1,30000);//�ȴ�30S����
            if(tmp==0)
            {
                bkp_write_data(BKP_DATA_0,0);
                ExecApp();
            }
            if(g_Buffer[0]!=BOOT_CONNECT)//�յ�����ָ��
                continue;

            if(Connect(100))//����������Ϣ��У��
            {
                bkp_write_data(BKP_DATA_0,0);
                ExecApp();//У�鲻ͨ����תAPP
            }
            else
            {
                AC_LED2;//�����ϣ���AC��
                break;
            }
        }
    }
    else if(((*(__IO uint32_t*)app_address) & 0x2FFE0000 ) == 0x20000000)
    {
        ExecApp();
    }
    else
    {
    }

    tmp = 0;
    updateTimeOut = 1;
    while(1)//�����ѭ�����Ѿ�������
    {

        if(updateTimeOut >= 60000 || (tmp == BOOT_READ_PAGE && updateTimeOut >= 500))//���Ӻ�û���·������ļ�����ʱ��ת��APP
        {
            updateTimeOut = 0;
            bkp_write_data(BKP_DATA_0,0);
            ExecApp();
        }


        if(BootComRecv(&tmp,1,100)==0)
            continue;


        switch(tmp)
        {
        case BOOT_SET_ADDR://
            updateTimeOut = 0;
            TOGGLE_AC_LED2;//���õ�ַ��AC����˸
            SetPageAddr();
            break;

        case BOOT_WRITE_PAGE://
            updateTimeOut = 1;
            TOGGLE_AC_LED2;//д���ݣ�AC����˸
            WritePage();
            break;

        case BOOT_READ_PAGE://
            updateTimeOut = 1;
            AC_LED2_OFF;
            TOGGLE_PD_LED1;//����У�飬PD����˸
            ReadPage();
            break;

        case BOOT_CONNECT:
            updateTimeOut = 0;
            Connect(1000);
            break;

        case BOOT_EXEC_APP:

            updateTimeOut = 0;

            if(BootComRecv(g_Buffer,1,1000))
            {
                bkp_write_data(BKP_DATA_0,0);

                //if(!g_Buffer[0])
                ExecApp();
            }
            break;

        default:
            DelayMs(1000);
            break;
        }



    }
}



//int main(void)
//{
//    systick_config();

//    /* enable the LEDs clock */
//    rcu_periph_clock_enable(RCU_GPIOC);
//    rcu_periph_clock_enable(RCU_GPIOE);

//    /* configure LEDs GPIO port */
//    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_2);
//    gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1);

//    /* reset LEDs GPIO pin */
//    gpio_bit_reset(GPIOC, GPIO_PIN_0| GPIO_PIN_2);
//    gpio_bit_reset(GPIOE, GPIO_PIN_0| GPIO_PIN_1);

//    while(1){
//        /* turn on LED1 */
//        gpio_bit_set(GPIOC, GPIO_PIN_0);
//        /* insert 200 ms delay */
//        delay_1ms(200);

//        /* turn on LED2 */
//        gpio_bit_set(GPIOC, GPIO_PIN_2);
//        /* insert 200 ms delay */
//        delay_1ms(200);

//        /* turn on LED3 */
//        gpio_bit_set(GPIOE, GPIO_PIN_0);
//        /* insert 200 ms delay */
//        delay_1ms(200);

//        /* turn on LED4 */
//        gpio_bit_set(GPIOE, GPIO_PIN_1);
//        /* insert 200 ms delay */
//        delay_1ms(200);

//        /* turn off LEDs */
//        gpio_bit_reset(GPIOC, GPIO_PIN_0 | GPIO_PIN_2);
//        gpio_bit_reset(GPIOE, GPIO_PIN_0 | GPIO_PIN_1);

//        /* insert 200 ms delay */
//        delay_1ms(200);
//    }
//}