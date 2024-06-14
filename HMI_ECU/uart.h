/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Author: Mariz
 *
 *******************************************************************************/
#include "std_types.h"

/*******************************************************************************
 *                               Definitions                                   *
 *******************************************************************************/

/* Outside the range of needed ascii */
#define CLEAR       100

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum
{
	FIVE_BITS, SIX_BITS, SEVEN_BITS, EIGHT_BITS, RESERVED1, RESERVED2, RESERVED3, NINE_BITS
}UART_BitData;

typedef enum
{
	ONE_BIT, TWO_BITS
}UART_StopBit;

typedef enum
{
	DISABLED, RESERVED, ENABLE_EVEN_PARITY, DISABLE_EVEN_PARITY
}UART_Parity;

typedef struct
{
	UART_BitData bit_data;
	UART_StopBit stop_bit;
	UART_Parity parity;
	uint32 baud_rate;
}UART_ConfigType;


/*******************************************************************************
 *                              Global variables                               *
 *******************************************************************************/
extern uint8 g_uart_buffer[20];

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initializing the UART device by:
 * 1. Setup the required Frame format and the UART baud rate
 * 2. Enable the UART.
 */
void UART_init(const UART_ConfigType*Config_Ptr);

/*
 * Description :
 * Responsible for sending a byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Responsible for receiving a byte from another UART device.
 */
uint8 UART_receiveByte(void);

/*
 * Description :
 * Responsible for Sending the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *str);

/*
 * Description :
 * Responsible for Receiving the required string until the '#' symbol through UART from another UART device.
 */
void UART_receiveString(uint8 *str);

/*
 * Description:
 * Clear the global buffer
 */
void UART_clearBuffer(void);


#ifndef UART_H_
#define UART_H_



#endif /* UART_H_ */
