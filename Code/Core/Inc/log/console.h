/**
 *     _______    _____     __     ______    _______        ____
 *    |   ____|  |     \   |  |   / _____)  |   ____|      /    \
 *    |  |__     |  |\  \  |  |  ( (____    |  |__        /  /\  \
 *    |   __|    |  | \  \ |  |   \____ \   |   __|      /  ____  \
 *    |  |____   |  |  \  \|  |   _____) )  |  |____    /  /    \  \
 *    |_______|  |__|   \_____|  (______/   |_______|  /__/      \__\
 *
 * @file    console.h
 * @Author  Dorian Dalbin
 * @Created	2023-08-03
 * @brief
 *
 **/
#ifndef INC_CONSOLE_H_
#define INC_CONSOLE_H_

#include <stdarg.h>
#include "colors.h"

/** \addtogroup sys
  * @{
  */

typedef enum
{
    LOGTYPE_UNKNOWN = 0,
    LOGTYPE_ENTER,
    LOGTYPE_EXIT,
    LOGTYPE_DEBUG,
    LOGTYPE_INFO,
    LOGTYPE_WARN,
    LOGTYPE_ERROR,
    LOGTYPE_ASSERT
}logtype_t;

/** \addtogroup sys_console System console
  * @{
  * The system console use a dedicated UART port to implement a diagnostic/debug terminal.<br/>
  * This feature implements a command parser, and a tiny printf-like output formating function
  * that shows time-stamped log messages.
  */

/** \addtogroup sys_console_init System console initialisation
  * @{
  */
/*!
 * \brief Console functions
 */
typedef struct
{
    /*!
     * \brief  Tx, send a char to console
     */
    uint32_t    ( *tx )( int8_t c );
    /*!
     * \brief  Rx receive a char from console
     */
    void    ( *rx )( int8_t c );
    /*!
	 * \brief  flush the Tx buffer
	 */
    uint32_t    ( *flush )(void);
}Console_t;



/** @} */


/** \addtogroup sys_console_io System console i/o operations
  * @{
  */


  #undef putchar
  #define putchar(c) console_putchar(c)

  //#undef puts
  //#define puts(s) _puts(s)

  #undef printf
  #define printf(args...) console_printf(args)



  /**
   * @brief Init the system console.
   *
   * This function enables the system console, and open the console UART port.
   * It then shows some init messages, indicating the following information:
   * - Firmware version
   * - CPU type and unique id
   * - Clock frequency
   */
  extern void console_init(void);

  /**
   * @brief Shutdown the system console.
   *
   * This function disables the system console, and close the console UART port.
   */
  extern void console_shutdown(void);

  extern void console_flush(void);

  /**
   * @brief Put the system in low-power mode.
   */
  extern void console_suspend(void);

  /**
   * @brief Resume the system console after CPU wakeup.
   */
  extern void console_resume(void);

  int console_putbuf(uint8_t *buf, uint16_t size);

  int console_printf(const char *fmt, ...);

  /**
   * @brief Send a log message to the console UART.
   * @param tag
   *   Log header tag
   * @param fmt
   *   Format string
   * @param ...
   *   String format arguments
   *
   * A log message is composed of the following substring :
   * - A log header
   * - A formatted string
   * - Line termination characters (CR+LF)
   */
  void console_logger(logtype_t logtype, char *level, char *color, char *module, char *file, const char *func, char *fmt, ...);

  void console_rx(char c);

/** @} */

/** @} */

/** @} */

#endif /* INC_CONSOLE_H_ */
