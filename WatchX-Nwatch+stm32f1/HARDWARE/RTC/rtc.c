#include "rtc.h"
#include "pcf8563_cfg.h"
#include "beep.h"


timeDate_s get_rtc(void)
{
	_PCF8563_Time_Typedef rtcTime;
	_PCF8563_Date_Typedef rtcDate;
	timeDate_s returnTime;
	unsigned char PCF_Century;
//PCF8563读取时间信息
  PCF8563_GetTime(PCF_Format_BIN,&rtcTime);  //以二进制读取
//PCF8563读取日期信息
  PCF8563_GetDate(PCF_Format_BIN,&PCF_Century,&rtcDate);

	returnTime.time.secs =rtcTime.RTC_Seconds;
	returnTime.time.mins =rtcTime.RTC_Minutes;
	returnTime.time.hour =rtcTime.RTC_Hours;
	returnTime.time.ampm = 0;
	returnTime.date.date = rtcDate.RTC_WeekDays;
	returnTime.date.day  = (day_t)rtcDate.RTC_Days;
	returnTime.date.month= (month_t)rtcDate.RTC_Months;
	returnTime.date.year = rtcDate.RTC_Years;

	
	if(returnTime.time.hour>21 ||returnTime.time.hour<6)
	{
		
		  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟 
  //初始化蜂鸣器对应引脚GPIOA1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//普通输出模式//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	}
	else
	{
				  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟 
  //初始化蜂鸣器对应引脚GPIOA1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//普通输出模式//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
  }
	
return returnTime;
	
}



void save_rtc(timeDate_s newtime)
{
	_PCF8563_Time_Typedef rtcTime;
	_PCF8563_Date_Typedef rtcDate;

	rtcTime.RTC_Seconds=newtime.time.secs ;
	rtcTime.RTC_Minutes=newtime.time.mins ;
	rtcTime.RTC_Hours=newtime.time.hour ;
	rtcDate.RTC_WeekDays=newtime.date.date ;
	rtcDate.RTC_Days=(day_t)newtime.date.day  ;
	rtcDate.RTC_Months=(month_t)newtime.date.month;
	rtcDate.RTC_Years=newtime.date.year ;

	
//PCF8563设置时间信息
  PCF8563_SetTime(PCF_Format_BIN,&rtcTime);  //以二进制读取
//PCF8563设置日期信息
  PCF8563_SetDate(PCF_Format_BIN,PCF_Century_20xx,&rtcDate);
	
	
return ;
	
}


