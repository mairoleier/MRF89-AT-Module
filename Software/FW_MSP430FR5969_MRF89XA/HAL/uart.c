/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include <string.h>
#include "include/uart.h"
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
uint8_t uart_rcv_byte;
uint8_t uart_index = 0;


// *************************************************************************************************
// @fn          Init_UART
// @brief       Initialize UART
// @param       none
// @return      none
// http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
// *************************************************************************************************
void UART_Init(void) {
	// Define UART ports
	GPIO_setAsPeripheralModuleFunctionOutputPin(UART_TX_PORT, UART_TX_PIN, GPIO_SECONDARY_MODULE_FUNCTION);	//TX
	GPIO_setAsPeripheralModuleFunctionInputPin(UART_RX_PORT, UART_RX_PIN, GPIO_SECONDARY_MODULE_FUNCTION);	//RX

	EUSCI_A_UART_disable(UART_BASE_ADDR);

	//Initialize UART
	EUSCI_A_UART_initParam param = {0};
	param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
	param.clockPrescalar = 52;
	param.firstModReg = 0;
	param.secondModReg = 0x49;
	param.parity = EUSCI_A_UART_NO_PARITY;
	param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
	param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
	param.uartMode = EUSCI_A_UART_MODE;
	param.overSampling = 1;

	if (STATUS_FAIL == EUSCI_A_UART_init(UART_BASE_ADDR, &param)) {
		return;
	}

	// Enable UART State Machine
	EUSCI_A_UART_enable(UART_BASE_ADDR);

	// Enable UART receive interrupt
	EUSCI_A_UART_enableInterrupt(UART_BASE_ADDR, EUSCI_A_UART_RECEIVE_INTERRUPT);
}


// *************************************************************************************************
// @fn          UART_Send_Byte
// @brief       Send char over UART
// @param       uint8 c				Char to send
// @return      none
// *************************************************************************************************
void UART_Send_Byte(uint8_t c) {
	while (!EUSCI_A_UART_getInterruptStatus(UART_BASE_ADDR, EUSCI_A_UART_TRANSMIT_INTERRUPT))
	{ ; }
	// Transmit character
	EUSCI_A_UART_transmitData(UART_BASE_ADDR, c);
}


// *************************************************************************************************
// @fn          UART_Send_Data
// @brief       Send string over UART
// @param       const char str			String to send
// @return      none
// *************************************************************************************************
void UART_Send_Data(const char *str) {
	while (*str)
		UART_Send_Byte(*str++);
}


// *************************************************************************************************
// @fn          sendByteToChar
// @brief
// @param
// @return      none
// *************************************************************************************************
void UART_Send_ByteToChar(uint8_t bytes[]) {
	char hexValue;

	hexValue = (unsigned char)((bytes[0] & 0xF0)) >> 4;   	// Get 4 bits of byte
	UART_Send_Byte(cByteToHex(hexValue));         			// Convert byte to char and send it

	hexValue = (unsigned char)(bytes[0] & 0x0F);  			// Get 4 bits of byte
	UART_Send_Byte(cByteToHex(hexValue));         			// Convert byte to char and send it
}


// *************************************************************************************************
// Function Name  : Check_Uart_Answer
// Description    : Compare UART received answer with your string
// Input          : char answer[]				String to compare with
// Return         : uint8_t		0				String matches
//  				uint8_t		1				String does not match
 // *************************************************************************************************
uint8_t Check_Uart_Answer(char answer[]) {
	if (strcmp(uart_data, answer) == 0) {					// IF answer is "OK" then module is working
		return 0;
	} else {
		return 1;
	}
}


// *************************************************************************************************
// Function Name  : Check_Uart_Answer_Block
// Description    : Compare part of the UART received answer with your string
// Input          : char answer[]				String to compare with
//				  : uint8_t size				How many chars to compare
// Return         : uint8_t		0				String matches
//  				uint8_t		1				String does not match
 // *************************************************************************************************
uint8_t Check_Uart_Answer_Block(char answer[], uint8_t size) {
	if (strncmp(uart_data, answer, size) == 0) {					// IF answer is "OK" then module is working
		return 0;
	} else {
		return 1;
	}
}


// *************************************************************************************************
// Function Name  : Clear_Uart_Data_Buf
// Description    : Overwrite UART1 data buffer with zeros
// Input          : None
// Return         : None
// *************************************************************************************************
void Clear_Uart_Data_Buf(void) {
    uint8_t i;
    for(i=0;i<MAX_UART_STR_LENGTH;i++)
        uart_data[i] = 0;
}


// *************************************************************************************************
// Function Name  : Clear_Work_Data_Buf
// Description    : Overwrite working data buffer with zeros
// Input          : None
// Return         : None
// *************************************************************************************************
void Clear_Work_Data_Buf(void) {
    uint8_t i,j;
    for (i=0; i<4; i++) {
    	for(j=0; j<MAX_UART_STR_LENGTH; j++)
    		data_p[i][j] = 0;
    }
}


// UART receive interrupt
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
	switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
	{
	case USCI_NONE:
		break;
	case USCI_UART_UCRXIFG:

		uart_rcv_byte = UCA0RXBUF;

		// If '\r' or '\n' received or buffer gets full then set flag that data received
		if ((uart_rcv_byte == '\r') || (uart_rcv_byte == '\n') || (uart_index >= MAX_UART_STR_LENGTH-1)) {
			if (uart_index != 0) {						// Some data is received

				// Handle new data received from uart
				memcpy(uart_data, uart_rcv_buf, uart_index);		// Copy received data into buffer
				uart_data[uart_index] = '\0';						// Add terminating NULL
				data_rcv_flag = 1;									// Set flag that data is ready for processing
				uart_index=0;										// Reset uart input buffer index
			}
		} else {													// Else just stor received chars into input buffer
			uart_rcv_buf[uart_index++] = uart_rcv_byte;
		}
		__bic_SR_register_on_exit(LPM3_bits); // Exit LPM0 on reti
		break;

	case USCI_UART_UCTXIFG: break;
	case USCI_UART_UCSTTIFG: break;
	case USCI_UART_UCTXCPTIFG: break;
	}
}

