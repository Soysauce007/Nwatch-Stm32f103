#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h" 

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//STM32内部FLASH读写 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 




u8 WriteToBackupReg(u16 index,u16 *FirstBackupData,u8 writetonum);

void ReadToBackupReg(u16 index,u16 *FirstBackupData,u8 readtonum);




#endif

















