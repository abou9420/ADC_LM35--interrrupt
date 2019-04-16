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

void ADC_IRQHandler(void);

uint32_t voltage=0, totalTemp=0;
float calVoltage=0;
uint8_t counOfsum=0, flagOfTemp=0;
uint16_t realTemp=0, temp=0;

void ADC_IRQHandler()
{
	if(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_1,ADC_DATA_DONE)==0)
		{
		  voltage = ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_1);
		  /*
		  	3.2volt    4096 digit
		  	x          voltage 
		  	x=(3.2*voltage)/4096
      */			
		  calVoltage = ( voltage * 3.2F ) / 4096;
		  //temp = (calVoltage*1000 /10);
		  temp = calVoltage*100;
			totalTemp += temp;
			counOfsum++;
			if(counOfsum==20)
			{
				flagOfTemp=1;
				realTemp=totalTemp/counOfsum;
				totalTemp=0;
				counOfsum=0;
			}
			ADC_StartCmd(LPC_ADC,ADC_START_NOW);
		}
    
}

int main()
{
	// variable 
	char	temper[5];
	uint8_t localTemp=0;
	uint8_t temper1[20];	
	lcd_init();
	lcd_clear();
	
	// pinsel difenation
	PINSEL_CFG_Type adcpin;
	adcpin.Funcnum = PINSEL_FUNC_1;
	adcpin.OpenDrain = PINSEL_PINMODE_NORMAL;
	adcpin.Pinmode = PINSEL_PINMODE_TRISTATE;
	adcpin.Pinnum = PINSEL_PIN_24;
	adcpin.Portnum =PINSEL_PORT_0;
	
	PINSEL_ConfigPin(&adcpin);
	
	NVIC_SetPriorityGrouping(0x07);
	
	// adc prepheral init
	ADC_Init(LPC_ADC,200000);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN1,ENABLE);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_1, ENABLE);
  
	NVIC_SetPriority(ADC_IRQn,NVIC_EncodePriority(0x07,0,1));
	NVIC_EnableIRQ(ADC_IRQn);
  HW_Init();
	//
	lcd_gotoxy(1,4);
	lcd_putsf("Temperatuer");
	ADC_StartCmd(LPC_ADC,ADC_START_NOW);
	
  Delay_RIT_Init();	
	while(1)
	{
		if(flagOfTemp==1)
		{
			flagOfTemp=0;
			localTemp=realTemp;
		}
			
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
