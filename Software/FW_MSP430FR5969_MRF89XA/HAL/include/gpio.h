#ifndef DRIVERS_GPIO_H_
#define DRIVERS_GPIO_H_

/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include <msp430.h>


/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/

/******** MRF89XA *********/
#define RF_IRQ_PORT_DIR		(P1DIR)
#define RF_IRQ_PORT_IN		(P1IN)
#define RF_IRQ_PORT_REN		(P1REN)
#define RF_IRQ_PORT_IE		(P1IE)
#define RF_IRQ_PORT_IES		(P1IES)
#define RF_IRQ_PORT_IFG		(P1IFG)

#define RF_IRQ0_PIN			BIT2	// P1.2
#define RF_IRQ1_PIN			BIT5	// P1.5

#define RF_RESET_PORT_DIR	(P4DIR)
#define RF_RESET_PORT_OUT	(P4OUT)
#define RF_RESET_PIN		BIT2	// P4.2


/********** LED ***********/
#define LED1_PORT_DIR      	(P1DIR)
#define LED1_PORT_OUT       (P1OUT)
#define LED1  	     		BIT0	// P1.0 (GREEN)

#define LED2_PORT_DIR      	(P4DIR)
#define LED2_PORT_OUT       (P4OUT)
#define LED2       			BIT6	// P4.6 (RED)



// IRQ definitions
#define RF_IRQ0_IN()  		{ RF_IRQ_PORT_DIR  &= ~RF_IRQ0_PIN; }
#define RF_IRQ1_IN()  		{ RF_IRQ_PORT_DIR  &= ~RF_IRQ1_PIN; }

// Enable interrupt
#define RF_IRQ0_IE()  		{ RF_IRQ_PORT_IE  |= RF_IRQ0_PIN; }
#define RF_IRQ1_IE()  		{ RF_IRQ_PORT_IE  |= RF_IRQ1_PIN; }

// Low to high edge
#define RF_IRQ0_IES()  		{ RF_IRQ_PORT_IES  &= ~RF_IRQ0_PIN; }
#define RF_IRQ1_IES()  		{ RF_IRQ_PORT_IES  &= ~RF_IRQ1_PIN; }

// Clear interrupt
#define RF_IRQ0_IFG()  		{ RF_IRQ_PORT_IFG  &= ~RF_IRQ0_PIN; }
#define RF_IRQ1_IFG()  		{ RF_IRQ_PORT_IFG  &= ~RF_IRQ1_PIN; }

// Pulldown resistor
#define RF_IRQ0_REN()  		{ RF_IRQ_PORT_REN  &= ~RF_IRQ0_PIN; }
#define RF_IRQ1_REN()  		{ RF_IRQ_PORT_REN  &= ~RF_IRQ1_PIN; }

// RF Module RESET pin
#define RF_RESET_OUT() 		{ RF_RESET_PORT_DIR |= RF_RESET_PIN; }
#define RF_RESET() 			{ RF_RESET_PORT_OUT |= RF_RESET_PIN; \
								__delay_cycles(20000); \
								RF_RESET_PORT_OUT &= ~RF_RESET_PIN;}
							// Keep RESET pin high for at least 100us


/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/


/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/

#endif 
