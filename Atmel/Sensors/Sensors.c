/*
 * Sensors.c
 *
 * Created: 2015-09-15 22:03:48
 *  Author: Rodos
 */ 

#include <avr/io.h>

 void Sensors_InitTimer(TC0_t * TC) {
	// set up timer for sensors
	TC->INTCTRLA = TC_OVFINTLVL_HI_gc;
	TC->INTCTRLB = TC_CCAINTLVL_HI_gc | TC_CCBINTLVL_HI_gc;
	TC->CTRLB = TC_WGMODE_NORMAL_gc;
	TC->CCA = 1200; // ~700ms
	TC->CCB = 1400; // ~800ms
	TC->PER = 2000; // overflow interruption every ~1000ms
	TC->CTRLA = TC_CLKSEL_DIV1024_gc;
 }