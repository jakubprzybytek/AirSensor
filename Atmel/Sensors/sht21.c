/*
 * sht21.c
 *
 * Created: 2015-08-15 12:41:06
 *  Author: Rodos
 */

#include <avr/io.h>
#include <util/delay.h>

#include "../LUFA/Drivers/Peripheral/TWI.h"

#define SHT21_SLAVE_ADDRESS	0b10000000
#define SHT21_TWI_TIMEOUT		10

uint8_t sht21NoHoldTemperatureAddress = 0b11110011;
uint8_t sht21NoHoldHumidityAddress = 0b11110101;

uint8_t sht21DataBuffer[3];

int Sht21_countTemperature(unsigned int raw) {
	return (175.72 * raw) / 655.36 - 4685.0;
}

int Sht21_countHumidity(unsigned int raw) {
	return (125.0 * raw) / 6553.6 - 60.0;
}

uint8_t Sht21_Init (TWI_t * twiname) {
	
	uint8_t userRegAddr = 0b11100111;
	uint8_t data;
	
	return TWI_ReadPacket(twiname, SHT21_SLAVE_ADDRESS, SHT21_TWI_TIMEOUT, &userRegAddr, 1, &data, 1);
}

uint8_t Sht21_StartTemperatureMeasurement (TWI_t *twiname) {
	return TWI_WritePacket(twiname, SHT21_SLAVE_ADDRESS, SHT21_TWI_TIMEOUT, &sht21NoHoldTemperatureAddress, 1, NULL, 0);
}

uint8_t Sht21_ReadTemperatureMeasurement (TWI_t *twiname, int *temperature) {

	uint8_t status;
	unsigned int rawTemperature;

	status = TWI_ReadPacket(twiname, SHT21_SLAVE_ADDRESS, SHT21_TWI_TIMEOUT, NULL, 0, sht21DataBuffer, 3);

	if (status != TWI_ERROR_NoError) {
		return status;
	}

	rawTemperature = ((sht21DataBuffer[0] << 8) | sht21DataBuffer[1]) & 0b1111111111111100;
	*temperature = Sht21_countTemperature(rawTemperature);

	return status;
}

uint8_t Sht21_StartHumidityMeasurement (TWI_t *twiname) {
	return TWI_WritePacket(twiname, SHT21_SLAVE_ADDRESS, SHT21_TWI_TIMEOUT, &sht21NoHoldTemperatureAddress, 1, NULL, 0);
}

uint8_t Sht21_ReadHumidityMeasurement (TWI_t *twiname, int *humidity) {

	uint8_t status;
	unsigned int rawHumidity;
	
	status = TWI_ReadPacket(twiname, SHT21_SLAVE_ADDRESS, SHT21_TWI_TIMEOUT, NULL, 0, sht21DataBuffer, 3);

	if (status != TWI_ERROR_NoError) {
		return status;
	}
	
	rawHumidity = ((sht21DataBuffer[0] << 8) | sht21DataBuffer[1]) & 0b1111111111111100;
	*humidity = Sht21_countHumidity(rawHumidity);
	
	return status;
}

uint8_t Sht21_ReadMeasurements (TWI_t *twiname, int *temperature, int *humidity) {
	
	uint8_t status;
	unsigned int buffer;
	
	status = TWI_WritePacket(twiname, SHT21_SLAVE_ADDRESS, SHT21_TWI_TIMEOUT, &sht21NoHoldTemperatureAddress, 1, NULL, 0);

	if (status != TWI_ERROR_NoError) {
		return status;
	}
	
	_delay_ms(100);
	
	status = TWI_ReadPacket(twiname, SHT21_SLAVE_ADDRESS, SHT21_TWI_TIMEOUT, NULL, 0, sht21DataBuffer, 3);

	if (status != TWI_ERROR_NoError) {
		return status;
	}
	
	buffer = ((sht21DataBuffer[0] << 8) | sht21DataBuffer[1]) & 0b1111111111111100;
	*temperature = Sht21_countTemperature(buffer);
	
	status = TWI_WritePacket(twiname, SHT21_SLAVE_ADDRESS, SHT21_TWI_TIMEOUT, &sht21NoHoldHumidityAddress, 1, NULL, 0);

	if (status != TWI_ERROR_NoError) {
		return status;
	}
	
	_delay_ms(100);
	
	status = TWI_ReadPacket(twiname, SHT21_SLAVE_ADDRESS, SHT21_TWI_TIMEOUT, NULL, 0, sht21DataBuffer, 3);

	if (status != TWI_ERROR_NoError) {
		return status;
	}
	
	buffer = ((sht21DataBuffer[0] << 8) | sht21DataBuffer[1]) & 0b1111111111111100;
	*humidity = Sht21_countHumidity(buffer);
	
	return status;
}