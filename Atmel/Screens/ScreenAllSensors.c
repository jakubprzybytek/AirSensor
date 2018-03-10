/*
 * ScreenAllSensors.c
 *
 * Created: 2015-09-21 21:38:24
 *  Author: Rodos
 */ 

#include "../twi.h"
#include "../Display/GUI.h"
#include "../Sensors/Sensors.h"
#include "../RTC/RTC.h"

void Screen_AllSensors_Init() {
	GUI_print3("All sensors", 0, 0, 0x88);
	GUI_print3("Batt:", 0, 10, 0x22);
	GUI_print3("SHT21:", 0, 20, 0x22);
	GUI_print3("BMP280:", 0, 30, 0x22);
	GUI_print3("MCP7940N:", 0, 48, 0x22);
}

void Screen_AllSensors_DisplaySensorsReadings(SensorsReading * sensors, DateTime * dateTime) {

	sprintf(buffer, "%ld.%02ldV/%ld.%02ldV", sensors->batVoltage / 100, sensors->batVoltage % 100, sensors->vccVoltage / 100, sensors->vccVoltage % 100);
	GUI_print3(buffer, 15, 10, 0xCC);

	if (sensors->sht21TwiResult == TWI_ERROR_NoError) {
		sprintf(buffer, "%d.%02d`C,%d.%01d%%", sensors->sht21Temp / 100, sensors->sht21Temp % 100, sensors->sht21Humidity / 10, sensors->sht21Humidity % 10);
		GUI_print3(buffer, 19, 20, 0xCC);
	} else {
		GUI_print3(twiErrorToA(sensors->sht21TwiResult), 0, 20, 0x66);
	}

	if (sensors->bmp280TwiResult == TWI_ERROR_NoError) {
		sprintf(buffer, "%ld.%02ldhP,%ld.%02ld`C", sensors->bmp280Pressure / 25600, (sensors->bmp280Pressure >> 8) % 100, sensors->bmp280Temp / 100, sensors->bmp280Temp % 100);
		GUI_print3(buffer, 0, 38, 0xCC);
	} else {
		GUI_print3(twiErrorToA(sensors->bmp280TwiResult), 0, 38, 0x66);
	}

	if (dateTime->mcp7940nResult == TWI_ERROR_NoError) {
		sprintf(buffer, "% 1d:%02d:%02d %d.%02d.20%02d", dateTime->hour, dateTime->minute, dateTime->second, dateTime->day, dateTime->month, dateTime->year);
		GUI_print3(buffer, 0, 56, 0xCC);
	} else {
		GUI_print3(twiErrorToA(dateTime->mcp7940nResult), 0, 56, 0x66);
	}
}