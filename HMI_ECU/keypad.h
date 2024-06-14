 /******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.h
 *
 *
 * Author: Mariz
 *
 *******************************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* For Standard KeyPad*/
 //#define STANDARD_KEYPAD

/* Keypad Configurations */

/* Number of Rows and Columns */
#define KEYPAD_NUMBER_OF_ROWS         4
#define KEYPAD_NUMBER_OF_COLS         4

/* Port and pins numbers */
#define KEYPAD_ROWS_PORT_ID           PORTA_ID
#define KEYPAD_COLS_PORT_ID           PORTD_ID

#define KEYPAD_FIRST_ROW_PIN_ID       PIN1_ID
#define KEYPAD_FIRST_COL_PIN_ID       PIN4_ID

/* Buttons logic of configuration*/
#define KEYPAD_BUTTON_PRESSED         LOGIC_LOW
#define KEYPAD_BUTTON_RELEASED        LOGIC_HIGH


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Get the Keypad pressed button
 */
uint8 KEYPAD_getPressedKey(void);

#endif /* KEYPAD_H_ */
