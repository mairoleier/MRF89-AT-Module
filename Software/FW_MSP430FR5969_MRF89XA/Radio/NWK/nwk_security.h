/*
 * nwk_security.h
 *
 *  Created on: 09.04.2015
 *      Author: mairo
 */

#ifndef NWK_NWK_SECURITY_H_
#define NWK_NWK_SECURITY_H_

/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include <stdint.h>

/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/
#define PAYLOAD_ENC_ON		1
#define PAYLOAD_ENC_OFF		0

/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/
uint8_t Payload_Encrypt(uint8_t *msg);
uint8_t Payload_Decrypt(uint8_t *msg);

/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/

#endif 
