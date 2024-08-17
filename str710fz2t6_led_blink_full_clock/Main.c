//Author mztulip
//mateusz@tulip.lol
#include "71x_lib.h"
#include "71x_type.h"

void delay(void)
{
	volatile int i = 0;
	for(i = 0; i < 1000000;i++)
	{
		
	}
}

void config_clock(void)
{
	//external clock generator 4MHz
	
	//Input to PLL is without division = 4Mhz
	RCCU_Div2Config(DISABLE);

	/* System clocks configuration ---------------------------------------------*/
  	/* MCLK = PCLK1 = PCLK2 = 24MHz*/ 
	// MCLK maximum allowed value when executing from flash 50MHz
	// PCLK maximum allowed value = 33MHz
 	/* Configure FCLK = RCLK / 1 */
	RCCU_PCLK1Config (RCCU_DEFAULT);

	/* Configure PCLK = RCLK / 1 */
	RCCU_PCLK2Config (RCCU_DEFAULT);

	/* Configure MCLK clock for the CPU, RCCU_DEFAULT = RCLK /1 */
	RCCU_MCLKConfig (RCCU_DEFAULT);

	/* Configure the PLL1 ( * 12 , / 4 ) */
	//4*24/4=24Mhz
	RCCU_PLL1Config (RCCU_PLL1_Mul_24, RCCU_Div_4) ;

	while(RCCU_FlagStatus(RCCU_PLL1_LOCK) == RESET)
	{
	/* Wait PLL to lock */
	}
	/* Select PLL1_Output as RCLK clock */
	RCCU_RCLKSourceConfig (RCCU_PLL1_Output) ;  

}

int main()
{      
	config_clock(); 
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