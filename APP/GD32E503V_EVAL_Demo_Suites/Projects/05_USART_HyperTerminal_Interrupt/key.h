#ifndef __KEY_H
#define __KEY_H
#include "gd32e50x.h"

#define  DC_KEY   	 gpio_input_bit_get(GPIOD, GPIO_PIN_2)
#define  USB_KEY   	 gpio_input_bit_get(GPIOD, GPIO_PIN_3)
#define  AC_KEY   	 gpio_input_bit_get(GPIOD, GPIO_PIN_4)



void key_init(void);
void KEY_Handel(void);
#endif
