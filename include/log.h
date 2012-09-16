/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Fractalis, a mandelbrot fractal viewer 
    Copyright (C) 2012  Dimitri 'skp' Sabadie <dimitri.sabadie@gmaile.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef LOG_H
#define LOG_H

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

char fr_logflag;

void FR_log(char logger, char const *log, ...);

#endif /* guard */
