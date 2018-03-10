/*
 * twi.c
 *
 * Created: 2015-09-21 21:46:38
 *  Author: Rodos
 */ 
#include <avr/io.h>

#include "twi.h"

char* twiErrorToA(uint8_t twiError) {
	switch (twiError) {
		case TWI_ERROR_BusFault:
		return "Bus fault";
		case TWI_ERROR_BusCaptureTimeout:
		return "Bus timeout";
		case TWI_ERROR_SlaveResponseTimeout:
		return "Slave timeout";
		case TWI_ERROR_SlaveNotReady:
		return "Slave not ready";
		case TWI_ERROR_SlaveNAK:
		return "Slave NAK";
	}
	return "No error";
}
