# PC4V2  Proto Controller 4volt 2 Channel (Another Stupid Acronym) 

Prototype test platform- Code application and devkit based hardware.
DC Motor controller with Display UI, Aimed to test small ~4volt ERM motors. 2 motor output channels.
Capable of providing 10bit waveform on 2 motor drive h-bridge channels at 100Hz update rate. 

The Hardware consists of the following:
1. Feather M0 Express w/Stacking headers
2. Feather OLED 128x32 with standard male header
3. 350mAH lipo - Adafruit- Fits between stacked feather boards. 
4. DRV8833 TI Drive Breakout
5. 3d Printed housing - Onshape: Proto_UIcontrol>>Enclosure4V2
6. See code for pinouts 
7. (Not Used)BS84 P-Chan Mosfet- OLED Pwr Switch
8. (Not USed)0402 15-20k ohm resistor (Pull up for pchannel)

Setting Up Arduino IDE:
1. Download latest IDE from Arduino. Built using (version 2.3.3)
2. If using version 1.6 or older - Download and install Adafruit Framework- 
 https://learn.adafruit.com/adafruit-arduino-ide-setup/arduino-1-dot-6-x-ide
3. If Using latest IDE- Adafruit boards are natively installed. 
4. Using IDE library Manager - Install the following libraries
	- Adafruit_NeoPixel
	- Adafruit_SSD1306
	- Adafruit_GFX
	- ArduinoLowPower
5. All other libs used should be native to arduino IDE. 
6. Locate the PC4V2_Prog1 folder within your Documents>Arduino sketch folder. 


Firmware Notes:
1. Firmware application built original in AS7 at LDC- modified to use Arduino framework
for this version. Based on PC4V1. Now named PC4V2
2. Git Repo will hold updated firmware application files. Along with support docs and support tools.
3. This is DEMO firmware only. No attempts to optimize have been made. All open source. 

FW Functional Description:
1. This application bascially updates a 10bit PWM value every 10milliseconds
   for two h-brige motor outputs. These values come from pre-defined arrays in mode_arrays.h
2. The app scans for valid button presses, and update the current mode_variables
3. If new information is avaialable- an update is made to the OLED 


Modifying Motor Waveform Patterns: 
Adjustments to ERM output patterns will be made within mode_arrays.h
This file can easily be modified for any waveform of interest.
Code must be re-compiled after changes are mode.  

Each mode has 2 arrays, 1 for each motor channel A/B.  

These simple integer arrays hold 10bit PWM duty cycle values. 
These values are processed at 100Hz. Array can be length 1 to MANY! Limited by prog flash space.
Values can be between 0(motor off), and 1023(Motor full pwer). Negative Values reverse motor
direction. DO NOT QUICKLY REVERSE MOTOR DIRECTION WITHOUT SLOWING TO A STOP FIRST. 
 
REVERSE DIR	OFF	FORWARD DIR
  -1023		0	1023

const int ExampArrayA[] = {1023} 		//Motor A forward dir 100% power
const int ExampArrayB[] = {0, -511, 0, -511}	//Motor B reverse dir staccato 50% power

