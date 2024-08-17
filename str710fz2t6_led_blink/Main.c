//Author mztulip
//mateusz@tulip.lol
#include "71x_lib.h"

void delay(void)
{
	volatile int i = 0;
	for(i = 0; i < 10000;i++)
	{
		
	}
}

int main()
{       
	// Configure P1.8 and P1.6, both pins are with LED
	GPIO_Config (GPIO1, 0x0100, GPIO_OUT_PP);
	GPIO_Config (GPIO1, 1<<(6), GPIO_OUT_PP);
	GPIO1->PD = 0xFFFF;
	while(1)
	{
		GPIO1->PD = (1<<6)|(1<<8);
		delay();
		GPIO1->PD = 0;
		delay();
	}

	return 0;
}