/*
 * internalClock.h
 *
 * Created: 13/05/2016 13:59:35
 *  Author: MORAND Baptiste
 */ 


#ifndef INTERNALCLOCK_H_
#define INTERNALCLOCK_H_
/************************************************************************/
/*INCLUDE                                                               */
/************************************************************************/
#include <asf.h>



/************************************************************************/
/* MACRO                                                                */
/************************************************************************/
#define FEATURE_RTC_CONTINUOUSLY_UPDATED
#define OSC32K	0x1
#define GCLKGEN2 0x02
#define GCLK_RTC 0x04

/************************************************************************/
/* GLOBAL                                                               */
/************************************************************************/

extern volatile uint16_t timeOut;
/************************************************************************/
/* function                                                             */
/************************************************************************/

/** 
@brief configure rtc for making an internal clock
*/
void configureInternalClock(void);
/** 
@brief call when the ISR is comming
*/
void isrInternalClok(void );
#endif /* INTERNALCLOCK_H_ */