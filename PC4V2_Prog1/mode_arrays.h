#ifndef mode_array_h
#define mode_array_h

#include <WString.h>

/*
Drive and LED Arrays are processed as follows- 
	EACH ROW ELEMENT IS PROCESSED PER MODE_MANANGER EVENT = Every 10ms
	So Each 100 array elements will provide 1second of motor waveform/pattern
  100HZ processing. So up to a 100Hz 'Pattern' can be modulated   DIRB-100%     MOTOR OFF   DIRA-100%
  Each array position holds a 10bit motor output pwm value.         -1023   ~~~~  0 ~~~~~    +1023.
  Negative values turn the motor in the reverse direction. Do not reverse motor quickly!!
  ex: const unsigned int DemoArray[] = {1023, 1023, 0, 0};  This array makes motor goto full power for 20ms, then Off for 20ms, repeat. 
  Use Python script- ExceltoCArray.py to simplify generating large arrays. 
  Or just hand build arrays below. 
  no immediate limit to array lengths. Limited by Flash memory- Constants store in Flash. 

  NOTE- as of 11/27/24-  In order to handle a second motor channel. Also have ability to have motorsA/B have different patterns,
  I just added 2 arrays for each mode A/B arrays. They can be completely different values and length. 
  driveA = Drive Channel A
  driveB = Drive Channel B  
  
  **!! DO NOT RECOMMEND QUICKLY REVERSING A MOTORS ROTATION DIR!!! No FW/HW protection built in. 
  Stop motor before reversal- exact timing specific to motor. 
*/
//######################### MODE 1 ARRAY ############################################//     Single DC Output Value (0 to +/-1023) = (0-100% Power), negative changes motor dir
const int driveA_mode1_array[]  = {510};    //Motor Channel A
const int driveB_mode1_array[]  = {510};    //Motor Channel B

//######################### MODE 2 ARRAY ############################################//     Single DC Output Value (-1023 <-> +1023) = (0-100% Power), negative changes motor dir
const int driveA_mode2_array[]   = {770};
const int driveB_mode2_array[]   = {770};

//######################### MODE 3 ARRAY ############################################//     Single DC Output Value (-1023 <-> +1023) = (0-100% Power), negative changes motor dir
const int driveA_mode3_array[]   = {1023};    //showing motors going opposite dir
const int driveB_mode3_array[]   = {-1023};   //showing motos going opposite dir

//######################### MODE 4 ARRAY ############################################//     10Hz Square Wave alternating ~50%/~100%
const int driveA_mode4_array[]   = {500, 500, 500, 500, 500, 1000, 1000, 1000, 1000, 1000};  
const int driveB_mode4_array[]   = {500, 500, 500, 500, 500, 1000, 1000, 1000, 1000, 1000};  

//######################### MODE 5 ARRAY ############################################//     20Hz Square Wave Alternating 0%/~100%
const int driveA_mode5_array[]   = {0, 0, 0, 0, 1000, 1000, 1000, 1000}; 
const int driveB_mode5_array[]   = {0, 0, 0, 0, 1000, 1000, 1000, 1000};  

//######################### MODE 6 ARRAY ############################################//     50Hz Square Wave ~75% power
const int driveA_mode6_array[]  = {820,0,820,0,820,0,820,0,820,0};                          //showing different array values per motor array
const int driveB_mode6_array[]  = {0,820,0,820,0,820,0,820,0,820};

//######################### MODE 7 ARRAY ############################################//     Whatever pattern
const int driveA_mode7_array[]  = {1020,1020,1020,1020,1020,800,800,800};
const int driveB_mode7_array[]  = {1020,1020,1020,1020,1020,800,800,800};

//######################### MODE 8 ARRAY ############################################//     Random long Pattern
const int driveA_mode8_array[]  = {200,400,800,800,800,800,400,200,200,400,400,600,600,700,800,1000,1000,1000,1000,900,700,400,200,200,200}; 
const int driveB_mode8_array[]  = {200,400,800,800,800,800,400,200,200,400,400,600,600,700,800,1000,1000,1000,1000,900,700,400,200,200,200};

//######################### MODE 9 ARRAY ############################################//     Whatever showing different array lengths
const int driveA_mode9_array[]  = {0,400,500,600,700,800,900,1023};
const int driveB_mode9_array[]  = {0,400,500,600,700,800,900,1023, 1023, 900, 800, 700, 600, 500, 400, 0};

//######################### MODE 10 ARRAY ###########################################//     Motors opposing sweep - Kinda Cool feeling 
const int driveA_mode10_array[] = {0,34,68,102,136,170,204,238,272,306,340,374,408,442,476,510,544,578,612,646,680,714,748,782,816,850,884,918,952,986,1020};
const int driveB_mode10_array[] = {1020,986,952,918,884,850,816,782,748,714,680,646,612,578,544,510,476,442,408,374,340,306,272,238,204,170,236,102,68,34,0};





//######################### SERIAL LED IDLE ARRAY {R, G, B}###################################//
const unsigned int serLed_idleArray[147][3] = { //Slow Heartbeat- Made in Excel. 
	{0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   //100ms
  {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   //200ms
  {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   //300ms
  {0,0,10},   {0,0,11},   {0,0,12},   {0,0,13},   {0,0,14},   {0,0,16},   {0,0,17},   {0,0,19},   {0,0,21},   {0,0,23},   //400ms
  {0,0,25},   {0,0,28},   {0,0,31},   {0,0,34},   {0,0,37},   {0,0,41},   {0,0,45},   {0,0,50},   {0,0,55},   {0,0,61},   //500ms
  {0,0,67},   {0,0,74},   {0,0,81},   {0,0,89},   {0,0,98},   {0,0,108},  {0,0,119},  {0,0,131},  {0,0,144},  {0,0,158},  //600ms
  {0,0,174},  {0,0,174},  {0,0,174},  {0,0,174},  {0,0,174},  {0,0,174},  {0,0,174},  {0,0,174},  {0,0,174},  {0,0,157},  //700ms
  {0,0,141},  {0,0,127},  {0,0,114},  {0,0,103},  {0,0,92},   {0,0,83},   {0,0,75},   {0,0,67},   {0,0,60},   {0,0,54},   //800ms
  {0,0,49},   {0,0,44},   {0,0,39},   {0,0,35},   {0,0,32},   {0,0,32},   {0,0,32},   {0,0,32},   {0,0,32},   {0,0,32},   //900ms
  {0,0,35},   {0,0,39},   {0,0,43},   {0,0,47},   {0,0,52},   {0,0,57},   {0,0,63},   {0,0,69},   {0,0,76},   {0,0,83},   //1000ms
  {0,0,92},   {0,0,101},  {0,0,100},  {0,0,100},  {0,0,100},  {0,0,100},  {0,0,87},   {0,0,74},   {0,0,62},   {0,0,53},   //1100ms
  {0,0,44},   {0,0,38},   {0,0,32},   {0,0,27},   {0,0,23},   {0,0,19},   {0,0,16},   {0,0,13},   {0,0,11},   {0,0,10},   //1200ms
  {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   //1300ms
  {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   //1400ms
  {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10},   {0,0,10}                                        //1500ms
};
                    

#endif
