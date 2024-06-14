/******************************************************************************
 *
 * File Name: Control_MC.c
 *
 * Author: Mariz
 *
 *******************************************************************************/
#include "external_eeprom.h"
#include "buzzer.h"
#include "dc_motor.h"
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "timer1.h"
#include "i2c.h"

#define MC_ADDRESS         0x01
#define EEPROM_ADDRESS     0x0311
#define CHANGE_PASS        7
#define CHECK              8
#define UNMATCHED_ACTION   9
#define PASS_SENT          10
#define MATCHED            11
#define UNMATCHED          12
#define CREATE_PASS        13

/**********************************************************************************
 *                         Functions Prototype
 *********************************************************************************/
/* Description: compare passwords */
void compare_passwords();

/* Description: Storing the password in EEPROM if the 2 received passwords are matched */
void save_password();

/* Description: Check the password if it equals to the saved one or not */
uint8 check_password();

/* Description: Responsible for doing the matched actions,
 * Note: it doesn't work when it is called back by the timer, bec, the functions
 * contain timer0 initialization, which take more time than that taken by 1 tick (1 sec)
 * and once the timer starts rotation it continues with the same direction throughout the program
 */
void action_for_matched_pass_inControl(void);

/* Description:Responsible for doing the matched actions */
void action_for_unmatched_pass_inControl(void);

/**********************************************************************************
 *                          Global Variables
 *********************************************************************************/
uint8 received_pass[5], received_confirmed_pass[5], saved_pass[5];
uint8 row=0, col=0, received=0, save=0, read=0, result=0, count=0,hmi_command=0;


int main()
{
	/* Enable I_bit */
	SREG|=(1<<7);

	/* Initialization of the UART driver with Baud-rate = 9600 bits/sec */
	UART_ConfigType uart_config={EIGHT_BITS,ONE_BIT,DISABLED,9600};
	UART_init(&uart_config);

	/* Initialization of the I2C driver with F(SCL)= 400kb/s */
	TWI_ConfigType twi_config={MC_ADDRESS,FAST_MODE};
	TWI_init(&twi_config);

	Timer1_ConfigType timer1_config={0,7813,COMPARE_MODE,CPU_CLK_PRESCALER_1024};

	/* Initialization of DC Motor driver */
	DcMotor_Init();

	/* Initialization of Buzzer driver */
	Buzzer_init();

	UART_clearBuffer();

	while(1)
	{
		/* Store the reply of the control ECU */
		if(g_uart_buffer[0]!=CLEAR)
		{
			hmi_command=g_uart_buffer[0];
		}
		if((read==0) ||(hmi_command==CREATE_PASS))
		{
			compare_passwords();
		}

		else if(save==1)
		{
			save_password();
		}

		else if(hmi_command==CHECK)
		{
			result=check_password();
			UART_sendByte(result);

			if(result==MATCHED)
			{
				action_for_matched_pass_inControl();
				UART_clearBuffer();
			}
			else
			{
				UART_clearBuffer();
				if(count==2)
				{
					hmi_command=UNMATCHED_ACTION;
					count=0;
				}
				else
				{
					count++;
					hmi_command=CLEAR;
				}
			}
		}

		else if(hmi_command==CHANGE_PASS)
		{
			result=check_password();
			UART_sendByte(result);

			if(result==MATCHED)
			{
				UART_clearBuffer();
				received=0; save=0; read=0; result=0;
				hmi_command=CREATE_PASS;
			}
			else
			{
				UART_clearBuffer();
				if(count==2)
				{
					hmi_command=UNMATCHED_ACTION;
					count=0;
				}
				else
				{
					count++;
					hmi_command=CLEAR;
				}
			}
		}

		else if(hmi_command==UNMATCHED_ACTION)
		{
			action_for_unmatched_pass_inControl();
			count=0;
		}
	}
}

/**********************************************************************************
 *                         Functions Definitions
 *********************************************************************************/

/* iterators */
uint8 x=0, y=0;
void compare_passwords()
{
	while((x<5) && (g_uart_buffer[x]!=CLEAR))
	{
		received_pass[x]=g_uart_buffer[x];
		x++;
	}

	while((x>=5) && (x<10) && (g_uart_buffer[x]!=CLEAR))
	{
		received_confirmed_pass[y]=g_uart_buffer[x];
		if(x==9)
		{
			UART_clearBuffer();
			received=1;
		}
		x++; y++;
	}
	if((received==1) && (x!=0))
	{
		/* Comparing the 2 received passwords */
		for(x=0; x<5; x++)
		{
			if(received_confirmed_pass[x]==received_pass[x])
			{
				if((x==4) && (received_confirmed_pass[x]==received_pass[x]))
				{
					save=1;
					read=1;
					UART_sendByte(MATCHED);
					x=0; y=0;
					received=0;
					UART_clearBuffer();
					break;
				}
			}
			else
			{
				read=1;
				save=0;
				UART_sendByte(UNMATCHED);
				x=0; y=0;
				received=0;
				UART_clearBuffer();
				break;
			}
		}
	}
}

void save_password()
{
	uint8 b=0;
	for(b=0; b<5; b++)
	{
		EEPROM_writeByte(EEPROM_ADDRESS+b,received_confirmed_pass[b]);
		_delay_ms(10);
	}
	save=0;
}

uint8 check_password()
{
	uint8 b=0;
	for(b=0; b<5; b++)
	{
		EEPROM_readByte(EEPROM_ADDRESS+b,(saved_pass+b));
		_delay_ms(10);
	}

	for(b=1; b<=5; b++)
	{
		if((g_uart_buffer[b]!=saved_pass[b]))
		{
			return UNMATCHED;
		}
		if((b==5) && (g_uart_buffer[b]==saved_pass[b]))
		{
			return MATCHED;
		}
	}

	UART_clearBuffer();
	return 0;
}

/* The Motor doesn't work with the called back function as it continue to rotate in
 * the same direction once it has started
 */
void action_for_matched_pass_inControl(void)
{
	/* Real time isn't accurate on the simulation */
	/* Rotate For 15 sec */
	DcMotor_Rotate(CW,100);
	_delay_ms(30000);

	/* Hold For 3 sec */
	DcMotor_Rotate(STOP,0);
	_delay_ms(4000);

	/* Rotate For 15 sec */
	DcMotor_Rotate(A_CW,100);
	_delay_ms(30000);

	DcMotor_Rotate(STOP,0);

}

/* The passed function to call_back function of the timer (timer1)*/
void action_for_unmatched_pass_inControl(void)
{
	/* activate the motor and the buzzer for 1 min */
	DcMotor_Rotate(A_CW,100);
	Buzzer_on();
	_delay_ms(60000);
	DcMotor_Rotate(STOP,0);
	Buzzer_off();
}
