#include "pic24_all.h"
#include <p33EP512GP806.h>
#include "pic24_ports_config.h"

#include <esos.h>
#include <esos_task.h>
#include "esos_comm.h"
#include "esos_f14ui.h"
#include "esos_pic24.h"
#include "esos_sensor.h"

#include "esos_lcd44780.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LCD_IS_READY ESOS_USER_FLAG_1

ESOS_USER_TASK(initLCDtest) {
	static char ac_testString[] = "LCD test";
    ESOS_TASK_BEGIN();
		while(esos_IsUserFlagClear(LCD_IS_READY)){
			esos_lcd44780_init();
			printf("init\n");
			ESOS_TASK_WAIT_TICKS(10);
			esos_lcd44780_configDisplay();
			printf("config\n");
			esos_lcd44780_writeChar(0,0, 'A');
			//esos_lcd44780_writeString( 0, 0, ac_testString ); //outputs test message to LCD top row, far left
			ESOS_TASK_WAIT_TICKS(1000);
			//esos_lcd44780_clearScreen();
			printf("clear\n");
			esos_SetUserFlag(LCD_IS_READY);
		}
	ESOS_TASK_END();
}


	
ESOS_USER_TASK(loop) {
	ESOS_TASK_BEGIN();
		while (esos_IsUserFlagSet(LCD_IS_READY)){
			esos_uiF14_turnLED2On();		
		}  
    ESOS_TASK_END();
}

void user_init(void){
	config_esos_uiF14();
	esos_uiF14_flashLED3(500);
	esos_RegisterTask(initLCDtest);
    esos_RegisterTask(loop);

}
