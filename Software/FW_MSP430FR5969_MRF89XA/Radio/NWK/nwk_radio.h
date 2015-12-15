/*
 * nwk_radio.h
 *
 *  Created on: 20.04.2015
 *      Author: mairo
 */

#ifndef NWK_NWK_RADIO_H_
#define NWK_NWK_RADIO_H_

/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include <stdint.h>

/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/

/**** PACKET DATA TYPES ***/
#define PKT_TYPE_ACK			0xFF


/*** PACKET CONTROL  ***/
#define PKT_CTRL				0			// Control byte type
#define PKT_CTRL_RETRY			(1<<0)		// 0 - first try; 1 - retry
#define PKT_CTRL_ACK_REPLY		(1<<1)		// 0 - packet is not ACK; 1 - packet is ACK
#define PKT_CTRL_ACK_REQ		(1<<2)		// 0 - no ACK required; 1 - ACK required
#define PKT_CTRL_REQUEST		(0<<3)		// 0 - request; 1 - answer
#define PKT_CTRL_ANSWER			(1<<3)		// 0 - request; 1 - answer
#define PKT_CTRL_HOPS			(2<<4)		// Number of allowed hops
#define PKT_CTRL_DEV_TYPE		(2<<6)		// Device type



/***** ACK DEFINITIONS ****/
#define PKT_ACK_ON				1
#define PKT_ACK_OFF				0


#define NWK_HEADER_SIZE			1
#define ENC_PACKET_SIZE			16

#define PKT_SRC_BYTE			2		// Source byte is third byte in the received packet
#define PKT_ENC_BYTE			3		// Encryption byte is 4th byte in the received packet
#define PKT_CTRL_BYTE			4		// Control byte defines
										// BIT1		PKT_CTRL_RETRY: 	0 - first try; 1 - retry
										// BIT2		Not yet defined
										// BIT3		PKT_CTRL_ACK_REQ: 	0 - no ACK required; 1 - ACK required
										// BIT4		PKT_CTRL_REQUEST	0 - request
										//			PKT_CTRL_ANSWER		1 - answer
										// BIT5-6	PKT_CTRL_HOPS		// Number of allowed hops
										// BIT7-8	Net yet defined

#define PKT_PAYLOAD_BYTE1		5		// First byte of the payload after CTRL byte

#define ACK_WAIT_TIMEOUT		500		// Faster network speed works with 100ms quite well, in case of ERROR, increase timeout value
										// For long packets (up to 64 bytes) a timeout up to 1000ms may be needed

/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/
uint8_t Radio_Send_Data(uint8_t *packet, uint8_t len, uint8_t dest_address, uint8_t encryption, uint8_t ack);
uint8_t Radio_Receive_Data(uint8_t *packet, uint8_t *length, uint16_t timeout, uint8_t *rssi);


/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/
uint8_t packet_len;


#endif 
