#ifndef AT_CMD_DEFINITIONS_H_
#define AT_CMD_DEFINITIONS_H_


/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/


/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/
// Command definitions for radio modes
#define RF_MODE_RX			1
#define RF_MODE_STANDBY		2
#define RF_MODE_SLEEP		3
#define RF_MODE_OFF			4

/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/


/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/
extern char CMD_AT[];
extern char CMD_AT_RST[];
extern char CMD_AT_VER[];
extern char CMD_AT_RFSEND[];
extern char CMD_AT_RFMODE[];
extern char CMD_AT_RFLADDR[];
extern char CMD_AT_RFNADDR[];
extern char CMD_AT_RFPOWER[];
extern char CMD_AT_RFCHANNEL[];
extern char CMD_AT_RFDRATE[];

extern char ANSW_SEND_OK[];
extern char ANSW_OK[];
extern char ANSW_ERROR[];
extern char ANSW_ERROR_UNK_CMD[];




#endif /* AT_CMD_DEFINITIONS_H_ */
