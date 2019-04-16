#include "lpc17xx_gpio.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_nvic.h"
#include "lpc17xx_uart.h"

/* Char LCD definition */
#define LCD_LPC1768       
#define LCD_PORT_2
#define LCD_RS     0
#define LCD_RW     1
#define LCD_E      2
#define LCD_DB4    4
#define LCD_DB5    5
#define LCD_DB6    6
#define LCD_DB7    7

#include "lcd.h"
#include <stdio.h>
#include "Delay_RIT.h"
#include "HardwareInit.h"


int main()
{
	// variable 
	char	temper[5];
	uint8_t localTemp=0;
	uint8_t temper1[20];	
	
	HW_Init();
	
	lcd_init();
	lcd_clear();
	lcd_gotoxy(1,4);
	lcd_putsf("Temperatuer");
	
	while(1)
	{
		localTemp= HW_ADC_Read();
		
		lcd_gotoxy(2,7);
		sprintf(temper ,"T=%d",localTemp);
		lcd_putsf(temper);
		
		uint8_t len = sprintf(temper1 ,"T=%u\n",localTemp);
		// send temp
		UART_Send(LPC_UART2,temper1,len,BLOCKING);
		Delay_RIT_ms(1000);
		
		/*uint16_t local=localTemp;
		UART_SendByte(LPC_UART2,(local>>8));
		Delay_RIT_ms(10);
		while(UART_CheckBusy(LPC_UART2));
		UART_SendByte(LPC_UART2,localTemp);
	  */
	}
	
}
