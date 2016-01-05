/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include <msp430.h>
#include "system.h"
#include "../../driverlib/MSP430FR5xx_6xx/driverlib.h"


/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/

/********* SPI ************/
// SPI Chip Select Port definition
#define SPI_CS_PORT_DIR     (P1DIR)
#define SPI_CS_PORT_SEL     (P1SEL)
#define SPI_CS_PORT_SEL2    (P1SEL2)
#define SPI_CS_PORT_OUT     (P1OUT)

#define SPI_CS_DATA_PIN    	BIT3	// P1.3
#define SPI_CS_CONF_PIN    	BIT4	// P1.4


/*** MOSI ***/
#define MOSI_PORT GPIO_PORT_P1
#define MOSI_PIN GPIO_PIN6			// P1.6

/*** MISO ***/
#define MISO_PORT GPIO_PORT_P1
#define MISO_PIN GPIO_PIN7			// P1.7

/*** CLK ***/
#define SPI_CLK_PORT GPIO_PORT_P2
#define SPI_CLK_PIN GPIO_PIN2		// P2.2



/*** SPI speed ***/
#define SPI_SPEED_MHZ		1		// SPI speen in MHz


/*** SPI Base Address ***/
#define SPI_BASE_ADDR 		EUSCI_B0_BASE

#define SPI_CLK_SOURCE		EUSCI_B_SPI_CLOCKSOURCE_SMCLK		// SPI clock source
																//	1. USCI_B_SPI_CLOCKSOURCE_ACLK
																//	2. USCI_B_SPI_CLOCKSOURCE_SMCLK
#define SPI_BYTE_ORDER		EUSCI_B_SPI_MSB_FIRST				// Byte order of SPI trasmission
																//	1. USCI_B_SPI_MSB_FIRST
																//	2. USCI_B_SPI_LSB_FIRST


/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/
void SPI_Init(void);
unsigned char Spi_Send(const unsigned char _data);


/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/

