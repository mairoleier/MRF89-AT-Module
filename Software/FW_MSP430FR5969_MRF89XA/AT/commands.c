/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include "../HAL/include/system.h"
#include "commands.h"
#include "../HAL/include/uart.h"

// Radio
#include "../Radio/LNK/radio.h"
#include "../Radio/NWK/nwk_radio.h"

// Communication
#include "../Radio/drivers/rf_spi.h"
/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/
char CMD_AT[] 				= {"AT"};
char CMD_AT_RST[] 			= {"AT+RST"};
char CMD_AT_VER[] 			= {"AT+VER"};

char CMD_AT_RFSEND[]		= {"AT+RFSEND="};
char CMD_AT_RFMODE[]		= {"AT+RFMODE="};
char CMD_AT_RFLADDR[]		= {"AT+LADDR="};
char CMD_AT_RFNADDR[]		= {"AT+NADDR="};
char CMD_AT_RFPOWER[]		= {"AT+RFPOWER="};
char CMD_AT_RFCHANNEL[]		= {"AT+RFCHANNEL="};
char CMD_AT_RFDRATE[]		= {"AT+RFDRATE="};

char ANSW_SEND_OK[]			= {"SEND OK\r\n"};
char ANSW_OK[]				= {"OK\r\n"};
char ANSW_ERROR[]			= {"ERROR:"};
char ANSW_ERROR_UNK_CMD[]	= {"ERROR:unknown command\r\n"};

/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/
uint8_t _Validate_Address(uint8_t addr);

/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/
char *remote_addr_p;
char *encryption_p;
char *ack_p;

int remote_addr;
uint8_t encryption;
uint8_t ack;
uint8_t payload_length;
uint8_t radio_mode_receive = 0;


// *************************************************************************************************
// @fn          Send_Rf_Data
// @brief       Send data over RF that is received from UART
// @param       None
// @return      None
// *************************************************************************************************
void Send_Rf_Data(void) {
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t a = 0;

	// Loop through whole received string to cut string into pieces divided with commas
	// Start from 10 because from that point real data starts, before that is only command
	for (i=10; i<strlen(uart_data); i++) {

		// Copy each char into data part until comma is found
		if (uart_data[i] != ',') {
			data_p[a][j++] = uart_data[i];						// Copy new char into new array
		} else {
			// Check if we are not going out of array
			if (a < AT_CMD_MAX_PARTS-1) {
				j = 0;		// Reset current data array counter to start from the beginning with the next array
				a++;		// Move to the next data array
			} else {
				data_p[a][j++] = uart_data[i];						// Copy new char into new array
			}
		}
	}

	// Allocate memory
	remote_addr_p 	= (char*) malloc(1);
	encryption_p 	= (char*) malloc(1);
	ack_p 			= (char*) malloc(1);

	strcpy(remote_addr_p, data_p[0]);
	remote_addr = String_To_Int(remote_addr_p);

	strcpy(encryption_p, data_p[1]);
	encryption = String_To_Int(encryption_p);

	strcpy(ack_p, data_p[2]);
	ack = String_To_Int(ack_p);

	payload_length = strlen(data_p[3]);
	exit_code = Radio_Send_Data((uint8_t *)data_p[3], payload_length, remote_addr, encryption, ack);

	if (exit_code == 0)
		UART_Send_Data(ANSW_OK);
	else {
		UART_Send_Data(ANSW_ERROR);
		UART_Send_ByteToChar(&exit_code);
		//UART_Send_Data(rf_error_code[exit_code].name);
		UART_Send_Data("\r\n");
	}

	// Free allocated memory
	free(remote_addr_p);
	free(encryption_p);
	free(ack_p);
}


// *************************************************************************************************
// @fn          Receive_Rf_Data
// @brief       Receive data from RF module and send it over UART
// @param       None
// @return      None
// *************************************************************************************************
void Receive_Rf_Data(void) {
	// Receive data, wait until 100ms until timeout and continue with other tasks
	uint8_t len = 0;
	if (Radio_Receive_Data(RxPacket, &len, 100, &rssi_rx) == EXIT_NO_ERROR) {
		uint8_t var;
		char *hexString;
		hexString = (char*) malloc(1);

		UART_Send_Data("DATA:");

		// Packet length
		sprintf(hexString,"%d",RxPacket[0]-5);	// Remove 3 first bytes
		UART_Send_Data(hexString);
		UART_Send_Data(",");

		// Source address
		sprintf(hexString,"%d",RxPacket[2]);
		UART_Send_Data(hexString);
		UART_Send_Data(",");

		// Destination address
		sprintf(hexString,"%d",RxPacket[1]);
		UART_Send_Data(hexString);
		UART_Send_Data(",");

		// Encryption byte
		sprintf(hexString,"%d",RxPacket[3]);
		UART_Send_Data(hexString);
		UART_Send_Data(",");

		// Received signal strength
		//int8_t rssi_dbm;
		//rssi_dbm = Radio_Calculate_RSSI(rssi_rx);
		sprintf(hexString,"%d",rssi_rx);
		UART_Send_Data(hexString);
		UART_Send_Data(",");

		// Data
		for (var = 5; var < len; ++var) {
			UART_Send_Byte(RxPacket[var]);
		}
		UART_Send_Data("\r\n");

		free(hexString);
	}
}

// *************************************************************************************************
// @fn          Send_Rf_Data
// @brief       Send data over RF that is received from UART
// @param       None
// @return      None
// *************************************************************************************************
void Set_Rf_Mode(uint8_t rf_mode) {

	// Convert ASCII to uint8
	char *rf_mode_p;
	rf_mode_p = (char*) malloc(1);
	strcpy(rf_mode_p, &uart_data[10]);
	rf_mode = String_To_Int(rf_mode_p);

	switch (rf_mode) {
	case RF_MODE_RX:
		if (Radio_Set_Mode(RADIO_RX) == EXIT_NO_ERROR) {
			UART_Send_Data(ANSW_OK);
			radio_mode_receive = 1;
		} else {
			UART_Send_Data(ANSW_ERROR);
			UART_Send_ByteToChar(&exit_code);
			//UART_Send_Data(rf_error_code[exit_code].name);
			UART_Send_Data("\r\n");
		}
		break;
	case RF_MODE_STANDBY:
		if (Radio_Set_Mode(RADIO_STANDBY) == EXIT_NO_ERROR) {
			UART_Send_Data(ANSW_OK);
			radio_mode_receive = 0;
		} else {
			UART_Send_Data(ANSW_ERROR);
			UART_Send_ByteToChar(&exit_code);
			//UART_Send_Data(rf_error_code[exit_code].name);
			UART_Send_Data("\r\n");
		}
		break;
	case RF_MODE_SLEEP:
		if (Radio_Set_Mode(RADIO_SLEEP) == EXIT_NO_ERROR) {
			UART_Send_Data(ANSW_OK);
			radio_mode_receive = 0;
		} else {
			UART_Send_Data(ANSW_ERROR);
			UART_Send_ByteToChar(&exit_code);
			//UART_Send_Data(rf_error_code[exit_code].name);
			UART_Send_Data("\r\n");
		}
		break;
	default:
		UART_Send_Data(ANSW_ERROR_UNK_CMD);
		break;
	}
}


// *************************************************************************************************
// @fn          Set_Rf_Local_Address
// @brief       Set local node address
// @param       None
// @return      None
// *************************************************************************************************
void Set_Rf_Local_Address(void) {
	uint8_t addr;

	// Copy new address into working array
	data_p[0][0] = uart_data[9];
	data_p[0][1] = uart_data[10];
	data_p[0][2] = uart_data[11];

	// Convert string to int
	addr = String_To_Int(data_p[0]);

	if ((addr < 1) || (addr > 255)) {
		UART_Send_Data(ANSW_ERROR);
		UART_Send_Data("Unknown address\r\n");
	} else {
		rf_data.local_addr = String_To_Int(data_p[0]);
		// Set new address into rf module register
		SPI_Conf_Write_Register(MRF89_REG_NADDSREG, rf_data.local_addr);
		UART_Send_Data(ANSW_OK);
	}
}


// *************************************************************************************************
// @fn          Set_Network_Address
// @brief       Set network address
// @param       None
// @return      None
// *************************************************************************************************
uint8_t Set_Network_Address(void) {
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t a = 0;
	uint8_t addr1, addr2, addr3, addr4;

	// Loop through whole received string to cut string into pieces divided with commas
	// Start from 10 because from that point real data starts, before that is only command
	for (i=10; i<strlen(uart_data); i++) {

		// Copy each char into data part until comma is found
		if (uart_data[i] != ':') {
			data_p[a][j++] = uart_data[i];				// Copy new char into new array
		} else {
			// Check if we are not going out of array
			if (a < AT_CMD_MAX_PARTS-1) {
				j = 0;									// Reset current data array counter to start from the beginning with the next array
				a++;									// Move to the next data array
			} else {
				data_p[a][j++] = uart_data[i];			// Copy new char into new array
			}
		}
	}

	// Convert address into integer
	addr1 = String_To_Int(data_p[0]);
	addr2 = String_To_Int(data_p[1]);
	addr3 = String_To_Int(data_p[2]);
	addr4 = String_To_Int(data_p[3]);

	// Validate new network address and set if OK
	if (_Validate_Address(addr1) == 0)
		rf_data.network_addr[0] = addr1;
	else
		return 1;

	if (_Validate_Address(addr2) == 0)
		rf_data.network_addr[1] = addr2;
	else
		return 1;

	if (_Validate_Address(addr3) == 0)
		rf_data.network_addr[2] = addr3;
	else
		return 1;

	if (_Validate_Address(addr4) == 0)
		rf_data.network_addr[3] = addr4;
	else
		return 1;

	// Configure Radio module with new network address
	SPI_Conf_Write_Register(MRF89_REG_SYNCV31REG, rf_data.network_addr[0]);
	SPI_Conf_Write_Register(MRF89_REG_SYNCV23REG, rf_data.network_addr[1]);
	SPI_Conf_Write_Register(MRF89_REG_SYNCV15REG, rf_data.network_addr[2]);
	SPI_Conf_Write_Register(MRF89_REG_SYNCV07REG, rf_data.network_addr[3]);


	// Send back answer that all set correctly
	UART_Send_Data(ANSW_OK);

	return 0;
}


// *************************************************************************************************
// @fn          Set_Rf_Power
// @brief       Set RF module TX power
// @param       None
// @return      None
// *************************************************************************************************
void Set_Rf_Power(void) {
	uint8_t pwr;

	// Copy new address into working array
	data_p[0][0] = uart_data[11];

	// Convert string to int
	pwr = String_To_Int(data_p[0]);

	if ((pwr < 1) || (pwr > 8)) {
		UART_Send_Data(ANSW_ERROR);
		UART_Send_Data("Unknown TX power mode\r\n");
	} else {
		switch (pwr) {
			case 1:	rf_data.tx_power = TXCON_N8DBM;	break;
			case 2:	rf_data.tx_power = TXCON_N5DBM;	break;
			case 3:	rf_data.tx_power = TXCON_N2DBM;	break;
			case 4:	rf_data.tx_power = TXCON_1DBM;	break;
			case 5:	rf_data.tx_power = TXCON_4DBM;	break;
			case 6:	rf_data.tx_power = TXCON_7DBM;	break;
			case 7:	rf_data.tx_power = TXCON_10DBM;	break;
			case 8:	rf_data.tx_power = TXCON_13DBM;	break;
		}

		// Set new address into rf module register
		Radio_Set_TxPower(rf_data.tx_power);
		UART_Send_Data(ANSW_OK);
	}
}


// *************************************************************************************************
// @fn          Set_Rf_Channel
// @brief       Set RF module channel number
// @param       None
// @return      None
// *************************************************************************************************
void Set_Rf_Channel(void) {
	uint8_t chan;

	// Copy new address into working array
	data_p[0][0] = uart_data[13];
	data_p[0][1] = uart_data[14];

	// Convert string to int
	chan = String_To_Int(data_p[0]);

	if ((chan < 1) || (chan > RF_CHANNEL_MAX_NUM)) {
		UART_Send_Data(ANSW_ERROR);
		UART_Send_ByteToChar((uint8_t *)ERR_UNKNOWN_CH_NO);
		UART_Send_Data(":Unknown radio channel\r\n");
	} else {
		rf_data.channel_no = chan;
		// Set new address into rf module register
		Radio_Set_Channel(rf_data.channel_no);
		UART_Send_Data(ANSW_OK);
	}
}



// *************************************************************************************************
// @fn          Set_Rf_Data_Rate
// @brief       Set RF module datarate
// @param       None
// @return      uint8_t 	0			No error
//							1			Error setting datarate
// *************************************************************************************************
uint8_t Set_Rf_Data_Rate(void) {
	uint8_t drate;

	// Copy new address into working array
	data_p[0][0] = uart_data[11];

	// Convert string to int
	drate = String_To_Int(data_p[0]);

	if ((drate < 1) || (drate > 4)) {
		UART_Send_Data(ANSW_ERROR);
		UART_Send_Data("Unknown datarate\r\n");
		return EXIT_NO_ERROR;
	} else {
		switch (drate) {
			case 1:	rf_data.data_rate = RF_DATA_RATE_2;		break;
			case 2:	rf_data.data_rate = RF_DATA_RATE_10;	break;
			case 3:	rf_data.data_rate = RF_DATA_RATE_33;	break;
			case 4:	rf_data.data_rate = RF_DATA_RATE_100;	break;
		}

		// Set new address into rf module register
		if (exit_code = Radio_Init(rf_data.data_rate, rf_data.tx_power, rf_data.channel_no)) {		// Initialize RF module with 2kbps speed
			UART_Send_Data(ANSW_ERROR);
			UART_Send_Data("Unknown datarate\r\n");
			return exit_code;
		} else {
			UART_Send_Data(ANSW_OK);
			return EXIT_NO_ERROR;
		}
	}
}


// *************************************************************************************************
// @fn          Print_Error
// @brief       Print code that is set when function exits. If error code is 0 then nothin is prnted
//				out and packet is received or sent correctly
// @param       uint8 error_code		Error code number that is set by the function
// @return      none
// *************************************************************************************************
void Print_Error(uint8_t code) {

	// Print out error code only if it is not 0
	if (code) {
		UART_Send_Data(ANSW_ERROR);
		UART_Send_ByteToChar(&code);
		UART_Send_Data("\r\n");
	}

	// If packet size has wrong length then reset radio module
	if (code == ERR_RX_WRONG_LENGTH) {
		Radio_Init(rf_data.data_rate, rf_data.tx_power, rf_data.channel_no);
	}

	// Reset exit code
	exit_code = 0;
}


// *************************************************************************************************
// @fn          _Validate_Address
// @brief       Check if (node or network) address is in allowed range
// @param       uint8_t addr			Address to be checked
// @return      uint8_t 0				Address is OK
//						1				Address is out of range
// *************************************************************************************************
uint8_t _Validate_Address(uint8_t addr) {
	if ((addr < 1) || (addr > 255))
		return 1;
	else
		return 0;
}
