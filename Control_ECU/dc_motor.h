/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: dc_motor.h
 *
 * Author: Mariz
 *
 *******************************************************************************/


#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include"std_types.h"

/**************************************************************
 *                    Definitions
 *************************************************************/

/* DC Motor terminal 1 configuration, Input 1 of the H_bridge*/
#define DC_MOTOR_TERMINAL1_PORT_ID         PORTA_ID
#define DC_MOTOR_TERMINAL1_PIN_ID          PIN5_ID

/* DC Motor terminal 1 configuration, Input 2 of the H_bridge*/
#define DC_MOTOR_TERMINAL2_PORT_ID         PORTA_ID
#define DC_MOTOR_TERMINAL2_PIN_ID          PIN6_ID

/* H_Bridge Enable bit */
#define H_BRIDGE_ENABLE_PORT_ID            PORTB_ID
#define H_BRIDGE_ENABLE_PIN_ID             PIN3_ID

/**************************************************************
 *                 Types Declarations
 *************************************************************/

typedef enum
{
	STOP, CW /*Clockwise*/, A_CW /*Anti_clockwise*/
}DcMotor_State;

/**************************************************************
 *                 Functions Prototypes
 *************************************************************/

/*
 * Description:
 * 1-The Function responsible for setup the direction for the two
 * motor pins through the GPIO driver.
 * 2-Stop the DC-Motor at the beginning through the GPIO driver.
 */
void DcMotor_Init(void);

/*
 * Description:
 * 1-The function responsible for rotate the DC Motor CW/ or A-CW or
 * stop the motor based on the state input state value.
 * 2-Send the required duty cycle to the PWM driver based on the
 * required speed value.
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DC_MOTOR_H_ */
