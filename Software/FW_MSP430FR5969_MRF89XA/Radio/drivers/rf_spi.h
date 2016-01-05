#ifndef RADIO_DRIVERS_RF_SPI_H_
#define RADIO_DRIVERS_RF_SPI_H_

/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include <stdint.h>


/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/
#define SPI_READ_CMD           0x40  // Read command (first bit is high)
#define SPI_WRITE_CMD          0x00  // Write command

/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/
//RF SPI and GPIO Initialization
void RF_SPI_Init(void);
void RF_IRQ_Init(void);

// Functions for register configurations
uint8_t SPI_Conf_Read_Register(uint8_t address);
uint8_t SPI_Conf_Write_Register(uint8_t address, uint8_t data);

// Functiond data read or write data
uint8_t SPI_Data_Read(void);
uint8_t SPI_Data_Write(uint8_t *packet, uint16_t len);


#endif /* RADIO_DRIVERS_RF_SPI_H_ */
