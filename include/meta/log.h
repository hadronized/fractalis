#ifndef MT_LOG_H
#define MT_LOG_H

#include <stdio.h>
#include <stdarg.h>

/* Log flag:
 * Bits (8):
 *   xx000000 -> LOG_NONE 
 *   xx000001 -> LOG_DEBUG
 *   xx000010 -> LOG_WARNING
 *   xx000100 -> LOG_INT_ERROR
 *   xx001000 -> LOG_EXT_ERROR
 *   xx001111 -> LOG_ALL
 */
#define LOG_NONE       0x00
#define LOG_DEBUG      0x01
#define LOG_WARNING    0x02
#define LOG_INT_ERROR  0x04
#define LOG_EXT_ERROR  0x08
#define LOG_ALL        0x0F

char mt_logflag;

void MT_log(char logger, char const *log, ...);

#endif /* guard */
