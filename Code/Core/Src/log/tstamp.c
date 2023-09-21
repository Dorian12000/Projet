/**
 *     _______    _____     __     ______    _______        ____
 *    |   ____|  |     \   |  |   / _____)  |   ____|      /    \
 *    |  |__     |  |\  \  |  |  ( (____    |  |__        /  /\  \
 *    |   __|    |  | \  \ |  |   \____ \   |   __|      /  ____  \
 *    |  |____   |  |  \  \|  |   _____) )  |  |____    /  /    \  \
 *    |_______|  |__|   \_____|  (______/   |_______|  /__/      \__\
 *
 * @file    tstamp.c
 * @Author  Dorian Dalbin
 * @Created	2023-08-03
 * @brief
 *
 **/

#include "log/types.h"

#include "log/tstamp.h"

#define TSTAMP_HZ    1000
#define MS_PER_S     1000

static volatile uint32_t tstamp_sec;
static volatile uint32_t uptime_sec;
static volatile uint32_t uptime_msec;

const int _ytab[2][12] = {
		{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
		{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

void tstamp_tick_s(void)
{
	tstamp_sec += 1;
	// uptime_sec += 1;
	// uptime_msec = 0;
}
void tstamp_init (void)
{
	tstamp_sec = 0;
	uptime_sec = 0;
	uptime_msec = 0;
}

void tstamp_set(uint32_t sec){
	tstamp_sec = sec;
}

uint32_t tstamp (uint32_t *ms)
{
	if (ms)
	{
		uint32_t sec;
		do {
			sec = tstamp_sec;
			*ms = uptime_msec;
		} while (sec != tstamp_sec);
		return sec;
	}

	return tstamp_sec;
}
void uptime_reset(void){
	uptime_sec = 0;
	uptime_msec = 0;
}

void uptime_tick_ms(uint32_t tick){
	uptime_msec += tick;
	while(uptime_msec >= MS_PER_S){
		uptime_sec++;
		// tstamp_sec ++;					// FIXME should be incremented by RTC every second
		uptime_msec -= MS_PER_S;
	}
}
void uptime_fix(int32_t tick){
	if(tick > 0){
		uptime_msec += tick;
		while(uptime_msec >= MS_PER_S){
			uptime_sec++;
			// tstamp_sec ++;
			uptime_msec -= MS_PER_S;
		}
	}
	else if(uptime_msec > tick){
		uptime_msec -= tick;
	}
	else{
		// TODO
	}
}

uint32_t uptime (uint32_t *ms)
{
	if (ms)
	{
		uint32_t sec;
		do {
			sec = uptime_sec;
			*ms = uptime_msec;
		} while (sec != uptime_sec);
		return sec;
	}

	return uptime_sec;
}

uint64_t uptime_ms(void){
	return (uptime_sec*1000 + uptime_msec);
}

uint32_t diff_uptime_ms(uint32_t sec, uint32_t ms){
	uint32_t diff = (uptime_sec - sec)*1000;
	if(uptime_msec < ms){
		diff += 1000 - ms + uptime_msec;
	}
	else{
		diff += uptime_msec - ms;
	}
	return diff;
}

SysTime_t SysTimeAdd( SysTime_t a, SysTime_t b )
{
    SysTime_t c =  { .Seconds = 0, .SubSeconds = 0 };

    c.Seconds = a.Seconds + b.Seconds;
    c.SubSeconds = a.SubSeconds + b.SubSeconds;
    if( c.SubSeconds >= 1000 )
    {
        c.Seconds++;
        c.SubSeconds -= 1000;
    }
    return c;
}

SysTime_t SysTimeSub( SysTime_t a, SysTime_t b )
{
    SysTime_t c = { .Seconds = 0, .SubSeconds = 0 };

    c.Seconds = a.Seconds - b.Seconds;
    c.SubSeconds = a.SubSeconds - b.SubSeconds;
    if( c.SubSeconds < 0 )
    {
        c.Seconds--;
        c.SubSeconds += 1000;
    }
    return c;
}

SysTime_t SysTimeGetMcuTime( void )
{
    SysTime_t calendarTime = { .Seconds = uptime_sec, .SubSeconds = uptime_msec };

    return calendarTime;
}

uint32_t SysTimeToMs( SysTime_t sysTime )
{
    uint32_t seconds = uptime_sec;
    uint32_t subSeconds = uptime_msec;

    SysTime_t deltaTime = { .Seconds = seconds, .SubSeconds = ( int16_t )subSeconds };

    SysTime_t calendarTime = SysTimeSub( sysTime, deltaTime );

		// TODO review with uptime_ms
    return calendarTime.Seconds * 1000 + calendarTime.SubSeconds;
}

SysTime_t SysTimeFromMs( uint32_t timeMs )
{
    uint32_t seconds = timeMs / 1000;
    uint32_t subSeconds = timeMs - seconds * 1000;
    SysTime_t sysTime = { .Seconds = seconds, .SubSeconds = ( int16_t )subSeconds };

    SysTime_t deltaTime = { .Seconds = uptime_sec, .SubSeconds = ( int16_t )uptime_msec };

	// TODO review with uptime_ms
    return SysTimeAdd( sysTime, deltaTime );
}

/**
 *
 * @param seconds
 * @param unixtime
 */
void to_unixtime(uint32_t seconds, tstamp_time_t *unixtime){

	uint32_t dayclock, dayno;
	uint16_t year = 1970;
	uint8_t mon, day, hour, min, sec;
	uint8_t wday;

	dayclock = (unsigned long) seconds % SECS_DAY;
	dayno = (unsigned long) seconds / SECS_DAY;

	sec = dayclock % 60;
	min = (dayclock % 3600) / 60;
	hour = dayclock / 3600;
	wday = (dayno + 4) % 7; // Day 0 was a thursday
	while (dayno >= (uint32_t) YEARSIZE(year)) {
		dayno -= YEARSIZE(year);
		year++;
	}
	year = year - 1900;
	mon = 0;
	while (dayno >= (uint32_t) _ytab[LEAPYEAR(year)][mon]) {
		dayno -= _ytab[LEAPYEAR(year)][mon];
		mon++;
	}
	day = dayno + 1;	// +1 ?
	mon = mon + 1;		// +1?         // month 1 is january
	year = year + 1900;

	unixtime->year = year;
	unixtime->month = mon;
	unixtime->day = day;
	unixtime->hour = hour;
	unixtime->minute = min;
	unixtime->second = sec;
	unixtime->wday = wday==0?7:wday;
}

uint32_t from_unixtime(tstamp_time_t *unixtime){
	uint32_t seconds = 0;

	seconds += unixtime->second;
	seconds += unixtime->minute*60;
	seconds += unixtime->hour*3600;

	seconds += (unixtime->day-1)*SECS_DAY;

	uint8_t mon = unixtime->month - 1;           // month 1 is january ==> 0
	uint16_t year = unixtime->year;
	while (mon > 0) {
		mon--;
		seconds += _ytab[LEAPYEAR(year)][mon]*SECS_DAY;
	}
	year--;
	while(year >= 1970) {
		seconds += YEARSIZE(year)*SECS_DAY;
		year--;
	}

	return seconds;
}
