/******************************************************************************
 *
 * File Name: HMI_MC.c
 *
 * Author: Mariz
 *
 *******************************************************************************/
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer1.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include<util/delay.h>

#define ENTER                61 /* ASCII of '=' */
#define STEP_1               1
#define STEP_2               2
#define STEP_3               3
#define STEP_4               4
#define STEP_5               5
#define CHANGE_PASS          7
#define CHECK                8
#define UNMATCHED_ACTION     9
#define PASS_SENT            10
#define MATCHED              11
#define UNMATCHED            12
#define CREATE_PASS          13


/**********************************************************************************
 *                         Called back Functions
 *********************************************************************************/

/* number of ticks isn't accurate i think bec, the function takes more than
 * one interrupt count so some ticks are missed so, the time isn't accurate
 */

uint8 interrupt_count=0;

/* the passed function to timer.1 call back function */
void action_for_matched_pass_inHMI(void)
{
	/* 15 sec*/
	if(interrupt_count<=30)
	{
		LCD_displayStringRowColumn(0,0,"    Door is      ");
		LCD_displayStringRowColumn(1,0,"   unlocking     ");
		interrupt_count++;
	}
	/* hold for 3 sec */
	else if((interrupt_count>30) && (interrupt_count<=36))
	{
		LCD_clearScreen();
		interrupt_count++;
	}

	else if((interrupt_count>36) && (interrupt_count<=66))
	{
		LCD_displayStringRowColumn(0,0,"    Door is       ");
		LCD_displayStringRowColumn(1,0,"    locking       ");
		interrupt_count++;
		if(g_Timer1_count>=66)
		{
			LCD_clearScreen();
			interrupt_count=0;
			Timer1_deInit();
		}
	}
}

void action_for_unmatched_pass_inHMI(void)
{
	/* activate the buzzer for 1 min */
	if(g_Timer1_count<=120)
	{
		LCD_displayStringRowColumn(0,0,"    ERROR        ");
		LCD_displayStringRowColumn(1,0,"   locking       ");
		if(g_Timer1_count>=120)
		{
			LCD_clearScreen();
			g_Timer1_count=0;
			Timer1_deInit();
		}
	}
}

/**********************************************************************************
 *                             Global Variables
 **********************************************************************************/

/* To store the password taken from user */
uint8 pass[5], confirmed_pass[5], next_step = STEP_1;
/* key: to receive the pressed key, final_key: to receive the "Enter"*/
uint8 key=0,final_key=0, entered=0;
uint8 row=0,col=0, enter_count=0,send_flag=0,count=0;

/* To receive the reply of the Control ECU */
char control_reply=0;

/**********************************************************************************
 *                         Functions Prototype
 *********************************************************************************/

/* Description: Take the password from the user to be saved */
void create_password();

/* Description: ask the user to enter the saved password */
void enter_saved_password();

/* Description: send the password to control to check it*/
void send_pass_to_control(uint8*pass);


int main()
{
	/* Enable I_bit */
	SREG|=(1<<7);

	/* Initialize the UART driver with Baud-rate = 9600 bits/sec */
	UART_ConfigType config={EIGHT_BITS,ONE_BIT,DISABLED,9600};
	UART_init(&config);

	/* Initialization of LCD */
	LCD_init();

	Timer1_ConfigType timer1_config={0,7813,COMPARE_MODE,CPU_CLK_PRESCALER_1024};

	UART_clearBuffer();

	while(1)
	{
		/* Store the reply of the control ECU */
		control_reply=g_uart_buffer[0];

		/* Step_1 */
		if(next_step==STEP_1)
		{
			create_password();
			if(control_reply==MATCHED)
			{
				next_step = STEP_2;
				control_reply=CLEAR;
				g_uart_buffer[0]=CLEAR;
			}
			else if(control_reply==UNMATCHED)
			{
				UART_sendByte(CREATE_PASS);
				key=0; final_key=0;
				row=0; col=0; enter_count=0;send_flag=0;count=0;
				control_reply=CLEAR;
				g_uart_buffer[0]=CLEAR;
				next_step=STEP_1;
			}
		}

		/* Step_2 */
		else if(next_step==STEP_2)
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0," + : Open Door");
			LCD_displayStringRowColumn(1,0," - : Change Pass");

			key=KEYPAD_getPressedKey();

			if(key=='+')
			{
				/* to prevent printing '*' in next steps depending on the last value of key*/
				key=ENTER;
				next_step = STEP_3;
			}
			else if(key=='-')
			{
				/* to prevent printing '*' in next steps depending on the last value of key*/
				key=ENTER;
				next_step = STEP_4;
			}
			else
			{
				key=KEYPAD_getPressedKey();
			}
			_delay_ms(300);
		}

		/* Step_3 */
		else if(next_step==STEP_3)
		{
			UART_clearBuffer();
			if(entered==0)
			{
				enter_saved_password();
			}
			else if(entered==1)
			{
				UART_sendByte(CHECK);
				send_pass_to_control(pass);
				UART_sendByte(PASS_SENT);

				if(control_reply==MATCHED)
				{
					Timer1_setCallBack(action_for_matched_pass_inHMI);
					Timer1_init(&timer1_config);
					next_step=STEP_2;
					control_reply=CLEAR;
					UART_clearBuffer();
					entered=0;
				}
				else if(control_reply==UNMATCHED)
				{
					control_reply=CLEAR;
					UART_clearBuffer();
					if(count==2)
					{
						next_step=STEP_5;
						count=0;
					}
					else
					{
						entered=0;
						next_step=STEP_3;
					}
					count++;
				}
			}
		}

		/* Step_4 */
		else if(next_step==STEP_4)
		{
			UART_clearBuffer();
			if(entered==0)
			{
				enter_saved_password();
			}
			else if(entered==1)
			{
				UART_sendByte(CHANGE_PASS);
				send_pass_to_control(pass);
				UART_sendByte(PASS_SENT);

				if(control_reply==MATCHED)
				{
					key=0; final_key=0; entered=0;
					row=0; col=0; enter_count=0;send_flag=0;
					next_step=STEP_1;
					control_reply=CLEAR;
					UART_clearBuffer();
				}
				else if(control_reply==UNMATCHED)
				{
					UART_clearBuffer();
					if(count==2)
					{
						next_step=STEP_5;
						count=0;
					}
					else
					{
						entered=0;
						next_step=STEP_4;
					}
					count++;

				}
			}
		}

		/* Step_5 */
		else if(next_step==STEP_5)
		{
			Timer1_setCallBack(action_for_unmatched_pass_inHMI);
			Timer1_init(&timer1_config);
			next_step=STEP_2;
			LCD_clearScreen();
		}
	}
}


/**********************************************************************************
 *                         Functions Definitions
 *********************************************************************************/
void create_password()
{
	/* Receive the reply of the Control ECU */
	if((final_key==ENTER) && (enter_count==0))
	{
		if(send_flag==0)
		{
			/* Send the password to the control ECU */
			uint8 b=0;
			for(b=0; b<5; b++)
			{
				UART_sendByte(pass[b]);
			}
			send_flag++;
		}
		/* Move the cursor to the next line */
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"Re-enter same ");
		LCD_displayStringRowColumn(1,0,"pass:");
		row=0;col=0;enter_count++;
		final_key=0;
	}

	/* To skip the loop of taking the password and go to the next step
	 * and To receive the correct "Enter" even if it is not pressed correctly
	 * from the first time
	 */
	else if((enter_count==1) && (final_key==ENTER))
	{
		final_key=0;
		if(send_flag==1)
		{
			/* Send the password to the control ECU */
			uint8 b=0;
			for(b=0; b<5; b++)
			{
				UART_sendByte(confirmed_pass[b]);
			}
		}
		send_flag=0;
	}

	/* To receive the correct "Enter" key from the user after entering the password */
	else if(row==5)
	{
		if(final_key!=ENTER)
		{
			final_key=KEYPAD_getPressedKey();
		}
	}

	/* To take the password for the first time */
	else
	{
		LCD_clearScreen();
		/* Move the cursor to the next line */
		LCD_displayStringRowColumn(0,0,"Please enter new");
		LCD_displayStringRowColumn(1,0,"pass:");
	}

	/* Taking the password from the user */
	while(row<5){

		key=KEYPAD_getPressedKey();

		if(key>=0 && (key!=ENTER))
		{
			/* Display after the end of the last word */
			LCD_moveCursor(1,col+5);
			LCD_displayCharacter('*');
			if(send_flag==0)
			{
				pass[row]=key;
			}
			else if(send_flag==1)
			{
				confirmed_pass[row]=key;
			}
			row++; col++;
		}
		/* Press Time */
		_delay_ms(300);
	}
}

void enter_saved_password()
{
	LCD_clearScreen();
	/* Move the cursor to the next line */
	LCD_moveCursor(0,0);
	LCD_displayString("Plz enter Pass:");
	row=0;col=0;
	/* Taking the password from the user */
	while(row<5){
		key=KEYPAD_getPressedKey();

		if((key>=0) && (key!=ENTER))
		{
			LCD_moveCursor(1,col);
			LCD_displayCharacter('*');
			pass[row]=key;
			row++; col++;
		}
		/* press time */
		_delay_ms(300);
	}
	final_key=KEYPAD_getPressedKey();
	_delay_ms(300);

	while(final_key!=ENTER)
	{
		final_key=KEYPAD_getPressedKey();
		_delay_ms(300);
	}
	if(final_key==ENTER)
	{
		entered=1;
	}
}

void send_pass_to_control(uint8*pass)
{
	uint8 x=0;
	for(x=0; x<5; x++)
	{
		UART_sendByte(pass[x]);
	}
}

