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

#include "log.h"

void __FR_vlog(FILE *file, char const *header, char const *log, va_list arg) {
  fprintf(file, "[TIME]: %s: ", header);
  vfprintf(file, log, arg);
  fprintf(file, "\n");
}

void FR_log(char logger, char const *log, ...) {
  va_list arg;

  if (fr_logflag == LOG_NONE)
    return;

  va_start(arg, log);

  if ((fr_logflag & logger & LOG_DEBUG) == LOG_DEBUG)
    __FR_vlog(stdout, "(DD)", log, arg);
  if ((fr_logflag & logger & LOG_WARNING) == LOG_WARNING)
    __FR_vlog(stdout, "(WW)", log, arg);
  if ((fr_logflag & logger & LOG_INT_ERROR) == LOG_INT_ERROR)
    __FR_vlog(stderr, "(IE)", log, arg);
  if ((fr_logflag & logger & LOG_EXT_ERROR) == LOG_EXT_ERROR)
    __FR_vlog(stderr, "(EE)", log, arg);
    va_end(arg);
}
