/*
 * rf_gpio.h
 *
 *  Created on: 24.11.2015
 *      Author: MAiro Leier
 */

#ifndef RADIO_DRIVERS_RF_GPIO_H_
#define RADIO_DRIVERS_RF_GPIO_H_


/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/

#define SPI_CS_DATA_OUT()   { SPI_CS_PORT_DIR |=  SPI_CS_DATA_PIN; }
#define SPI_CS_DATA_HI()    { SPI_CS_PORT_OUT |=  SPI_CS_DATA_PIN; }
#define SPI_CS_DATA_LO()    { SPI_CS_PORT_OUT &= ~SPI_CS_DATA_PIN; }

#define SPI_CS_CONF_OUT()   { SPI_CS_PORT_DIR |=  SPI_CS_CONF_PIN; }
#define SPI_CS_CONF_HI()    { SPI_CS_PORT_OUT |=  SPI_CS_CONF_PIN; }
#define SPI_CS_CONF_LO()    { SPI_CS_PORT_OUT &= ~SPI_CS_CONF_PIN; }

#endif /* RADIO_DRIVERS_RF_GPIO_H_ */
