#include "beep.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//蜂鸣器驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

// Buzzer
// Timer1 is used for buzzing

#include "common.h"
#include "delay.h"



static byte buzzLen;
static millis8_t startTime;
static buzzFinish_f onFinish;
static tonePrio_t prio;
static uint icr;
static uint ocr;


static void stop(void);



//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init_Init(u32 Freq)
{                                                
     


     TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
     NVIC_InitTypeDef NVIC_InitStructure;
 
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
 
     TIM_TimeBaseStructure.TIM_Period = 4000/Freq -1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值     计数到5000为500ms
     TIM_TimeBaseStructure.TIM_Prescaler =71; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
     TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
     TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
     TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
 
     NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
     NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
     TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设



                                                    
}  



//蜂鸣器初始化
//初始化PF8为输出口		    
//BEEP IO初始化
void buzzer_init()
{
	TIM3_PWM_Init_Init(1000);
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟 
  //初始化蜂鸣器对应引脚GPIOA1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//普通输出模式//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3                                                                    

  GPIO_ResetBits(GPIOA,GPIO_Pin_1);  //蜂鸣器对应引脚GPIOA1拉低， 


}

// Non-blocking buzz
void buzzer_buzz(byte len, tone_t tone, vol_t volType, tonePrio_t _prio, buzzFinish_f _onFinish)
{
	if(_prio < prio)
		return;
	else if(tone == TONE_STOP)
	{
		stop();
		return;
	}


	
	// Tell power manager that we are busy buzzing
//	pwrmgr_setState(PWR_ACTIVE_BUZZER, PWR_STATE_IDLE);

	prio = _prio;
	onFinish = _onFinish;
	buzzLen = len;
	startTime = millis();

	// Silent pause tone   //无声暂停音
	if(tone == TONE_PAUSE)
	{
//		CLEAR_BITS(TCCR1A, COM1A1, COM1A0);
//		power_timer1_disable();
  	TIM_Cmd(TIM3, DISABLE);  //使能TIM3                                                                    
	  BEEP=0; //PB.5 输出低 

		return;
	}

	// Workout volume
	byte vol;

	switch(volType)
	{
		case VOL_UI:
			vol = appConfig.volUI;
			break;
		case VOL_ALARM:
			vol = appConfig.volAlarm;
			break;
		case VOL_HOUR:
			vol = appConfig.volHour;
			break;
		default:
			vol = 2;
			break;
	}

	// Pulse width goes down as freq goes up
	// This keeps power consumption the same for all frequencies, but volume goes down as freq goes up

	vol--;
//	if(vol > 2)     //		byte volumes;初始化的适合值是255
//		return;

  icr = tone  ;
	ocr = icr * (256/vol);
	

  TIM3_PWM_Init_Init(icr);
	
//	TIFR1 = 0;
//	SET_BITS(TIMSK1, TOIE1);
//	TCNT1 = 0;
//	OCR1A = ocr;
//	ICR1 = icr;
	//SET_BITS(TCCR1A, COM1A1, COM1A0);
}
/*
#include "led.h"

void buzzer_buzzb(byte len, tone_t tone, vol_t volType)
{
	(void)(volType);

	led_flash(LED_GREEN, 50, 255);
	led_flash(LED_RED, 50, 255);

	power_timer1_enable();
	TCCR1A |= _BV(COM1A1)|_BV(COM1A0);

//	static uint vol = 0;
//	vol++;
//	if(vol > 790)
//		vol = 1;
//	OCR1A = vol;

//	if(vol > 3)
//		vol = 0;
//	if(vol == 0)
//		OCR1A = tone; // normal
//	else if(vol == 1)
//		OCR1A = (tone * 2) - 50; // quiet
//	else if(vol == 2)
//		OCR1A = (tone / 2); // loud
//	else if(vol == 3)
//		OCR1A = (tone / 4); // loader (acually quiter)

	OCR1A = (tone * 2) - 100;
	ICR1 = tone * 2;
	while(len--)
	{
		delay(1);
		led_update();
	}		
//	delay(20);
	TCCR1A &= ~(_BV(COM1A1)|_BV(COM1A0));
	power_timer1_disable();
}
*/
// Are we buzzing?
bool buzzer_buzzing()
{
	return buzzLen;
}

// See if its time to stop buzzing
void buzzer_update()
{
	
	if(buzzLen && (millis8_t)(millis() - startTime) >= buzzLen)
	{
		stop();

		if(onFinish != NULL)
			onFinish();
	}

			
	
	
}

static void stop()
{
//	CLEAR_BITS(TCCR1A, COM1A1, COM1A0);
//	power_timer1_disable();
//    TIM_SetCompare1(TIM3,0);
	 BEEP=0; //PA.1 输出低 
   TIM_Cmd(TIM3, DISABLE);  //停止TIM3 
	
	buzzLen = 0;
	prio = PRIO_MIN;

//	pwrmgr_setState(PWR_ACTIVE_BUZZER, PWR_STATE_NONE);
}

// Sometimes the buzzer kind of 'pops' from a bad waveform output (the first HIGH pulse is too long)
// Here we wait until a timer overflow and then turn on the timer output
// It still sometimes pops, but much less so than turning on the timer output in buzzer_buzz()
 //定时器中断函数处理。 //TIM3通用定时器
 void TIM3_IRQHandler(void)
{
		if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)//判断中断标志是否发生
		{
			 TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);     //手动清除中断标志位

			static 	uint  time;
			time++;
		  if(time<icr)  PAout(1)=1;
			else PAout(1)=0;
			if(time>= ocr) time=0;
			
	 }
}





