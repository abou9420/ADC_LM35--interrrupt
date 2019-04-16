#include "Delay_RIT.h"
#include "lpc17xx_rit.h"
#include "lpc17xx_libcfg_default.h"


volatile uint8_t ritFlag=0;

void RIT_IRQHandler(void);

void RIT_IRQHandler()
{
	RIT_GetIntStatus(LPC_RIT);
	ritFlag=1;
}
void Delay_RIT_Init()
{
	RIT_Init(LPC_RIT);
	NVIC_EnableIRQ(RIT_IRQn);
}
void Delay_RIT_ms(uint32_t TimeMs)
{
	ritFlag=0;
	RIT_TimerConfig(LPC_RIT,TimeMs);
	RIT_Cmd(LPC_RIT,ENABLE);
	while(ritFlag==0);
	RIT_Cmd(LPC_RIT,DISABLE);
}
