/*
 * Inputs.h
 *
 * Created: 2015-09-21 11:37:48
 *  Author: Rodos
 */ 


#ifndef INPUTS_H_
#define INPUTS_H_

#define EVENTS_NO_ACTION 0x00

#define EVENTS_PRESSED_SWITCH_1 0x01
#define EVENTS_PRESSED_SWITCH_2 0x02
#define EVENTS_ENCODER_LEFT 0x03
#define EVENTS_ENCODER_RIGHT 0x04

#define EVENTS_TEMPERATURE_UPDATED 0x11
#define EVENTS_HUMIDITY_UPDATED 0x12
#define EVENTS_PREASSURE_UPDATED 0x13

#define EVENTS_BATT_VOLTAGE_READ 0x20
#define EVENTS_VCC_VOLTAGE_READ 0x21

void Events_SetStatus(uint8_t newAction);

uint8_t Events_GetStatus();

#endif /* INPUTS_H_ */
