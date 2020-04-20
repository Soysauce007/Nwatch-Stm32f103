/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"


void drawBattery()
{
	
	
//	float voltage=0;//改成u16会导致无法正常显示
//	float bat=0;
//	voltage=Get_Adc_Average(0,100)*(3.3/4096)*2;
//	if((4.2-voltage)<0.9)//经测量断电时电压为3.3V左右，电压区间位3.3v-4.21v
//	 bat=(voltage-3.3)/0.9*100;
//	else
//	 bat=100;
	
	
	uint battLevel =    battery_voltage();       
	const byte* battIcon;
	if(battLevel < 3100)
		battIcon = battIconEmpty;
	else if(battLevel < 3200)
		battIcon = battIconLow;
	else if(battLevel < 3300)
		battIcon = battIconHigh;
	else
		battIcon = battIconFull;

	draw_bitmap(0, FRAME_HEIGHT - 8, battIcon, 16, 8, NOINVERT, 0);
}
