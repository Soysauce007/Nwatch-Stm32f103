/*
   Project: N|Watch
   Author: Zak Kemble, contact@zakkemble.co.uk
   Copyright: (C) 2013 by Zak Kemble
   License: GNU GPL v3 (see License.txt)
   Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
*/

#include "common.h"
#include "oled.h"
#if COMPILE_TORCH

static uint strobe;

static bool btnExit(void);
static bool btnFlashRate(void);
static display_t draw(void);

void torch_open()
{
	pwrmgr_setState(PWR_ACTIVE_DISPLAY, PWR_STATE_BUSY);
  menu_close();
  strobe = 0;
  display_setDrawFunc(draw);
  buttons_setFuncs(btnExit, btnFlashRate, btnExit);
}

static bool btnExit()
{
	pwrmgr_setState(PWR_ACTIVE_DISPLAY, PWR_STATE_NONE);		
  OLED_ColorTurn(appConfig.invert);
  display_load();
  return true;
}

static bool btnFlashRate()
{
  if (strobe < 500)
    strobe += 50 * ((strobe / 50) + 1);
  else
    strobe = 0;
  return true;
}

static display_t draw()
{
  static bool invert;
  static millis_t lastStrobe;

  if (strobe)
  {
    millis_t now = millis();
    if (now - lastStrobe >= strobe)
    {
      lastStrobe = now;
      invert = !invert;
      OLED_ColorTurn(invert);
      led_flash(invert ? LED_GREEN : LED_RED, 20, 255);
    }
    return DISPLAY_BUSY;
  }

	
	
    OLED_ColorTurn(true);
    led_flash(LED_GREEN, 100, 255);
    led_flash(LED_RED, 100, 255);
  
  return DISPLAY_DONE;
}

#endif
