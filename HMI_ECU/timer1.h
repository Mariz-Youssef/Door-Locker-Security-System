/******************************************************************************
 *
 * Module: Timer1
 *
 * File Name: timer1.h
 *
 * Author: Mariz
 *
 *******************************************************************************/

#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"

/******************************************************************************
 *                        Types Declaration
 *****************************************************************************/

/* Frequency Prescaler */
typedef enum{
	NO_CLK, CPU_CLK, CPU_CLK_PRESCALER_8, CPU_CLK_PRESCALER_64,CPU_CLK_PRESCALER_256,
	CPU_CLK_PRESCALER_1024
}Timer1_Prescaler;

/* Timer1 Mode*/
typedef enum{
	NORMAL_MODE, COMPARE_MODE
}Timer1_Mode;

/* Compare Output Mode, non-PWM (values of COM1A1/COM1B1,COM1A0/COM1B0) in TCCR1A register */
typedef enum{
	/* For OC1A/OC1B Channels */
	CHANNELS_DISCONNECTED = 0, TOGGLE_ON_COMPARE_MATCH = 5,
	CLEAR_ON_COMPARE_MATCH = 10, SET_ON_COMPARE_MATCH = 15
}Compare_Output_Mode;

typedef struct{
	uint16 initial_value;
	uint16 compare_value; // it will be used in compare mode only.
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
}Timer1_ConfigType;

/******************************************************************************
 *                         Global Variables
 *****************************************************************************/

extern uint8 volatile g_Timer1_count;

/******************************************************************************
 *                        Functions Prototypes
 *****************************************************************************/

/*
 * Description:
 * Function to initialize the Timer driver
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr);

/*
 * Description:
 * Function to disable the Timer1
 */
void Timer1_deInit(void);

/*
 * Description:
 * Function to set the Call Back function address
 */
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER1_H_ */
