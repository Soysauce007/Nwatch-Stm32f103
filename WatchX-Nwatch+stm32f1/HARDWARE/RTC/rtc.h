#ifndef __RTC_H
#define __RTC_H
#include "sys.h" 
#include "typedefs.h"

timeDate_s get_rtc(void);

void save_rtc(timeDate_s newtime);

#endif   //__RTC_H

