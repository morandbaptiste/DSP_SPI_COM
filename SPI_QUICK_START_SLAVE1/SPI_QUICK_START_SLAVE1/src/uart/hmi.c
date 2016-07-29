/*
 * uartHMI.c
 *
 * Created: 12/05/2016 22:11:48
 *  Author: MORAND Baptiste
 */ 
#include "hmi.h"
/************************************************************************/
/* GLOBAL                                                               */
/************************************************************************/

static struct usart_module hmiUart;
/************************************************************************/
/* FUNCTION                                                             */
/************************************************************************/
void configureHMI(void){
		struct usart_config usart_conf;
		usart_get_config_defaults(&usart_conf);
		// Define as STDIO Standart of C, STDIO use the EDBG serial for default. see file conf_uart_serial.h
		usart_conf.mux_setting = CONF_STDIO_MUX_SETTING;//USART_RX_1_TX_0_XCK_1
		usart_conf.pinmux_pad0 = CONF_STDIO_PINMUX_PAD0;//PINMUX_PA04D_SERCOM0_PAD0 multiplexing SERCOM0
		usart_conf.pinmux_pad1 = CONF_STDIO_PINMUX_PAD1;//PINMUX_PA05D_SERCOM0_PAD1 multilexing SERCOM0
		usart_conf.pinmux_pad2 = CONF_STDIO_PINMUX_PAD2;//PINMUX_UNUSED 
		usart_conf.pinmux_pad3 = CONF_STDIO_PINMUX_PAD3;//PINMUX_UNUSED
		usart_conf.baudrate    = CONF_STDIO_BAUDRATE;//9600
		stdio_serial_init(&hmiUart, CONF_STDIO_USART_MODULE, &usart_conf);
		usart_enable(&hmiUart);
}