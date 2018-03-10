/*
 * GUI.h
 *
 * Created: 2015-06-10 21:27:02
 *  Author: Rodos
 */ 


#ifndef GUI_H_
#define GUI_H_

#include <stdbool.h>

char buffer[22];

void GUI_Init();

void GUI_Shutdown();

void GUI_Clean();

/*
void GUI_print(char* text, unsigned char x, unsigned char y, unsigned char pattern, const FONT_INFO *font);

void GUI_print2(char* text, unsigned char x, unsigned char y, unsigned char pattern, const FONT_INFO *font);
*/
void GUI_print3(char* text, unsigned char x, unsigned char y, unsigned char pattern);

#endif /* GUI_H_ */