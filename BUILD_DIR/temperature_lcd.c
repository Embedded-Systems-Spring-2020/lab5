#include "pic24_all.h"
#include <p33EP512GP806.h>
#include "pic24_ports_config.h"

#include <esos.h>
#include <esos_task.h>
#include "esos_comm.h"
#include "esos_f14ui.h"
#include "esos_pic24.h"
#include "esos_sensor.h"
#include "esos_pic24_sensor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <esos_lcd44780.h>

#define CUSTOM_CHARS 7
#define CHAR_POSITIONS 8

void _init_custom_chars();
void _temp_to_graph(uint8_t);

/**********************************************************************/


#define LCD_IS_READY  ESOS_USER_FLAG_1
//esos_ClearUserFlag(LCD_IS_READY);
ESOS_USER_TASK(initLCDtest) {
	char ac_testString[] = "LCD test";
    ESOS_TASK_BEGIN();
		while (esos_IsUserFlagClear(LCD_IS_READY)){
			
			esos_lcd44780_configDisplay();
			esos_lcd44780_writeString( 0, 0, ac_testString ); 
			ESOS_TASK_WAIT_TICKS(1000);
			esos_lcd44780_clearScreen();
			esos_SetUserFlag(LCD_IS_READY);
		}
	ESOS_TASK_END();
}
/*************************************************************************/
ESOS_CHILD_TASK(pot_display_LCD, uint16_t u16_num2graph){  //visual display of data
	//shift 16 bit num2graph 8 bits right for display
	uint8_t u8_scaled_data = u16_num2graph / 16;
	char ac_pot_top_row[] = "pot 0x";
	
	//for slider graphic
	char ac_pot_bottom_row[] = "--------";  
	uint8_t u8_slider_pos = 0;
	char ac_slider[] = "|";
	
	ESOS_TASK_BEGIN();
		esos_lcd44780_writeChar( 0, 6, __esos_u8_GetMSBHexCharFromUint8(u8_scaled_data));
		esos_lcd44780_writeChar( 0, 7, __esos_u8_GetLSBHexCharFromUint8(u8_scaled_data));
		esos_lcd44780_writeString( 0, 0, ac_pot_top_row);
		esos_lcd44780_writeString( 1, 0, ac_pot_bottom_row);
		u8_slider_pos = u16_num2graph / 512;  //ADC output 2^12 into 8 increments 
		esos_lcd44780_writeString( 1, u8_slider_pos, ac_slider);
	ESOS_TASK_END();
}
/**************************************************************************/
ESOS_CHILD_TASK(temp_display_LCD, uint16_t u16_num2graph){  //visual display of data
	uint64_t u64_temp_data; //need 64 bit to handle big number calc tho result will be small
	uint8_t u8_data_value;
	char ac_pot_top_row[] = "LM60";  
	char ac_pot_bottom_row[] = "C"; 
	char c_DecDisplay1;
	char c_DecDisplay2;
	ESOS_TASK_BEGIN();
		esos_lcd44780_writeString( 0, 0, ac_pot_top_row);
		esos_lcd44780_writeString( 1, 2, ac_pot_bottom_row);

		u64_temp_data = (uint64_t)(((300000 * u16_num2graph/4096) - 42400) / 625);

		u8_data_value = (uint8_t) u64_temp_data;
		
		c_DecDisplay1 = '0' + (uint8_t)(u64_temp_data / 10);
		c_DecDisplay2 = '0' + (uint8_t)(u64_temp_data % 10);
		esos_lcd44780_writeChar( 1, 0, c_DecDisplay1);
		esos_lcd44780_writeChar( 1, 1, c_DecDisplay2);

		_temp_to_graph(u8_data_value);
	ESOS_TASK_END();
}
/****************************************************************************/
#define DISPLAY_TEMP ESOS_USER_FLAG_2	
ESOS_USER_TASK(loop) {
	static uint16_t u16_data;
	
	static ESOS_TASK_HANDLE th_pot_display_LCD;
	static ESOS_TASK_HANDLE th_temp_display_LCD;
	ESOS_TASK_BEGIN();
		while(1){
			if (esos_IsUserFlagClear(DISPLAY_TEMP)){
				ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH02, ESOS_SENSOR_VREF_3V3);	
				ESOS_TASK_WAIT_SENSOR_READ(u16_data, ESOS_SENSOR_ONE_SHOT, ESOS_SENSOR_FORMAT_BITS);
				esos_lcd44780_clearScreen();
				ESOS_ALLOCATE_CHILD_TASK(th_pot_display_LCD);
				ESOS_TASK_SPAWN_AND_WAIT(th_pot_display_LCD, pot_display_LCD, u16_data);
				ESOS_SENSOR_CLOSE();
			}else {
				ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH03, ESOS_SENSOR_VREF_3V0);	
				ESOS_TASK_WAIT_SENSOR_READ(u16_data, ESOS_SENSOR_ONE_SHOT, ESOS_SENSOR_FORMAT_BITS);
				esos_lcd44780_clearScreen();
				ESOS_ALLOCATE_CHILD_TASK(th_temp_display_LCD);
				ESOS_TASK_SPAWN_AND_WAIT(th_temp_display_LCD, temp_display_LCD, u16_data);
				ESOS_SENSOR_CLOSE();
			}
			if (esos_uiF14_isSW3Pressed()){  //SW3 toggles display between pot and temp
				ESOS_TASK_WAIT_UNTIL(esos_uiF14_isSW3Released());
				if (esos_IsUserFlagClear(DISPLAY_TEMP)){
					esos_SetUserFlag(DISPLAY_TEMP);
				}
				else if (esos_IsUserFlagSet(DISPLAY_TEMP)){
					esos_ClearUserFlag(DISPLAY_TEMP);
				}
			}
		}
		  
    ESOS_TASK_END();
}

void user_init(void){
	esos_uiF14_flashLED3(500);
    config_esos_uiF14();
	//esos_RegisterTask(initLCDtest);
    esos_RegisterTask(loop);
	esos_lcd44780_init();
	esos_lcd44780_configDisplay();
	esos_lcd44780_clearScreen();
	_init_custom_chars();
}

void _init_custom_chars() {  //these are the custom characters needed to create thermometer
	int i;
	//zero bars and 8 bars are already characters
	uint8_t aau8_char_data[CUSTOM_CHARS][CHAR_POSITIONS] = {
		 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},	// one slice
		 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF},
		 {0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF},
		 {0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF},
		 {0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
		 {0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
		 {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}	// seven slices
	};

	for (i = 0; i < CUSTOM_CHARS; i++) {
		esos_lcd44780_setCustomChar(i, aau8_char_data[i]);
	}
}

void _temp_to_graph(uint8_t temp) { //writes the 'rising thermometer' effect
	switch (temp) {
		case 20:
			esos_lcd44780_writeChar(1, 7, 0xFE);
			esos_lcd44780_writeChar(0, 7, 0xFE);
			break;
		case 21:
			esos_lcd44780_writeChar(1, 7, 0x00); //one
			esos_lcd44780_writeChar(0, 7, 0xFE);
			break;
		case 22:
			esos_lcd44780_writeChar(1, 7, 0x01); //two
			esos_lcd44780_writeChar(0, 7, 0xFE);
			break;
		case 23:
			esos_lcd44780_writeChar(1, 7, 002); //three
			esos_lcd44780_writeChar(0, 7, 0xFE);
			break;
		case 24:
			esos_lcd44780_writeChar(1, 7, 0x03); //four
			esos_lcd44780_writeChar(0, 7, 0xFE);
			break;
		case 25:
			esos_lcd44780_writeChar(1, 7, 0x04); //five
			esos_lcd44780_writeChar(0, 7, 0xFE);
			break;
		case 26:
			esos_lcd44780_writeChar(1, 7, 0x05); //six
			esos_lcd44780_writeChar(0, 7, 0xFE);
			break;
		case 27:
			esos_lcd44780_writeChar(1, 7, 0x06); // seven
			esos_lcd44780_writeChar(0, 7, 0xFE);
			break;
		case 28:
			esos_lcd44780_writeChar(1, 7, 0x07); //full
			esos_lcd44780_writeChar(0, 7, 0xFE);
			break;


		case 29:
			//bottom full
			esos_lcd44780_writeChar(1, 7, 0xFF);

			//top changes
			esos_lcd44780_writeChar(0, 7, 0x01); // one 
			break;
		case 30:
			esos_lcd44780_writeChar(1, 7, 0xFF);

			esos_lcd44780_writeChar(0, 7, 0x02); // two
			break;
		case 31:
			esos_lcd44780_writeChar(1, 7, 0xFF);

			esos_lcd44780_writeChar(0, 7, 0x03); // three
			break;
		case 32:
			esos_lcd44780_writeChar(1, 7, 0xFF);

			esos_lcd44780_writeChar(0, 7, 0x04); // four
			break;
		case 33:
			esos_lcd44780_writeChar(1, 7, 0xFF);

			esos_lcd44780_writeChar(0, 7, 0x05); // five 
			break;
		case 34:
			esos_lcd44780_writeChar(1, 7, 0xFF);

			esos_lcd44780_writeChar(0, 7, 0x06); // six
			break;
		case 35:
			esos_lcd44780_writeChar(1, 7, 0xFF);

			esos_lcd44780_writeChar(0, 7, 0x07); // seven
			break;
	}
}

