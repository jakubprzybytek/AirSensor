/*
 * BattVoltage.h
 *
 * Created: 2015-10-02 22:05:53
 *  Author: Rodos
 */ 


#ifndef BATTVOLTAGE_H_
#define BATTVOLTAGE_H_

void BattVoltage_Init(ADC_t * advVcc, ADC_t * adcBatt, TC0_t * TC);

long BattVoltage_ReadBatt(ADC_t * adcBatt);
long BattVoltage_ReadVcc(ADC_t * advVcc);

void BattVoltage_Enable();
void BattVoltage_Disable();

#endif /* BATTVOLTAGE_H_ */