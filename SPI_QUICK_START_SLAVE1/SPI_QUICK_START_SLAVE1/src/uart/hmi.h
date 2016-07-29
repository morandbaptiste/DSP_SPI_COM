/*
 * uartHMI.h
 *
 * Created: 12/05/2016 22:12:20
 *  Author: MORAND Baptiste
 */ 


#ifndef UARTHMI_H_
#define UARTHMI_H_
/************************************************************************/
/* include directories                                                  */
/************************************************************************/

#include <board.h>
#include <stdio_serial.h>
#include <asf.h>
/************************************************************************/
/* MACRO                                                                */
/************************************************************************/
#define LENGTH_QUEUE 50
//We define the strandart port connected to the debugger
#define CONF_STDIO_USART_MODULE  EDBG_CDC_MODULE
#define CONF_STDIO_MUX_SETTING   EDBG_CDC_SERCOM_MUX_SETTING
#define CONF_STDIO_PINMUX_PAD0   EDBG_CDC_SERCOM_PINMUX_PAD0
#define CONF_STDIO_PINMUX_PAD1   EDBG_CDC_SERCOM_PINMUX_PAD1
#define CONF_STDIO_PINMUX_PAD2   EDBG_CDC_SERCOM_PINMUX_PAD2
#define CONF_STDIO_PINMUX_PAD3   EDBG_CDC_SERCOM_PINMUX_PAD3
#define CONF_STDIO_BAUDRATE      57600//57600
/************************************************************************/
/* GLOBAL VAR                                                           */
/************************************************************************/


/************************************************************************/
/* FUNCTION                                                             */
/************************************************************************/
/**
 @brief  configure peripheral UART from the debugger, debugger is connected to c stdio function					                                    
*/
void configureHMI(void);
/**
 @brief	Task of HMI, use to communicate via UART to EDBG					                                    
*/
void HMITask(void);
/** 
@brief add the char* to the queue of the uartQueue.
@param send : char* add in the uartQueue*
@return true if the char is add to the queue
*/
bool sendHMI(const char send[]);




#endif /* UARTHMI_H_ */