/*
 * main.h
 *
 *  Created  on: 06.02.2015
 *  Author: Mairo Leier, Maksim Gorev
 *
 *  Version: 0.3		15.10.2015
 *  Fix: radio.c:	Radio_Set_Mode()		If radio is set to Rx then it is initialized again because if it
 *  										has been previously in sleep mode then all configuration is lost
 *
 */
/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/

// System
#include "HAL/include/system.h"
#include "HAL/include/uart.h"
#include "HAL/include/spi.h"

// RF LNK Layer
#include "Radio/LNK/radio.h"

// RF Network Layer
#include "Radio/NWK/nwk_security.h"
#include "Radio/NWK/nwk_radio.h"

// AT specific commands
#include "AT/commands.h"
/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/


/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/



/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/
uint8_t exit_code = 0;		// Exit code number that is set after function exits


/***************************************************************************************************
 *         Main section                                                                            *
 ***************************************************************************************************/
void main(void) {

	// Initialize system
	exit_code = System_Init();
	Print_Error(exit_code);

	UART_Send_Data("\r\nMRF89XA Wireless Module\r\n");
	UART_Send_Data("Version 0.1\r\n");

	// Initialize default RF settings
	rf_data.local_addr 	 	= DEF_LOCAL_ADDR;
	rf_data.network_addr[0] = DEF_NETWORK_ADDR1;
	rf_data.network_addr[1] = DEF_NETWORK_ADDR2;
	rf_data.network_addr[2] = DEF_NETWORK_ADDR3;
	rf_data.network_addr[3] = DEF_NETWORK_ADDR4;
	rf_data.tx_power 		= DEF_TX_POWER;
	rf_data.channel_no		= DEF_RF_CHANNEL;
	rf_data.data_rate		= DEF_RF_DATA_RATE;


	while (1)
	{
		__bis_SR_register(LPM3_bits | GIE);       // Enter LPM3, interrupts enabled

		exit_code = 0;

		if (data_rcv_flag) {
			data_rcv_flag = 0;

			if (Check_Uart_Answer(CMD_AT) == 0)
			{
				UART_Send_Data(ANSW_OK);
			}
			else if (Check_Uart_Answer(CMD_AT_RST) == 0)
			{
				UART_Send_Data(ANSW_OK);
				System_Reset();
			}
			else if (Check_Uart_Answer(CMD_AT_VER) == 0)
			{
				UART_Send_Data(ANSW_OK);
				UART_Send_Data("AT:0.1 SW:0.1\r\n");
				break;
			}
			else if (Check_Uart_Answer_Block(CMD_AT_RFSEND,10) == 0)
			{
				Send_Rf_Data();
			}
			else if (Check_Uart_Answer_Block(CMD_AT_RFMODE,10) == 0)
			{
				Set_Rf_Mode(uart_data[10]);
			}
			else if (Check_Uart_Answer_Block(CMD_AT_RFLADDR,9) == 0)
			{
				Set_Rf_Local_Address();
			}
			else if (Check_Uart_Answer_Block(CMD_AT_RFNADDR,9) == 0)
			{
				if (Set_Network_Address() == 1) {
					UART_Send_Data(ANSW_ERROR);
					UART_Send_Data("Unknown address\r\n");
				}
			}
			else if (Check_Uart_Answer_Block(CMD_AT_RFCHANNEL,12) == 0)
			{
				Set_Rf_Channel();
			}
			else if (Check_Uart_Answer_Block(CMD_AT_RFPOWER,11) == 0)
			{
				Set_Rf_Power();
			}
			else if (Check_Uart_Answer_Block(CMD_AT_RFDRATE,11) == 0)
			{
				Set_Rf_Data_Rate();
			}

			// Clear working buffers after each command
			Clear_Uart_Data_Buf();
			Clear_Work_Data_Buf();

		}	/* END: data_rcv_flag */

		// if rf module is in receive mode
		if (radio_mode_receive == 1) {
			Receive_Rf_Data();
		}

	}	/* END: WHILE */
}		/* END: main */




