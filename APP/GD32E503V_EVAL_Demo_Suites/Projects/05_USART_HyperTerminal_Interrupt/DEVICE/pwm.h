#ifndef __PWM_H
#define __PWM_H
#include "gd32e50x.h"
#include "gd32e503v_eval.h"

#define  FAN_on  gpio_bit_set(GPIOD, GPIO_PIN_13)
#define  FAN_off  gpio_bit_reset(GPIOD, GPIO_PIN_13)

void pwm_init(void);
void pwm_gpio_config(void);
void MOS_Boost_PWMInit(void);
void fan_pwm_enable(void);
void fan_stop(void);
void fan_on(void);
void fan_set_duty(uint8_t duty);

#endif