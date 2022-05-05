#include "pwm.h"


void pwm_init(void)
{
    pwm_gpio_config();
    MOS_Boost_PWMInit();//MOS升压PWM
    //fan_pwm_enable();//风扇PWM
    //fan_stop();
    //fan_set_duty(50);
}

void pwm_gpio_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);


    rcu_periph_clock_enable(RCU_AF);
    /*Configure PA0 PA1 PA2(TIMER1 CH0 CH1 CH2) as alternate function*/

    gpio_init(GPIOD,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_14);		//Remap: TIMER3_CH2风扇PWM
    gpio_pin_remap_config( GPIO_TIMER3_REMAP, ENABLE);

    gpio_init(GPIOC,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_9);		//Alternate2: TIMER7_CH3  MOS升压PWM

}


//MOS升压 timer7
void MOS_Boost_PWMInit(void)
{
    /* -----------------------------------------------------------------------
        TIMER1 configuration: generate 3 PWM signals with 3 different duty cycles:
        TIMER1CLK = SystemCoreClock / 180 = 1MHz
        TIMER1 channel0 duty cycle = (4000/ 16000)* 100  = 25%
        TIMER1 channel1 duty cycle = (8000/ 16000)* 100  = 50%
        TIMER1 channel2 duty cycle = (12000/ 16000)* 100 = 75%
        ----------------------------------------------------------------------- */
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER7);


    timer_deinit(TIMER7);


    /* 初始化MOS升压PWM 50kHz*/
    timer_initpara.prescaler         = 1;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 1799;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER7,&timer_initpara);

    /* CH0,CH1 and CH2 configuration in PWM mode */
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER7,TIMER_CH_3,&timer_ocintpara);

    /*  MOS升压PWM 50%*/
    timer_channel_output_pulse_value_config(TIMER7,TIMER_CH_3,900);
    timer_channel_output_mode_config(TIMER7,TIMER_CH_3,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER7,TIMER_CH_3,TIMER_OC_SHADOW_DISABLE);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER7);


    timer_primary_output_config(TIMER7,ENABLE);//timer0 和 Timer7
    /* auto-reload preload enable */
    timer_enable(TIMER7);
}

//风扇PWM timer3
void fan_pwm_enable(void)
{
    /* -----------------------------------------------------------------------
    TIMER1 configuration: generate 3 PWM signals with 3 different duty cycles:
    TIMER1CLK = SystemCoreClock / 180 = 1MHz
    TIMER1 channel0 duty cycle = (4000/ 16000)* 100  = 25%
    TIMER1 channel1 duty cycle = (8000/ 16000)* 100  = 50%
    TIMER1 channel2 duty cycle = (12000/ 16000)* 100 = 75%
    ----------------------------------------------------------------------- */
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER3);

    //timer_deinit(TIMER3);

    /* 初始化风扇PWM  50kHz*/
    timer_initpara.prescaler         = 1;//17
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 4499;//500
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER3,&timer_initpara);

    /* CH0,CH1 and CH2 configuration in PWM mode */
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER3,TIMER_CH_2,&timer_ocintpara);

    /* 风扇PWM  20%*/
//    timer_channel_output_pulse_value_config(TIMER3,TIMER_CH_2,720);
//    timer_channel_output_mode_config(TIMER3,TIMER_CH_2,TIMER_OC_MODE_PWM0);
//    timer_channel_output_shadow_config(TIMER3,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER3);

    /* auto-reload preload enable */
    timer_enable(TIMER3);
}

void FAN_Handel(void)
{
    fan_pwm_enable();
    fan_set_duty(50);
    fan_on();
}

//风扇停止
void fan_stop(void)
{
    rcu_periph_clock_disable(RCU_TIMER3);
    timer_deinit(TIMER3);
}

void fan_on(void)
{
    FAN_on;
    __NOP();
    __NOP();
}

//设置风扇占空比 duty-占空比 放大100倍
void fan_set_duty(uint8_t duty)
{
    uint16_t temp;

//	if(duty < 100)
//		duty = 100 - duty;

    temp = 45 * duty;

    timer_channel_output_pulse_value_config(TIMER3,TIMER_CH_2,temp);
    timer_channel_output_mode_config(TIMER3,TIMER_CH_2,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER3,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);
}