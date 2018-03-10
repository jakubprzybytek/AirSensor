/*
 * ScreenAllSensors.h
 *
 * Created: 2015-09-21 21:38:13
 *  Author: Rodos
 */ 


#ifndef SCREENALLSENSORS_H_
#define SCREENALLSENSORS_H_

void Screen_AllSensors_Init();

void Screen_AllSensors_DisplaySensorsReadings(SensorsReading * sensors, DateTime * dateTime);

#endif /* SCREENALLSENSORS_H_ */