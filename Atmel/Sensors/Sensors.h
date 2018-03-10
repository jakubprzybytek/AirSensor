/*
 * Sensors.h
 *
 * Created: 2015-09-15 22:25:12
 *  Author: Rodos
 */ 


#ifndef SENSORS_H_
#define SENSORS_H_

typedef struct {
	uint8_t			bmp280TwiResult;
	unsigned long	bmp280Pressure;
	long			bmp280Temp;

	uint8_t			sht21TwiResult;
	int				sht21Temp;
	int				sht21Humidity;

	long			batVoltage;
	long			vccVoltage;
} SensorsReading;

void Sensors_InitTimer(TC0_t * TC);

#endif /* SENSORS_H_ */