/**
 * ===============================================================================
 * Name        	:	e9.1.c
 * Author      	:	Bonino, Francisco Ignacio
 * Version     	: 	1.0
 * Copyright   	: 	None
 * Description	: 	Second ADC excercise - Digital Electronics III
 * Activity	:	Write a program that uses four ADC conversion channels in burst
			mode, considering CCLK = 100MHz and 50KHz on each channel.
 * Date		:	22/11/2021 (dd/mm/yyyy)
 * ===============================================================================
 */

#include "lpc17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_adc.h"

#define PORT(x) x

void cfg_adc_pin(uint8_t, uint8_t);
void cfg_adc(void);

int main(void)
{
	for (uint8_t i = 23; i < 25; i++) // ADC0.0-3
		cfg_adc_pin(PORT(0), i);
	
	cfg_adc();
	
	while (1);
	
	return 0;
}

void cfg_adc_pin(uint8_t port, uint8_t pin)
{
	PINSEL_CFG_Type cfg;
	
	cfg.Portnum = port;
	cfg.Pinnum = pin;
	cfg.Funcnum = 1; // ADC
	cfg.Pinmode = PINSEL_PINMODE_TRISTATE;
	
	PINSEL_ConfigPin(&cfg);
}

void cfg_adc(void)
{
	ADC_Init(LPC_ADC, 200000); // 200KHz rate >> 50KHz on each channel
	ADC_BurstCmd(LPC_ADC, 1);
	ADC_StartCmd(LPC_ADC, ADC_START_CONTINUOUS);
	
	for (uint8_t i = 0; i < 2; i++)
	{
		ADC_ChannelCmd(LPC_ADC, i, ENABLE);
		ADC_IntConfig(LPC_ADC, i, SET);
	}
	
	ADC_IntConfig(LPC_ADC, ADC_ADINTENG, DISABLE);
	
	NVIC_EnableIRQ(ADC_IRQn);
}