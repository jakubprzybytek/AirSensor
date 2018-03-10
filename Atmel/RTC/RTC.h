/*
 * RTC.h
 *
 * Created: 2015-10-08 18:07:20
 *  Author: Rodos
 */ 


#ifndef RTC_H_
#define RTC_H_

typedef struct {
	uint8_t		mcp7940nResult;
	uint8_t		hour;
	uint8_t		minute;
	uint8_t		second;
	uint8_t		day;
	uint8_t		month;
	uint8_t		year;
} DateTime;

#endif /* RTC_H_ */