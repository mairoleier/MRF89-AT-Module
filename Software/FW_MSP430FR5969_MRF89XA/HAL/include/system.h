#ifndef SYSTEM_H_
#define SYSTEM_H_

/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gpio.h"
#include "../../driverlib/MSP430FR5xx_6xx/driverlib.h"

/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/
/* System speed settings. Available options:
 * SYSTEM_SPEED_1MHZ
 * SYSTEM_SPEED_8MHZ
 * SYSTEM_SPEED_12MHZ
 * SYSTEM_SPEED_16MHZ */
#define SYSTEM_SPEED_MHZ	SYSTEM_SPEED_8MHZ		// MCU speed in MHz. Can be between 1 to 16 or 25 MHz depending on the MCU


// LED definitions
#define LED1_TOGGLE()			{ LED1_PORT_OUT ^= LED1; }
#define LED1_ON()				{ LED1_PORT_OUT |= LED1; }
#define LED1_OFF()				{ LED1_PORT_OUT &= ~LED1; }

#define LED2_TOGGLE()			{ LED2_PORT_OUT ^= LED2; }
#define LED2_ON()				{ LED2_PORT_OUT |= LED2; }
#define LED2_OFF()				{ LED2_PORT_OUT &= ~LED2; }

// System speed settings
#define SYSTEM_SPEED_1MHZ		1
#define SYSTEM_SPEED_8MHZ		8
#define SYSTEM_SPEED_12MHZ		12
#define SYSTEM_SPEED_16MHZ		16


/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/
extern uint8_t exit_code;


/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/
uint8_t System_Init(void);
void System_Set_Speed(uint8_t mhz);

int cByteToHex(char input);
int String_To_Int (char *std);
int Trans_Int(char c);
int atoi(const char *s);
void itoa(long unsigned int value, char* result, int base);
uint32_t UIntToBCD (uint16_t UIntValue);

void Blink_LED(uint8_t led);
uint8_t Delay_Millis(uint16_t ms);

void System_Reset(void);

#endif /* SYSTEM_H_ */
