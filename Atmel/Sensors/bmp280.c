/*
 * sht21.c
 *
 * Created: 2015-08-15 12:41:06
 *  Author: Rodos
 */

#include <avr/io.h>

#include "../LUFA/Drivers/Peripheral/TWI.h"

#define BMP280_SLAVE_ADDRESS	0b11101100
#define BMP280_TWI_TIMEOUT		10

#define BMP280_MODE_SLEEP 0
#define BMP280_MODE_FORCED 2
#define BMP280_MODE_NORMAL 3

uint8_t bmp280SlaveAddress = BMP280_SLAVE_ADDRESS;

uint8_t bmp280InternalCalib1Address = 0x88;
uint8_t bmp280InternalCalib2Address = 0x8E;
uint8_t bmp280InternalCalib3Address = 0x94;
uint8_t bmp280InternalCalib4Address = 0x9A;
uint8_t bmp280InternalCtrlMeasAddress = 0xF4;
uint8_t bmp280InternalConfigAddress = 0xF5;
uint8_t bmp280InternalMeasurementsAddress = 0xF7;

uint8_t bmp280DataBuffer[6];

unsigned int dig_T1;
int dig_T2, dig_T3;
unsigned int dig_P1;
int dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
// t_fine carries fine temperature as global value
long t_fine;
long bmp280_compensate_T_int32(long adc_T)
{
	long var1, var2, T;
	var1 = ((((adc_T>>3) - ((long)dig_T1<<1))) * ((long)dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((long)dig_T1)) * ((adc_T>>4) - ((long)dig_T1))) >> 12) * ((long)dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
unsigned long bmp280_compensate_P_int64(long adc_P)
{
	long long var1, var2, p;
	var1 = ((long long)t_fine) - 128000;
	var2 = var1 * var1 * (long long)dig_P6;
	var2 = var2 + ((var1*(long long)dig_P5)<<17);
	var2 = var2 + (((long long)dig_P4)<<35);
	var1 = ((var1 * var1 * (long long)dig_P3)>>8) + ((var1 * (long long)dig_P2)<<12);
	var1 = (((((long long)1)<<47)+var1))*((long long)dig_P1)>>33;
	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576-adc_P;
	p = (((p<<31)-var2)*3125)/var1;
	var1 = (((long long)dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((long long)dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((long long)dig_P7)<<4);
	return (unsigned long)p;
}

uint8_t Bmp280_SetMode (TWI_t * twiname, uint8_t mode) {
	uint8_t toWrite = 0b10101010 | mode; // pressure oversampling x16, temperature oversampling x2
	return TWI_WritePacket(twiname, BMP280_SLAVE_ADDRESS, BMP280_TWI_TIMEOUT, &bmp280InternalCtrlMeasAddress, 1, &toWrite, 1);
}

uint8_t Bmp280_Init (TWI_t * twiname) {
	// read calibration parameters
	uint8_t status = TWI_ReadPacket(twiname, BMP280_SLAVE_ADDRESS, BMP280_TWI_TIMEOUT, &bmp280InternalCalib1Address, 1, bmp280DataBuffer, 6);
	if (status != TWI_ERROR_NoError) {
		return status;
	}

	dig_T1 = bmp280DataBuffer[1] << 8 | bmp280DataBuffer[0];
	dig_T2 = bmp280DataBuffer[3] << 8 | bmp280DataBuffer[2];
	dig_T3 = bmp280DataBuffer[5] << 8 | bmp280DataBuffer[4];

	status = TWI_ReadPacket(twiname, BMP280_SLAVE_ADDRESS, BMP280_TWI_TIMEOUT, &bmp280InternalCalib2Address, 1, bmp280DataBuffer, 6);
	if (status != TWI_ERROR_NoError) {
		return status;
	}

	dig_P1 = bmp280DataBuffer[1] << 8 | bmp280DataBuffer[0];
	dig_P2 = bmp280DataBuffer[3] << 8 | bmp280DataBuffer[2];
	dig_P3 = bmp280DataBuffer[5] << 8 | bmp280DataBuffer[4];

	status = TWI_ReadPacket(twiname, BMP280_SLAVE_ADDRESS, BMP280_TWI_TIMEOUT, &bmp280InternalCalib3Address, 1, bmp280DataBuffer, 6);
	if (status != TWI_ERROR_NoError) {
		return status;
	}

	dig_P4 = bmp280DataBuffer[1] << 8 | bmp280DataBuffer[0];
	dig_P5 = bmp280DataBuffer[3] << 8 | bmp280DataBuffer[2];
	dig_P6 = bmp280DataBuffer[5] << 8 | bmp280DataBuffer[4];

	status = TWI_ReadPacket(twiname, BMP280_SLAVE_ADDRESS, BMP280_TWI_TIMEOUT, &bmp280InternalCalib4Address, 1, bmp280DataBuffer, 6);
	if (status != TWI_ERROR_NoError) {
		return status;
	}

	dig_P7 = bmp280DataBuffer[1] << 8 | bmp280DataBuffer[0];
	dig_P8 = bmp280DataBuffer[3] << 8 | bmp280DataBuffer[2];
	dig_P9 = bmp280DataBuffer[5] << 8 | bmp280DataBuffer[4];

	uint8_t toWrite = 0b10110100; // t_sb = 1000ms, IIR 2
	status = TWI_WritePacket(twiname, BMP280_SLAVE_ADDRESS, BMP280_TWI_TIMEOUT, &bmp280InternalConfigAddress, 1, &toWrite, 1);

	if (status != TWI_ERROR_NoError) {
		return status;
	}

	return Bmp280_SetMode(twiname, BMP280_MODE_NORMAL);
}

uint8_t Bmp280_ReadMeasurements (TWI_t *twiname, unsigned long *preassure, long *temperature) {

	long preassureRaw, temperatureRaw;

	uint8_t status = TWI_ReadPacket(twiname, BMP280_SLAVE_ADDRESS, BMP280_TWI_TIMEOUT, &bmp280InternalMeasurementsAddress, 1, bmp280DataBuffer, 6);

	if (status != TWI_ERROR_NoError) {
		return status;
	}

	preassureRaw = ((long)bmp280DataBuffer[0] << 12) | (bmp280DataBuffer[1] << 4) | (bmp280DataBuffer[2] >> 4);	
	temperatureRaw = ((long)bmp280DataBuffer[3] << 12) | (bmp280DataBuffer[4] << 4) | (bmp280DataBuffer[5] >> 4);

	*preassure = bmp280_compensate_P_int64(preassureRaw);
	*temperature = bmp280_compensate_T_int32(temperatureRaw);

	return TWI_ERROR_NoError;
}
