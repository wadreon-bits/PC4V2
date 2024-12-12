#ifndef pc4v_support_h
#define pc4v_support_h
#include <Adafruit_NeoPixel.h>    //Adafruit Repos - Sketch>>Include Library>>Manage Libraries
#include "Timer.h"                //Belive this is Native? 
#include "ArduinoLowPower.h"      //Search Lib Manager
#include <WString.h>              //Belive this is Native? 
#include "Arduino.h"              //Native Lib 
#include "variant.h"              //Native Lib 
#include <Adafruit_SSD1306.h>     //Adafruit Repos - 
#include <Adafruit_GFX.h>         //Adafruit Repos - 
#include <Wire.h>                 //Adafruit Repos - 
#include "mode_arrays.h"          //program header file 
//#include "pyscript_led_array.h"   //program header file  

//#include <samd21/include/samd21.h>      //not needed with arduino framework. Use for AS7 framework
//#include <samd21/include/samd21e18a.h>  //not needed with arduino framework. Use for AS7 
//#include <samd21e18a.h>
/*
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif  
*/
#define LDC_HW_TYPE			0x01	// this should be driven from hw pin 
#define LDC_HW_VER			0x10	// this should be driven from hw pin 

#define TARGET_HW_TYPE	0x01		//What is the target hardware board for this software??
#define TARGET_HW_VER		0x10		// 0xMAJOR|MINOR

#define PACKG_TARGET_HW_TYPE			//this should be stored in the variant file!!
										// this check should be performed in the variant files
#define PACKG_TARGET_HW_VER				// Managed in variant files check to see if 
										// should the package check the HW???  '
										// 

//##############################################################
//  Preprocessor #DEFINES
//##############################################################
#define FW_VERSION			"R2.0"	  //version of this application. Update with Any changes made
#define FW_VER_DATE     "121124"  //Date of fw version update. Update when version changes. 
//#define DEBUG_PRINTS            //comment out for formal release
#define OLED_CONNECTED 
//#define ENABLE_TIMERSCOPE       // If enabled- Can Scope LED output for timer calibration etc
#define APP_BASE_TIME		    10    // ms - careful adjust this- 
#define TEN_MS_PER			    1     // multiple of APP_BASE_TIME [ms]
#define BTN_SCAN_PER		    2     // multiple of APP_BASE_TIME [ms]
#define SLEEP_TIMEOUT		    5     // Seconds of inactive until low power sleep
#define BTN_SHORTPRESS_TIME	3     //*APP_BASE_TIME*BTN_SCAN_PER = 60ms
#define BTN_NORMPRESS_TIME  6     //*APP_BASE_TIME*BTN_SCAN_PER = 120ms
#define BTN_LONGPRESS_TIME  50    //*APP_BASE_TIME*BTN_SCAN_PER = 1000ms
#define APP_LVC             3.49  // Lipo Low Voltage Cutoff. Governed by OLED pwred by 3.3LDO
#define NONE				        0

//##############################################################
//  I/O PIN ASSIGNMENTS
//##############################################################
// Feather M0 Express has PWM on the following pins: A2, A3, A4, 
// SCK, MOSI, MISO, D0, RX, D1, TX, SDA, SCL, D5, D6, D9, D10, D11, D12, D13, NEOPIXEL. There is NO PWM on: A0, A1, A5.
// For a true DAC 10bit- USE A0. 
// These constants won't change. They're used to give names to the pins used:
//const int analogOutPin = A0;   // Analog input pin that the potentiometer is attached to
#define MISO          28    //defined for convenience- Silkscreen on feathers do not show DIO# for all pins
#define MOSI          29    //defined for convenience- Silkscreen on feathers do not show DIO# for all pins
#define SCK           30    //defined for convenience- Silkscreen on feathers do not show DIO# for all pins

//##############  Application PIN ASSIGMENTS #####################
#define SER_LED			  8     //NeoPixel
//#define DRIVE_ISENSEA A1  //Drive Current Ref
//#define DRIVE_ISENSEB A5  //Drive Current Ref
#define OLED_EN_PIN   15    //FeatherM0:  A1

#define DRIVE_A1_PIN	10        //
#define DRIVE_A2_PIN  11        //
#define DRIVE_B1_PIN	12        //
#define DRIVE_B2_PIN  16        //  FeatherM0: A2
#define DRIVE_SLP_PIN 17        //  FeatherM0: A3
#define DRIVE_FLT_PIN 18        //  FeatherM0: A4

#define BTN_A_PIN     9           //OLED Feather Pinout, Shared with Vbat Analog
#define BTN_B_PIN     6           //OLED Feather Pinout
#define BTN_C_PIN     5           //OLED Feather Pinout

#define BTN_1_PIN		  BTN_A_PIN   //UP
#define BTN_2_PIN		  BTN_C_PIN   //DOWN

#define APP_LED			  BRD_LED

#define BRD_LED			  13
/* pin defines SAMD21 E18??*/

/***************UI BUTTON MASKS*************************/
#define BTN_UP			  BTN_A_PIN
#define BTN_DWN			  BTN_C_PIN
//btn masks for case loop
#define BTN_ALL_MASK	3
#define BTN_UP_MASK		2
#define BTN_DWN_MASK	1


enum appState_e{
  BOOT,
  INITSLEEP,
  INITWAKE,
  LOWBATT,
  USERMODE,
  AUX
};

enum userMode_e{
  MODE1,
  MODE2,
  MODE3,
  MODE4,
  MODE5,
  MODE6,
  MODE7,
  MODE8,
  MODE9,
  MODE10
};
#endif