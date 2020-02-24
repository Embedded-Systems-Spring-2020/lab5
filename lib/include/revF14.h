// prevents errors if header is included multiple times and begins "definition" 
// of the REVF14_H token
#ifndef REVF14_H    
#define REVF14_H
//#define NUM_UART_MODS 1
#include "pic24_all.h"

// The following definitions are more or less directly from the code examples 
// supplied in Microprocessors
// https://www.google.com/url?q=http%3A%2F%2Fcourses.ece.msstate.edu%2Fece3724%2Fmain_pic24%2Flabs%2Ffiles%2Fpic24_code_examples.zip&sa=D&sntz=1&usg=AFQjCNGoPEjqijnThxBXTiBlNnussuN9Yw
// The difference being the pins they are connected to (RF4 instead of RB14, etc)
// Specific files used: ledflash_timer.c, ledsw1_cn.c
//

// LED1 - Red LED (Top, connected to RF4)
#define CONFIG_LED1() CONFIG_RF4_AS_DIG_OUTPUT()
#define LED1  (_LATF4)

// LED2 - Yellow LED (Middle, connected to RB14)
#define CONFIG_LED2() CONFIG_RB14_AS_DIG_OUTPUT()
#define LED2  (_LATB14)

// LED3 - Green LED (Bottom, connected to RB15)
#define CONFIG_LED3() CONFIG_HB_LED()

#define LED3  (_LATB15)

// SW1 - Connected to RB13
void CONFIG_SW1()  {
  CONFIG_RB13_AS_DIG_INPUT();
  ENABLE_RB13_PULLUP();
  // Give the pullup some time to take effect.
  DELAY_US(1);
}

#define SW1_PRESSED (_RB13 == 0)
#define SW1_RELEASED (_RB13 == 1)


// SW2 - Connected to RB12
void CONFIG_SW2()  {
  CONFIG_RB12_AS_DIG_INPUT();
  ENABLE_RB12_PULLUP();
  // Give the pullup some time to take effect.
  DELAY_US(1);
}

#define SW2_PRESSED (_RB12 == 0)
#define SW2_RELEASED (_RB12 == 1)


// SW3 - Connected to RC15
void CONFIG_SW3()  {
  CONFIG_RC15_AS_DIG_INPUT();
  ENABLE_RC15_PULLUP();
  // Give the pullup some time to take effect.
  DELAY_US(1);
}

#define SW3_PRESSED (_RC15 == 0)
#define SW3_RELEASED (_RC15 == 1)

//************RPG**********
void CONFIG_RPG()  {
  CONFIG_RB8_AS_DIG_INPUT();
  CONFIG_RB9_AS_DIG_INPUT();
  ENABLE_RB8_PULLUP();
  ENABLE_RB9_PULLUP();
  // Give the pullup some time to take effect.
  DELAY_US(1);
}
#define RPGA (_RB8)
#define RPGB (_RB9)


//************LCD**********
void CONFIG_LCD() {
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


#endif // closes the #ifndef block
