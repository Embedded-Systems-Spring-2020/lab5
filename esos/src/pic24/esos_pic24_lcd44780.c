/*
 * "Copyright (c) 2008 Robert B. Reese, Bryan A. Jones, J. W. Bruce ("AUTHORS")"
 * All rights reserved.
 * (R. Reese, reese_AT_ece.msstate.edu, Mississippi State University)
 * (B. A. Jones, bjones_AT_ece.msstate.edu, Mississippi State University)
 * (J. W. Bruce, jwbruce_AT_ece.msstate.edu, Mississippi State University)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the authors appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS"
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Please maintain this header in its entirety when copying/modifying
 * these files.
 *
 *
 */

/**
 * \addtogroup ESOS_Task_LCD_Service
 * @{
 */
 
/*** I N C L U D E S *************************************************/
#include "esos_pic24_lcd.h"

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

/*** T H E   C O D E *************************************************/
void __esos_lcd44780_pic24_config ( void )
{
    // Set up the hardware aspects of the HWxxx interface of the LCD module service
    //    direction, thresholds, etc beyond what is already done in esos_lcd44780_config()


}

void __esos_lcd44780_pic24_setDataPins(uint8_t u8_data) {
	// write the hardware-specific code to take the u8_data passed in
	// and place it on the appropriate data pins

	// Assuming that the first bit of u8_data is the DB0 and the last bit of 
	// u8_data is the DB7

	uint8_t u8_data_working_copy = u8_data;
	int lsb_value = 0;

	for (int i = 0; i < 8; i++) {
		lsb_value = u8_data & 0x01;
		switch(i) {
			case 0:
				LCD_D0 = lsb_value;
				break;
			case 1:
				LCD_D1 = lsb_value;
				break;
			case 2:
				LCD_D2 = lsb_value;
				break;
			case 3:
				LCD_D3 = lsb_value;
				break;
			case 4:
				LCD_D4 = lsb_value;
				break;
			case 5:
				LCD_D5 = lsb_value;
				break;
			case 6:
				LCD_D6 = lsb_value;
				break;		
			case 7:
				LCD_D7 = lsb_value;
				break;
		}
		u8_data_working_copy = u8_data_working_copy >> 1;
	}
}

uint8_t __esos_lcd44780_pic24_getDataPins(void) {
	// write the hardware-specific code to read the appropriate data pins
	// and create the uint8 data to return to the caller
	
}

void __esos_lcd44780_pic24_configDataPinsAsInput( void ) {
	// write the hardware-specific code to set the LCD character module
	// data pins to be "inputs" to prepare for a read of the LCD module

	// connected to LCDD0 - LCDD7
	CONFIG_RE0_AS_DIG_INPUT();
	CONFIG_RE1_AS_DIG_INPUT();
	CONFIG_RE2_AS_DIG_INPUT();
	CONFIG_RE3_AS_DIG_INPUT();
	CONFIG_RE4_AS_DIG_INPUT();
	CONFIG_RE5_AS_DIG_INPUT();
	CONFIG_RE6_AS_DIG_INPUT();
	CONFIG_RE7_AS_DIG_INPUT();

	// connected to LCDE
	CONFIG_RD10_AS_DIG_INPUT();

	// connected to LCDRW (0/L: Write, 1/H: Read)
	CONFIG_RD11_AS_DIG_INPUT();

	// connected to LCDRS 
	CONFIG_RC12_AS_DIG_INPUT();
	
}

void __esos_lcd44780_pic24_configDataPinsAsOutput( void ) {
	// write the hardware-specific code to set the LCD character module
	// data pins to be "outputs" to prepare for a write to the LCD module
	
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
