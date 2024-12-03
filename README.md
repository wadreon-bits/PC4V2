# PC4V2  Proto Controller 4volt 2 Channel (Another Stupid Acronym) 

Prototype test platform- Code application and devkit based hardware.
DC Motor controller with Display UI, Aimed to test small ERM motors. 2 channel output.
Capable of providing 12bit waveform on 2 motor drive channels at 100Hz update rate. 

The Hardware consists of the following:
1. Feather M0 Express w/Stacking headers
2. Feather OLED 128x32 with standard male header
3. 350mAH lipo
4. DRV8833 TI Drive Breakout
5. 3d Printed housing - Onshape: MotorTester4V2-R5 3d model 

If adding low power disable to OLED- Following also needed
5. BS84 P-Chan Mosfet- OLED Pwr Switch
6. 0402 15-20k ohm resistor (Pull up for pchannel)


Firmware Notes:
1. Firmware application built original in AS7 at LDC- modified to use Arduino framework
for this version. Based on PC4V1. Now named PC4V2
2. Git Repo will hold updated firmware application files. Along with support docs and support tools.


Operational Function:
1. Main adjustments made to ERM output patterns will be made within mode_arrays.h
This file can easily be modified for any waveform of interest. Each mode had 2 arrays,
1 for each motor channel A/B.  These simple integer arrays hold 10bit PWM duty cycle values. 
These values are processed at 100Hz. Array can be length 1 to MANY! Limited by prog flash space.
Values can be between 0(motor off), and 1023(Motor full pwer) 

