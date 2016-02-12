/***************************************************************************************************
 *	        Include section					                       		   					       *
 ***************************************************************************************************/
#include "include/system.h"
#include "include/timer.h"
#include "include/uart.h"

#include "../Radio/drivers/rf_spi.h"
#include "../Radio/LNK/radio.h"

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


// *************************************************************************************************
// @fn          Init_System
// @brief       Initialize system
// @param       none
// @return      none
// *************************************************************************************************
uint8_t System_Init(void) {
	exit_code = 0;

	// Stop watchdog timer
	WDT_A_hold(WDT_A_BASE);

	// Disable the GPIO power-on default high-impedance mode to activate previously configured port settings
	//PMM_unlockLPM5();		// Driverlib version gives a warning
	PM5CTL0 &= ~LOCKLPM5;

	// Configure CPU clock
	System_Set_Speed(SYSTEM_SPEED_MHZ);


	// Ports
	LED1_PORT_DIR |=  (LED1);				// Set LED1 pin as output
	LED1_PORT_OUT &= ~(LED1);				// Set LED1 pin to LOW

	LED2_PORT_DIR |=  (LED2);				// Set LED2 pin as output
	LED2_PORT_OUT &= ~(LED2);				// Set LED2 pin to LOW

	RF_RESET_OUT();							// Set RF module reset pin as output

	// Communication
	UART_Init();		//
	RF_SPI_Init();			// RF module interface

	// RF Module initial configruation
	if (exit_code = Radio_Init(rf_data.data_rate, rf_data.tx_power, rf_data.channel_no))		// Initialize RF module with 2kbps speed
		return exit_code;

	// Set RF module channel
	if (exit_code = Radio_Set_Channel(rf_data.channel_no))
		return exit_code;

	__enable_interrupt();

	return EXIT_NO_ERROR;
}


// *************************************************************************************************
// @fn          System_Set_Speed
// @brief       Initialize CPU with selected speed
// @param       uint8 mhz		MCU speed in MHz. Available options:
//									SYSTEM_SPEED_1MHZ
//									SYSTEM_SPEED_8MHZ
//									SYSTEM_SPEED_12MHZ
//									SYSTEM_SPEED_16MHZ
// @return      none
// *************************************************************************************************
void System_Set_Speed(uint8_t mhz) {
	//Info: MSP430FR5696 family guide p.93
	CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_6);

	//Set ACLK=VLOCLK
	CS_initClockSignal(CS_ACLK,CS_VLOCLK_SELECT,CS_CLOCK_DIVIDER_1);

	//Set SMCLK = DCO with frequency divider of 1
	CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);

	//Set MCLK = DCO with frequency divider of 1
	CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
}


// *************************************************************************************************
// @fn          cByteToHex
// @brief       Convert byte to char
// @param
// @return      uint8 result            Outputs char
// *************************************************************************************************
int cByteToHex(char input) {
  char result = 0;
  switch (input) {
    case 0x00:  result = '0';   break;
    case 0x01:  result = '1';   break;
    case 0x02:  result = '2';   break;
    case 0x03:  result = '3';   break;
    case 0x04:  result = '4';   break;
    case 0x05:  result = '5';   break;
    case 0x06:  result = '6';   break;
    case 0x07:  result = '7';   break;
    case 0x08:  result = '8';   break;
    case 0x09:  result = '9';   break;

    case 0x0A:  result = 'A';   break;
    case 0x0B:  result = 'B';   break;
    case 0x0C:  result = 'C';   break;
    case 0x0D:  result = 'D';   break;
    case 0x0E:  result = 'E';   break;
    case 0x0F:  result = 'F';   break;
  }
  return result;
}



int Trans_Int(char c)
{
	switch (c)
	{
	case '0': 	return 0;
	case '1': 	return 1;
	case '2': 	return 2;
	case '3': 	return 3;
	case '4': 	return 4;
	case '5': 	return 5;
	case '6': 	return 6;
	case '7': 	return 7;
	case '8': 	return 8;
	case '9': 	return 9;
	case '\r': 	return 10;
	default: 	return -1;
	}
}


int String_To_Int (char *std)  //std is a pointer to a string
{
	int i;
	int aa;     //length of string excluding trailing \0
	int bb;     //intermediate value
	int cc = 0;    //intermediate addend
	int ee;     //10s value for multiplier
	aa = strlen(std);  //returns
	for (i = 0; i < aa; i++)
	{
		switch (i)        //assumes string is 4 or less digits
		{
		case 3: {ee = 1000; break;}
		case 2: {ee = 100; break;}
		case 1: {ee = 10; break;}
		case 0: {ee = 1; break;}
		}
		bb = Trans_Int(std[aa-1-i]) * ee;
		cc = cc + bb;
	}
	return cc;
}


// Convert ASCII to integer
int atoi(const char *s) {
  int a; // Accumulator
  char c; // Current character
  int  d; // Current digit
  int i; // Array index

  i = 0; // Init array index
  a = 0; // Init accumulator to zero.
  c = s[i]; // Get first character
  i++; //Advance the index

  // Convert while not null terminator and is a numeric character
  while((c!=0)&&(c>='0')&&(c<='9'))
  {
    d = c - '0'; // Convert character to digit
    a = a * 10;  // Make room for digit
    a = a + d; // Add digit to accumulator
    c = s[i]; // Get next character
    i++; //Advance the index
  }
  return(a);
}

// Convert integer to ASCII
void itoa(long unsigned int value, char* result, int base)
    {
      // check that the base if valid
      if (base < 2 || base > 36) { *result = '\0';}

      char* ptr = result, *ptr1 = result, tmp_char;
      int tmp_value;

      do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
      } while ( value );

      // Apply negative sign
      if (tmp_value < 0) *ptr++ = '-';
      *ptr-- = '\0';
      while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
      }

    }

// Convert uint8 to byte
uint32_t UIntToBCD (uint16_t UIntValue) {
  uint32_t converted;
  uint8_t i;
  converted = 0;
  for (i = 0; i < 16; ++i) {
    converted = __bcd_add_long (converted , converted );
    if (( UIntValue & BITF) != 0) {
      converted = __bcd_add_long (converted , 1);
    }
    UIntValue <<= 1;
  }
  return converted;
}



// *************************************************************************************************
// @fn          System_Reset
// @brief       Cause the sysem POR reset
// @param		None
// @return      None
// *************************************************************************************************
void System_Reset(void) {
	// Violation of the WDT password
	WDTCTL = 0xDEAD;

	// Alternative workarounds
	//PMMCTL0 = PMMPW | PMMSWBOR;   // This triggers a Software BOR
	//PMMCTL0 = PMMPW | PMMSWPOR;   // This triggers a Software POR
}


