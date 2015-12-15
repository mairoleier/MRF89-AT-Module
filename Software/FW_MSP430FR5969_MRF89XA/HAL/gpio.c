/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include "include/gpio.h"

// *************************************************************************************************
// @fn          PORT2_VECTOR
// @brief       Port2 interrupt vectors
// @param		none
// @return      none
// *************************************************************************************************
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {

	// This interrupt wakes up MCU after SYNC word has arrived and goes into Radio_Rx to start measuring RSSI values
	if (RF_IRQ_PORT_IFG & RF_IRQ0_PIN) {
		RF_IRQ0_IFG();		// Clear interrupt flag; IRQ0TXST - transmit start with IRQ0 bit
		_NOP();
	}

	// This interrupt indicates when receied data is ready to in RF module
	if (RF_IRQ_PORT_IFG & RF_IRQ1_PIN) {
		RF_IRQ1_IFG();		// Clear interrupt flag; TXDONE
		_NOP();
	}

	// Wake up from low power mode
	__bic_SR_register_on_exit(LPM0_bits);
}
