/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"
#include "led.h"

#define OPTION_COUNT	5

static prev_menu_s prevMenuData;

static void mSelect(void);
static void itemLoader(byte);
static void setBrightness(void);
static void setInvert(void);
static void setRotate(void);
#if COMPILE_ANIMATIONS
static void setAnimations(void);
#endif
static void setMenuOptions(void);

void mDisplayOpen()
{
	setMenuInfo(OPTION_COUNT, MENU_TYPE_ICON, PSTR(STR_DISPLAYMENU));
	setMenuFuncs(MENUFUNC_NEXT, mSelect, MENUFUNC_PREV, itemLoader);

//	setMenuOptions();

	setPrevMenuOpen(&prevMenuData, mDisplayOpen);

	beginAnimation2(NULL);
}

static void mSelect()
{
	bool isExiting = exitSelected();
	if(isExiting)
		appconfig_save();
	setPrevMenuExit(&prevMenuData);
	doAction(exitSelected());
	
	
}

static void itemLoader(byte num)
{
	UNUSED(num);
	setMenuOptions();
	addBackOption();
}

static void setBrightness()
{
 	byte brightness = appConfig.brightness;;
	if(brightness > 4)
		brightness = 0;
		brightness++;
	appConfig.brightness = brightness;
	
	int _brightness=0;

	switch(brightness)           //设置画笔颜色
	{
		case 0:	_brightness= 0x01; break;
		case 1:	_brightness= 0x0f; break;
		case 2:	_brightness= 0x5f; break;
		case 3:	_brightness= 0xff; break;
	}	

	
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register    // 设置对比度
	OLED_WR_Byte(_brightness,OLED_CMD);// Set SEG Output Current Brightness // 1~255; 默认0x7f, 越大越亮
 
}

static void setInvert()
{
	appConfig.invert = !appConfig.invert;
	 
	OLED_ColorTurn(appConfig.invert);
}

static void setRotate() 
{
	
	bool Rotate =  appConfig.display180;
	appConfig.display180 = !Rotate;

	OLED_DisplayTurn(appConfig.display180);
	

}

static void setLEDs() {
  appConfig.CTRL_LEDs = !appConfig.CTRL_LEDs;
	
	
	
	
}

#if COMPILE_ANIMATIONS
static void setAnimations()
{

	appConfig.animations = !appConfig.animations;
}
#endif

static void setMenuOptions()
{
//	setMenuOption_P(0, PSTR(STR_BRIGHTNESS), menu_brightness[appConfig.brightness], setBrightness);
//	setMenuOption_P(1, PSTR(STR_INVERT), menu_invert, setInvert);
//	setMenuOption_P(2, PSTR(STR_ROTATE), menu_rotate, setRotate);
//#if COMPILE_ANIMATIONS
//	setMenuOption_P(3, PSTR(STR_ANIMATIONS), menu_anim[appConfig.animations], setAnimations);
//#endif
//  setMenuOption_P(4, PSTR(STR_LEDS), menu_LEDs[appConfig.CTRL_LEDs], setLEDs);
//	

	
	setMenuOption_P(0, PSTR(STR_BRIGHTNESS), menu_brightness[appConfig.brightness], setBrightness);
	setMenuOption_P(1, PSTR(STR_INVERT), menu_invert, setInvert);
	setMenuOption_P(2, PSTR(STR_ROTATE), menu_rotate, setRotate);
  #if COMPILE_ANIMATIONS
	setMenuOption_P(3, PSTR(STR_ANIMATIONS), menu_anim[appConfig.animations], setAnimations);
#endif
	setMenuOption_P(4, PSTR(STR_LEDS), menu_LEDs[appConfig.CTRL_LEDs], setLEDs);
	

}
