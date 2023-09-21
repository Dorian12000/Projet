/**
 *     _______    _____     __     ______    _______        ____
 *    |   ____|  |     \   |  |   / _____)  |   ____|      /    \
 *    |  |__     |  |\  \  |  |  ( (____    |  |__        /  /\  \
 *    |   __|    |  | \  \ |  |   \____ \   |   __|      /  ____  \
 *    |  |____   |  |  \  \|  |   _____) )  |  |____    /  /    \  \
 *    |_______|  |__|   \_____|  (______/   |_______|  /__/      \__\
 *
 * @file    types.h
 * @Author  Dorian Dalbin
 * @Created	2023-08-03
 * @brief
 *
 **/

#ifndef INC_TYPES_H_
#define INC_TYPES_H_


#include <stdint.h>
#include <stddef.h>


/** \addtogroup types Basic C types and macros
  * @{
  */

/** \addtogroup types_const Basic constants
  * @{
  */

#define UNUSED(X) (void)X

#ifndef bool
typedef uint8_t		bool;
#endif

#ifndef char_t
typedef char	char_t;
#endif

#ifndef TimerTime_t
typedef uint64_t TimerTime_t;
#endif

/**
 * @brief Boolean constant values
 */
#ifdef __cplusplus
typedef bool bool_t;
#else
#undef TRUE
#undef FALSE
typedef enum {
	FALSE = 0,
	TRUE = !FALSE
} bool_t;
#endif
#ifndef false
#define false	FALSE
#endif
#ifndef true
#define true	TRUE
#endif
/**
 * @brief Null pointer value
 */
#ifndef NULL
#define NULL            (void *)(0)
#endif

/** @} */

/** \addtogroup types_ops Useful macro operations
  * @{
  */

/**
 * @brief # define value stringification
 */
#define _STR(x) #x
#define STR(x) _STR(x)

/**
 * @brief Number of elements of a statically declared array
 */
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

/**
 * @brief Ceiling integer division
 */
#define CEIL_DIV(x, y) (((x)+(y)-1) / (y))

/**
 * @brief Minimum of two values
 */
#ifndef MIN
#define MIN(x,y) (((x)<(y))?(x):(y))
#endif

/**
 * @brief Maximum of two values
 */
#ifndef MAX
#define MAX(x,y) (((x)>(y))?(x):(y))
#endif

/** @} */

/** \addtogroup types_struct Structure element operations
  * @{
  */

/**
 * @brief Get address offset of an element within a structure
 * @param type:	  the type of the container struct this is embedded in.
 * @param member: the name of the member within the struct.
 * @return The offset of the member within structure type.
 */
#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(type, member) __compiler_offsetof(type,member)
#else
#define offsetof(type, member) ((unsigned long) &((type *)0)->member)
#endif

/**
 * @brief Cast a member of a structure out to the containing structure
 * @param ptr:    the pointer to the member.
 * @param type:	  the type of the container struct this is embedded in.
 * @param member: the name of the member within the struct.
 * @return The address of the structure that contains the member.
 */
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

typedef struct sTimer1ms{
	unsigned long long int lasttick;
	unsigned long long int counter;
	unsigned int running;
}Timer1ms_t;


typedef enum{
	success = 0,
	unknownerror,
	incorrectsize,
	mallocfail,
	decodefail,
	encodefail,			// 5
	filternotready,
	memoryfail,
	wrongparameter,
	memoryfull,
	endoffile,			// 10
	filenotexist,
	empty,
	wrongtimestamp,
	notenoughdata,
	tokennotauthorized,	// 15
	server_down,
	transmission_failed,
	transmission_no_response,
	transmission_lost,
	connexion_lost,
	busy,
	notfound
}returncode_t;

/** @} */
#define ONE_SECOND_IN_MS 			(1000)
#define ONE_MINUTE_IN_S 			(60)
#define ONE_MINUTE_IN_MS 			(ONE_MINUTE_IN_S*ONE_SECOND_IN_MS)
#define ONE_HOUR_IN_MINUTES 		(60)
#define ONE_HOUR_IN_SECONDS			(3600)
#define ONE_DAY_IN_MINUTES 			(24 * ONE_HOUR_IN_MINUTES)
#define ONE_DAY_IN_SECONDS 			(24 * ONE_HOUR_IN_SECONDS)
#define ONE_DAY_IN_MS 	 			(ONE_DAY_IN_MINUTES * ONE_MINUTE_IN_MS)
#define ONE_WEEK_IN_DAYS 			(7)
#define ONE_WEEK_IN_MS	 			(ONE_WEEK_IN_DAYS * ONE_DAY_IN_MS)
/** @} */

/** @} */

#endif /* INC_TYPES_H_ */
