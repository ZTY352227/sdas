#include "key.h"
#include "systick.h"
#include "gpio.h"
#include "buzzer.h"
#include "ht1621b.h"

void key_init(void)
{
    //DC KEY ON
    /* enable the key   GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOD);
    /* configure button pin as input */
    gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

    //USB KEY ON
    /* enable the key   GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOD);
    /* configure button pin as input */
    gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3);

    //AC KEY ON
    /* enable the key   GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOD);
    /* configure button pin as input */
    gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
}

void KEY_Handel(void)
{
    static uint8_t dc_flag = 0;
    static uint8_t usb_flag = 0;
    static uint8_t ac_flag = 0;
    //DC���������¼�
    /* check whether the button is pressed */
    if(SET == DC_KEY)
    {
        DelayMS(20);

        /* check whether the button is pressed */
        if(SET == DC_KEY)
        {
            //���봦���¼�
            if(dc_flag == 0)
            {
				DC_12V_ON();// 12V �����
				BUZZER_ON();
				//todo ��ʾ
                dc_flag = 1;
            }
            else
            {
                DC_12V_OFF();// 12V �����
				BUZZER_OFF();
                dc_flag = 0;
            }
        }

        while(SET == DC_KEY);
    }

    //USB���������¼�
    /* check whether the button is pressed */
    if(SET == USB_KEY)
    {
        DelayMS(20);

        /* check whether the button is pressed */
        if(SET == USB_KEY)
        {
            //���봦���¼�
            if(usb_flag == 0)
            {
                USB1_ON();// USB1 �����
                USB2_ON();// USB2 �����
                PD1_ON();// PD1 �����
                PD2_ON();// PD2 �����
				BUZZER_ON();
				//todo ��ʾ
                usb_flag = 1;
            }
            else
            {
                USB1_OFF();// USB1 �����
                USB2_OFF();// USB2 �����
                PD1_OFF();// PD1 �����
                PD2_OFF();// PD2 �����
				BUZZER_OFF();
                usb_flag = 0;
            }
        }
        while(SET == USB_KEY);
    }

    //AC���������¼�
    /* check whether the button is pressed */
    if(SET == AC_KEY)
    {
        DelayMS(20);

        /* check whether the button is pressed */
        if(SET == AC_KEY)
        {
            //���봦���¼�
            if(ac_flag == 0)
            {
				AC_ON();
				BUZZER_ON();
                ac_flag = 1;
            }
            else
            {
				AC_OFF();
				BUZZER_OFF();
                ac_flag = 0;
            }
        }
        while(SET == AC_KEY);
    }
}