/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/

#include "include/spi.h"
#include "../driverlib/MSP430FR5xx_6xx/driverlib.h"


/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/


/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/


/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/

// *************************************************************************************************
// @fn          Init_SPI
// @brief       Initialize UART
//				Pinout: 	CLK  = P1.5 (OUT)
//							SOMI = P1.6 (IN)
//							SIMO = P1.7 (OUT)
// 							CS   = P1.3 (OUT)
// @param       none
// @return      none
// *************************************************************************************************
void SPI_Init(void) {
	// Configure SPI
	// Enable SW reset
	EUSCI_B_SPI_disable(SPI_BASE_ADDR);

	GPIO_setAsPeripheralModuleFunctionOutputPin(MOSI_PORT, MOSI_PIN, GPIO_SECONDARY_MODULE_FUNCTION);			//MOSI
	GPIO_setAsPeripheralModuleFunctionInputPin(MISO_PORT, MISO_PIN, GPIO_SECONDARY_MODULE_FUNCTION);			//MISO
	GPIO_setAsPeripheralModuleFunctionOutputPin(SPI_CLK_PORT, SPI_CLK_PIN, GPIO_SECONDARY_MODULE_FUNCTION);		//SCLK

	EUSCI_B_SPI_initMasterParam param = {0};
	param.msbFirst = EUSCI_B_SPI_MSB_FIRST;


	param.selectClockSource = SPI_CLK_SOURCE;
	param.clockSourceFrequency = SYSTEM_SPEED_MHZ*1000000;
	param.desiredSpiClock = SPI_SPEED_MHZ*1000000;

#if SPI_MODE == 0
		param.clockPhase = EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
		param.clockPolarity = EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
#elif SPI_MODE == 1
		param.clockPhase = EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXTT;
		param.clockPolarity = EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
#elif SPI_MODE == 2
		param.clockPhase = EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
		param.clockPolarity = EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
#elif SPI_MODE == 3
		param.clockPhase = EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXTT;
		param.clockPolarity = EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
#endif

	// Init SPI with parameters specified earlier
		EUSCI_B_SPI_initMaster(SPI_BASE_ADDR, &param);

	//Enable SPI module
		EUSCI_B_SPI_enable(SPI_BASE_ADDR);

}


// *************************************************************************************************
// @fn          Spi_Send
// @brief       Send data over SPI
// @param       const unsigned char _data
// @return      unsigned char
// *************************************************************************************************
unsigned char Spi_Send(const unsigned char _data) {
    //Transmit Data to slave
    EUSCI_B_SPI_transmitData(SPI_BASE_ADDR, _data);

	//USCI TX buffer ready?
	while(!EUSCI_B_SPI_getInterruptStatus(SPI_BASE_ADDR, EUSCI_B_SPI_RECEIVE_INTERRUPT))
		{ ; }

    return EUSCI_B_SPI_receiveData(SPI_BASE_ADDR);
}

