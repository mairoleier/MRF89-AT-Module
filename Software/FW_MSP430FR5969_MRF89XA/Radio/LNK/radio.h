#ifndef RADIO_H_
#define RADIO_H_

/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include <stdint.h>
#include "../mrf89xa.h"


/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/

/*********************************************/
/*   START OF USER DEFINABLE RF SETTINGS     */
/*********************************************/

// Network address
#define DEF_NETWORK_ADDR1	1
#define DEF_NETWORK_ADDR2	2
#define DEF_NETWORK_ADDR3	3
#define DEF_NETWORK_ADDR4	4

// Node address
#define DEF_LOCAL_ADDR		10		// Default local address; can be changed dynamically


/* RF speed settings. Available options:
 * RF_DATA_RATE_2			Range +6dB
 * RF_DATA_RATE_10			Range -1dB
 * RF_DATA_RATE_33			Range -2dB
 * RF_DATA_RATE_100			Range -5.5dB  */
#define DEF_RF_DATA_RATE	RF_DATA_RATE_33


/* TX Power. Available options:
 * TXCON_N8DBM		-8dBm
 * TXCON_N5DBM		-5dBm
 * TXCON_N2DBM		-2dBm
 * TXCON_1DBM		+1dBm
 * TXCON_4DBM		+4dBm
 * TXCON_7DBM		+7dBm
 * TXCON_10DBM		+10dBm
 * TXCON_13DBM		+13dBm */
#define DEF_TX_POWER		TXCON_13DBM


/* RF Channel. Available options
 * 1		862 000 kHz
 * 2		862 400 kHz
 * 3		862 800 kHz
 * 4		863 200 kHz
 * 5		863 600 kHz
 * 6		864 000 kHz
 * 7		864 400 kHz
 * 8		864 800 kHz
 * 9		865 200 kHz
 * 10		865 600 kHz
 * 11		866 000 kHz
 * 12		866 400 kHz
 * 13		866 800 kHz
 * 14		867 200 kHz
 * 15		867 600 kHz
 * 16		868 000 kHz
 * 17		868 400 kHz
 * 18		868 800 kHz
 * 19		869 200 kHz
 * 20		869 600 kHz
 * 21		870 000 kHz */
#define DEF_RF_CHANNEL		11

/*********************************************/
/*     END OF USER DEFINABLE RF SETTINGS     */
/*********************************************/

#define RF_BUFFER_SIZE			64
#define PAYLOAD_MAX_SIZE		60
#define PAYLOAD_MAX_ENC_SIZE	16		//Encrypted payload can be up to 3x16 = 48 bytes but currently
										// automatic packet cutting is not yet implemented

/***** RF speed settings *****/
#define RF_DATA_RATE_2			2000
#define RF_DATA_RATE_10			10000
#define RF_DATA_RATE_33			33333
#define RF_DATA_RATE_100		100000


// Max RF Channel number
#define RF_CHANNEL_MAX_NUM		21


/***** Radio modes *****/
#define RADIO_STANDBY		1
#define RADIO_SLEEP			2
#define RADIO_RX			3


/***** LNK Layer RF ERROR CODES *****/
#define EXIT_NO_ERROR			0
#define ERR_RX_TIMEOUT			11		// Hex: 0x0B
#define ERR_TX_TIMEOUT			12		// Hex: 0x0C
#define ERR_RX_WRONG_LENGTH		14		// Hex: 0x0E
#define ERR_RF_FREQ_FIX_TIMEOUT	15		// Hex: 0x0F
//#define ERR_OUT_OF_MEMORY		16		// Hex: 0x10
#define ERR_UNKNOWN_CH_NO		30		// Hex: 0x1E
#define ERR_UNKNOWN_RF_SPEED	31		// Hex: 0x1F

/***** NWK Layer RF ERROR CODES *****/
#define ERR_RX_DATA_NOT_MATCH	17		// Hex: 0x11
#define ERR_NO_ACK				20		// Hex: 0x14
#define ERR_PAYLOAD_TOO_BIG		21		// Hex: 0x15

struct Rf_Error_Code_Def {
	uint8_t code;
	char *name;
};

static const struct Rf_Error_Code_Def rf_error_code[] = {
		/*  Code	Name		*/
		{	EXIT_NO_ERROR, 			"No error" },
		{	ERR_RX_TIMEOUT, 		"Rx timeout" },
		{	ERR_TX_TIMEOUT, 		"Tx timeout" },
		{	ERR_RX_WRONG_LENGTH,	"Rx data has wrong length" },
		{	ERR_RF_FREQ_FIX_TIMEOUT,"RF frequency fix timeout" },
		{	ERR_UNKNOWN_CH_NO,		"Unknown channel number" },
		{	ERR_UNKNOWN_RF_SPEED,	"Unknown RF speed" },
		{	ERR_RX_DATA_NOT_MATCH,	"Received data do not match" },
		{	ERR_NO_ACK,				"No ACK" },
		{	ERR_PAYLOAD_TOO_BIG,	"Payload too big" },
};


// normal struct
struct rf_struct
{
  uint8_t 	local_addr;
  uint8_t 	network_addr[4];
  uint8_t 	tx_power;
  uint8_t 	channel_no;
  uint8_t 	enc_key[16];
  uint32_t 	data_rate;
};

struct rf_struct rf_data;

/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/
uint8_t Radio_Init(uint32_t bps, uint8_t power, uint8_t channel);
uint8_t Radio_Tx(uint8_t packet[], uint8_t length, uint8_t destination);
uint8_t Radio_Rx(uint8_t packet[], uint8_t *length,  uint16_t timeout_ms, uint8_t *rssi);
uint8_t Radio_Set_Channel(uint8_t channel);
uint8_t Radio_Set_TxPower(uint8_t power);
uint8_t Radio_Set_Mode(uint8_t mode);

uint8_t Radio_Get_RSSI(void);
uint8_t Radio_Calculate_RSSI(uint8_t rssi);


/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/
uint8_t TxPacket[RF_BUFFER_SIZE];
uint8_t RxPacket[RF_BUFFER_SIZE];
uint8_t rssi_rx;
extern uint8_t exit_code;


#endif /* RADIO_H_ */
