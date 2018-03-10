/*
 * oledDriver.h
 *
 * Created: 2015-06-07 12:16:43
 *  Author: Rodos
 */ 
#define F_CPU 2000000UL

#include <avr/io.h>
#include <util/delay.h>

#ifndef OLEDDRIVER_H_
#define OLEDDRIVER_H_

#define OLED_OUT PORTA.OUT

#define OLED_INIT_PORTS PORTB.DIRSET = PIN0_bm | PIN4_bm; PORTF.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN6_bm;

#define OLED_DIGITAL_ON		PORTF.OUTCLR = PIN6_bm;
#define OLED_DIGITAL_OFF	PORTF.OUTSET = PIN6_bm;

#define OLED_ANALOG_ON		PORTB.OUTCLR = PIN4_bm;
#define OLED_ANALOG_OFF		PORTB.OUTSET = PIN4_bm;

#define OLED_RESET_HIGH		PORTF.OUTSET = PIN0_bm;
#define OLED_RESET_LOW		PORTF.OUTCLR = PIN0_bm;

#define OLED_W_HIGH			PORTF.OUTSET = PIN1_bm;
#define OLED_W_LOW			PORTF.OUTCLR = PIN1_bm;

#define OLED_DATA			PORTF.OUTSET = PIN2_bm;
#define OLED_COMMAND		PORTF.OUTCLR = PIN2_bm;

#define OLED_CHIP_DISABLE	PORTB.OUTSET = PIN0_bm;
#define OLED_CHIP_SELECT	PORTB.OUTCLR = PIN0_bm;

#define OLED_RD_HIGH		PORTF.OUTSET = PIN3_bm;
#define OLED_RD_LOW			PORTF.OUTCLR = PIN3_bm;

#define OLED_READ			OLED_RD_LOW OLED_RD_HIGH;
#define OLED_WRITE			OLED_W_LOW OLED_W_HIGH;

void OLED_Set_Remap_Format(unsigned char d);

void OLED_Set_Start_Line(unsigned char d);

void OLED_Set_Display_Offset(unsigned char d);

void OLED_Set_Display_Mode(unsigned char d);

void OLED_Set_Multiplex_Ratio(unsigned char d);

void OLED_Set_Master_Config(unsigned char d);

void OLED_Set_Display_On_Off(unsigned char d);

void OLED_Set_Gray_Scale_Table();

void OLED_Set_Column_Address(unsigned char a, unsigned char b);

void OLED_Set_Row_Address(unsigned char a, unsigned char b);

void OLED_Draw_Rectangle(unsigned char startCol, unsigned char startRow, unsigned char endCol, unsigned char endRow, unsigned char pattern);

#endif /* OLEDDRIVER_H_ */