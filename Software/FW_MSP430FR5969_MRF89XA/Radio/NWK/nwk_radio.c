/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include "nwk_security.h"
#include "nwk_radio.h"
#include "../LNK/radio.h"

#include "../../HAL/include/uart.h"
#include "../../HAL/include/system.h"

/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/


/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/
uint8_t _Modify_Packet_Header(uint8_t *packet, uint8_t packet_length, uint8_t header[]);

/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/
uint8_t nwk_header[NWK_HEADER_SIZE];

// *************************************************************************************************
// @fn          Radio_Send_Data
// @brief       Send data in NWK layer using wireless radio.
// @param       uint8 *packet                   Data packet (payload) that is being sent
//              uint8 len                       Length of the data packet
//              uint8 dest_address              Address where the packet is sent
//              uint8 encryption                Encrypt the packet payload
//                          PAYLOAD_ENC_OFF     Encrypt the payload
//                          PAYLOAD_ENC_OFF     Do not encrypt the payload
//              uint8 ack                       Receive ACK after data was sent
//                          PCKT_ACK_ON         Ask for ACK
//                          PCKT_ACK_OFF        Do not ask for ACK
// @return      uint8  exit_code				Return code after function exits
// *************************************************************************************************
uint8_t Radio_Send_Data(uint8_t *packet, uint8_t len, uint8_t dest_address, uint8_t encryption, uint8_t ack) {
	exit_code = 0;	// Function exit code

	// Check if payload is bigger than allowed
	if (len > PAYLOAD_MAX_SIZE) {
		return ERR_PAYLOAD_TOO_BIG;
	}

	if (encryption) {
		if (len > PAYLOAD_MAX_ENC_SIZE) {
			return ERR_PAYLOAD_TOO_BIG;
		}
	}

	// If encryption enabled, add one byte to the unencrypted payload
	if (encryption)
		nwk_header[0] = PAYLOAD_ENC_ON;
	else
		nwk_header[0] = PAYLOAD_ENC_OFF;

	// Add encryption byte to the beginning of the packet and get new length of the packet
	 len = _Modify_Packet_Header(packet, len, nwk_header);

	// Modify CTRL byte to ask for ACK
	if (ack) {
		// packet[] does not contain length, DST bytes and starts from ENC byte
		packet[1] |= PKT_CTRL_ACK_REQ;
	}

	// Encrypt payload
	if (encryption) {
		Payload_Encrypt(packet+1);		// Add +1 to set encryption bit unencrypted
		len = ENC_PACKET_SIZE+2;		// Set new length because each packet is encrypted with 128 bits = 16 bytes
	}

	/* DEBUG: Print out data that was sent */
	/*
	uint8 dbg_cntr;
	UART_Send_Data("\r\nNWK Sending:");
	if (encryption)
		UART_Send_Data(" (ENC):");
	for (dbg_cntr = 0; dbg_cntr < len; dbg_cntr++) {
		UART0_Send_ByteToChar(&packet[dbg_cntr]);
	}
	UART_Send_Data("\r\n");
	*/
	/* END OF DEBUG */

	// Send packet
	exit_code = Radio_Tx(packet, len, dest_address);
	if (exit_code)
		return exit_code;
	// DO NOT PUT ANYTHING THAT CAUSE DELAY FROM HERE TO ACK RECEIVING!!!

	// Receive ACK
	if (ack) {
		uint16_t timeout = ACK_WAIT_TIMEOUT;
		uint8_t cntr;

		for (cntr=RF_BUFFER_SIZE-1; cntr > 0; cntr--)
			RxPacket[cntr] = 0;

		// Recieve ACK
		exit_code = Radio_Rx(RxPacket, &len, timeout, &rssi_rx);

		// If exit_code is not 0 then return and cancel all processing
		if (exit_code)
			return exit_code;

		// Decrypt payload
		if (encryption) {
			// Skip first 4 bytes as they are LEN, DST, SRC, ENC
			Payload_Decrypt(RxPacket+4);
		}

		// Check if received packet type is ACK (ACK bit(!) in CTRL byte)
		if (RxPacket[PKT_CTRL_BYTE] & PKT_CTRL_ACK_REPLY) {

			// If packet type is ACK then check if received ACK byte itselt is ACK type
			if ((RxPacket[PKT_PAYLOAD_BYTE1]) != PKT_TYPE_ACK) {
				Radio_Set_Mode(RADIO_STANDBY);
				return ERR_NO_ACK;
			}
			_NOP();

		}

		/* DEBUG: Print out ACK */
		/*UART_Send_Data("\r\nNWK ACK:");
		for (cntr = 0; cntr < len; ++cntr) {
			UART0_Send_ByteToChar(&(RxPacket[cntr]));
		}*/
		/* END OF DEBUG */

		// If no ACK then repeat N times and then go to sleep
		Radio_Set_Mode(RADIO_STANDBY);
	}

	return exit_code;
}


// *************************************************************************************************
// @fn          Radio_Receive_Data
// @brief       Receive data in NWK layer using wireless radio.
// @param       uint8 *packet			Data packet (payload) that is being sent
//				uint8  length			Length of the data packet
//				uint16 timeout			Timeout in millisecons how long to wait until packet received
//				uint8 *rssi				Get the signal strength during packet receiving
// @return      uint8  exit_code		Return code after function exits
// *************************************************************************************************
uint8_t Radio_Receive_Data(uint8_t *packet, uint8_t *length, uint16_t timeout, uint8_t *rssi) {

	uint32_t timeout_ms = 0;		// Timeout in ms before ACK is sent
	uint32_t timeout_ack = SYSTEM_SPEED_MHZ*10*timeout_ms;
	uint8_t dest_address;
	uint8_t ack_status;				// Get the ACK status from the packet header
	uint8_t encryption;				// Get the encryption key from the packet header
	uint8_t exit_code = 0;			// Function exit code

	// Receive data
	exit_code = Radio_Rx(packet, length, timeout, rssi);

	// Get encryption byte, which is 4th byte in the received packet [LEN | DST | SRC | ENC | CTRL_BYTE | ...]
	encryption = packet[PKT_ENC_BYTE];

	// Decrypt the rest of the packet starting from next byte after encryption byte
	if (encryption)
		Payload_Decrypt(packet+PKT_ENC_BYTE+1);

	// Extract the source byte; source of the packet
	dest_address = packet[PKT_SRC_BYTE];

	// Check if we need to send ACK back
	ack_status = packet[PKT_CTRL_BYTE] & PKT_CTRL_ACK_REQ;

	// If ACK sending enabled
	if (ack_status) {
		// Clean Tx Packet array
		uint8_t cntr;
		for (cntr=RF_BUFFER_SIZE-1; cntr > 0; cntr--)
			TxPacket[cntr] = 0;

		packet_len = 0;		// Reset packet length

		// Modify CTRL byte and set ACK and ANSWER bit to high
		TxPacket[packet_len] = PKT_CTRL | PKT_CTRL_ACK_REPLY | PKT_CTRL_ANSWER;

		packet_len++;

		TxPacket[packet_len++] = PKT_TYPE_ACK;

		// If encryption enabled, change ENC byte to ON
		if (encryption)
			nwk_header[0] = PAYLOAD_ENC_ON;
		else
			nwk_header[0] = PAYLOAD_ENC_OFF;

		// Add security byte to the packet header and get new length of the packet
		packet_len = _Modify_Packet_Header(TxPacket, packet_len, nwk_header);

		// Encrypt payload
		// TODO: Encrypted packets are limited with 16 bytes currently. Change this to allow longer packets
		if (encryption) {
			packet_len = ENC_PACKET_SIZE+2;
			Payload_Encrypt(TxPacket+1);
		}

		// Add some delay because another side is not yet ready to receive ACK
		while (timeout_ack--) {
			_NOP();
		}

		// Send ACK
		exit_code = Radio_Tx(TxPacket, packet_len, dest_address);
	}
	// Switch back to receive mode
	Radio_Set_Mode(RADIO_RX);

	return exit_code;
}


// *************************************************************************************************
// @fn          _Modify_Packet_Header
// @brief       Add additional encryption header (1 byte) to the packet
// @param       uint8 *packet			Data packet (payload) that is being sent
//				uint8  packet_length	Size of the *packet (payload)
//				uint8  header[]			Additional header that will be added to the beginning of the packet
// @return      uint8 (new_length)		New length of the packet (payload)
// *************************************************************************************************
uint8_t _Modify_Packet_Header(uint8_t *packet, uint8_t packet_length, uint8_t header[]) {
	uint8_t var;
	uint8_t payload_offset = 1;						// Number of bytes to add to the packet => size of header
													// For some reason sizeof returns actual size increased by 1
	uint8_t preserve_first_bytes = 0;				// Number of bytes in the beginning of packet that are not allowed to move

	for (var=packet_length; var > preserve_first_bytes; var--) {
		packet[var+payload_offset-1] = packet[var-1];
	}
	packet[preserve_first_bytes+0] = header[0];

	return packet_length+payload_offset;
}
