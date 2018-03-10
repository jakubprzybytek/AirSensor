/*
 * sht21.h
 *
 * Created: 2015-08-15 12:41:17
 *  Author: Rodos
 */ 


#ifndef SHT21_H_
#define SHT21_H_

uint8_t Sht21_Init (TWI_t * twiname);

uint8_t Sht21_StartTemperatureMeasurement (TWI_t *twiname);

uint8_t Sht21_ReadTemperatureMeasurement (TWI_t *twiname, int *temperature);

uint8_t Sht21_StartHumidityMeasurement (TWI_t *twiname);

uint8_t Sht21_ReadHumidityMeasurement (TWI_t *twiname, int *humidity);

uint8_t Sht21_ReadMeasurements (TWI_t *twiname, int *temperature, int *humidity);

#endif /* SHT21_H_ */