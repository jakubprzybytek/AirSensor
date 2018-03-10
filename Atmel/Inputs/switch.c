/*
 * switch.c
 *
 * Created: 2015-09-15 17:46:55
 *  Author: Rodos
 */ 

#include <avr/io.h>

void Switches_Init() {
	PORTE.DIRCLR = PIN4_bm | PIN5_bm;
	PORTE.PIN4CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTE.PIN5CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTE.INT0MASK = PIN4_bm;
	PORTE.INT1MASK = PIN5_bm;
	PORTE.INTCTRL = PORT_INT0LVL_LO_gc | PORT_INT1LVL_LO_gc;
}