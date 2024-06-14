/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: dc_motor.c
 *
 * Author: Mariz
 *
 *******************************************************************************/
#include "dc_motor.h"
#include "gpio.h"
#include "pwm.h"

void DcMotor_Init(void){
	/* Setup the direction for the two motor pins */
	GPIO_setupPinDirection(DC_MOTOR_TERMINAL1_PORT_ID,DC_MOTOR_TERMINAL1_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_TERMINAL2_PORT_ID,DC_MOTOR_TERMINAL2_PIN_ID,PIN_OUTPUT);

	/* Stop at the DC-Motor at the beginning */
	GPIO_writePin(DC_MOTOR_TERMINAL1_PORT_ID,DC_MOTOR_TERMINAL1_PIN_ID,LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_TERMINAL2_PORT_ID,DC_MOTOR_TERMINAL2_PIN_ID,LOGIC_LOW);
}

void DcMotor_Rotate(DcMotor_State state,uint8 speed){

	switch(state){
	case STOP:
		GPIO_writePin(DC_MOTOR_TERMINAL1_PORT_ID,DC_MOTOR_TERMINAL1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_TERMINAL2_PORT_ID,DC_MOTOR_TERMINAL2_PIN_ID,LOGIC_LOW);
		break;
	case CW:
		GPIO_writePin(DC_MOTOR_TERMINAL1_PORT_ID,DC_MOTOR_TERMINAL1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_TERMINAL2_PORT_ID,DC_MOTOR_TERMINAL2_PIN_ID,LOGIC_HIGH);
		break;
	case A_CW:
		GPIO_writePin(DC_MOTOR_TERMINAL1_PORT_ID,DC_MOTOR_TERMINAL1_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_TERMINAL2_PORT_ID,DC_MOTOR_TERMINAL2_PIN_ID,LOGIC_LOW);
		break;
	}
	PWM_Timer0_Start(speed);
}
