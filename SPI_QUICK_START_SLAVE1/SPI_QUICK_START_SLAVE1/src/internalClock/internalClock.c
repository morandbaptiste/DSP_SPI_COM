/*
 * internalClock.c
 *
 * Created: 13/05/2016 13:59:02
 *  Author: MORAND Baptiste
 */ 


#include "internalClock.h"
//struct rtc_module rtc_instance;
volatile uint16_t ledCount=0;
volatile uint16_t timeOut;
volatile bool stateLed;
void isrInternalClok(void){
		timeOut++;
		if(ledCount>=500){
			port_pin_set_output_level(LED_0_PIN, stateLed);
			stateLed=!stateLed;
			ledCount=0;
		}
		else{
			ledCount++;
		}
}
void RTC_Handler( void ){
	//pc.printf("ISR");
	RTC->MODE0.INTFLAG.bit.CMP0=1;		//disable the flag COMPARE
	RTC->MODE0.INTFLAG.bit.OVF=1;		//disable the flag COMPARE
	
	isrInternalClok();
}

void configureInternalClock(void){
	GCLK->GENCTRL.reg=0b00000000000000010000010000000010;
	//Generic Clock Generator Division
	GCLK->GENDIV.reg= 0b00000000000000000000000100000010;
	GCLK->CLKCTRL.reg=0x4204;
	//peripheral configuration
	
	RTC->MODE0.CTRL.bit.PRESCALER=0x0;	//prescaller at 0
	RTC->MODE0.CTRL.bit.MODE=0x0;			//mode 0 : 32 bit counter
	RTC->MODE0.CTRL.bit.SWRST=0;			//no software reset
	RTC->MODE0.CTRL.bit.MATCHCLR=1;			//automatic reset counter
	
	//event configuration
	RTC->MODE0.INTENSET.bit.OVF=1;		//enable the event overflow
	RTC->MODE0.INTENSET.bit.CMP0=1;		//enable the event compare
	RTC->MODE0.INTFLAG.bit.CMP0=1;		//disable the flag COMPARE
	RTC->MODE0.INTFLAG.bit.OVF=1;		//disable the flag COMPARE
	RTC->MODE0.COUNT.bit.COUNT=0;			//set the RTC value
	RTC->MODE0.COMP[0].reg =32;

	RTC->MODE0.CTRL.reg |= RTC_MODE0_CTRL_ENABLE;

	NVIC_SetPriority (RTC_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	NVIC_EnableIRQ(RTC_IRQn);
	
}