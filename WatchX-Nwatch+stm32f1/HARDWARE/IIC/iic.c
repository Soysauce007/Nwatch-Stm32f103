#include "iic.h"
#include "delay.h"

/******************************************************************************
* Function Name --> IIC_GPIO_Init
* Description   --> GPIO初始化
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void IIC_GPIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(IIC_SCL_PORT_RCC|IIC_SDA_PORT_RCC,ENABLE);
    
	GPIO_InitStructure.GPIO_Pin=IIC_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(IIC_SCL_PORT,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
 
	IIC_SDA = 1;  //置IIC总线空闲
	IIC_SCL = 1;
}
 
/******************************************************************************
* Function Name --> SDA_OUT
* Description   --> SDA输出配置	
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void IIC_SDA_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
}
 
/******************************************************************************
* Function Name --> SDA_IN
* Description   --> SDA输入配置
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void IIC_SDA_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
}
 
/******************************************************************************
* Function Name --> IIC启动
* Description   --> SCL高电平期间，SDA由高电平突变到低电平时启动总线
*                   SCL: __________
*                                  \__________
*                   SDA: _____
*                             \_______________
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void IIC_Start(void)
{
	IIC_SDA_OUT();	//设置成输出
 
	IIC_SDA = 1;	//为SDA下降启动做准备
	IIC_SCL = 1;	//在SCL高电平时，SDA为下降沿时候总线启动
	
#if _USER_DELAY_CLK==1  /* 定义了则使用延时函数来改变通讯频率 */
	
	IIC_Delay();
	IIC_SDA = 0;	//突变，总线启动
	IIC_Delay();
	IIC_SCL = 0;
	IIC_Delay();
 
#else  /* 否则不使用延时函数改变通讯频率 */
	
	IIC_SDA = 0;	//突变，总线启动
	IIC_SCL = 0;
 
#endif  /* end __USER_DELAY_CLK */
}
 
/******************************************************************************
* Function Name --> IIC停止
* Description   --> SCL高电平期间，SDA由低电平突变到高电平时停止总线
*                   SCL: ____________________
*                                  __________
*                   SDA: _________/
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void IIC_Stop(void)
{
	IIC_SDA_OUT();	//设置成输出
 
	IIC_SDA = 0;	//为SDA上升做准备
	
#if _USER_DELAY_CLK==1  /* 定义了则使用延时函数来改变通讯频率 */
 
	IIC_Delay();
	IIC_SCL = 1;	//在SCL高电平时，SDA为上升沿时候总线停止
	IIC_Delay();
	IIC_SDA = 1;	//突变，总线停止
	IIC_Delay();
 
#else  /* 否则不使用延时函数改变通讯频率 */
 
	IIC_SCL = 1;	//在SCL高电平时，SDA为上升沿时候总线停止
	IIC_SDA = 1;	//突变，总线停止
 
#endif  /* end __USER_DELAY_CLK */
}
 
/******************************************************************************
* Function Name --> 主机向从机发送应答信号
* Description   --> none
* Input         --> a：应答信号
*                      0：应答信号
*                      1：非应答信号
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void IIC_Ack(u8 a)
{
	IIC_SDA_OUT();	//设置成输出
 
	if(a)	IIC_SDA = 1;	//放上应答信号电平
	else	IIC_SDA = 0;
	
#if _USER_DELAY_CLK==1  /* 定义了则使用延时函数来改变通讯频率 */
 
	IIC_Delay();
	IIC_SCL = 1;	//为SCL下降做准备
	IIC_Delay();
	IIC_SCL = 0;	//突变，将应答信号发送过去
	IIC_Delay();
 
#else  /* 否则不使用延时函数改变通讯频率 */
 
	IIC_SCL = 1;	//为SCL下降做准备
	IIC_SCL = 0;	//突变，将应答信号发送过去
 
#endif  /* end __USER_DELAY_CLK */
}
 
/******************************************************************************
* Function Name --> 向IIC总线发送一个字节数据
* Description   --> none
* Input         --> dat：要发送的数据
* Output        --> none
* Reaturn       --> ack：返回应答信号
******************************************************************************/
u8 IIC_Write_Byte(u8 dat)
{
	u8 i;
	u8 iic_ack=0;	//iic应答标志
 
	IIC_SDA_OUT();	//设置成输出
 
	for(i = 0;i < 8;i++)
	{
		if(dat & 0x80)	IIC_SDA = 1;	//判断发送位，先发送高位
		else	IIC_SDA = 0;
 
#if _USER_DELAY_CLK==1  /* 定义了则使用延时函数来改变通讯频率 */
			
		IIC_Delay();
		IIC_SCL = 1;	//为SCL下降做准备
		IIC_Delay();
		IIC_SCL = 0;	//突变，将数据位发送过去
		dat<<=1;	//数据左移一位
	}	//字节发送完成，开始接收应答信号
 
	IIC_SDA = 1;	//释放数据线
 
	IIC_SDA_IN();	//设置成输入
 
	IIC_Delay();
	IIC_SCL = 1;	//为SCL下降做准备
	IIC_Delay();
 
#else  /* 否则不使用延时函数改变通讯频率 */
 
		IIC_SCL = 1;	//为SCL下降做准备
		IIC_SCL = 0;	//突变，将数据位发送过去
		dat<<=1;	//数据左移一位
	}	//字节发送完成，开始接收应答信号
 
	IIC_SDA = 1;	//释放数据线
 
	IIC_SDA_IN();	//设置成输入
 
	IIC_SCL = 1;	//为SCL下降做准备
 
#endif  /* end __USER_DELAY_CLK */
	
	iic_ack |= IN_SDA;	//读入应答位
	IIC_SCL = 0;
	return iic_ack;	//返回应答信号
}
 
/******************************************************************************
* Function Name --> 从IIC总线上读取一个字节数据
* Description   --> none
* Input         --> none
* Output        --> none
* Reaturn       --> x：读取到的数据
******************************************************************************/
u8 IIC_Read_Byte(void)
{
	u8 i;
	u8 x=0;
 
	IIC_SDA = 1;	//首先置数据线为高电平
 
	IIC_SDA_IN();	//设置成输入
 
	for(i = 0;i < 8;i++)
	{
		x <<= 1;	//读入数据，高位在前
 
#if _USER_DELAY_CLK==1  /* 定义了则使用延时函数来改变通讯频率 */
 
		IIC_Delay();
		IIC_SCL = 1;	//突变
		IIC_Delay();
		
		if(IN_SDA)	x |= 0x01;	//收到高电平
 
		IIC_SCL = 0;
		IIC_Delay();
	}	//数据接收完成
 
#else  /* 否则不使用延时函数改变通讯频率 */
 
		IIC_SCL = 1;	//突变
		
		if(IN_SDA)	x |= 0x01;	//收到高电平
 
		IIC_SCL = 0;
	}	//数据接收完成
 
#endif  /* end __USER_DELAY_CLK */
 
	IIC_SCL = 0;
 
	return x;	//返回读取到的数据
}
