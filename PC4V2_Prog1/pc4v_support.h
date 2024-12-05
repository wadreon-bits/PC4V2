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
#include "pyscript_led_array.h"   //program header file  


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

#define TARGET_HW_TYPE		0x01		//What is the target hardware board for this software??
#define TARGET_HW_VER		0x10		// 0xMAJOR|MINOR

#define PACKG_TARGET_HW_TYPE			//this should be stored in the variant file!!
										// this check should be performed in the variant files
#define PACKG_TARGET_HW_VER				// Managed in variant files check to see if 
										// should the package check the HW???  '
										// 
					
//there will be following
//Hw version- what board build is it?? PCB version?
//package version- what framework?? variants/pins etc?? 
// can define LDC_PACKAGE_VER in the 
//hw version must be compatible with framework version
//hw version could be read from board?? 
	// loaded into bootloader?
	// read on free pin?? 
//application version must be compatible with both of above
//application version
/*
#ifndef __SAMD21E18A__
	#error "Unsupported Hardware!!- Use correct LDC Hardware type"
#endif

#if (LDC_HW_TYPE != TARGET_HW_TYPE)
	#error "Unsupported Hardware!!- Use correct LDC Hardware type"
#endif
*/
/*
#ifndef (TARGET_HW_TYPE)
	#error "Please set target hardware type"
#endif

#if (LDC_HW_TYPE != TARGET_HW_TYPE)
	#error "Unsupported Hardware!!- Use correct LDC Hardware type"
#endif

#ifndef (TARGET_HW_VER)
	#error "Please set target hardware version"
#endif

#if (LDC_HW_VER != TARGET_HW_VER)
	#warning "Actual Hardware version does not match target version "
#endif

*/
enum dispState_e{
  BOOT,
  INITSLEEP,
  INITWAKE,
  LOWBATT,
  USERMODE,
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