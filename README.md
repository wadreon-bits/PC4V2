# PC4V2  Proto Controller 4volt 2 Channel (Another Stupid Acronym) 

Prototype test platform- Code application and devkit based hardware.
DC Motor controller with Display UI, Aimed to test small ~4volt ERM motors. 2 motor output channels.
Capable of providing 10bit waveform on 2 motor drive h-bridge channels at 100Hz update rate. 

Repository Directory: (protocontrol_4V2)
- ArrayBuilding_Excel 	- Method to build waveforms with excel. Ask for demo.
- Control_Enclosure_R1 	- MCADD files for device enclosure
- PC4V2_Prog1		- Arduino Sketch Folder
- ProtoCheatSheet.doc	- Quick Device instructions
- README.md		- .GIT repo readme doc


Using The Assembled Device:
1. Charge for approx 1hour if discharged -uUSB. Fully charged should read around 4.15vdc or more when device unplugged. 
2. Press either button to wake device
3. Press UP/DWN to select mode#(User setting? Level? Idk)
4. Set to mode#0 to turn device off- will resume sleep in a few seconds.
5. As shipped-The modes are programmed as follows
	- #1-3 are constant speed, 50%, 75%, 100%(motors a/b opp directions)
	- #4-10 are various on/off, and sweeping patterns. Easy to mod. 
	- Can read mode_arrays.h within the code to get a better idea of the patterns programmed. Easy to mod.
6. Alternative hidden mode- Press and hold both buttons simultaneously for >1sec to enter/leave the direct motor drive mode.
	- Use Up/Dwn buttons to set the desired %power to both motor channels. 0%=OFF 100%=ON   
	- NOTE! Most ERM motors will NOT start spinning until >15-20% power is applied. Mode starts at 0.0%.
	- It will stay awake in this mode. Press both btns >1sec again to return to user mode.


Setting Up Arduino IDE: (Updated after R2.2 - after RyanR first load)
1. Download latest IDE from Arduino. Built using (version 2.3.3)
2. In order to add Adafruit framework- perform following
 - File>>Preferences  towards bttm-
 - Copy the following link into 'Additional boards managers URLs:
 - https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
3. IDE Left pane >> BOARDS MANAGER>> Search for Adafruit SAMD
 - Install the SAMD package that includes the FeatherM0 Express
4. Using IDE library Manager - Search for & Install the following libraries
	- Adafruit_NeoPixel
	- Adafruit_SSD1306
	- Adafruit_GFX
	- ArduinoLowPower
6. Lastly - Move the 'PC4V2_Prog1' folder into your Arduino Sketch folder--> Documents>Arduino. 
 - Depending on windows setup- Documents might be a few places - Check onedrive
 - Can check in IDE-  File>>Preferences>>Sketchbook Location: 
7. Tool chain is setup and ready to use. Can edit, compile, and flash devices 

Programming Device:
1. Plug into PC USB port
2. Open Arduino IDE
3. Tools>>Boards  - Select Adafruit Feather M0 Express
4. With a Tack or Paperclip-  Double click the pinhole button on the front button face- proto_UIcontrol. 
5. After the double-click, the feather should re-mount as a new USB com-port. 
	- Helpful note- The Board LED will slowly pulse Red when in proper bootloader mode. Board LED Can been seen right next to feather USB port. 
6. Tools>>Port  - Select the port that now shows (Adafruit Feather M0 Express (SAMD21)).
7. Sketch>>Upload.  The IDE will begin compiling, then begin flash programming the device. 
8. Programming should complete with no warnings or errors. 

The Hardware consists of the following:
1. Feather M0 Express w/Stacking headers
2. Feather OLED 128x32 with standard male header
3. 350mAH lipo - Adafruit- Fits between stacked feather boards. 
4. DRV8833 TI Drive Breakout
5. 3d Printed housing - Onshape: Proto_UIcontrol>>Enclosure4V2
6. See code for pinouts 
7. (Not Used)BS84 P-Chan Mosfet- OLED Pwr Switch
8. (Not USed)0402 15-20k ohm resistor (Pull up for pchannel)

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

