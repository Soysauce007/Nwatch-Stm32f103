/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/ 
 */
#include "delay.h"
#include "common.h"



#define LENTH(Buffer) sizeof(Buffer)	 		  	//数组长度	

#define EEPROM_CHECK_NUM 0x1234
 
#define BKPCheck_SAVE_ADDR   BKP_DR1 	//设置BKP第一个地址保持第一次读写的标志位

appconfig_s appConfig;     //appconfig_s的长度为8


//static appconfig_s eepConfig EEMEM = {
//	0, // sleep timeout (this value * 5 = sleep timeout in secs)
//	false, // invert
//#if COMPILE_ANIMATIONS
//	true, // animations
//#endif
//	true, // 180 rotate
//	false, // FPS
//	TIMEMODE_24HR,
//	{
//		255 // volume + brightness (all max)
//	}
//};

u16  BKPbuf[8];

u16  BKPDataRR[]={BKP_DR2,BKP_DR3,BKP_DR4,BKP_DR5,BKP_DR6,
BKP_DR7,BKP_DR8,BKP_DR9,BKP_DR10,BKP_DR11,BKP_DR12,BKP_DR13,BKP_DR14,
BKP_DR15,BKP_DR16,BKP_DR17,BKP_DR18,BKP_DR19,BKP_DR20,BKP_DR21,
 BKP_DR22,BKP_DR23,BKP_DR24/*,BKP_DR25,BKP_DR26,BKP_DR27,BKP_DR28,BKP_DR29,
BKP_DR30,BKP_DR31,BKP_DR32,BKP_DR33,BKP_DR34,BKP_DR35,BKP_DR36*/};


void appconfig_init()
{
	 u16 BKPCheck  ;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //使能PWR和BKP外设时钟  
  RTC_WaitForLastTask(); //等待最近一次对RTC寄存器的写操作完成
	  PWR_BackupAccessCmd(ENABLE); //使能后备寄存器访问 

  BKPCheck =BKP_ReadBackupRegister(BKPCheck_SAVE_ADDR); 

		
  appConfig = *(appconfig_s *) malloc(sizeof(appconfig_s));
  memset(&appConfig, 0x00, LENTH(appconfig_s));

	if(BKPCheck == EEPROM_CHECK_NUM)
	{
//		ReadToBackupReg(BKPbuf,LENTH(BKPbuf));
		int index =0;
	appConfig.sleepTimeout = BKP_ReadBackupRegister(BKPDataRR[index]); index++;
	appConfig.invert       = BKP_ReadBackupRegister(BKPDataRR[index]); index++;
#if COMPILE_ANIMATIONS
	appConfig.animations  =  BKP_ReadBackupRegister(BKPDataRR[index]); index++;
#endif
	appConfig.display180  =  BKP_ReadBackupRegister(BKPDataRR[index]); index++;
  appConfig.CTRL_LEDs   =  BKP_ReadBackupRegister(BKPDataRR[index]); index++;
	appConfig.showFPS     =  BKP_ReadBackupRegister(BKPDataRR[index]); index++;
	appConfig.timeMode    = (timemode_t) BKP_ReadBackupRegister(BKPDataRR[index]); index++;
	appConfig.volumes     =  BKP_ReadBackupRegister(BKPDataRR[index]); index++;
		
	}

	else
	{
    BKPCheck = EEPROM_CHECK_NUM;
    BKP_WriteBackupRegister(BKPCheck_SAVE_ADDR,EEPROM_CHECK_NUM);	
		
		
   memset(&BKPbuf, 0x0000, LENTH(BKPbuf));
		
					int index =0;
	 BKP_WriteBackupRegister(BKPDataRR[index],BKPbuf[index]); index++;
	 BKP_WriteBackupRegister(BKPDataRR[index],BKPbuf[index]); index++;
#if COMPILE_ANIMATIONS
	BKP_WriteBackupRegister(BKPDataRR[index],BKPbuf[index]); index++;
#endif
	BKP_WriteBackupRegister(BKPDataRR[index],BKPbuf[index]); index++;
  BKP_WriteBackupRegister(BKPDataRR[index],BKPbuf[index]); index++;
	BKP_WriteBackupRegister(BKPDataRR[index],BKPbuf[index]); index++;
	BKP_WriteBackupRegister(BKPDataRR[index],BKPbuf[index]); index++;
	BKP_WriteBackupRegister(BKPDataRR[index],BKPbuf[index]); index++;
		
		appconfig_reset();
	}

	
	
	
	if(appConfig.sleepTimeout > 12)
		appConfig.sleepTimeout = 0;
}

void appconfig_save()
{			   

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //使能PWR和BKP外设时钟  
  RTC_WaitForLastTask(); //等待最近一次对RTC寄存器的写操作完成
	PWR_BackupAccessCmd(ENABLE); //使能后备寄存器访问 
	
	 BKPbuf[0]=(u16)appConfig.sleepTimeout;
	 BKPbuf[1]=(u16)appConfig.invert ;
#if COMPILE_ANIMATIONS
	 BKPbuf[2]=(u16)appConfig.animations;
#endif
	 BKPbuf[3]=(u16)appConfig.display180;
   BKPbuf[4]=(u16)appConfig.CTRL_LEDs;
	 BKPbuf[5]=(u16)appConfig.showFPS;
	 BKPbuf[6]=(u16)appConfig.timeMode ;
	 BKPbuf[7]=(u16)appConfig.volumes;
	
	
			int index =0;
	 BKP_WriteBackupRegister(BKPDataRR[index],BKPbuf[index]); index++;
	 BKP_WriteBackupRegister(BKPDataRR[index],BKPbuf[index]); index++;
#if COMPILE_ANIMATIONS
	BKP_WriteBackupRegister(BKPDataRR[index],BKPbuf[index]); index++;
#endif
	BKP_WriteBackupRegister(BKPDataRR[index],BKPbuf[index]); index++;
  BKP_WriteBackupRegister(BKPDataRR[index],BKPbuf[index]); index++;
	BKP_WriteBackupRegister(BKPDataRR[index],BKPbuf[index]); index++;
	BKP_WriteBackupRegister(BKPDataRR[index],BKPbuf[index]); index++;
	BKP_WriteBackupRegister(BKPDataRR[index],BKPbuf[index]); index++;
	
	
	
//	    WriteToBackupReg(BKPbuf,LENTH(BKPbuf));

	
}

void appconfig_reset()
{
	appConfig.sleepTimeout = 1;
	appConfig.invert = false;
#if COMPILE_ANIMATIONS
	appConfig.animations = true;
#endif
	appConfig.display180 = true;
  appConfig.CTRL_LEDs = false;
	appConfig.showFPS = true;
	appConfig.timeMode = TIMEMODE_24HR;
	appConfig.volumes = 255;
	
	appconfig_save();

	alarm_reset();
}
