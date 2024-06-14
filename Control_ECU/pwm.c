/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.c
 *
 * Author: Mariz
 *
 *******************************************************************************/
#include "pwm.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "common_macros.h"

void PWM_Timer0_Start(uint8 duty_cycle){

    /* Equation of OCR value calculation, Duty cycle=((OCR0+1)/256)*100,
     * where 256 is the number of counts (8-bits register)
     * (OCR0+1) because the timer counts from 0 to 255, at the count number 256 it returns to zero.
     */
	uint8 ocr0_value = (uint8)(((float)(duty_cycle*0.01))*256-1);

	/* Set Timer Initial Value to 0 */
	TCNT0 = 0;

	/* Compare value */
	OCR0  = ocr0_value;

	/*Configure PB3/OC0 as output pin, where the PWM signal is generated */
	SET_BIT(DDRB,PB3);

	/*Fast PWM mode, non inverting mode, clock = F_CPU/8 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}
