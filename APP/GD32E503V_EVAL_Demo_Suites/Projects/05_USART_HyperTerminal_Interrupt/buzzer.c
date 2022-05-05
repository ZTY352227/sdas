#include "buzzer.h"


void buzzer_gpio_init(void)
{
	/* enable  clock */

	rcu_periph_clock_enable(RCU_GPIOC);

    /* configure GPIO port */ 
	
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
	BUZZER_OFF();
}