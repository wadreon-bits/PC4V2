#ifndef mode_array_h
#define mode_array_h


#include <WString.h>

/*
Drive and LED Arrays are processed as follows- 
	EACH ROW ELEMENT IS PROCESSED PER MODE_MANANGER EVENT = 10ms
	Each 100 Row elements will provide 1second of events/values
  100HZ processing. 

  Use Python script- ExceltoCArray.py to simplify generated large arrays. 
  Or just hand build arrays below. Make certain to include the proper array length []
  no immediate limit to array lengths. Limited by RAM. 

  NOTE- as of 11/27/24-  No modification have been made to include handling new 2nd motor channel-
  Just using existing arrays below for both motorA and motorB. Could seperate and modify code to read from seperate arrays for each motor.
  
  
*/
//######################### MODE 1 ARRAY ############################################//
volatile const unsigned int driveA_mode1_array[10]  = {320,0,320,0,320,0,320,0,320,0};
volatile const unsigned int driveB_mode1_array[10]  = {320,0,320,0,320,0,320,0,320,0};

//######################### MODE 2 ARRAY ############################################//
volatile const unsigned int driveA_mode2_array[1]   = {500};
volatile const unsigned int driveB_mode2_array[1]   = {500};

//######################### MODE 3 ARRAY ############################################//
volatile const unsigned int driveA_mode3_array[1]   = {500};
volatile const unsigned int driveB_mode3_array[1]   = {500};

//######################### MODE 4 ARRAY ############################################//
volatile const unsigned int driveA_mode4_array[1]   = {500};
volatile const unsigned int driveB_mode4_array[1]   = {500};

//######################### MODE 5 ARRAY ############################################//
volatile const unsigned int driveA_mode5_array[1]   = {500};
volatile const unsigned int driveB_mode5_array[1]   = {500};

//######################### MODE 6 ARRAY ############################################//
volatile const unsigned int drive_mode6_array[10] = {320,0,320,0,320,0,320,0,320,0};
volatile const unsigned int driveA_mode6_array[10] = {820,0,820,0,820,0,820,0,820,0};
volatile const unsigned int driveB_mode6_array[10] = {820,0,820,0,820,0,820,0,820,0};

//######################### MODE 7 ARRAY ############################################//
volatile const unsigned int drive_mode7_array[10] = {320,320,320,320,320,0,0,0,0,0};
volatile const unsigned int driveA_mode7_array[10] = {320,320,320,320,320,0,0,0,0,0};
volatile const unsigned int driveB_mode7_array[10] = {320,320,320,320,320,0,0,0,0,0};

//######################### MODE 8 ARRAY ############################################//
volatile const unsigned int drive_mode8_array[20] = {5,20,40,80,120,160,180,220,260,300, 300, 300, 300, 260,220,180,140,100,60,20};  
volatile const unsigned int driveA_mode8_array[20] = {5,20,40,80,120,160,180,220,260,300, 300, 300, 300, 260,220,180,140,100,60,20}; 
volatile const unsigned int driveB_mode8_array[20] = {5,20,40,80,120,160,180,220,260,300, 300, 300, 300, 260,220,180,140,100,60,20}; 

//######################### MODE 9 ARRAY ############################################//
volatile const unsigned int drive_mode9_array[10] = {5,40,80,120,160,200,240,280,320,320};
volatile const unsigned int driveA_mode9_array[10] = {5,40,80,120,160,200,240,280,320,320};
volatile const unsigned int driveB_mode9_array[10] = {5,40,80,120,160,200,240,280,320,320};

//######################### MODE 10 ARRAY ###########################################//
volatile const unsigned int drive_mode10_array[31] = {0,34,68,102,136,170,204,238,272,306,340,374,408,442,476,510,544,578,612,646,680,714,748,782,816,850,884,918,952,986,1020};
volatile const unsigned int driveA_mode10_array[31] = {0,34,68,102,136,170,204,238,272,306,340,374,408,442,476,510,544,578,612,646,680,714,748,782,816,850,884,918,952,986,1020};
volatile const unsigned int driveB_mode10_array[31] = {0,34,68,102,136,170,204,238,272,306,340,374,408,442,476,510,544,578,612,646,680,714,748,782,816,850,884,918,952,986,1020};





//######################### SERIAL LED IDLE ARRAY ###################################//
const unsigned int serLed_idleArray[147][3] = {
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
