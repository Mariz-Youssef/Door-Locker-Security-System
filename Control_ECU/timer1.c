/******************************************************************************
 *
 * Module: Timer1
 *
 * File Name: timer1.c
 *
 * Author: Mariz
 *
 *******************************************************************************/
#include "timer1.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "common_macros.h"


/******************************************************************************
 *                        Global Variables
 *****************************************************************************/

/* To store the address of the call back function */
static volatile void (*g_Timer1_Call_Back_Ptr)(void);

/* To know the interrupt number*/
uint8 volatile g_Timer1_count=0;

/******************************************************************************
 *                        Interrupt Service Routine
 *****************************************************************************/

/* For Normal Mode */
ISR(TIMER1_OVF_vect)
{
	g_Timer1_count++;

	if(g_Timer1_Call_Back_Ptr!=NULL_PTR)
	{
		/* Calling back the passed function */
		(*g_Timer1_Call_Back_Ptr)();
	}
}

/* For compare mode */
ISR (TIMER1_COMPA_vect)
{
	g_Timer1_count++;

	if(g_Timer1_Call_Back_Ptr!=NULL_PTR)
	{
		/* Calling back the passed function */
		(*g_Timer1_Call_Back_Ptr)();
	}
}

/******************************************************************************
 *                       Functions Definitions
 *****************************************************************************/

void Timer1_init(const Timer1_ConfigType * Config_Ptr){
	/* Setting the initial count */
	TCNT1 = Config_Ptr->initial_value;

	/* Setting the configured compare value */
	OCR1A = Config_Ptr->compare_value;

	TCCR1A = (1<<FOC1A) | (1<<FOC1B);

	/*Setting overflow or CTC Mode*/
	TCCR1B = (TCCR1B & 0xF7) | (Config_Ptr->mode << 3);


	if(Config_Ptr->mode == NORMAL_MODE)
	{
		/* Enable Overflow Interrupt */
		TIMSK |= (1<<TOIE1);
	}
	else
	{
		/* Enable Compare A Interrupt and setting the channels */
		TIMSK |= (1<<OCIE1A);
		TCCR1A= (TCCR1A & 0x0F)|(CHANNELS_DISCONNECTED<<4);
	}

	/*Setting the Prescaler*/
	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler & 0x07);
}

void Timer1_deInit(void)
{
	/* Disable Timer1, disconnect the clk */
	TCCR1B=0;
}

void Timer1_setCallBack(void(*a_ptr)(void))
{
	g_Timer1_Call_Back_Ptr = a_ptr;
}
