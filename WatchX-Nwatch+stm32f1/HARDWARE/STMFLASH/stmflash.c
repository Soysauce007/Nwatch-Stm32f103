#include "stmflash.h"
#include "delay.h"
#include "usart.h" 
#include "common.h"


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
//本页内容来自：http://www.openedv.com/posts/list/45775.htm 感谢大佬们提供的资料
////////////////////////////////////////////////////////////////////////////////// 

 
 



u16  BKPDataR[]={BKP_DR2,BKP_DR3,BKP_DR4,BKP_DR5,BKP_DR6,
BKP_DR7,BKP_DR8,BKP_DR9,BKP_DR10,BKP_DR11,BKP_DR12,BKP_DR13,BKP_DR14,
BKP_DR15,BKP_DR16,BKP_DR17,BKP_DR18,BKP_DR19,BKP_DR20,BKP_DR21,
 BKP_DR22,BKP_DR23,BKP_DR24/*,BKP_DR25,BKP_DR26,BKP_DR27,BKP_DR28,BKP_DR29,
BKP_DR30,BKP_DR31,BKP_DR32,BKP_DR33,BKP_DR34,BKP_DR35,BKP_DR36*/};

//_calendar_obj calendar;//时钟结构体
/*
功能：将形参数据uint16_t Data写入某某号备份域数据寄存器uint16_t BKP_DR；
注意：在本函数中形参“uint16_t BKP_DR”是uint16_t类型，因为备份域只能读写该类型数据；
同时又代表备份域数据寄存器的寄存器号；
小容量的CM3只有20字节的存储空间，大容量84字节；对应地址偏移：0x04 到 0x28，0x40到0xBC
复位值：0x0000 0000
void BKP_WriteBackupRegister(uint16_t BKP_DR, uint16_t Data)
{
 __IO uint32_t tmp = 0;//tmp是u32类型，不是指针！只是代表指针的u32类型编号；

 //  Check the parameters
  assert_param(IS_BKP_DR(BKP_DR));
//#define BKP_BASE  (APB1PERIPH_BASE + 0x6C00);#define APB1PERIPH_BASE  PERIPH_BASE
 //#define PERIPH_BASE  ((uint32_t)0x400000 00)
  tmp = (uint32_t)BKP_BASE; //BKP_BASE =(uint32_t)0x40000000+0x6C00;
  tmp += BKP_DR;

  *(__IO uint32_t *) tmp = Data;//将tmp强制类型转换为“__IO uint32_t *”，再赋值；
}
*/

u8 WriteToBackupReg(u16 index,u16 *FirstBackupData,u8 writetonum)
{
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //使能PWR和BKP外设时钟  
	RCC_RTCCLKCmd(ENABLE);
 PWR_BackupAccessCmd(ENABLE); //使能后备寄存器访问
 RTC_WaitForLastTask(); //等待最近一次对RTC寄存器的写操作完成

  RTC_WaitForLastTask(); //等待最近一次对RTC寄存器的写操作完成
  for(;index<writetonum;index++)
   {
   // BKPDataR[0]就是BKP_DR2寄存器，即数据从BKP_DR2寄存器开始写入  
     BKP_WriteBackupRegister(BKPDataR[index],FirstBackupData[index]); 
		 
//   delay_ms(50);
   debug_printf("写入的数据=：%c\n",FirstBackupData[index]); //将写入的数据打印出来
   }
return 0;
}




void ReadToBackupReg(u16 index,u16 *FirstBackupData,u8 readtonum)
{
 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //使能PWR和BKP外设时钟  
  PWR_BackupAccessCmd(ENABLE); //使能后备寄存器访问 
  RTC_WaitForLastTask(); //等待最近一次对RTC寄存器的写操作完成

	RCC_RTCCLKCmd(ENABLE);
 for(;index<readtonum;index++)
   {
   //  FirstBackupData[index]用以存放读到的数据 ,从BKP_DR2寄存器开始读
     FirstBackupData[index]=BKP_ReadBackupRegister(BKPDataR[index]); 
 //   delay_ms(50);
   debug_printf("读到RTC备份域数据寄存器的值BKP_DRx=：%c\n",FirstBackupData[index]);
   }

}


//主程序：
//int main(void)
//{
// 

// u16 rb[20];//用以存放读到的RTC备份域数据寄存器数据
////u16  buf[]={0x12a,0x13a,0x14a,0x15a,0x16a,0x17a,0x18a,0x19a,0x45,0x34,
////0xaaaa,0xbbbb,0xcccc,0x12a,0x13a,0x14a,0x15a,0x16a,0x17a,0x18a,0x19a};
// u16 n, m=0;
//  u8  buf[]={"hello!i'm wangyan."};

//   
// uart_init(9600);
// delay_init();
// //  RTC_Init();
// 

//  WriteToBackupReg((u16*)buf,sizeof(buf));// 写入缓冲字符串
//    delay_ms(1000); 
//    
////    printf("写入的数据数sizeof(buf)=：%d\n",sizeof(buf));
//  
//   delay_ms(1000);
//   ReadToBackupReg(rb,sizeof(buf));//  从BKP_DR2寄存器开始读,一共读出20个数据
//      delay_ms(1000);


// /* 单独读出某个寄存器的值用以验证是否写入成功；打印出12a */
// //  m=BKP_ReadBackupRegister(BKP_DR14);
////    printf("读到RTC备份域数据寄存器的值BKP_DR14=：%x\n",m);
//  //   n=BKP_ReadBackupRegister(BKP_DR1);
////    printf("读到RTC备份域数据寄存器的值BKP_DR1=：%x\n",n);
// while (1)
// {   };

//}









