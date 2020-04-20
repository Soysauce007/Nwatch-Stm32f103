/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

// Battery
// Battery is connected to a P-MOSFET then to a voltage divider. This is so we don't have a load of current being wasted through the voltage divider all the time.
// Polling based, interrupts not needed here since a conversion only takes ~500us and is only done every-so-often

#include "common.h"

#define MAX_VOLTAGE  4.21
#define ADC_MAX      4096   //这里需要修改
#define MAX_ADCVAL	((uint16_t)(ADC_MAX)/2/16)

/*
#if MAX_ADCVAL > ADC_MAX
//	#undef MAX_ADCVAL
//	#define MAX_ADCVAL ADCMAX
	#warning "MAX_ADCVAL > ADCMAX"
#endif
*/

// Resolution = MAX_VOLTAGE / MAX_ADCVAL
// Resolution = 4.88mV

static uint voltage;
static byte lastSecs;
static byte changeCount;

void battery_init()
{
	// MOSFET pin
  Adc_Init();		  		//ADC初始化

}

// Set next update to happen in a x seconds
void battery_setUpdate(byte secs)
{
	changeCount = secs;
	lastSecs = timeDate.time.secs;
}

void battery_update()
{
	// See if seconds has changed
	if(lastSecs == timeDate.time.secs)
		return;
	lastSecs = timeDate.time.secs;

	if(changeCount)
	{
		changeCount--;
		return;
	}

	// Next update in 5 seconds
	battery_setUpdate(3);

	battery_updateNow();
}

// Update voltage
void battery_updateNow()
{
	// Get ADC value
	u16 adc = Get_Adc_Average(0,5);

	// Convert ADC value to voltage
	
	voltage = ((uint32_t)adc) ;
}

// Get voltage
uint battery_voltage()
{
	return voltage;
}
