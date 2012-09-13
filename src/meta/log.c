#include <meta/log.h>

void __MT_vlog(FILE *file, char const *header, char const *log, va_list arg) {
  fprintf(file, "[TIME]: %s: ", header);
  vfprintf(file, log, arg);
  fprintf(file, "\n");
}

void MT_log(char logger, char const *log, ...) {
  va_list arg;

  if (mt_logflag == LOG_NONE)
    return;

  va_start(arg, log);
  /* check if the logger is set */
  if ((mt_logflag & logger & LOG_DEBUG) == LOG_DEBUG)
    __MT_vlog(stdout, "(DD)", log, arg);
  if ((mt_logflag & logger & LOG_WARNING) == LOG_WARNING)
    __MT_vlog(stdout, "(WW)", log, arg);
  if ((mt_logflag & logger & LOG_INT_ERROR) == LOG_INT_ERROR)
    __MT_vlog(stderr, "(IE)", log, arg);
  if ((mt_logflag & logger & LOG_EXT_ERROR) == LOG_EXT_ERROR)
    __MT_vlog(stderr, "(EE)", log, arg);
    va_end(arg);
}
