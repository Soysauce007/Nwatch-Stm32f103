#include "pcf8563_cfg.h"
 
 unsigned char buffer1[4]; 
 unsigned char buffer[4];     
 
 
//首先BIN转BCD功能：
static unsigned char RTC_BinToBcd2(unsigned char BINValue)
{
	unsigned char bcdhigh = 0;
	
	while (BINValue >= 10)
	{
		bcdhigh++;
		BINValue -= 10;
	}
    
	return ((unsigned char)(bcdhigh << 4) | BINValue);
}
//.然后BCD转BIN功能：
static unsigned char RTC_Bcd2ToBin(unsigned char BCDValue)
{
	unsigned char tmp = 0;
	
	tmp = ((unsigned char)(BCDValue & (unsigned char)0xF0) >> (unsigned char)0x04) * 10;
	return (tmp + (BCDValue & (unsigned char)0x0F));
}

//然后PCF8563某寄存器写入一个字节数据
void PCF8563_Write_Byte(unsigned char REG_ADD, unsigned char dat)
{
	IIC_Start();                          //启动IIC
	if(!(IIC_Write_Byte(PCF8563_Write)))  //发送写命令并检查应答位
	{
		IIC_Write_Byte(REG_ADD);          //写入地址
		IIC_Write_Byte(dat);              //发送数据
	}
	IIC_Stop();                           //关闭IIC
} 
//PCF8563某寄存器读取一个字节数据
unsigned char PCF8563_Read_Byte(unsigned char REG_ADD)
{
	unsigned char ReData;                 //定义变量接收数据
	
	IIC_Start();                          //启动IIC
	if(!(IIC_Write_Byte(PCF8563_Write)))  //发送写命令并检查应答位
	{
		IIC_Write_Byte(REG_ADD);          //确定要操作的寄存器
		IIC_Start();                      //重启总线
		IIC_Write_Byte(PCF8563_Read);     //发送读取命令
		ReData = IIC_Read_Byte( );        //读取数据
		IIC_Ack(1);                       //发送非应答信号结束数据传送
	}
	IIC_Stop();                           //关闭IIC
	return ReData;
}

//PCF8563写入多组数据
void PCF8563_Write_nByte(unsigned char REG_ADD, unsigned char num, unsigned char *pBuff)
{
	unsigned char i = 0;                  //定义i辅助循环发送
		 	
	IIC_Start();                          //启动IIC
	if(!(IIC_Write_Byte(PCF8563_Write)))  //发送写命令并检查应答位
	{
		IIC_Write_Byte(REG_ADD);          //定位起始寄存器地址
		for(i = 0;i < num;i++)            //循环num次，达到多个数据写入
		{
			IIC_Write_Byte(*pBuff);       //写入数据
			pBuff++;                      //数据缓存地址增加
		}
	}
	IIC_Stop();                           //关闭IIC
}

//PCF8563读取多组数据
void PCF8563_Read_nByte(unsigned char REG_ADD, unsigned char num, unsigned char *pBuff)
{
	unsigned char i = 0;                  //定义变量i辅助多次读取
	
	IIC_Start();                          //启动IIC
	if(!(IIC_Write_Byte(PCF8563_Write)))  //发送写命令并检查应答位
	{
		IIC_Write_Byte(REG_ADD);          //定位起始寄存器地址
		IIC_Start();                      //重启总线
		IIC_Write_Byte(PCF8563_Read);     //发送读取命令
		for(i = 0;i < num;i++)            //循环num次完成多次读取
		{
			*pBuff = IIC_Read_Byte();     //读取数据
			if(i == (num - 1)) IIC_Ack(1);//发送非应答信号，知道完成数据量
			else IIC_Ack(0);              //发送应答信号
			pBuff++;                      //读取缓冲区地址增加
		}
	}
	IIC_Stop();	                          //关闭IIC
}

//PCF8563检测是否存在
unsigned char PCF8563_Check(void)
{
	unsigned char test_value = 0;
	unsigned char Time_Count = 0;          //定时器倒计时数据缓存
 
	if(PCF8563_Read_Byte(PCF8563_Address_Timer) & 0x80)             //如果打开了定时器，则先关闭
	{
		PCF8563_Write_Byte(PCF8563_Address_Timer, PCF_Timer_Close); //先关闭定时器
		Time_Count = PCF8563_Read_Byte(PCF8563_Address_Timer_VAL);  //先保存计数值
	}
 
	PCF8563_Write_Byte(PCF8563_Address_Timer_VAL, PCF8563_Check_Data);  //写入检测值
	for(test_value = 0;test_value < 50;test_value++)  {}  //延时一定时间再读取
	test_value = PCF8563_Read_Byte(PCF8563_Address_Timer_VAL);  //再读取回来
 
	if(Time_Count != 0)  //启动了定时器功能，则恢复
	{
		PCF8563_Write_Byte(PCF8563_Address_Timer_VAL, Time_Count);  //恢复现场
		PCF8563_Write_Byte(PCF8563_Address_Timer, PCF_Timer_Open);  //启动定时器
	}
 
	if(test_value != PCF8563_Check_Data)  return 1;  //器件错误或者损坏
	
	return 0;  //正常
}

//PCF8563启动
void PCF8563_Start(void)
{
	unsigned char temp = 0;
	
	temp = PCF8563_Read_Byte(PCF8563_Address_Control_Status_1);  //读取控制/状态寄存器1
	if (temp & PCF_Control_ChipStop)
	{
		temp &= PCF_Control_ChipRuns;  //运行芯片
	}
	if ((temp & (1<<7)) == 0)  //普通模式
	{
		temp &= PCF_Control_TestcClose;  //电源复位模式失效
	}
	PCF8563_Write_Byte(PCF8563_Address_Control_Status_1, temp);  //再写入数值
}

//PCF8563停止
void PCF8563_Stop(void)
{
	unsigned char temp = 0;
	
	temp = PCF8563_Read_Byte(PCF8563_Address_Control_Status_1);  //读取控制/状态寄存器1
	temp |= PCF_Control_ChipStop;  //停止运行
	PCF8563_Write_Byte(PCF8563_Address_Control_Status_1, temp);  //再写入数值
}

//PCF8563设置运行模式
void PCF8563_SetMode(unsigned char Mode)
{
	unsigned char temp = 0;
	
	temp = PCF8563_Read_Byte(PCF8563_Address_Control_Status_1);  //读取寄存器值
	if (Mode == PCF_Mode_EXT_CLK)  //EXT_CLK测试模式
	{
		temp |= PCF_Control_Status_EXT_CLKMode;
	}
	else if (Mode == PCF_Mode_Normal)
	{
		temp &= PCF_Control_Status_NormalMode;
		temp &= ~(1<<3);  //电源复位功能失效
	}
	PCF8563_Write_Byte(PCF8563_Address_Control_Status_1, temp);
}


//PCF8563设置电源复位功能开启与关闭
void PCF8563_SetPowerReset(unsigned char NewState)
{
	unsigned char TestC = 0;
	
	TestC = PCF8563_Read_Byte(PCF8563_Address_Control_Status_1);  //获取寄存器值
	TestC &= ~(1<<3);  //清除之前设置
	if (NewState == PCF8563_PowerResetEnable)  //复位功能有效
	{
		TestC |= PCF8563_PowerResetEnable;
	}
	else if (NewState == PCF8563_PowerResetDisable)
	{
		TestC &= ~PCF8563_PowerResetEnable;  //失效，普通模式是值逻辑0，即失效
	}
	PCF8563_Write_Byte(PCF8563_Address_Control_Status_1, TestC);  //写入数值
}


//PCF8563设置输出频率
void PCF8563_SetCLKOUT(_PCF8563_CLKOUT_Typedef* PCF_CLKOUTStruct)
{
	unsigned char tmp = 0;
	
	tmp = PCF8563_Read_Byte(PCF8563_Address_CLKOUT);  //读取寄存器值
	tmp &= 0x7c;  //清除之前设置
	if (PCF_CLKOUTStruct->CLKOUT_NewState == PCF_CLKOUT_Open)
	{
		tmp |= PCF_CLKOUT_Open;
	}
	else
	{
		tmp &= PCF_CLKOUT_Close;
	}
	tmp |= PCF_CLKOUTStruct->CLKOUT_Frequency;
	
	PCF8563_Write_Byte(PCF8563_Address_CLKOUT, tmp);
}

//PCF8563设置定时器
void PCF8563_SetTimer(_PCF8563_Timer_Typedef* PCF_TimerStruct)
{
	unsigned char Timer_Ctrl = 0;
	unsigned char Timer_Value = 0;
	
	Timer_Ctrl = PCF8563_Read_Byte(PCF8563_Address_Timer);  //获的控制寄存器值
	Timer_Value = PCF8563_Read_Byte(PCF8563_Address_Timer_VAL);  //获取倒计时数值
	//
	//先停止定时器
	//
	Timer_Ctrl &= PCF_Timer_Close;
	PCF8563_Write_Byte(PCF8563_Address_Timer, Timer_Ctrl);
	
	Timer_Ctrl &= 0x7c;  //清除定时器之前设置
	
	if (PCF_TimerStruct->RTC_Timer_NewState == PCF_Timer_Open)  //开启
	{
		Timer_Ctrl |= PCF_Timer_Open;
		Timer_Ctrl |= PCF_TimerStruct->RTC_Timer_Frequency;  //填上新的工作频率
		if (PCF_TimerStruct->RTC_Timer_Value)  //需要填上新的计数值
		{
			Timer_Value = PCF_TimerStruct->RTC_Timer_Value;  //填上新的计数值
		}
	}
	else
	{
		Timer_Ctrl &= PCF_Timer_Close;
	}
	PCF8563_Write_Byte(PCF8563_Address_Timer_VAL, Timer_Value);  //写入倒计时数值
	
	if (PCF_TimerStruct->RTC_Timer_Interrupt == PCF_Time_INT_Open)  //开启了中断输出
	{
		Timer_Value = PCF8563_Read_Byte(PCF8563_Address_Control_Status_2);  //获取控制/状态寄存器2数值
		Timer_Value &= PCF_Time_INT_Close;  //清除定时器中断使能
		Timer_Value &= ~(1<<2);  //清除定时器中断标志
		Timer_Value &= ~(1<<4);  //当 TF 有效时 INT 有效 (取决于 TIE 的状态) 
		Timer_Value |= PCF_Time_INT_Open;  //开启定时器中断输出
		PCF8563_Write_Byte(PCF8563_Address_Control_Status_2, Timer_Value);
	}
	else
	{
		Timer_Value = PCF8563_Read_Byte(PCF8563_Address_Control_Status_2);  //获取控制/状态寄存器2数值
		Timer_Value &= PCF_Time_INT_Close;  //清除定时器中断使能
		Timer_Value |= PCF_Time_INT_Open;  //开启定时器中断输出
		PCF8563_Write_Byte(PCF8563_Address_Control_Status_2, Timer_Value);
	}
	
	PCF8563_Write_Byte(PCF8563_Address_Timer, Timer_Ctrl);  //设置定时器控制寄存器
}

//设置时间，主要用于后台调用，或者初始化时间用
void PCF8563_Set_Times(unsigned char PCF_Format,\
                       unsigned char PCF_Century,\
	                   unsigned char Year, unsigned char Month, unsigned char Date, unsigned char Week,\
                       unsigned char Hour, unsigned char Minute)
{
	_PCF8563_Time_Typedef Time_InitStructure;
	_PCF8563_Date_Typedef Date_InitStructure;
	
	if (PCF_Format == PCF_Format_BIN)
	{
		//
		//判断数据是否符合范围
		//
		if (Year > 99)   Year  = 0;  //恢复00年
		if (Month > 12)  Month = 1;  //恢复1月
		if (Date > 31)   Date  = 1;  //恢复1日
		if (Week > 6)    Week  = 1;  //恢复星期一
		
		if (Hour > 23)    Hour   = 0;  //恢复0小时
		if (Minute > 59)  Minute = 0;  //恢复0分钟
		
		//
		//转换一下
		//
		Date_InitStructure.RTC_Years    = RTC_BinToBcd2(Year);
		Date_InitStructure.RTC_Months   = RTC_BinToBcd2(Month);
		Date_InitStructure.RTC_Days     = RTC_BinToBcd2(Date);
		Date_InitStructure.RTC_WeekDays = RTC_BinToBcd2(Week);
		
		Time_InitStructure.RTC_Hours    = RTC_BinToBcd2(Hour);
		Time_InitStructure.RTC_Minutes  = RTC_BinToBcd2(Minute);
	}
	Time_InitStructure.RTC_Seconds = 0x00;  //恢复0秒
	Time_InitStructure.RTC_Seconds &= PCF_Accuracy_ClockYes;  //保证准确的时间
	//
	//判断世纪位
	//
	if (PCF_Century == PCF_Century_19xx)
	{
		Date_InitStructure.RTC_Months |= PCF_Century_SetBitC;
	}
	else
	{
		Date_InitStructure.RTC_Months &= ~PCF_Century_SetBitC;
	}
	//
	//写入信息到寄存器
	//
	buffer[0] = Time_InitStructure.RTC_Seconds;
	buffer[1] = Time_InitStructure.RTC_Minutes;
	buffer[2] = Time_InitStructure.RTC_Hours;
	PCF8563_Write_nByte(PCF8563_Address_Seconds, 3, buffer);  //写入时间信息
	
	buffer[0] = Date_InitStructure.RTC_Days;
	buffer[1] = Date_InitStructure.RTC_WeekDays;
	buffer[2] = Date_InitStructure.RTC_Months;
	buffer[3] = Date_InitStructure.RTC_Years;
	PCF8563_Write_nByte(PCF8563_Address_Days,    4, buffer);  //写入日期信息
}


//PCF8563写入时间信息
void PCF8563_SetTime(unsigned char PCF_Format, _PCF8563_Time_Typedef* PCF_DataStruct)
{
	if (PCF_Format == PCF_Format_BIN)  //十进制格式，需要转换一下
	{
		//
		//判断数值是否在范围之内
		//
		if (PCF_DataStruct->RTC_Hours > 23)    PCF_DataStruct->RTC_Hours   = 0;  //恢复0小时
		if (PCF_DataStruct->RTC_Minutes > 59)  PCF_DataStruct->RTC_Minutes = 0;  //恢复0分钟
		if (PCF_DataStruct->RTC_Seconds > 59)  PCF_DataStruct->RTC_Seconds = 0;  //恢复0秒
		//
		//需要转换一下
		//
		PCF_DataStruct->RTC_Hours   = RTC_BinToBcd2(PCF_DataStruct->RTC_Hours);
		PCF_DataStruct->RTC_Minutes = RTC_BinToBcd2(PCF_DataStruct->RTC_Minutes);
		PCF_DataStruct->RTC_Seconds = RTC_BinToBcd2(PCF_DataStruct->RTC_Seconds);
	}
	//
	//拷贝数据
	//
	buffer[0] = PCF_DataStruct->RTC_Seconds;
	buffer[1] = PCF_DataStruct->RTC_Minutes;
	buffer[2] = PCF_DataStruct->RTC_Hours;
	//
	//写入数据到寄存器
	//
	PCF8563_Write_nByte(PCF8563_Address_Seconds, 3, buffer);
}


//PCF8563读取时间信息
void PCF8563_GetTime(unsigned char PCF_Format, _PCF8563_Time_Typedef* PCF_DataStruct)
{
	/* 读取寄存器数值 */
	PCF8563_Read_nByte(PCF8563_Address_Seconds, 3, buffer);     //调用读多为函数，起始地址为秒，长度3，存入buffer数组
	
	/* 屏蔽无效位,将时分秒寄存器的值分别放入数组 */
	buffer[0] &= PCF8563_Shield_Seconds;
	buffer[1] &= PCF8563_Shield_Minutes;
	buffer[2] &= PCF8563_Shield_Hours;
	
	//判断需要的数据格式，
	if (PCF_Format == PCF_Format_BIN)
	{
		PCF_DataStruct->RTC_Hours   = RTC_Bcd2ToBin(buffer[2]);
		PCF_DataStruct->RTC_Minutes = RTC_Bcd2ToBin(buffer[1]);
		PCF_DataStruct->RTC_Seconds = RTC_Bcd2ToBin(buffer[0]);
	}
	else if (PCF_Format == PCF_Format_BCD)
	{
		//
		//拷贝数据
		//
		PCF_DataStruct->RTC_Hours   = buffer[2];
		PCF_DataStruct->RTC_Minutes = buffer[1];
		PCF_DataStruct->RTC_Seconds = buffer[0];
	}
}

//PCF8563写入日期信息
void PCF8563_SetDate(unsigned char PCF_Format, unsigned char PCF_Century, _PCF8563_Date_Typedef* PCF_DataStruct)
{
	if (PCF_Format == PCF_Format_BIN)  //十进制格式，需要转换一下
	{
		//
		//判断数值是否在范围之内
		//
		if (PCF_DataStruct->RTC_Years > 99)    PCF_DataStruct->RTC_Years    = 0;  //恢复00年
		if (PCF_DataStruct->RTC_Months > 12)   PCF_DataStruct->RTC_Months   = 1;  //恢复1月
		if (PCF_DataStruct->RTC_Days > 31)     PCF_DataStruct->RTC_Days     = 1;  //恢复1日
		if (PCF_DataStruct->RTC_WeekDays > 6)  PCF_DataStruct->RTC_WeekDays = 1;  //恢复星期一
		//
		//需要转换一下
		//
		PCF_DataStruct->RTC_Years    = RTC_BinToBcd2(PCF_DataStruct->RTC_Years);
		PCF_DataStruct->RTC_Months   = RTC_BinToBcd2(PCF_DataStruct->RTC_Months);
		PCF_DataStruct->RTC_Days     = RTC_BinToBcd2(PCF_DataStruct->RTC_Days);
		PCF_DataStruct->RTC_WeekDays = RTC_BinToBcd2(PCF_DataStruct->RTC_WeekDays);
	}
	//
	//判断世纪位
	//
	if (PCF_Century == PCF_Century_19xx)
	{
		PCF_DataStruct->RTC_Months |= PCF_Century_SetBitC;
	}
	else
	{
		PCF_DataStruct->RTC_Months &= ~PCF_Century_SetBitC;
	}
	//
	//数据拷贝
	//
	buffer1[0] = PCF_DataStruct->RTC_Days;
	buffer1[1] = PCF_DataStruct->RTC_WeekDays;
	buffer1[2] = PCF_DataStruct->RTC_Months;
	buffer1[3] = PCF_DataStruct->RTC_Years;
	//
	//写入数据到寄存器
	//
	PCF8563_Write_nByte(PCF8563_Address_Days, 4, buffer1);
}

//PCF8563读取日期信息
void PCF8563_GetDate(unsigned char PCF_Format, unsigned char *PCF_Century, _PCF8563_Date_Typedef* PCF_DataStruct)
{
	//
	//读取全部寄存器数值
	//
	PCF8563_Read_nByte(PCF8563_Address_Days, 4, buffer1);
	//
	//判断世纪位数值
	//
	if (buffer1[2] & PCF_Century_SetBitC)
	{
		*PCF_Century = 1;
	}
	else
	{
		*PCF_Century = 0;
	}
	//
	//屏蔽无效位
	//
	buffer1[0] &= PCF8563_Shield_Days;
	buffer1[1] &= PCF8563_Shield_WeekDays;
	buffer1[2] &= PCF8563_Shield_Months_Century;
	buffer1[3] &= PCF8563_Shield_Years;
	
	//
	//判断需要的数据格式
	//
	if (PCF_Format == PCF_Format_BIN)
	{
		PCF_DataStruct->RTC_Years    = RTC_Bcd2ToBin(buffer1[3]);
		PCF_DataStruct->RTC_Months   = RTC_Bcd2ToBin(buffer1[2]);
		PCF_DataStruct->RTC_Days     = RTC_Bcd2ToBin(buffer1[0]);
		PCF_DataStruct->RTC_WeekDays = RTC_Bcd2ToBin(buffer1[1]);
	}
	else if (PCF_Format == PCF_Format_BCD)
	{
		//
		//拷贝数据
		//
		PCF_DataStruct->RTC_Years    = buffer1[3];
		PCF_DataStruct->RTC_Months   = buffer1[2];
		PCF_DataStruct->RTC_Days     = buffer1[0];
		PCF_DataStruct->RTC_WeekDays = buffer1[1];
	}
}



