//开发环境： keil5  stm32f103c8t6  1.3寸oled屏幕
//
//Nwatch已经成功移植到了Stm32啦，目前已经完成的移植内容为：
//                                                     移植到stm32f103c8t6上、并且绘制了PCB、绘制了另一个表盘、RTC时钟、动画显示、断电flash模拟eeprom，及其自带的所有图标，Sleep睡眠模式（低功耗模式），电压检测
//                                 待完成：             闹铃的BKP存储 、音量调节 、USB检测 、植入蓝牙 、角加速度传感器
//
//可实现的效果：闹钟、手电筒、秒表计时、3D动画演示、游戏（赛车、破坏小球）
//             设置->日期调整、（睡眠时间设置）、显示设置、版本信息查看、FPS显示
//感兴趣的小伙伴可以自己焊接哦，成本粗略计算在100		
//         
//目前64k空间使用情况为：Program Size: Code=36034 RO-data=10414 RW-data=432 ZI-data=4944     51824B
//主要的菜单界面可以阅读借鉴：setting.c 的迭代器函数  static void itemLoader(byte num)   同时注意 #define OPTION_COUNT	5中间的OPTION_COUNT 根据时间情况进行增加减少
//                          void setMenuOption_P(byte num, const char* name, const byte* icon, menu_f actionFunc)  中的actionFunc  根据输入的Go to Definition  of "xxxxx" 就可以进入对应的文件进行修改，也可以模仿进行添加文件
//                        
//还有一种菜单模式，可以借鉴 timedate.c 和 diag.c
//
//设计缺陷：
//1.由于当时想将蓝牙与这个结合在一起，所以预留usart1给蓝牙模块了,所以没有外接ch340芯片，需要st-link或者j-link下载程序
//2.没设计电源按键和硬件复位，如果程序死机，要么选择程序刷固件，要么等待电池狗带，或者取下电源线。
//3.程序下载时，保证单片机正常供电，我使用的st-link不供电。
//4.板子设计的时候没有考虑外接的mpu6050和pw02的高度，所以存在缺陷，
//5.时钟芯片的封装大了一号，但是也能焊上去。
//6.为了考虑手工焊接，大部分元器件我使用的都是0805封装，所以相对紧凑一点
//7.在自己修改程序的时候，建议注释掉以下两句：（不然就只能用按键在亮屏时刻烧写固件，因为停机模式所以时钟停止工作！）
//     	WWDG_Init(0x7F,0X5F,WWDG_Prescaler_8); 	//计数器值为7f,窗口寄存器为5f,分频数为8	       
//      pwrmgr_update();     //开启停机模式
//8.蜂鸣器带有一丝杂音，硬件缺陷啊。抄的板子，不会改
//9.usb插入无法检测
//10.音量调节无法实现
//移植和改动文件程序的时候需要主要：  common.h
//对于预编译不满意的可以修改：        config.h
//
//使用的屏幕是中景园电子的1.3寸oled 非常厉害，我使用的是他家的模拟spi 
//
//能力实在有限，希望各位大佬批评指正！！！
//
//                                                   作者：YUMMY
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "oled.h"
#include "common.h"
#include "key.h"
#include "wwdg.h"

void c_setup()
{
  SystemInit();	
 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();             //初始化延时函数
#if COMPILE_UART
		uart_init(UART_BAUD);		//初始化串口波特率为115200
#endif
  // Everything else
	memset(&oledBuffer, 0x00, FRAME_BUFFER_SIZE);
	OLED_Init();              //初始化oled 四线模拟spi模式
	led_init();               //初始化LED
  buzzer_init();	
  global_init();
	battery_init();
	appconfig_init();
	alarm_init();             //依旧无法储存闹钟，每次重启以后需要自定义

  buttons_init();
  millis_init();
  time_init();
	rtc_init();
	
	// Set watchface
	display_set(watchface_normal);

	display_load();

	WWDG_Init(0x7F,0X5F,WWDG_Prescaler_8); 	//计数器值为7f,窗口寄存器为5f,分频数为8	   

 //初始化的时候，设置需要改变的状态	
	OLED_ColorTurn(appConfig.invert );
}

void c_loop()
{
    time_update();
  if(pwrmgr_userActive())
  {
      battery_update();
      buttons_update();
  }
    
  	buzzer_update();
	
    led_update();
	
	
#if COMPILE_STOPWATCH
  stopwatch_update();
#endif

  if(pwrmgr_userActive())
  {
     alarm_update();
     display_update();
  }

 pwrmgr_update();     //开启停机模式
 

}




int main(void)
{ 

	c_setup();	    //初始化

  while(1)
		{

	    c_loop(); //循环			
			if(KEY3 ==0 ){        //主界面的按键四是重新复位
	    __set_FAULTMASK(1);
     NVIC_SystemReset(); } 

	  }




}







