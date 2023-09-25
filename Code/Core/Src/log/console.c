/**
 *     _______    _____     __     ______    _______        ____
 *    |   ____|  |     \   |  |   / _____)  |   ____|      /    \
 *    |  |__     |  |\  \  |  |  ( (____    |  |__        /  /\  \
 *    |   __|    |  | \  \ |  |   \____ \   |   __|      /  ____  \
 *    |  |____   |  |  \  \|  |   _____) )  |  |____    /  /    \  \
 *    |_______|  |__|   \_____|  (______/   |_______|  /__/      \__\
 *
 * @file    console.c
 * @Author  Dorian Dalbin
 * @Created	2023-08-03
 * @brief
 *
 **/

#include <stdarg.h>
#include <string.h>
#include "log/types.h"
#include "log/logger.h"

#include "log/colors.h"
#include "log/tstamp.h"

#include "../../Inc/log/console.h"

#include "../../Inc/usart.h"

#define _LOGBUF_SIZE_       255

static char _logbuf[_LOGBUF_SIZE_];
static uint16_t _logbuf_i = 0;
static uint8_t _log_route = 0;  // 0: PUTCHAR / 1: BUFFER
static uint8_t logger_used = 0;
static uint8_t _uart_ready = 1;

static inline void _preparebuf(void){
	_logbuf_i = 0;
	_log_route = 1;
}
static inline void _addtobuf(char c){
	if(_logbuf_i < _LOGBUF_SIZE_){
		_logbuf[_logbuf_i] = c;
		_logbuf_i += 1;
	}
}
static inline void _sendbuf(void){
	uint16_t i=0;
	for(i=0; i<_logbuf_i; i++){
		HAL_UART_Transmit(&huart1,&(_logbuf[i]),1,10);
	}
	_log_route = 0;
}

/* in addition to usual console, print all chars in USB buffer */
static inline void _putchar(char c)
{
	if(_log_route == 0) {
		HAL_UART_Transmit(&huart1,&c,1,10);
	}
	else{
		_addtobuf(c);
	}
}



void _puts(const char *str)
{
	while (*str != '\0') {
		_putchar(*(str++));
	}
}
static void _putval(unsigned int value, int base, int fill_n, char fill_char)
{
	char digits[10];
	int i, n;

	for (i = 0; i < sizeof(digits); i++) {
		digits[i] = value % base;
		value /= base;
	}

	for (n = sizeof(digits)-1; (n >= 0) && (digits[n] == 0); n--);
	//printf("|%+d|", n);

	if (n < 0) {
		for (i = 1; i < fill_n; i++) {
			_putchar(fill_char);
		}
		_putchar('0');
	}
	else {
		for (i = (n+1); i < fill_n; i++) {
			_putchar(fill_char);
		}

		for (i = n; i >= 0; i--) {
			char c = digits[i];
			if (c < 10)
				_putchar('0'+c);
			else
				_putchar('A'-10+c);
		}
	}
}


int _vprintf(const char *fmt, va_list ap)
{
	int float_precision = -1;
	while (*fmt != '\0') {
		char c = *(fmt++);

		if (c == '%') {
			char fill_char = 0;
			int fill_n = 0;
			char *s;
			int d;
			float f;
			unsigned int u;
			unsigned long ul;
			uint8_t *buf;
			uint16_t i=0;

			c = *fmt;

			// check if float precision
			if(c == '.'){
				fmt++;
				c = *fmt;
				if ((c >= '0') && (c <= '9')) {
					float_precision = c - '0';
				}
				fmt++;
			}

			/* Parse fill characters */
			if (((c >= '0') && (c <= '9')) || (c == ' ')) {
				if ((c == '0') || (c == ' ')) {
					fill_char = c;
					fmt++;
				}
				else {
					fill_char = ' ';
				}

				s = (char *) fmt;
				while ((*s >= '0') && (*s <= '9')) {
					fill_n *= 10;
					fill_n += *s - '0';
					s++;
				}
				fmt = s;
			}

			switch (*fmt++) {
			case 'b':               /* buffer */
				buf = va_arg(ap, uint8_t *);
				u = va_arg(ap, unsigned int);
				_puts("0x[");
				for(i=0; i<u; i++){
					if(i>0) {
						_putchar(' ');
					}
					if (buf[i] < 0x10) {
						_putchar('0');
					}
					_putval(buf[i], 16, fill_n, fill_char);
				}
				_putchar(']');
				break;
			case 's':              /* string */
				s = va_arg(ap, char *);
				if(fill_n) { /* case of '%08s' */
					fill_n -= strlen(s);
					while(fill_n--) _putchar(' ');
				}
				_puts(s);
				break;
			case 'd':              /* int */
				d = va_arg(ap, int);
				if (d < 0) {
					_putchar('-');
					d = -d;
				}
				_putval(d, 10, fill_n, fill_char);
				break;
			case 'u':              /* unsigned int */
				u = va_arg(ap, unsigned int);
				_putval(u, 10, fill_n, fill_char);
				break;
			case 'x':              /* unsigned int, hexadecimal */
			case 'X':
				u = va_arg(ap, unsigned int);
				_putval(u, 16, fill_n, fill_char);
				break;
			case 'p':
				ul = va_arg(ap, unsigned long);
				_putchar('0');
				_putchar('x');
				_putval(ul, 16, 8, '0');
				break;
			case 'c':              /* char */
				/* need a cast here since va_arg only
			     takes fully promoted types */
				c = (char) va_arg(ap, int);
				_putchar(c);
				break;
			case 'f':
				// float value
				// %.2f
				f = va_arg(ap, double);
				d = f;
				if (d < 0) {
					_putchar('-');
					d = -d;
				}
				_putval(d, 10, fill_n, fill_char);
				if(float_precision > 0){
					_putchar('.');
					f = f - d;		// remove entire part
					while(float_precision > 0){
						f = f * 10;
						float_precision -= 1;
						if((unsigned int)f == 0){
							_putchar('0');
						}
					}
					_putval((unsigned int)f, 10, fill_n, fill_char);
				}
				break;
			case '%':
				_putchar('%');
				break;
			default:
				_putchar(c);
				break;
			}
		}
		else {
			_putchar(c);
		}

	}

	return 0;
}

int console_putchar(int c)
{
	_preparebuf();

	_putchar(c);

	_sendbuf();

	_puts("\r\n");

	return 0;
}

int console_putbuf(uint8_t *buf, uint16_t size){
	if(logger_used == 1) {
		return 0;
	}
	logger_used = 1;

	_preparebuf();

	uint16_t i;
	for(i=0; i<size; i++) {
		_putchar(buf[i]);
	}

	_sendbuf();

	// _puts("\r\n");

	logger_used = 0;
	return 0;
}
int console_printf(const char *fmt, ...)
{
	_preparebuf();

	va_list ap;

	va_start(ap, fmt);
	_vprintf(fmt, ap);
	va_end(ap);

	_sendbuf();

	_puts("\r\n");

	return 0;
}

void console_logger(logtype_t logtype, char *level, char *color, char *module, char *file, const char *func, char *fmt, ...)
{
	if(logger_used == 1) {
		return;
	}
	logger_used = 1;
	va_list ap;
	uint32_t ms;
	uint32_t sec = uptime(&ms);
	uint8_t i;
	char c;

	_puts(color);

	// print the timestamp
	_putval(sec, 10, 6, ' ');
	_putchar('.');
	_putval(ms, 10, 3, '0');

	_putchar(' ');
	for (i = 0; i < 6; i++) {
		c = ' ';
		if (*level != '\0'){
			c = *(level++);
		}
		_putchar(c);
	}
	_putchar(' ');
	_putchar(' ');

	_preparebuf();

	for (i = 0; i < 12; i++) {
		c = ' ';
		if (*module != '\0'){
			c = *(module++);
		}
		_putchar(c);
	}

	_puts(" - ");

	//	_puts(file);
	//	_puts(", in ");

	_puts(func);
	if(fmt){
		_puts(": ");

		va_start(ap, fmt);
		_vprintf(fmt, ap);
		va_end(ap);
	}

	_sendbuf();

	_puts("\r\n");

/*
#if LOG_TYPE == LOG_TYPE_HOST
	_send_log_to_host(sec, ms, logtype, _logbuf, _logbuf_i);
#endif*/
logger_used = 0;
}

