#ifndef __BUZZER_H
#define __BUZZER_H

#include "gd32e50x.h"
#include "gd32e503v_eval.h"

#define BUZZER_OFF()   	 gpio_bit_set(GPIOC, GPIO_PIN_13)   	//����
#define BUZZER_ON()   	 gpio_bit_reset(GPIOC, GPIO_PIN_13)   	//����

void buzzer_gpio_init(void);

#endif