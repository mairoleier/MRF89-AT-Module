#ifndef UART_H_
#define UART_H_

/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include "../../HAL/include/system.h"

/***************************************************************************************************
 *	        Define section					                       		   					       *
 ***************************************************************************************************/
#define MAX_UART_STR_LENGTH 	64		// UART max incoming string size


/******** UART GPIO *********/
// TX	P.2.0
#define UART_TX_PORT 		GPIO_PORT_P2
#define UART_TX_PIN 		GPIO_PIN0

// RX	P.2.1
#define UART_RX_PORT 		GPIO_PORT_P2
#define UART_RX_PIN 		GPIO_PIN1


/*** UART Base Address ***/
#define UART_BASE_ADDR 		EUSCI_A0_BASE


/***************************************************************************************************
 *	        Prototype section					                       							   *
 ***************************************************************************************************/
void UART_Init(void);

void UART_Send_Byte(unsigned char c);
void UART_Send_Data(const char *str);
void UART_Send_ByteToChar(unsigned char bytes[]);

void Clear_Uart_Data_Buf(void);
void Clear_Work_Data_Buf(void);

uint8_t UART_Receive_Data(uint8_t *uart_data_buf);
uint8_t Check_Uart_Answer(char answer[]);
uint8_t Check_Uart_Answer_Block(char answer[], uint8_t size);

/***************************************************************************************************
 *	        Global Variable section  				                            				   *
 ***************************************************************************************************/
uint8_t uart_buf_size;
char uart_rcv_buf[MAX_UART_STR_LENGTH];
char uart_data[MAX_UART_STR_LENGTH];
uint8_t data_rcv_flag;

char data_p[4][64];

#endif /* UART_H_ */
