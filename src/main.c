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
	float localTemp=0;
	uint8_t temper1[20];	
	uint8_t dataPrt[3];
	
	// Hardware Init
	HW_Init();
	
	// Lcd Init
	lcd_init();
	
	lcd_clear();
	// lcd_gotoxy(1,4);
	// lcd_putsf("Temperatuer");
	
	while(1)
	{
		localTemp= HW_ADC_Read();
		
		// lcd_gotoxy(2,7);
		// sprintf(temper ,"T=%0.1f",localTemp);
		// lcd_putsf(temper);
		
		// send temp
		uint8_t len = sprintf(temper1 ,"T=%0.1f\n",localTemp);
		UART_Send(LPC_UART2,temper1,len,BLOCKING);
		Delay_RIT_ms(1000);
		
		/*uint16_t local=localTemp;
		UART_SendByte(LPC_UART2,(local>>8));
		Delay_RIT_ms(10);
		while(UART_CheckBusy(LPC_UART2));
		UART_SendByte(LPC_UART2,localTemp);
	  */
		
		// Reseive Data Byte mode
		// if(((LPC_UART2->LSR)>>0)&0x01)
		// {
		// 	// Data is ready
		// 	uint8_t data= UART_ReceiveByte(LPC_UART2);
		// 	if(data=='a' || data== 'A' )
		// 	{
		// 		UART_Send(LPC_UART2,temper1,len,BLOCKING);
		// 	}
		// }
		/*-----------------------*/
			/* |Header|command|END|  */
			/* | 0xAA |  1    |0xbb| show tempreture on LCD */
      /* | 0xAA |  1    |0xbb| clear tempreture on LCD */			
			/*-----------------------*/
		// Reseive Data Array mode
		
    UART_Receive(LPC_UART2,dataPrt,3,BLOCKING);
    
		if( dataPrt[0]==0xAA && dataPrt[1]==1 && dataPrt[2]==0xbb)
		{
	    lcd_clear();
			lcd_gotoxy(1,4);
	    lcd_putsf("Temperatuer");
			lcd_gotoxy(2,7);
		  sprintf(temper ,"T=%0.1f",localTemp);
		  lcd_putsf(temper);
		}
		else if(dataPrt[0]==0xAA && dataPrt[1]==2 && dataPrt[2]==0xbb)
		{ 
			lcd_clear();
			lcd_gotoxy(1,3);
      lcd_putsf("Project By:");
			lcd_gotoxy(2,1);
		  lcd_putsf("Aboubakr Samadi");
			
		}
		
	}
	
}
