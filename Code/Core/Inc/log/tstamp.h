/**
 *     _______    _____     __     ______    _______        ____
 *    |   ____|  |     \   |  |   / _____)  |   ____|      /    \
 *    |  |__     |  |\  \  |  |  ( (____    |  |__        /  /\  \
 *    |   __|    |  | \  \ |  |   \____ \   |   __|      /  ____  \
 *    |  |____   |  |  \  \|  |   _____) )  |  |____    /  /    \  \
 *    |_______|  |__|   \_____|  (______/   |_______|  /__/      \__\
 *
 * @file    tstamp.h
 * @Author  Dorian Dalbin
 * @Created	2023-08-03
 * @brief
 *
 **/

#ifndef INC_TSTAMP_H_
#define INC_TSTAMP_H_

#include "types.h"

#define YEAR0                   2000
#define EPOCH_YR                1970
#define SECS_DAY                (24L * 60L * 60L)
#define LEAPYEAR(year)          (!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define YEARSIZE(year)          (LEAPYEAR(year) ? 366 : 365)
#define FIRSTSUNDAY(timp)       (((timp)->tm_yday - (timp)->tm_wday + 420) % 7)
#define FIRSTDAYOF(timp)        (((timp)->tm_wday - (timp)->tm_yday + 420) % 7)

#define CODE_JANUARY(year)			(LEAPYEAR(year) ? 5 : 6)
#define CODE_FEBRUARY(year)			(LEAPYEAR(year) ? 1 : 2)
#define CODE_MARCH					2
#define CODE_APRIL					5
#define CODE_MAY					0
#define CODE_JUNE					3
#define CODE_JULY					5
#define CODE_AUGUST					1
#define CODE_SEPTEMBER				4
#define CODE_OCTOBER				6
#define CODE_NOVEMBER				2
#define CODE_DECEMBER				4

typedef struct
{
	uint16_t year;                  // real year
	uint8_t month;                  // 1..12
	uint8_t day;                    // 1..31
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t wday;                   // 1..7
}tstamp_time_t;

void tstamp_init(void);
void tstamp_set(uint32_t sec);
void tstamp_tick_s(void);

uint32_t tstamp (uint32_t *ms);
uint32_t uptime (uint32_t *ms);
uint64_t uptime_ms(void);

uint32_t diff_uptime_ms(uint32_t sec, uint32_t ms);

void uptime_reset(void);
void uptime_tick_ms(uint32_t tick);
void uptime_fix(int32_t tick);

//uint32_t tstamp_ticks();
//uint32_t tstamp_ms ();
//uint32_t tstamp_us ();

/*!
 * \brief Structure holding the system time in seconds and milliseconds.
 */
typedef struct SysTime_s
{
    uint32_t Seconds;
    int16_t  SubSeconds;
}SysTime_t;

/*!
 * \brief Adds 2 SysTime_t values
 *
 * \param a Value
 * \param b Value to added
 *
 * \retval result Addition result (SysTime_t value)
 */
SysTime_t SysTimeAdd( SysTime_t a, SysTime_t b );

/*!
 * \brief Subtracts 2 SysTime_t values
 *
 * \param a Value
 * \param b Value to be subtracted
 *
 * \retval result Subtraction result (SysTime_t value)
 */
SysTime_t SysTimeSub( SysTime_t a, SysTime_t b );

/*!
 * \brief Gets current MCU system time
 *
 * \retval sysTime    Current seconds/sub-seconds since Mcu started
 */
SysTime_t SysTimeGetMcuTime( void );

uint32_t SysTimeToMs( SysTime_t sysTime );
SysTime_t SysTimeFromMs( uint32_t timeMs );

#define SysTimeGet()            SysTimeGetMcuTime()

void to_unixtime(uint32_t seconds, tstamp_time_t *unixtime);
uint32_t from_unixtime(tstamp_time_t *unixtime);
#endif /* INC_TSTAMP_H_ */
