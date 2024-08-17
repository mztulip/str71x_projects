//Author mztulip
//mateusz@tulip.lol
#include "71x_lib.h"
#include "71x_type.h"
#include <stdint.h>
#include <stdio.h>

#define UART0_Rx_Pin (0x0001 << 8)  /*TQFP 64: pin N� 63, TQFP 144 pin N� 143*/
#define UART0_Tx_Pin (0x0001 << 9)  /*TQFP 64: pin N� 64, TQFP 144 pin N� 144*/

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

void uart_clock_config(void)
{
	/* Enable UART0 clock on APB1 */
	  /* Enable UART0 clock on APB1 */
  	APB_ClockConfig (APB1, ENABLE, UART0_Periph | UART1_Periph
                    | UART2_Periph | UART3_Periph);
 

	/* Enable GPIO0 clock on APB2 */
	APB_ClockConfig (APB2, ENABLE, GPIO0_Periph );

	/* GPIO configuration ------------------------------------------------------*/

	/*  Configure the GPIO pins */
	GPIO_Config(GPIO0, UART0_Tx_Pin, GPIO_AF_PP);
	GPIO_Config(GPIO0, UART0_Rx_Pin, GPIO_IN_TRI_CMOS);
  
}

void led_config()
{
	// Configure P1.8 and P1.6, both pins are with LED
	GPIO_Config (GPIO1, 0x0100, GPIO_OUT_PP);
	GPIO_Config (GPIO1, 1<<(6), GPIO_OUT_PP);
	GPIO1->PD = 0;
}

void uart_config()
{
	UART_OnOffConfig(UART0, ENABLE); 

	/*  Disable FIFOs */      
	UART_FifoConfig (UART0, DISABLE); 

	/*  Reset the UART_RxFIFO */     
	UART_FifoReset  (UART0 , UART_RxFIFO); 

	/*  Reset the UART_TxFIFO */
	UART_FifoReset  (UART0 , UART_TxFIFO); 

	/*  Disable Loop Back */
	UART_LoopBackConfig(UART0 , DISABLE);                                           
	/* Configure the UARTX as following:
							- Baudrate = 9600 Bps
							- No parity
							- 8 data bits
							- 1 stop bit */
	UART_Config(UART0, 9600, UART_NO_PARITY, UART_1_StopBits, UARTM_8D);

	UART_RxConfig(UART0 ,ENABLE); 
}

void led1_toggle(void)
{
	GPIO1->PD ^= (1<<8);
}

void led2_toggle(void)
{
	GPIO1->PD ^= (1<<6);
}


void write_byte(uint8_t c)
{
	UART_ByteSend(UART0, &c);
      
	while(!((UART_FlagStatus(UART0)) & UART_TxEmpty))
	{
	/* wait until the data transmission is finished */
	} 
}

void write_serial(char *str)
{
  uint8_t i = 0;
  const uint8_t limit = 100;
  while(str[i]!=0 && i < limit)
  {
	write_byte(str[i]);
    i++;
  }
}

int main()
{      
	config_clock(); 
	uart_clock_config();
	led_config();
	uart_config();

	write_byte('a');
	write_serial("\n\rhello world\n\r");
	printf("\n\rhello printf");

	while(1)
	{
		led1_toggle();
		printf("\n\rLoop");
		delay();
	}

	return 0;
}