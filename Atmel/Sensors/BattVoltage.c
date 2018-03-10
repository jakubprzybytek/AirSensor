/*
 * BattVoltage.c
 *
 * Created: 2015-10-02 22:06:03
 *  Author: Rodos
 */ 
#include <avr/io.h>

void BattVoltage_Init(ADC_t * adcVcc, ADC_t * adcBatt, TC0_t * tc) {
	PORTB.DIRCLR = PIN5_bm;

	adcVcc->CTRLA = ADC_ENABLE_bm;
	adcVcc->CTRLB = ADC_RESOLUTION_12BIT_gc;
	adcVcc->PRESCALER = ADC_PRESCALER_DIV8_gc;
	adcVcc->REFCTRL = ADC_REFSEL_INTVCC2_gc | ADC_BANDGAP_bm;
	adcVcc->EVCTRL = ADC_EVSEL_1234_gc | ADC_EVACT_CH0_gc;

	adcVcc->CH0.CTRL = ADC_CH_INPUTMODE_INTERNAL_gc;
	adcVcc->CH0.INTCTRL =  ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc;
	adcVcc->CH0.MUXCTRL = ADC_CH_MUXINT_BANDGAP_gc;
	//adcVcc->CH0.MUXCTRL = ADC_CH_MUXINT_SCALEDVCC_gc;

	adcBatt->CTRLA = ADC_ENABLE_bm;
	adcBatt->CTRLB = ADC_RESOLUTION_12BIT_gc;
	adcBatt->PRESCALER = ADC_PRESCALER_DIV8_gc;
	adcBatt->REFCTRL = ADC_REFSEL_INT1V_gc | ADC_BANDGAP_bm;
	adcBatt->EVCTRL = ADC_EVSEL_1234_gc | ADC_EVACT_CH0_gc;

	adcBatt->CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;
	adcBatt->CH0.INTCTRL =  ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc;
	adcBatt->CH0.MUXCTRL = ADC_CH_MUXPOS_PIN5_gc;

	EVSYS.CH1MUX = EVSYS_CHMUX_TCD0_OVF_gc;

	tc->CTRLB = TC_WGMODE_NORMAL_gc;
	tc->INTCTRLB = TC_CCAINTLVL_LO_gc;
	tc->CCA = 3998;
	tc->PER = 4000; // 1000ms
	tc->CTRLA = TC_CLKSEL_DIV1024_gc;
}

void BattVoltage_Enable() {
	PORTB.OUTSET = PIN6_bm;
}

void BattVoltage_Disable() {
	PORTB.OUTCLR = PIN6_bm;
}

long BattVoltage_ReadBatt(ADC_t * adcBatt) {
	long temp = adcBatt->CH0.RES;
	return temp * 10 / 76;
}

long BattVoltage_ReadVcc(ADC_t * advVcc) {
	long temp = advVcc->CH0.RES;
	return 901120 / temp ;
}
