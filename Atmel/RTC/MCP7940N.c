/*
 * MCP7940N.c
 *
 * Created: 2015-10-08 17:51:34
 *  Author: Rodos
 */ 

#include "../LUFA/Drivers/Peripheral/TWI.h"
#include "RTC.h"

#define BCD_TO_INT(x) ((x & 0x0f) + ((x & 0xf0) >> 4) * 10)

#define MCP7940N_SLAVE_ADDRESS		0b11011110
#define MCP7940N_TWI_TIMEOUT		10

uint8_t mcp7940nTimeAddress = 0x00;

uint8_t mcp7940nBuffer[7];

void MCP7940N_ReadTime (TWI_t * twiname, DateTime * dateTime) {
	dateTime->mcp7940nResult = TWI_ReadPacket(twiname, MCP7940N_SLAVE_ADDRESS, MCP7940N_TWI_TIMEOUT, &mcp7940nTimeAddress, 1, mcp7940nBuffer, 7);

	if (dateTime->mcp7940nResult == TWI_ERROR_NoError) {
		dateTime->second = BCD_TO_INT(mcp7940nBuffer[0] & 0b01111111);
		dateTime->minute = BCD_TO_INT(mcp7940nBuffer[1]);
		dateTime->hour = BCD_TO_INT(mcp7940nBuffer[2]);
		dateTime->day = BCD_TO_INT(mcp7940nBuffer[4]);
		dateTime->month = BCD_TO_INT(mcp7940nBuffer[5] & 0b00011111);
		dateTime->year = BCD_TO_INT(mcp7940nBuffer[6]);
	}
}

void MCP7940N_WriteTime (TWI_t * twiname, DateTime * dateTime) {
	uint8_t toWrite = 0b10100010;
	dateTime->mcp7940nResult = TWI_WritePacket(twiname, MCP7940N_SLAVE_ADDRESS, MCP7940N_TWI_TIMEOUT, &mcp7940nTimeAddress, 1, &toWrite, 1);
}