# Door-Locker-Security-System
## Project description
Implementing a system to unlock a door using a password.  
* The project is implemented based on the layered architecture model.
* Using two ATmega32 Microcontrollers:
  * HMI_ECU (Human Machine Interface) 
  * Control_ECU  
* Drivers:
 GPIO, LCD, Keypad, Timer, UART, I2C, EEPROM, PWM, Buzzer and DC-motor

## HMI_ECU (Human Machine Interface):
 * it is responsible for interaction with the user. It takes inputs through a keypad and displays messages on the LCD.  
 * it contains:  
   * In the Hardware Abstraction Layer (HAL): LCD and Keypad drivers.  
   * In the Microcontroller Abstraction Layer (MCAL): GPIO, UART, and Timer drivers.  

 ## Control_ECU:
 * it is responsible for all the processing and decisions in the system like password checking, open the door and activate the system alarm.  
 * it contains:  
   * In the Hardware Abstraction Layer (HAL): Buzzer, EEPROM, and DC-motor drivers.  
   * In the Microcontroller Abstraction Layer (MCAL): GPIO, UART, I2C, and Timer drivers.   
 
 
 
