/*
 * sht21.h
 *
 * Created: 2015-08-15 12:41:17
 *  Author: Rodos
 */ 


#ifndef BMP280_H_
#define BMP280_H_

uint8_t Bmp280_Init (TWI_t * twiname);

uint8_t Bmp280_ReadMeasurements (TWI_t *twiname, unsigned long *preassure, long *temperature);

#endif /* BMP280_H_ */