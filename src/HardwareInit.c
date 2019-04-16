#include"HardwareInit.h"

#include "lpc17xx_gpio.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_nvic.h"
#include <Delay_RIT.h>
#include <lpc17xx_uart.h>

//
//void ADC_IRQHandler(void);
//
//uint32_t voltage=0, totalTemp=0;
//float calVoltage=0;
//uint8_t counOfsum=0;
//uint16_t realTemp=0;
//volatile uint8_t  flagOfTemp=0;
//
//void ADC_IRQHandler()
//{
//	if(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_1,ADC_DATA_DONE)==0)
//		{
//			uint16_t temp=0;
//		  voltage = ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_1);
//		  /*
//		  	3.2volt    4096 digit
//		  	x          voltage 
//		  	x=(3.2*voltage)/4096
//        */			
//		  calVoltage = ( voltage * 3.2F ) / 4096;
//		  //temp = (calVoltage*1000 /10);
//		  temp = calVoltage*100;
//			totalTemp += temp;
//			counOfsum++;
//			if(counOfsum==20)
//			{
//				flagOfTemp=1;
//				realTemp=totalTemp/counOfsum;
//				totalTemp=0;
//				counOfsum=0;
//			}
//			else ADC_StartCmd(LPC_ADC,ADC_START_NOW);
//		}
//    
//}
//
/*uint16_t HW_ADC_Read()
{
	flagOfTemp=0;
	ADC_StartCmd(LPC_ADC,ADC_START_NOW);
	while(flagOfTemp==0);
	return realTemp;
}
*/
void HW_Init()
{
	// pinsel difenation for TXD2, port0.10
	PINSEL_CFG_Type usartPin;
	usartPin.Funcnum   = PINSEL_FUNC_1;
	usartPin.OpenDrain = PINSEL_PINMODE_NORMAL;
	usartPin.Pinmode   = PINSEL_PINMODE_TRISTATE;
	usartPin.Pinnum    = PINSEL_PIN_10;
	usartPin.Portnum   = PINSEL_PORT_0;
	PINSEL_ConfigPin(&usartPin);
	
	
	// pinsel difenation for RXD2, port0.11
	usartPin.Pinnum    = PINSEL_PIN_11;
	PINSEL_ConfigPin(&usartPin);	
	Delay_RIT_Init();
  
	// UART prepheral init
	UART_CFG_Type UartInint;
	UartInint.Baud_rate = 115200;
	UartInint.Databits  = UART_DATABIT_8;
	UartInint.Parity    = UART_PARITY_NONE;
	UartInint.Stopbits  = UART_STOPBIT_1;
	UART_Init(LPC_UART2,&UartInint);
	UART_TxCmd(LPC_UART2,ENABLE);
	
	/* NVIC_SetPriorityGrouping(0x07);

	// pinsel difenation for adc, port0.24
	PINSEL_CFG_Type adcpin;
	adcpin.Funcnum = PINSEL_FUNC_1;
	adcpin.OpenDrain = PINSEL_PINMODE_NORMAL;
	adcpin.Pinmode = PINSEL_PINMODE_TRISTATE;
	adcpin.Pinnum = PINSEL_PIN_24;
	adcpin.Portnum =PINSEL_PORT_0;
	PINSEL_ConfigPin(&adcpin);
	
	// adc prepheral init
	ADC_Init(LPC_ADC,200000);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN1,ENABLE);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_1, ENABLE);
  */
	
	/*
	NVIC_SetPriority(ADC_IRQn,NVIC_EncodePriority(0x07,0,1));
	NVIC_EnableIRQ(ADC_IRQn);
	*/
}

