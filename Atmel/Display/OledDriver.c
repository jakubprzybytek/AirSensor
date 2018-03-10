/*
 * OledDriver.c
 *
 * Created: 2015-06-10 21:24:13
 *  Author: Rodos
 */ 
#define F_CPU 2000000UL

#include <util/delay.h>

#include "OledDriver.h"

void OLED_Set_Remap_Format(unsigned char d)
{
	OLED_COMMAND
	OLED_OUT = 0xA0; OLED_WRITE
	OLED_OUT = d; OLED_WRITE
}

void OLED_Set_Start_Line(unsigned char d)
{
	OLED_COMMAND
	OLED_OUT = 0xA1; OLED_WRITE
	OLED_OUT = d; OLED_WRITE
}

void OLED_Set_Display_Offset(unsigned char d)
{
	OLED_COMMAND
	OLED_OUT = 0xA2; OLED_WRITE
	OLED_OUT = d; OLED_WRITE
}

void OLED_Set_Display_Mode(unsigned char d)
{
	OLED_COMMAND
	OLED_OUT = 0xA4 | d; OLED_WRITE
	_delay_ms(1);
}

void OLED_Set_Multiplex_Ratio(unsigned char d)
{
	OLED_COMMAND
	OLED_OUT = 0xA8 | d; OLED_WRITE
	_delay_ms(1);
}

void OLED_Set_Master_Config(unsigned char d)
{
	OLED_COMMAND
	OLED_OUT = 0xad; OLED_WRITE
	_delay_ms(1);
	OLED_OUT = 0x02 | d; OLED_WRITE
	_delay_ms(1);
}

void OLED_Set_Display_On_Off(unsigned char d)
{
	OLED_COMMAND
	OLED_OUT = 0xAE | d; OLED_WRITE
	_delay_ms(100);
}

void OLED_Set_Gray_Scale_Table()
{
	OLED_COMMAND
	OLED_OUT = 0xB8; OLED_WRITE
	OLED_OUT = 0x01; OLED_WRITE
	OLED_OUT = 0x11; OLED_WRITE
	OLED_OUT = 0x22; OLED_WRITE
	OLED_OUT = 0x32; OLED_WRITE
	OLED_OUT = 0x43; OLED_WRITE
	OLED_OUT = 0x54; OLED_WRITE
	OLED_OUT = 0x65; OLED_WRITE
	OLED_OUT = 0x76; OLED_WRITE
}

void OLED_Set_Column_Address(unsigned char a, unsigned char b)
{
	OLED_COMMAND
	OLED_OUT = 0x15; OLED_WRITE
	OLED_OUT = a; OLED_WRITE
	OLED_OUT = b; OLED_WRITE
}

void OLED_Set_Row_Address(unsigned char a, unsigned char b)
{
	OLED_COMMAND
	OLED_OUT = 0x75; OLED_WRITE
	OLED_OUT = a + 4; OLED_WRITE
	OLED_OUT = b + 4; OLED_WRITE
}

void OLED_Draw_Rectangle(unsigned char startCol, unsigned char startRow, unsigned char endCol, unsigned char endRow, unsigned char pattern)
{
	OLED_COMMAND
	OLED_OUT = 0x24; OLED_WRITE
	OLED_OUT = startCol; OLED_WRITE
	OLED_OUT = startRow; OLED_WRITE
	OLED_OUT = endCol; OLED_WRITE
	OLED_OUT = endRow; OLED_WRITE
	OLED_OUT = pattern; OLED_WRITE
	
	_delay_us(200);
}