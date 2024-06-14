/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Author: Mariz
 *
 *******************************************************************************/
#include "uart.h"
#include <avr/io.h>
#include "common_macros.h"
#include<avr/interrupt.h>

/******************************************************************************
 *                        Global Variables
 *****************************************************************************/

uint8 g_uart_buffer[20];
uint8 g_uart_counter=0;

/******************************************************************************
 *                        Interrupt Service Routine
 *****************************************************************************/

/* in order to make the receiveByte function by interrupt:
 * the value of the UDR is put in a global value in ISR
 * and to receive a string correctly, g_uart_buffer[] is used as a buffer
 * to store the UDR value at interrupt in order not to be missed
 * so, i've implemented the UART_receiveString in ISR
 * and we needen't to call it in the application code
 */

ISR(USART_RXC_vect)
{
	g_uart_buffer[g_uart_counter] = UDR;
	if(g_uart_buffer[g_uart_counter]=='#')
	{
		g_uart_buffer[g_uart_counter]='\0';
	}
	g_uart_counter++;
}

/******************************************************************************
 *                       Functions Definitions
 *****************************************************************************/

void UART_init(const UART_ConfigType*Config_Ptr)
{
	uint16 ubrr_value=0;
	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);

	/* RX Complete Interrupt Enable
	 * Receiver Enable
	 * Transmitter Enable
	 * */
	UCSRB|=(1<<RXCIE)|(1<<RXEN)|(1<<TXEN);

	/*
	 * Configuring the required:
	 * parity type,stop bit, character size,
	 */
	UCSRC|=(1<<URSEL)|(1<<(Config_Ptr->parity))|(1<<(Config_Ptr->stop_bit))|(1<<(Config_Ptr->bit_data));

	/*Calculating the UBRR value according to the required baud rate */
	ubrr_value = (uint16)(((F_CPU / ((Config_Ptr->baud_rate) * 8UL))) - 1);

	UBRRL = ubrr_value;
	UBRRH = ubrr_value>>8;;
}

void UART_sendByte(const uint8 data)
{
	/* Waiting until the transmit buffer becomes empty (ready for transmission a new byte)*/
	while(BIT_IS_CLEAR(UCSRA,UDRE));
	UDR = data;
}

/* another receive function by polling to be used by the other device*/

uint8 UART_receiveByte(void)
{
	/* Wait until the receive buffer get the data */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}
	return UDR;
}

void UART_sendString(const uint8 *str)
{
	while(*str != '\0')
	{
		UART_sendByte(*str);
		str++;
	}
}

/* another receive string function by polling
 * (i don't use it in this project)
 */
void UART_receiveString(uint8 *str)
{
	uint8 i=0;
	/*Receive the first byte*/
	str[i]=UART_receiveByte();

	/* then check for the '#'*/
	while(str[i]!='#')
	{
		i++;
		str[i]=UART_receiveByte();
	}
	str[i]='\0';
}

void UART_clearBuffer(void)
{
	g_uart_counter=0;
	while(g_uart_counter<20)
	{
		g_uart_buffer[g_uart_counter]=CLEAR;
		g_uart_counter++;
	}
	g_uart_counter=0;
}
