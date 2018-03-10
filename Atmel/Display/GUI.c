/*
 * GUI.c
 *
 * Created: 2015-06-10 21:08:21
 *  Author: Rodos
 */ 

#include <string.h>

#include "GUI.h"
#include "OledDriver.h"

#include "../Font/font.h"

void GUI_Init() {
	OLED_INIT_PORTS

	OLED_RD_HIGH
	OLED_RESET_HIGH
	OLED_CHIP_SELECT

	OLED_DIGITAL_ON
	_delay_ms(1);

	OLED_RESET_LOW
	_delay_ms(1);

	OLED_RESET_HIGH
	OLED_ANALOG_ON
	_delay_ms(1);

	OLED_COMMAND

	OLED_Set_Display_On_Off(0x00);

	OLED_Set_Master_Config(0x00);
	//OLED_Set_Gray_Scale_Table();
	OLED_Set_Remap_Format(0x52);
	OLED_Set_Multiplex_Ratio(64);
	OLED_Set_Start_Line(0);
	OLED_Set_Display_Offset(0);
		
	OLED_Set_Display_On_Off(0x01);

	GUI_Clean();
}

void GUI_Shutdown() {
	OLED_Set_Display_On_Off(0x00);

	OLED_ANALOG_OFF
	_delay_ms(100);

	OLED_DIGITAL_OFF
}

void GUI_Clean() {
	OLED_Draw_Rectangle(0, 0, 63, 79, 0x00);
}

/*
unsigned int lengthInPixels(char* text, FONT_INFO *font) {
	unsigned char length = 0;
	for (unsigned char i = 0; i < strlen(text); i++) {
		if (text[i] == ' ') {
			length += font->spaceWidth;
		} else {
			unsigned char charWidth = font->descriptors[text[i] - font->firstChar].width;
			length += charWidth;
			if (charWidth % 2) {
				length++;
			}
		}
	}
	return length;
}

void GUI_print(char* text, unsigned char x, unsigned char y, unsigned char pattern, FONT_INFO *font) {
	OLED_Set_Column_Address(x, x + (lengthInPixels(text, font) + 1) / 2 - 1);
	OLED_Set_Row_Address(y, y + font->heightInBits - 1);

	OLED_DATA
	
	for (unsigned int j = 0; j < font->heightInBits; j++) {

		for (unsigned char k = 0; k < strlen(text); k++) {

			if (text[k] == ' ') { // space
				unsigned char charWidth = font->spaceWidth;

				for (unsigned char i = 0; i < (charWidth + 1) / 2; i++) {
					OLED_OUT = 0x00; OLED_WRITE
				}

			} else { // any other char

				unsigned char charIndex = text[k] - font->firstChar;

				unsigned char charWidth = font->descriptors[charIndex].width;
				unsigned char charWidthInBytes = (charWidth + 7) / 8;
				unsigned int charOffset = font->descriptors[charIndex].offset;

				unsigned char charByte = font->bitmaps[charOffset + j * charWidthInBytes];
				for (unsigned char i = 0; i < (charWidth + 1) / 2; i++) {
					unsigned char output = ((charByte & 0x80) ? 0xf0 : 0x00) | ((charByte & 0x40) ? 0x0f : 0x00);
					OLED_OUT = output & pattern; OLED_WRITE
					if (i % 4 == 3) {
						charByte = font->bitmaps[++charOffset + j * charWidthInBytes];
					} else {
						charByte <<= 2;
					}
				}
			}
		}
	}
}

unsigned char buffer[64][11];

void GUI_print2(char* text, unsigned char x, unsigned char y, unsigned char pattern, FONT_INFO *font) {

	unsigned char textLengthInBiPixels = 0;
	
	for (unsigned char j = 0; j < font->heightInBits; j++) {

		unsigned char bufferIndex = 0;

		for (unsigned char k = 0; k < strlen(text); k++) {

			if (text[k] == ' ') { // space
				unsigned char charWidth = font->spaceWidth;

				for (unsigned char i = 0; i < (charWidth + 1) / 2; i++) {
					buffer[bufferIndex++][j] = 0x00;
				}

			} else { // any other char

				unsigned char charIndex = text[k] - font->firstChar;

				unsigned char charWidth = font->descriptors[charIndex].width;
				unsigned char charWidthInBytes = (charWidth + 7) / 8;
				unsigned int charOffset = font->descriptors[charIndex].offset;

				unsigned char charByte = font->bitmaps[charOffset + j * charWidthInBytes];
				
				for (unsigned char i = 0; i < (charWidth + 1) / 2; i++) {

					unsigned char output = ((charByte & 0x80) ? 0xf0 : 0x00) | ((charByte & 0x40) ? 0x0f : 0x00);
					buffer[bufferIndex++][j] = output & pattern;

					if (i % 4 == 3) {
						charByte = font->bitmaps[++charOffset + j * charWidthInBytes];
					} else {
						charByte <<= 2;
					}
				}
			}
		}

		textLengthInBiPixels = bufferIndex;
	}

	OLED_Set_Remap_Format(0x52);

	OLED_Set_Column_Address(x, x + textLengthInBiPixels - 1);
	OLED_Set_Row_Address(y, y + font->heightInBits - 1);

	OLED_DATA

	for (unsigned int m = 0; m < font->heightInBits; m++) {
		for (unsigned char l = 0; l < textLengthInBiPixels; l++) {
			OLED_OUT = buffer[l][m]; OLED_WRITE
		}
	}
}
*/
void GUI_print3(char* text, unsigned char x, unsigned char y, unsigned char pattern) {
	
	uint8_t strLen = strlen(text);
	
	OLED_Set_Remap_Format(0x56);

	OLED_Set_Column_Address(x, x + strLen * 3 - 1);
	OLED_Set_Row_Address(y, y + 6);

	OLED_DATA
	
	unsigned char left, right;
	
	for (uint8_t i = 0; i < strLen; i++)	{
		
		unsigned int charIndex = (text[i] - ' ') * 5;
		
		for (uint8_t k = 0; k < 3; k++) {
		
			left = font_5x7_data[charIndex++];
			right = k < 2 ? font_5x7_data[charIndex++] : 0;
		
			for (uint8_t j = 0; j < 7; j++) {
				unsigned char output = ((left & 0x01) ? 0xf0 : 0x00) | ((right & 0x01) ? 0x0f : 0x00);
				OLED_OUT = output & pattern;
				OLED_WRITE
			
				right = right >> 1;
				left = left >> 1;
			}
		}
	}
	
}