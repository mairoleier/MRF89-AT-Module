/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include "../../HAL/include/spi.h"
#include "rf_spi.h"
#include "rf_gpio.h"

/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/


/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/


/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/
unsigned char dummy;


// *************************************************************************************************
// @fn          RF_SPI_Init
// @brief       Enable RF Chip Select pins + Initialize SPI system
// @param       none
// @return      none
// *************************************************************************************************
void RF_SPI_Init(void) {
	SPI_CS_DATA_OUT();
	SPI_CS_DATA_HI();
	SPI_CS_CONF_OUT();
	SPI_CS_CONF_HI();

	SPI_Init();
}


// *************************************************************************************************
// @fn          RF_IRQ_Init
// @brief       Initialise IRQ signals associated with MRF module.
// @param       none
// @return      none
// *************************************************************************************************
void RF_IRQ_Init(void) {
	// IRQ0 - RX:SYNC WORD / TX: wake up MCU
	// IRQ1 - RX:CRC OK / TX:TXDONE interrupt
	// Used to wake up MCU and start reading RSSI during RF data receiving

	// Set pin as input
	RF_IRQ0_IN();
	RF_IRQ1_IN();

	// Enable pull-up resistor
	RF_IRQ0_REN();
	RF_IRQ1_REN();

	// Set interrupt edge LOW to HIGH edge
	RF_IRQ0_IES();
	RF_IRQ1_IES();

	// Enable interrupt
	RF_IRQ0_IE();
	RF_IRQ1_IE();

	// Clear pending interrupts
	RF_IRQ0_IFG();
	RF_IRQ1_IFG();
}


// *************************************************************************************************
// @fn          SPI_Conf_Read_Register
// @brief       Initialize system
// @param       none
// @return      none
// *************************************************************************************************
unsigned char SPI_Conf_Read_Register(unsigned char address) {
	unsigned char Rx_buf;

	SPI_CS_CONF_LO();

	dummy = Spi_Send((address << 1) | SPI_READ_CMD);
	Rx_buf = Spi_Send(0x00);

	SPI_CS_CONF_HI();

	return Rx_buf;
}


// *************************************************************************************************
// @fn          SPI_Data_Read_Register
// @brief       Initialize system
// @param       unsigned char data
// @return      none
// *************************************************************************************************
unsigned char SPI_Data_Read(void) {
	unsigned char Rx_buf;

	SPI_CS_DATA_LO();
	Rx_buf = Spi_Send(0x00);
	SPI_CS_DATA_HI();

	return Rx_buf;
}


// *************************************************************************************************
// @fn          SPI_Conf_Write_Register
// @brief       Write to the register
// @param       uint8 address       Register address
//              uint8 data          Configuration data
// @return      uint8 Rx_buf        Result of configuration
// *************************************************************************************************
unsigned char SPI_Conf_Write_Register(unsigned char address, unsigned char data) {
	unsigned char Rx_buf;

	SPI_CS_CONF_LO();

	dummy = Spi_Send((address << 1) | SPI_WRITE_CMD);
	Rx_buf = Spi_Send(data);

	SPI_CS_CONF_HI();

	return Rx_buf;
}


// *************************************************************************************************
// @fn          SPI_Data_Write_Register
// @brief       Write to the register
// @param       uint8 data          Configuration data
// @return      uint8 Rx_buf        Result of configuration
// *************************************************************************************************
unsigned char SPI_Data_Write(unsigned char *packet, unsigned int len) {
	unsigned char Rx_buf;

	while (len-- > 0) {
		SPI_CS_DATA_LO();
		Rx_buf = Spi_Send(*packet++);
		SPI_CS_DATA_HI();
	}

	return Rx_buf;
}
