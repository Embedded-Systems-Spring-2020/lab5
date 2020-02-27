#include "pic24_all.h"
#include <p33EP512GP806.h>
#include "pic24_ports_config.h"

#include <esos.h>
#include <esos_task.h>
#include "esos_comm.h"
#include "esos_f14ui.h"
#include "esos_pic24.h"
#include "esos_sensor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**********************************************************************/
#define LCD_IS_READY ESOS_USER_FLAG_1
esos_ClearUserFlag(LCD_IS_READY);
ESOS_USER_TASK(initLCDtest) {
	char ac_testString[] = "LCD test";
    ESOS_TASK_BEGIN();
		while (esos_IsUserFlagClear){
			esos_lcd44780_init();
			esos_lcd44780_configDisplay();
			esos_lcd44780_writeString( 0, 0, ac_testString ); outputs test message to LCD top row, far left
			ESOS_TASK_WAIT_TICKS(1000);
			esos_lcd44780_clearScreen();
			esos_SetUserFlag(LCD_IS_READY);
		}
	ESOS_TASK_END();
}
/*************************************************************************/
ESOS_CHILD_TASK(th_pot_display_LCD, uint16_t u16_num2graph){  //visual display of data
	uint8_t u8_slider_pos = 0;
	char ac_pot_top_row[] = "pot 0x";  //need to write num2graph somehow
	char ac_pot_bottom_row[] = "________";  //would rather use char D2
	char ac_slider[] = "|";
	ESOS_TASK_BEGIN();
		esos_lcd44780_writeString( 0, 0, ac_pot_top_row);
		esos_lcd44780_writeString( 1, 0, ac_pot_bottom_row);
		u8_slider_pos = u16_num2graph / 512;  //ADC output 2^12 into 8 increments 
		esos_lcd44780_writeString( 1, u8_slider_pos, ac_slider);
	ESOS_TASK_END();
}
/**************************************************************************/
ESOS_CHILD_TASK(th_pot_display_LCD, uint16_t u16_num2graph){  //visual display of data
	
	char ac_pot_top_row[] = "LM60";  
	char ac_pot_bottom_row[] = "C";  
	char ac_slider[] = "|";
	ESOS_TASK_BEGIN();
		esos_lcd44780_writeString( 0, 0, ac_pot_top_row);
		esos_lcd44780_writeString( 1, 2, ac_pot_bottom_row);
		//need conversion to C and diplay on LCD
	ESOS_TASK_END();
}
/****************************************************************************/
#define DISPLAY_TEMP ESOS_USER_FLAG_2	
ESOS_USER_TASK(loop) {
	static uint16_t u16_data;
	static ESOS_TASK_HANDLE th_pot_display_LCD;
	ESOS_TASK_BEGIN();
		while (esos_IsUserFlagSet(LCD_IS_READY)){
			if (esos_IsUserFlagClear(DISPLAY_TEMP)){
				ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH02, ESOS_SENSOR_VREF_3V3);	
				ESOS_TASK_WAIT_SENSOR_QUICK_READ(u16_data);
				ESOS_ALLOCATE_CHILD_TASK(th_pot_display_LCD);
				ESOS_TASK_SPAWN_AND_WAIT(th_pot_display_LCD, u16_data);
			
			}else {
				ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH03, ESOS_SENSOR_VREF_3V0);	
				ESOS_TASK_WAIT_SENSOR_QUICK_READ(u16_data);
				ESOS_ALLOCATE_CHILD_TASK(th_temp_display_LCD);
				ESOS_TASK_SPAWN_AND_WAIT(th_temp_display_LCD, u16_data);
			}
			if (esos_uiF14_isSW3Pressed()){  //SW3 toggles display between pot and temp
				ESOS_TASK_WAIT_UNTIL_UIF14_SW3_RELEASED();
				if (esos_IsUserFlagClear(DISPLAY_TEMP){
					esos_SetUserFlag(DISPLAY_TEMP);
				}
				else if (esos_IsUserFlagSet(DISPLAY_TEMP){
					esos_ClearUserFlag(DISPLAY_TEMP);
				}
			}	
		}  
    ESOS_TASK_END();
}

void user_init(void){
	esos_uif14_flashLED3(500);
    config_esos_uiF14();
	esos_RegisterTask(initLCDtest);
    esos_RegisterTask(loop);

}

