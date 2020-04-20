/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2014 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"
#include "delay.h"
#include "sys.h"
#include "rtc.h"
#include "led.h"
#include "iic.h"
#include "pcf8563_cfg.h"


//#if RTC_SRC == RTC_SRC_INTERNAL

void rtc_init()
{
	IIC_GPIO_Init();
	PCF8563_Start();
	PCF8563_SetMode(PCF_Mode_Normal);
}

// dont forget about millis()!
// square wave
// should tick twice per sec
// 1 per sec when in sleep

void rtc_tempUpdate()
{
	// read internal temperature sensor
}

void rtc_tempGet(rtc_temperature_s* temp)
{
	temp->whole = 0;
	temp->frac = 0;
}


void rtc_save(timeDate_s *timeDate)
{
	
  save_rtc(*timeDate);
	led_flash(LED_RED, LED_FLASH_FAST, LED_BRIGHTNESS_MAX);

	
}	
void rtc_get(timeDate_s *timeDate)
{
	
	*timeDate =get_rtc();
	
}
void rtc_sqw(rtc_sqw_t sqw)
{

}	

void rtc_setUserAlarmWake(alarm_s *alarm)
{
//  RTC_Alarm_Set(2020,02 , alarm->days,alarm->hour,alarm->min,0);
}	

void rtc_setSystemAlarmWake(alarm_s *alarm)
{
//  RTC_Alarm_Set(2020,02 , alarm->days,alarm->hour,alarm->min,0);
}

bool rtc_userAlarmState()
{
 return RTC_GetFlagStatus(RTC_IT_ALR);
}

bool rtc_systemAlarmState()
{
  return RTC_GetFlagStatus(RTC_IT_ALR);
}


//	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);//πÿ±’ƒ÷÷”A 




//#endif
