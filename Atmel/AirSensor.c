/*
 * AirSensor.c
 *
 * Created: 2015-09-12 12:21:21
 *  Author: Rodos
 */ 


#include <stddef.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "AirSensor.h"

#include "Display/GUI.h"

#include "Events.h"
#include "Inputs/Switch.h"
#include "Inputs/Encoder.h"

#include "twi.h"
#include "Sensors/Sensors.h"
#include "Sensors/sht21.h"
#include "Sensors/bmp280.h"
#include "Sensors/BattVoltage.h"

#include "RTC/RTC.h"
#include "RTC/MCP7940N.h"

#include "Screens/ScreenWeather.h"
#include "Screens/ScreenAllSensors.h"

#define SCREENS 2

SensorsReading sensors;
DateTime dateTime;

// switch 1 int
ISR (PORTE_INT0_vect) {
	Events_SetStatus(EVENTS_PRESSED_SWITCH_1);
	MCP7940N_ReadTime(&TWIE, &dateTime);
}

// switch 2 int
ISR (PORTE_INT1_vect) {
	Events_SetStatus(EVENTS_PRESSED_SWITCH_2);
}

// temp and pressure interrupt 1
ISR (TCC0_CCA_vect) {
	LED_TOGGLE
	int temp;
	sensors.sht21TwiResult = Sht21_ReadTemperatureMeasurement(&TWIC, &temp);
	if (sensors.sht21TwiResult == TWI_ERROR_NoError) {
		sensors.sht21TwiResult = Sht21_StartHumidityMeasurement(&TWIC);

		if (temp != sensors.sht21Temp) {
			sensors.sht21Temp = temp;
			Events_SetStatus(EVENTS_TEMPERATURE_UPDATED);
		}
	}
}

// temp and pressure interrupt 2
ISR (TCC0_CCB_vect) {
	LED_TOGGLE
	int temp;
	sensors.sht21TwiResult = Sht21_ReadHumidityMeasurement(&TWIC, &temp);
	if (sensors.sht21TwiResult == TWI_ERROR_NoError) {
		sensors.sht21TwiResult = Sht21_StartTemperatureMeasurement(&TWIC);
		
		if (temp != sensors.sht21Humidity) {
			sensors.sht21Humidity = temp;
			Events_SetStatus(EVENTS_HUMIDITY_UPDATED);
		}
	}
}

// temp and pressure interrupt 3
ISR (TCC0_OVF_vect) {
	LED_TOGGLE
	unsigned long tempP;
	long tempT;
	sensors.bmp280TwiResult = Bmp280_ReadMeasurements(&TWIC, &tempP, &tempT);

	if (tempP != sensors.bmp280Pressure) {
		sensors.bmp280Pressure = tempP;
		sensors.bmp280Temp = tempT;
		Events_SetStatus(EVENTS_PREASSURE_UPDATED);
	}
}

// encoder interrupt
ISR (TCC1_CCA_vect) {
	TCC1.CNT = 8;
	Events_SetStatus(EVENTS_ENCODER_RIGHT);
}

// encoder interrupt
ISR (TCC1_OVF_vect) {
	TCC1.CNT = 8;
	Events_SetStatus(EVENTS_ENCODER_LEFT);
}

// enabling ADC for bat voltage conversion
ISR (TCD0_CCA_vect) {
	BattVoltage_Enable();
}

// ADCB - bat voltage conversion finished
ISR (ADCB_CH0_vect) {
	long temp = BattVoltage_ReadBatt(&ADCB);
	BattVoltage_Disable();
	if (temp != sensors.batVoltage) {
		sensors.batVoltage = temp;
		Events_SetStatus(EVENTS_BATT_VOLTAGE_READ);
	}
}

// ADCA - internal/vcc voltage conversion finished
ISR (ADCA_CH0_vect) {
	long temp = BattVoltage_ReadVcc(&ADCA);
	if (temp != sensors.vccVoltage) {
		sensors.vccVoltage = temp;
		Events_SetStatus(EVENTS_VCC_VOLTAGE_READ);
	}
}

void InitScreen(uint8_t screenNumber) {
	switch (screenNumber) {
		case 0 :
			Screen_Weather_Init();
		break;
		case 1 :
			Screen_AllSensors_Init();
		break;
	}
}

void DisplayScreen(uint8_t screenNumber) {
	switch (screenNumber) {
		case 0 :
			Screen_Weather_DisplaySensorsReadings(&sensors);
		break;
		case 1 :
			Screen_AllSensors_DisplaySensorsReadings(&sensors, &dateTime);
		break;
	}
}

uint8_t SP_ReadCalibrationByte(uint8_t index) {
	uint8_t result;

	/* Load the NVM Command register to read the calibration row. */
	NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
	result = pgm_read_byte(index);

	/* Clean up NVM Command register. */
	NVM_CMD = NVM_CMD_NO_OPERATION_gc;

	return result;
} 	

void ADC_CalibrationValues_Load(ADC_t * adc)
{
	if (&ADCA == adc) {
		/* Get ADCACAL0 from production signature . */
		adc->CALL = SP_ReadCalibrationByte(PRODSIGNATURES_ADCACAL0);
		adc->CALH = SP_ReadCalibrationByte(PRODSIGNATURES_ADCACAL1);
	} else {
		/* Get ADCBCAL0 from production signature  */
		adc->CALL = SP_ReadCalibrationByte(PRODSIGNATURES_ADCBCAL0);
		adc->CALH = SP_ReadCalibrationByte(PRODSIGNATURES_ADCBCAL1);
	}
}

int main(void)
{
	PORTA.DIR = 0x0ff; PORTA.OUT = 0x0;
	PORTB.DIR = 0x0ff; //PORTB.OUT = 0x0;
	PORTC.DIR = 0x0ff; PORTC.OUT = 0x0;
	PORTD.DIR = 0x0ff; PORTD.OUT = 0x0;
	PORTE.DIR = 0x0ff; PORTE.OUT = 0x0;
	PORTF.DIR = 0x0ff; PORTF.OUT = 0x0;

	ADC_CalibrationValues_Load(&ADCA);
	ADC_CalibrationValues_Load(&ADCB);

	LED_INIT
	Switches_Init();
	Encoder_Init(&TCC1);

	TWI_Init(&TWIC, TWI_BAUD_FROM_FREQ((long long) 400000));
	Sht21_Init(&TWIC);
	Bmp280_Init(&TWIC);

	TWI_Init(&TWIE, TWI_BAUD_FROM_FREQ((long long) 400000));

	BattVoltage_Init(&ADCA, &ADCB, &TCD0);

	sensors.sht21TwiResult = Sht21_StartTemperatureMeasurement(&TWIC);
	MCP7940N_WriteTime(&TWIE, &dateTime);
	MCP7940N_ReadTime(&TWIE, &dateTime);

	Sensors_InitTimer(&TCC0);

	// enable interrupts
	PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_LOLVLEN_bm;
	sei();

	uint8_t eventsStatus = EVENTS_NO_ACTION;
	uint8_t currentScreen = 0;

    while(1)
    {
		GUI_Init();
		InitScreen(currentScreen);

		while ((eventsStatus = Events_GetStatus()) != EVENTS_PRESSED_SWITCH_2) {

			if (eventsStatus == EVENTS_ENCODER_LEFT) {
				currentScreen = (currentScreen + SCREENS - 1) % SCREENS;
				GUI_Clean();
				InitScreen(currentScreen);
			}

			if (eventsStatus == EVENTS_ENCODER_RIGHT) {
				currentScreen = (currentScreen + 1) % SCREENS;
				GUI_Clean();
				InitScreen(currentScreen);
			}

			DisplayScreen(currentScreen);

			sprintf(buffer, "%02d/%02d", TCC1.CNT, eventsStatus);
			GUI_print3(buffer, 45, 48, 0x22);
		}

		GUI_Shutdown();

		while ((eventsStatus = Events_GetStatus()) != EVENTS_PRESSED_SWITCH_2) {
		}
    }
}