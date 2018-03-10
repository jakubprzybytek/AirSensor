/*
 * ScreenWeather.c
 *
 * Created: 2015-09-21 12:20:35
 *  Author: Rodos
 */ 

#include "../twi.h"
#include "../Display/GUI.h"
#include "../Sensors/Sensors.h"

void Screen_Weather_Init() {
	GUI_print3("Weather", 0, 0, 0x88);
	GUI_print3("    Temp:", 0, 12, 0x44);
	GUI_print3("Pressure:", 0, 22, 0x44);
	GUI_print3("Humidity:", 0, 32, 0x44);
	GUI_print3("Dewpoint:", 0, 42, 0x44);
}

void Screen_Weather_DisplaySensorsReadings(SensorsReading * sensors) {

	if (sensors->sht21TwiResult == TWI_ERROR_NoError) {
		sprintf(buffer, "%d.%02d`C", sensors->sht21Temp / 100, sensors->sht21Temp % 100);
		GUI_print3(buffer, 27, 12, 0xCC);

		sprintf(buffer, "%d.%01d%%", sensors->sht21Humidity / 10, sensors->sht21Humidity % 10);
		GUI_print3(buffer, 27, 32, 0xCC);

		int dewPoint = sensors->sht21Temp - (1000 - sensors->sht21Humidity) * 2;
		sprintf(buffer, "%d.%02d`C", dewPoint / 100, dewPoint % 100);
		GUI_print3(buffer, 27, 42, 0xCC);
	} else {
		GUI_print3(twiErrorToA(sensors->sht21TwiResult), 27, 12, 0x44);
	}

	if (sensors->bmp280TwiResult == TWI_ERROR_NoError) {
		sprintf(buffer, "%ld.%02ldhP", sensors->bmp280Pressure / 25600, (sensors->bmp280Pressure >> 8) % 100);
		GUI_print3(buffer, 27, 22, 0xCC);
	} else {
		GUI_print3(twiErrorToA(sensors->bmp280TwiResult), 27, 22, 0x44);
	}
}