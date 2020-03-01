#include "pic24_all.h"
#include <p33EP512GP806.h>
#include "pic24_ports_config.h"

#include <esos.h>
//#include <esos_task.h>
#include "esos_comm.h"
#include "esos_f14ui.h"
#include "esos_pic24.h"
#include "esos_sensor.h"

#include "esos_lcd44780.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pic24_all.h>
#include <pic24_ports_config.h>

/*********************************************************
Test of each pin connectivity from pin 4 through 13 on LCD module
Press SW1 to sequence
***********************************************************/

//Pin Configs
#define LCD_D0 (_RE0)
#define LCD_D1 (_RE1)
#define LCD_D2 (_RE2)
#define LCD_D3 (_RE3)
#define LCD_D4 (_RE4)
#define LCD_D5 (_RE5)
#define LCD_D6 (_RE6)
#define LCD_D7 (_RE7)

#define LCD_E (_RD10)
#define LCD_RW (_RD11)
#define LCD_RS (_RC12)

void configPins(void) {
	
  	// connected to LCDD0 - LCDD7
	CONFIG_RE0_AS_DIG_OUTPUT();
	CONFIG_RE1_AS_DIG_OUTPUT();
	CONFIG_RE2_AS_DIG_OUTPUT();
	CONFIG_RE3_AS_DIG_OUTPUT();
	CONFIG_RE4_AS_DIG_OUTPUT();
	CONFIG_RE5_AS_DIG_OUTPUT();
	CONFIG_RE6_AS_DIG_OUTPUT();
	CONFIG_RE7_AS_DIG_OUTPUT();

	// connected to LCDE
	CONFIG_RD10_AS_DIG_OUTPUT();

	// connected to LCDRW (0/L: Write, 1/H: Read)
	CONFIG_RD11_AS_DIG_OUTPUT();

	// connected to LCDRS 
	CONFIG_RC12_AS_DIG_OUTPUT();

}

void clearPins(void){
		LCD_D0 = 0;
		LCD_D1 = 0;
		LCD_D2 = 0;
		LCD_D3 = 0;
		LCD_D4 = 0;
		LCD_D5 = 0;
		LCD_D6 = 0;
		LCD_D7 = 0;
		LCD_E  = 0;
		LCD_RW = 0;
		LCD_RS = 0;
}	

ESOS_USER_TASK(loop) {
	ESOS_TASK_BEGIN();
	configPins();
	while(1){
		ESOS_TASK_WAIT_UNTIL (esos_uiF14_isSW1Pressed ());
		ESOS_TASK_WAIT_TICKS(15);
		ESOS_TASK_WAIT_UNTIL(esos_uiF14_isSW1Released ());
		clearPins();
		LCD_RS = 1; //pin 4 to 14 on LCD module
		printf("pin4\n");
		ESOS_TASK_WAIT_UNTIL (esos_uiF14_isSW1Pressed ());
		ESOS_TASK_WAIT_TICKS(15);
		ESOS_TASK_WAIT_UNTIL(esos_uiF14_isSW1Released ());
		clearPins();
		LCD_RW = 1;
		printf("pin5\n");
		ESOS_TASK_WAIT_UNTIL (esos_uiF14_isSW1Pressed ());
		ESOS_TASK_WAIT_TICKS(15);
		ESOS_TASK_WAIT_UNTIL(esos_uiF14_isSW1Released ());
		clearPins();
		LCD_E = 1;
		printf("pin6\n");
		ESOS_TASK_WAIT_UNTIL (esos_uiF14_isSW1Pressed ());
		ESOS_TASK_WAIT_TICKS(15);
		ESOS_TASK_WAIT_UNTIL(esos_uiF14_isSW1Released ());
		clearPins();
		LCD_D0 = 1;
		printf("pin7\n");
		ESOS_TASK_WAIT_UNTIL (esos_uiF14_isSW1Pressed ());
		ESOS_TASK_WAIT_TICKS(15);
		ESOS_TASK_WAIT_UNTIL(esos_uiF14_isSW1Released ());
		clearPins();
		LCD_D1 = 1;
		printf("pin8\n");
		ESOS_TASK_WAIT_UNTIL (esos_uiF14_isSW1Pressed ());
		ESOS_TASK_WAIT_TICKS(15);
		ESOS_TASK_WAIT_UNTIL(esos_uiF14_isSW1Released ());
		clearPins();
		LCD_D2 = 1;
		printf("pin9\n");
		ESOS_TASK_WAIT_UNTIL (esos_uiF14_isSW1Pressed ());
		ESOS_TASK_WAIT_TICKS(15);
		ESOS_TASK_WAIT_UNTIL(esos_uiF14_isSW1Released ());
		clearPins();
		LCD_D3 = 1;
		printf("pin10\n");
		ESOS_TASK_WAIT_UNTIL (esos_uiF14_isSW1Pressed ());
		ESOS_TASK_WAIT_TICKS(15);
		ESOS_TASK_WAIT_UNTIL(esos_uiF14_isSW1Released ());
		clearPins();
		LCD_D4 = 1;
		printf("pin11\n");
		ESOS_TASK_WAIT_UNTIL (esos_uiF14_isSW1Pressed ());
		ESOS_TASK_WAIT_TICKS(15);
		ESOS_TASK_WAIT_UNTIL(esos_uiF14_isSW1Released ());
		clearPins();
		LCD_D5 = 1;
		printf("pin12\n");
		ESOS_TASK_WAIT_UNTIL (esos_uiF14_isSW1Pressed ());
		ESOS_TASK_WAIT_TICKS(15);
		ESOS_TASK_WAIT_UNTIL(esos_uiF14_isSW1Released ());
		clearPins();
		LCD_D6 = 1;
		printf("pin13\n");
		ESOS_TASK_WAIT_UNTIL (esos_uiF14_isSW1Pressed ());
		ESOS_TASK_WAIT_TICKS(15);
		ESOS_TASK_WAIT_UNTIL(esos_uiF14_isSW1Released ());
		clearPins();
		LCD_D7 = 1;
		printf("pin14\n");
	}
		
    ESOS_TASK_END();
}

void user_init(void){
	config_esos_uiF14();
	esos_uiF14_flashLED3(500);
    esos_RegisterTask(loop);

}