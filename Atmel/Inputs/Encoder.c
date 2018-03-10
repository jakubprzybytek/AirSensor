/*
 * Encoder.c
 *
 * Created: 2015-09-15 22:36:42
 *  Author: Rodos
 */ 

#include <avr/io.h>

void Encoder_Init(TC1_t * TC) {
	// set up port pins
	PORTC.DIRCLR = PIN3_bm | PIN4_bm;
	PORTC.PIN3CTRL = PORT_ISC_LEVEL_gc | PORT_OPC_PULLUP_gc;
	PORTC.PIN4CTRL = PORT_ISC_LEVEL_gc | PORT_OPC_PULLUP_gc;
	
	// set up event channel
	EVSYS.CH0MUX = EVSYS_CHMUX_PORTC_PIN3_gc;
	EVSYS.CH0CTRL = EVSYS_QDEN_bm |	EVSYS_DIGFILT_8SAMPLES_gc;

	// set up counter
	TC->INTCTRLA = TC_OVFINTLVL_LO_gc;
	TC->INTCTRLB = TC_CCAINTLVL_LO_gc;
	TC->CTRLA = TC_CLKSEL_EVCH0_gc;
	TC->CTRLD = TC_EVACT_QDEC_gc | TC_EVSEL_CH0_gc;
	TC->CCA = 5;
	TC->CNT = 8;
	TC->PER = 11;
}