 /******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.c
 *
 * Author: Mariz
 *
 *******************************************************************************/
#include "keypad.h"
#include "gpio.h"
#include <util/delay.h>

/*******************************************************************************
 *                      Private Functions Prototypes                         *
 *******************************************************************************/
#ifndef STANDARD_KEYPAD
#if(KEYPAD_NUMBER_OF_COLS==3)
/*
 * Description :
 * Update the keypad pressed button value with the correct one in keypad 4x3 shape
 */
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number);

#elif(KEYPAD_NUMBER_OF_COLS==4)
/*
 * Description :
 * Update the keypad pressed button value with the correct one in keypad 4x4 shape
 */
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number);

#endif

/*For Standard Keypad*/
#endif

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

uint8 KEYPAD_getPressedKey(void)
{
	/* Algorithm (Scanning the matrix):
	 * At first, all the of the rows and columns are set to be inputs,
	 * Then looping over the rows and for each iteration one row is set to be an output
	 * and set to (0 or 1 depends on the button 's configuration (KEYPAD_BUTTON_PRESSED)),
	 * then looping over the columns to check if there is a pin reads (0 or 1 same signal
	 * of the row), this means that the button intersects this column and row is pressed
	 */
	/* For Rows */
	GPIO_setupPinDirection(KEYPAD_ROWS_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROWS_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID+1, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROWS_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID+2, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROWS_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID+3, PIN_INPUT);

	/* For Columns */
	GPIO_setupPinDirection(KEYPAD_COLS_PORT_ID, KEYPAD_FIRST_COL_PIN_ID, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COLS_PORT_ID, KEYPAD_FIRST_COL_PIN_ID+1, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COLS_PORT_ID, KEYPAD_FIRST_COL_PIN_ID+2, PIN_INPUT);
#if(KEYPAD_NUMBER_OF_COLS==4)
	GPIO_setupPinDirection(KEYPAD_COLS_PORT_ID, KEYPAD_FIRST_COL_PIN_ID+3, PIN_INPUT);
#endif

	while(1){
		uint8 row,col;
		for(row=0; row<KEYPAD_NUMBER_OF_ROWS; row++)
		{
			/* Each row is set to be output */
			GPIO_setupPinDirection(KEYPAD_ROWS_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID+row, PIN_OUTPUT);
			GPIO_writePin(KEYPAD_ROWS_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID+row, KEYPAD_BUTTON_PRESSED);
			/* iterating over the columns */
			for(col=0; col<KEYPAD_NUMBER_OF_COLS; col++)
			{
				if(GPIO_readPin(KEYPAD_COLS_PORT_ID,KEYPAD_FIRST_COL_PIN_ID+col)==KEYPAD_BUTTON_PRESSED)
				{
#ifndef STANDARD_KEYPAD
#if(KEYPAD_NUMBER_OF_COLS==3)
					return KEYPAD_4x3_adjustKeyNumber((row*KEYPAD_NUMBER_OF_COLS)+col+1);
#elif(KEYPAD_NUMBER_OF_COLS==4)
					return KEYPAD_4x4_adjustKeyNumber((row*KEYPAD_NUMBER_OF_COLS)+col+1);
#endif
#else
					return (row*KEYPAD_NUMBER_OF_COLS)+col+1;
#endif
				}
			}
			GPIO_setupPinDirection(KEYPAD_ROWS_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID+row, PIN_INPUT);
			_delay_ms(5); /*small delay to fix CPU load issue in proteus */
		}
	}
}

#ifndef STANDARD_KEYPAD

#if(KEYPAD_NUMBER_OF_COLS==3)

static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number)
{
	uint8 keypad_correct_button;
	switch(button_number)
	{
		case 10: keypad_correct_button = '*'; // ASCII Code of *
				 break;
		case 11: keypad_correct_button = 0;
				 break;
		case 12: keypad_correct_button = '#'; // ASCII Code of #
				 break;
		default: keypad_correct_button = button_number;
				break;
	}
	return keypad_correct_button;
}

#elif (KEYPAD_NUMBER_OF_COLS == 4)

static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number)
{
	uint8 keypad_correct_button;
	switch(button_number)
	{
		case 1: keypad_correct_button = 7;
				break;
		case 2: keypad_correct_button = 8;
				break;
		case 3: keypad_correct_button = 9;
				break;
		case 4: keypad_correct_button = '/'; // ASCII Code of /
				break;
		case 5: keypad_correct_button = 4;
				break;
		case 6: keypad_correct_button = 5;
				break;
		case 7: keypad_correct_button = 6;
				break;
		case 8: keypad_correct_button = '*'; /* ASCII Code of '*' */
				break;
		case 9: keypad_correct_button = 1;
				break;
		case 10: keypad_correct_button = 2;
				break;
		case 11: keypad_correct_button = 3;
				break;
		case 12: keypad_correct_button = '-'; /* ASCII Code of '-' */
				break;
		case 13: keypad_correct_button = 13;  /* ASCII of Enter */
				break;
		case 14: keypad_correct_button = 0;
				break;
		case 15: keypad_correct_button = '='; /* ASCII Code of '=' */
				break;
		case 16: keypad_correct_button = '+'; /* ASCII Code of '+' */
				break;
		default: keypad_correct_button = button_number;
				break;
	}
	return keypad_correct_button;
}

#endif

#endif /* STANDARD_KEYPAD */
