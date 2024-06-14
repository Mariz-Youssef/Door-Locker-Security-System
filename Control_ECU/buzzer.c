/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.c
 *
 * Author: Mariz
 *
 *******************************************************************************/

#include "buzzer.h"
#include "gpio.h"

void Buzzer_init()
{
	/* Setting the direction of the Buzzer pin as output*/
	GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT);

	/* Turn off The Buzzer at the beginning */
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN,LOGIC_LOW);
}

void Buzzer_on(void)
{
	/* Enable the Buzzer */
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN,LOGIC_HIGH);
}

void Buzzer_off(void)
{
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN,LOGIC_LOW);
}
