#ifndef mode_array_h
#define mode_array_h

#include <WString.h>
#define arrayLen(x) sizeof(x)/sizeof(x[0])  //custom macro for sizing motor/led arrays. 
/*
Drive and LED Arrays are processed as follows- 
	EACH ROW ELEMENT IS PROCESSED PER MODE_MANANGER EVENT = Every 10ms
	So Each 100 array elements will provide 1second of motor waveform/pattern
  100HZ processing. So up to a 100Hz 'Pattern' can be modulated   DIRB-100%     MOTOR OFF   DIRA-100%
  Each array position holds a 10bit motor output pwm value.         -1023   ~~~~  0 ~~~~~    +1023.
  Negative values turn the motor in the reverse direction. Do not reverse motor quickly!!
  ex: const unsigned int DemoArray[] = {1023, 1023, 0, 0};  This array makes motor goto full power for 20ms, then Off for 20ms, repeat. 
  Many methods to visualize and simplify building the waveform pattern
  Use Python script- ExceltoCArray.py to simplify generating large arrays. 
  Excel- Build pattern, equation, graph etc.  Can export simple .CSV data. Simple! 
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
const int driveA_mode1[]  = {900};    //Motor Channel A
const int driveB_mode1[]  = {900};    //Motor Channel B

//######################### MODE 2 ARRAY ############################################//     Single DC Output Value (-1023 <-> +1023) = (0-100% Power), negative changes motor dir
const int driveA_mode2[]   = {770};
const int driveB_mode2[]   = {770};

//######################### MODE 3 ARRAY ############################################//     Single DC Output Value (-1023 <-> +1023) = (0-100% Power), negative changes motor dir
const int driveA_mode3[]   = {1023};    //showing motors going opposite dir
const int driveB_mode3[]   = {-1023};   //showing motos going opposite dir

//######################### MODE 4 ARRAY ############################################//     10Hz Square Wave alternating ~50%/~100%
const int driveA_mode4[]   = {500, 500, 500, 500, 500, 1000, 1000, 1000, 1000, 1000};  
const int driveB_mode4[]   = {500, 500, 500, 500, 500, 1000, 1000, 1000, 1000, 1000};  

//######################### MODE 5 ARRAY ############################################//     20Hz Square Wave Alternating 0%/~100%
const int driveA_mode5[]   = {0, 0, 0, 0, 1000, 1000, 1000, 1000}; 
const int driveB_mode5[]   = {0, 0, 0, 0, 1000, 1000, 1000, 1000};  

//######################### MODE 6 ARRAY ############################################//     50Hz Square Wave ~75% power
const int driveA_mode6[]  = {820,0,820,0,820,0,820,0,820,0};                          //showing different array values per motor array
const int driveB_mode6[]  = {0,820,0,820,0,820,0,820,0,820};

//######################### MODE 7 ARRAY ############################################//     Whatever pattern
const int driveA_mode7[]  = {1020,1020,1020,1020,1020,800,800,800};
const int driveB_mode7[]  = {1020,1020,1020,1020,1020,800,800,800};

//######################### MODE 8 ARRAY ############################################//     Random long Pattern
const int driveA_mode8[]  = {200,400,800,800,800,800,400,200,200,400,400,600,600,700,800,1000,1000,1000,1000,900,700,400,200,200,200}; 
const int driveB_mode8[]  = {200,400,800,800,800,800,400,200,200,400,400,600,600,700,800,1000,1000,1000,1000,900,700,400,200,200,200};

//######################### MODE 9 ARRAY ############################################//     Whatever showing different array lengths
const int driveA_mode9[]  = {0,400,500,600,700,800,900,1023};
const int driveB_mode9[]  = {0,400,500,600,700,800,900,1023, 1023, 900, 800, 700, 600, 500, 400, 0};

//######################### MODE 10 ARRAY ###########################################//     Motors opposing sweep - Kinda Cool feeling 
const int driveA_mode10[] = {0,34,68,102,136,170,204,238,272,306,340,374,408,442,476,510,544,578,612,646,680,714,748,782,816,850,884,918,952,986,1020};
const int driveB_mode10[] = {1020,986,952,918,884,850,816,782,748,714,680,646,612,578,544,510,476,442,408,374,340,306,272,238,204,170,236,102,68,34,0};

//######################### MODE xx ARRAY - RampingSweeps1 - elaborated on mode9array###########################################//
//const int driveA_modeXX[] = {0,25,50,75,100,125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750,775,800,825,850,875,900,925,950,975,1000,1023,0,34,68,102,136,170,204,238,272,306,340,374,408,442,476,510,544,578,612,646,680,714,748,782,816,850,884,918,952,986,1020,0,51,102,153,204,255,306,357,408,459,510,561,612,663,714,765,816,867,918,969,1020,0,102,204,306,408,510,612,714,816,918,1020};
//const int driveB_modeXX[] = {1020,995,970,945,920,895,870,845,820,795,770,745,720,695,670,645,620,595,570,545,520,495,470,445,420,395,370,345,320,295,270,245,220,195,170,145,120,95,70,45,20,0,1020,986,952,918,884,850,816,782,748,714,680,646,612,578,544,510,476,442,408,374,340,306,272,238,204,170,136,102,68,34,0,1020,969,918,867,816,765,714,663,612,561,510,459,408,357,306,255,204,153,102,51,0,1020,918,816,714,612,510,408,306,204,102,0};







const unsigned int usermode_leds[10][3] = {{0,0,20},{0,0,50},{0,0,100},{0,0,150},{0,0,250},{150,0,150},{20,120,75},{150,50,20},{20,150,30},{75,37,0}};
const int *ptr_arraysA[] = {driveA_mode1, driveA_mode2, driveA_mode3, driveA_mode4, driveA_mode5, driveA_mode6, driveA_mode7, driveA_mode8, driveA_mode9, driveA_mode10};
const int *ptr_arraysB[] = {driveB_mode1, driveB_mode2, driveB_mode3, driveB_mode4, driveB_mode5, driveB_mode6, driveB_mode7, driveB_mode8, driveB_mode9, driveB_mode10};
const int arrayA_lengths[10]= {arrayLen(driveA_mode1), arrayLen(driveA_mode2), arrayLen(driveA_mode3), arrayLen(driveA_mode4), arrayLen(driveA_mode5), arrayLen(driveA_mode6), arrayLen(driveA_mode7), arrayLen(driveA_mode8), arrayLen(driveA_mode9), arrayLen(driveA_mode10)};
const int arrayB_lengths[10]= {arrayLen(driveB_mode1), arrayLen(driveB_mode2), arrayLen(driveB_mode3), arrayLen(driveB_mode4), arrayLen(driveB_mode5), arrayLen(driveB_mode6), arrayLen(driveB_mode7), arrayLen(driveB_mode8), arrayLen(driveB_mode9), arrayLen(driveB_mode10)};
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
