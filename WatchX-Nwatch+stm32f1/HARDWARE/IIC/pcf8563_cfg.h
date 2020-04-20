#ifndef _pcf8563_h_ 
#define _pcf8563_h_
 
 
 
/****************************** Support C++ **********************************
**/
#ifdef __cplusplus
	extern "C"{
#endif		
/**
  *****************************************************************************
**/
	
 
extern unsigned char buffer1[4]; 
extern unsigned char buffer[4];        
 
/******************************************************************************
                             外部函数头文件
                  应用到不同的外设头文件请在这里修改即可                        
******************************************************************************/
 
#include "iic.h"
 
/******************************************************************************
                                 参数宏定义
******************************************************************************/
 
#define PCF8563_Check_Data                       (unsigned char)0x55  //检测用，可用其他数值
 
#define PCF8563_Write                            (unsigned char)0xa2  //写命令
#define PCF8563_Read                             (unsigned char)0xa3  //读命令，或者用（PCF8563_Write + 1）
 
//
//电源复位功能
//
#define PCF8563_PowerResetEnable                 (unsigned char)0x08
#define PCF8563_PowerResetDisable                (unsigned char)0x09
 
//
//世纪位操作定义
//
#define PCF_Century_SetBitC                      (unsigned char)0x80
 
#define PCF_Century_19xx                         (unsigned char)0x03
#define PCF_Century_20xx                         (unsigned char)0x04
 
//
//数据格式
//
#define PCF_Format_BIN                           (unsigned char)0x01
#define PCF_Format_BCD                           (unsigned char)0x02
 
//
//设置PCF8563模式用
//
#define PCF_Mode_Normal                          (unsigned char)0x05
#define PCF_Mode_EXT_CLK                         (unsigned char)0x06
 
#define PCF_Mode_INT_Alarm                       (unsigned char)0x07
#define PCF_Mode_INT_Timer                       (unsigned char)0x08
 
/******************************************************************************
                             参数寄存器地址宏定义
******************************************************************************/
 
#define PCF8563_Address_Control_Status_1         (unsigned char)0x00  //控制/状态寄存器1
#define PCF8563_Address_Control_Status_2         (unsigned char)0x01  //控制/状态寄存器2
 
#define PCF8563_Address_CLKOUT                   (unsigned char)0x0d  //CLKOUT频率寄存器
#define PCF8563_Address_Timer                    (unsigned char)0x0e  //定时器控制寄存器
#define PCF8563_Address_Timer_VAL                (unsigned char)0x0f  //定时器倒计数寄存器
 
#define PCF8563_Address_Years                    (unsigned char)0x08  //年
#define PCF8563_Address_Months                   (unsigned char)0x07  //月
#define PCF8563_Address_Days                     (unsigned char)0x05  //日
#define PCF8563_Address_WeekDays                 (unsigned char)0x06  //星期
#define PCF8563_Address_Hours                    (unsigned char)0x04  //小时
#define PCF8563_Address_Minutes                  (unsigned char)0x03  //分钟
#define PCF8563_Address_Seconds                  (unsigned char)0x02  //秒
 
#define PCF8563_Alarm_Minutes                    (unsigned char)0x09  //分钟报警
#define PCF8563_Alarm_Hours                      (unsigned char)0x0a  //小时报警
#define PCF8563_Alarm_Days                       (unsigned char)0x0b  //日报警
#define PCF8563_Alarm_WeekDays                   (unsigned char)0x0c  //星期报警
 
/******************************************************************************
                               参数屏蔽宏定义
******************************************************************************/
 
#define PCF8563_Shield_Control_Status_1          (unsigned char)0xa8
#define PCF8563_Shield_Control_Status_2          (unsigned char)0x1f
 
#define PCF8563_Shield_Seconds                   (unsigned char)0x7f
#define PCF8563_Shield_Minutes                   (unsigned char)0x7f
#define PCF8563_Shield_Hours                     (unsigned char)0x3f
 
#define PCF8563_Shield_Days                      (unsigned char)0x3f
#define PCF8563_Shield_WeekDays                  (unsigned char)0x07
#define PCF8563_Shield_Months_Century            (unsigned char)0x1f
#define PCF8563_Shield_Years                     (unsigned char)0xff
 
#define PCF8563_Shield_Minute_Alarm              (unsigned char)0x7f
#define PCF8563_Shield_Hour_Alarm                (unsigned char)0x3f
#define PCF8563_Shield_Day_Alarm                 (unsigned char)0x3f
#define PCF8563_Shield_WeekDays_Alarm            (unsigned char)0x07
 
#define PCF8563_Shield_CLKOUT_Frequency          (unsigned char)0x03
#define PCF8563_Shield_Timer_Control             (unsigned char)0x03
#define PCF8563_Shield_Timer_Countdown_Value     (unsigned char)0xff
 
/******************************************************************************
                              PCF8563寄存器结构定义                          
******************************************************************************/
 
/**
   ==================================================================
                               全部寄存器结构
   ==================================================================
**/
typedef struct
{
	unsigned char Control_Status_1;      //控制寄存器1
	unsigned char Control_Status_2;      //控制寄存器2
	unsigned char Seconds;               //秒寄存器
	unsigned char Minutes;               //分钟寄存器
	unsigned char Hours;                 //小时寄存器
	unsigned char Days;                  //日期寄存器
	unsigned char WeekDays;              //星期寄存器，数值范围：0 ~ 6
	unsigned char Months_Century;        //月份寄存器，bit7为世纪位，0：指定世纪数为20xx；1：指定世纪数为19xx
	unsigned char Years;                 //年寄存器
	unsigned char Minute_Alarm;          //分钟报警寄存器
	unsigned char Hour_Alarm;            //小时报警寄存器
	unsigned char Day_Alarm;             //日期报警寄存器
	unsigned char WeekDays_Alarm;        //星期报警寄存器，数值范围：0 ~ 6
	unsigned char CLKOUT_Frequency;      //频率管脚输出控制寄存器
	unsigned char Timer_Control;         //定时器控制寄存器
	unsigned char Timer_Countdown_Value; //定时器计数寄存器
}_PCF8563_Register_Typedef;
 
/**
   ==================================================================
                               时间信息结构
   ==================================================================
**/
typedef struct
{
	unsigned char RTC_Hours;    //小时
	unsigned char RTC_Minutes;  //分钟
	unsigned char RTC_Seconds;  //秒钟
	unsigned char Reseved;      //保留
}_PCF8563_Time_Typedef;
 
 
/**
   ==================================================================
                               日期信息结构
   ==================================================================
**/
typedef struct
{
	unsigned char RTC_Years;    //年份
	unsigned char RTC_Months;   //月份
	unsigned char RTC_Days;     //日期
	unsigned char RTC_WeekDays; //星期，数值范围：0 ~ 6
}_PCF8563_Date_Typedef;
 
 
 
/**
   ==================================================================
                               闹铃信息结构
   ==================================================================
**/
typedef struct
{
	unsigned short Reseved;               //保留
	unsigned char RTC_AlarmDays;          //日期闹铃
	unsigned char RTC_AlarmWeekDays;      //星期闹铃，数值范围：0 ~ 6
	unsigned char RTC_AlarmHours;         //小时闹铃
	unsigned char RTC_AlarmMinutes;       //分钟闹铃
	unsigned char RTC_AlarmNewState;      //闹铃开关
                                        //其值有RTC_AlarmNewState_Open、RTC_AlarmNewState_Close、RTC_AlarmNewState_Open_INT_Enable
                                        //只使用其中一个即可
	unsigned char RTC_AlarmType;          //报警类型，
                                        //其值有RTC_AlarmType_Minutes、RTC_AlarmType_Hours、RTC_AlarmType_Days、RTC_AlarmType_WeekDays
                                        //多个报警类型打开，请用或关系合并
}_PCF8563_Alarm_Typedef;
 
//
//闹铃开关
//
#define RTC_AlarmNewState_Open                   (u8)0x01
#define RTC_AlarmNewState_Close                  (u8)0x02  //闹铃关闭，并且关闭中断输出
#define RTC_AlarmNewState_Open_INT_Enable        (u8)0x04  //闹铃开启并开启中断输出
//
//定义闹铃类型
//
#define RTC_AlarmType_Minutes                    (unsigned char)0x01
#define RTC_AlarmType_Hours                      (unsigned char)0x02
#define RTC_AlarmType_Days                       (unsigned char)0x04
#define RTC_AlarmType_WeekDays                   (unsigned char)0x08
 
/**
   ==================================================================
                               频率输出信息结构
   ==================================================================
**/
typedef struct
{
	unsigned char CLKOUT_Frequency;  //输出频率选择
	                                 //PCF_CLKOUT_F32768 --> 输出32.768KHz
                                     //PCF_CLKOUT_F1024  --> 输出1024Hz
                                     //PCF_CLKOUT_F32    --> 输出32Hz
                                     //PCF_CLKOUT_F1     --> 输出1Hz
	unsigned char CLKOUT_NewState;   //输出状态
	                                 //PCF_CLKOUT_Close  --> 输出被禁止并设成高阻抗
	                                 //PCF_CLKOUT_Open   --> 输出有效
}_PCF8563_CLKOUT_Typedef;
 
//
//频率输出寄存器 --> 0x0d
//
#define PCF_CLKOUT_Open                          (unsigned char)(1<<7)     //CLKOUT输出有效
#define PCF_CLKOUT_Close                         (unsigned char)(~(1<<7))  //CLKOUT输出被禁止并设成高阻抗
 
#define PCF_CLKOUT_F32768                        (unsigned char)0x00  //输出32.768KHz
#define PCF_CLKOUT_F1024                         (unsigned char)0x01  //输出1024Hz
#define PCF_CLKOUT_F32                           (unsigned char)0x02  //输出32Hz
#define PCF_CLKOUT_F1                            (unsigned char)0x03  //输出1Hz
 
/**
   ==================================================================
                               定时器信息结构
   ==================================================================
**/
typedef struct
{
	unsigned char RTC_Timer_Value;      //定时器计数器数值，设置定时时长，不需要的直接填0即可
	unsigned char RTC_Timer_Frequency;  //定时器工作频率
	                                    //PCF_Timer_F4096 --> 定时器时钟频率为4096Hz
	                                    //PCF_Timer_F64   --> 定时器时钟频率为64Hz
	                                    //PCF_Timer_F1    --> 定时器时钟频率为1Hz
	                                    //PCF_Timer_F160  --> 定时器时钟频率为1/60Hz
	unsigned char RTC_Timer_NewState;   //开启状态
	                                    //PCF_Timer_Close  --> 关闭
	                                    //PCF_Timer_Open   --> 开启
	unsigned char RTC_Timer_Interrupt;  //是否设置中断输出
	                                    //PCF_Time_INT_Close --> 关闭
	                                    //PCF_Time_INT_Open  --> 开启
}_PCF8563_Timer_Typedef;
 
//
//定时器控制寄存器 --> 0x0e
//
#define PCF_Timer_Open                           (unsigned char)(1<<7)     //定时器有效
#define PCF_Timer_Close                          (unsigned char)(~(1<<7))  //定时器无效
 
#define PCF_Timer_F4096                          (unsigned char)0x00  //定时器时钟频率为4096Hz
#define PCF_Timer_F64                            (unsigned char)0x01  //定时器时钟频率为64Hz
#define PCF_Timer_F1                             (unsigned char)0x02  //定时器时钟频率为1Hz
#define PCF_Timer_F160                           (unsigned char)0x03  //定时器时钟频率为1/60Hz
 
//
//中断输出开关
//
#define PCF_Time_INT_Open                        (unsigned char)(1<<0)     //定时器中断有效
#define PCF_Time_INT_Close                       (unsigned char)(~(1<<0))  //定时器中断无效
 
/******************************************************************************
                                 参数宏定义
******************************************************************************/
 
//
//控制/状态寄存器1 --> 0x00
//
#define PCF_Control_Status_NormalMode            (unsigned char)(~(1<<7))  //普通模式
#define PCF_Control_Status_EXT_CLKMode           (unsigned char)(1<<7)     //EXT_CLK测试模式
#define PCF_Control_ChipRuns                     (unsigned char)(~(1<<5))  //芯片运行
#define PCF_Control_ChipStop                     (unsigned char)(1<<5)     //芯片停止运行，所有芯片分频器异步置逻辑0
#define PCF_Control_TestcClose                   (unsigned char)(~(1<<3))  //电源复位功能失效（普通模式时置逻辑0）
#define PCF_Control_TestcOpen                    (unsigned char)(1<<3)     //电源复位功能有效
 
//
//控制/状态寄存器2 --> 0x01
//
#define PCF_Control_TI_TF1                       (unsigned char)(~(1<<4))  //当TF有效时INT有效，（取决于TIE的状态）
#define PCF_Control_TI_TF2                       (unsigned char)(1<<4)     //INT脉冲有效，（取决于TIE的状态）
                                                                           //注意：若AF和AIE有有效时，则INT一直有效
#define PCF_Control_ClearAF                      (unsigned char)(~(1<<3))  //清除报警
#define PCF_Control_ClearTF                      (unsigned char)(~(1<<2))
                                                                           //当报警发生时，AF被值逻辑1；在定时器倒计数结束时，
                                                                           //TF被值逻辑1，他们在被软件重写前一直保持原有值，
                                                                           //若定时器和报警中断都请求时，中断源有AF和TF决定，
                                                                           //若要使清除一个标志位而防止另一标志位被重写，应运用逻辑
                                                                           //指令AND
#define PCF_Alarm_INT_Open                       (unsigned char)(1<<1)     //报警中断有效
#define PCF_Alarm_INT_Close                      (unsigned char)(~(1<<1))  //报警中断无效
 
//
//秒寄存器 --> 0x02
//
#define PCF_Accuracy_ClockNo                     (unsigned char)(1<<7)     //不保证准确的时钟/日历数据
#define PCF_Accuracy_ClockYes                    (unsigned char)(~(1<<7))  //保证准确的时钟/日历数据
 
//
//分钟闹铃寄存器 --> 0x09
//
#define PCF_Alarm_MinutesOpen                    (unsigned char)(~(1<<7))  //分钟报警有效
#define PCF_Alarm_MinutesClose                   (unsigned char)(1<<7)     //分钟报警无效
 
//
//小时闹铃寄存器 --> 0x0a
//
#define PCF_Alarm_HoursOpen                      (unsigned char)(~(1<<7))  //小时报警有效
#define PCF_Alarm_HoursClose                     (unsigned char)(1<<7)     //小时报警无效
 
//
//日期闹铃寄存器 --> 0x0b
//
#define PCF_Alarm_DaysOpen                       (unsigned char)(~(1<<7))  //日报警有效
#define PCF_Alarm_DaysClose                      (unsigned char)(1<<7)     //日报警无效
 
//
//星期闹铃寄存器 --> 0x0c
//
#define PCF_Alarm_WeekDaysOpen                   (unsigned char)(~(1<<7))  //星期报警有效
#define PCF_Alarm_WeekDaysClose                  (unsigned char)(1<<7)     //星期报警无效
 
/******************************************************************************
                                  外部功能函数                      
******************************************************************************/
 
void          PCF8563_Write_Byte                 (unsigned char REG_ADD, unsigned char dat);  //PCF8563某寄存器写入一个字节数据
unsigned char PCF8563_Read_Byte                  (unsigned char REG_ADD);  //PCF8563某寄存器读取一个字节数据
void          PCF8563_Write_nByte                (unsigned char REG_ADD, unsigned char num, unsigned char *pBuff);  //PCF8563写入多组数据
void          PCF8563_Read_nByte                 (unsigned char REG_ADD, unsigned char num, unsigned char *pBuff);  //PCF8563读取多组数据
 
unsigned char PCF8563_Check                      (void);  //PCF8563检测是否存在
void          PCF8563_Start                      (void);  //PCF8563启动
void          PCF8563_Stop                       (void);  //PCF8563停止
 
void          PCF8563_SetMode                    (unsigned char Mode);  //PCF8563设置运行模式
void          PCF8563_SetPowerReset              (unsigned char NewState);  //PCF8563设置电源复位功能开启与关闭
 
void          PCF8563_SetCLKOUT                  (_PCF8563_CLKOUT_Typedef* PCF_CLKOUTStruct);  //PCF8563设置输出频率
void          PCF8563_SetTimer                   (_PCF8563_Timer_Typedef* PCF_TimerStruct);  //PCF8563设置定时器
 
void          PCF8563_Set_Times                  (unsigned char PCF_Format,\
                                                  unsigned char PCF_Century,\
	                                              unsigned char Year, unsigned char Month, unsigned char Date, unsigned char Week,\
                                                  unsigned char Hour, unsigned char Minute);  //设置时间，主要用于后台调用，或者初始化时间用
 
void          PCF8563_SetRegister                (unsigned char PCF_Format, unsigned char PCF_Century, _PCF8563_Register_Typedef* PCF_DataStruct);  //PCF8563写入寄存器
void          PCF8563_GetRegister                (unsigned char PCF_Format, unsigned char *PCF_Century, _PCF8563_Register_Typedef* PCF_DataStruct);  //PCF8563读取寄存器
 
void          PCF8563_SetTime                    (unsigned char PCF_Format, _PCF8563_Time_Typedef* PCF_DataStruct);  //PCF8563写入时间信息
void          PCF8563_GetTime                    (unsigned char PCF_Format, _PCF8563_Time_Typedef* PCF_DataStruct);  //PCF8563读取时间信息
 
void          PCF8563_SetDate                    (unsigned char PCF_Format, unsigned char PCF_Century, _PCF8563_Date_Typedef* PCF_DataStruct);  //PCF8563写入日期信息
void          PCF8563_GetDate                    (unsigned char PCF_Format, unsigned char *PCF_Century, _PCF8563_Date_Typedef* PCF_DataStruct);  //PCF8563读取日期信息
 
void          PCF8563_SetAlarm                   (unsigned char PCF_Format, _PCF8563_Alarm_Typedef* PCF_DataStruct);  //PCF8563写入闹铃信息
void          PCF8563_GetAlarm                   (unsigned char PCF_Format, _PCF8563_Alarm_Typedef* PCF_DataStruct);  //PCF8563读取闹铃信息
                                                  
void PCF8563_SetDT( void );   
void PF8563_GetFunc( _PCF8563_Time_Typedef* PCF_TimeStruct, _PCF8563_Date_Typedef* PCF_DataStruct );                                                  
 
 
/**
  ****************************** Support C++ **********************************
**/
#ifdef __cplusplus
	}
#endif
/**
  *****************************************************************************
**/
 
 
#endif  /* end pcf8563.h */

	
	
	
