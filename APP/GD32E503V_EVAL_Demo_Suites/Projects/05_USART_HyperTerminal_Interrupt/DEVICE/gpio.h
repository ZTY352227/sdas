#ifndef __GPIO_H
#define __GPIO_H

#include "gd32e503v_eval.h"

#define	DC_12V_ON() 	gpio_bit_set(GPIOB, GPIO_PIN_6)
#define	DC_12V_OFF() 	gpio_bit_reset(GPIOB, GPIO_PIN_6)

#define	USB1_ON()		gpio_bit_set(GPIOB, GPIO_PIN_7)
#define	USB1_OFF()		gpio_bit_reset(GPIOB, GPIO_PIN_7)

#define	USB2_ON()		gpio_bit_set(GPIOD, GPIO_PIN_1)
#define	USB2_OFF()		gpio_bit_reset(GPIOD, GPIO_PIN_1)

#define	PD1_ON()		gpio_bit_set(GPIOB, GPIO_PIN_9)
#define	PD1_OFF()		gpio_bit_reset(GPIOB, GPIO_PIN_9)

#define	PD2_ON()		gpio_bit_set(GPIOB, GPIO_PIN_8)
#define	PD2_OFF()		gpio_bit_reset(GPIOB, GPIO_PIN_8)

#define	AC_ON()			gpio_bit_set(GPIOC, GPIO_PIN_7)
#define	AC_OFF()		gpio_bit_reset(GPIOC, GPIO_PIN_7)


void user_gpio_init(void);

#endif
