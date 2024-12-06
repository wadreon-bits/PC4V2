/*
111824
PC4V2 - MotorTester1S 
Proto Control 4Volts Dual HBridge 2 channel output

FW Versions:
1.0		111824		Working rebuild of PC4V1 built for LDC. Refactored code for Arduino Framework. Sequences array pwm data to 2 H-bridge channels. 
1.1   120124    First major working release to git- Could be used in field-
1.2   120524    Changed btn_scan() function.  
Arduino Setup Notes: 
See pc4v_support.h for all the libraries needed. Must follow adafruit instructions on installing their libraries. Such as GFX etc.
- Adafruit libraries are all located on a github repo. New version of arduino IDE is can search and install adafruit libs
Sketch >> Include Library >> Library Manager >> Manage Libraries >> Search for library 

HARDWARE NOTES:
Devkit Based Hardware- 
FeatherM0Express
FeatherOLED
DRV8833 Breakout

SOME BUTTON DEFINITIONS - REMOVE NO LONGER NEEDED FOR BASIC UI 
UP - Generic Up index for UI
DWN - Generic Dwn index for UI

Primary Functions to Adjust for Product Interface -
	- app_update()    App Timing			      Manages	primary application timing/time slicing
	- btn_scan()      BTN UI/Ergonomics			Manages Button ergo- press timing, clicks, etc
	- mode_manager()	Functional Modes		  Manages enabling motor/led modes and handling periodic mode events
  - disp_manager()  OLED Interface        Single function to write/read from OLED display. 
	
Notes-
Arduino makes code writing FAST!!! I hate the IDE- USE AS7 or VScode 

TODO 1203- Prior to send
- Clean up float use in batt read
- Modify and insert useable arrays for all modes
- clean up flags vs events? 
- 
*/
//##############################################################
//  Preamble Include definitions
//##############################################################
#include "pc4v_support.h"

//##############################################################
//  FUNCTION PROTOTYPES
//##############################################################
/*
// Proper .C - populate Function proto's. For compiling with standard C compiler 
// DELETED - ARDUINO FRAMEWORK DOE SALL THIS
*/
#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif
//End of Auto generated function prototypes by Atmel Studio

//##############################################################
//  Preprocessor #DEFINES
//##############################################################
#define FW_VERSION			"R1.2"	  //version of this application. Update with Any changes made
#define FW_VER_DATE     "120624"  //Date of fw version update. Update when version changes. 
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

/*MACROS*/  
#define WIRE Wire 
#define arrayLen(x) sizeof(x)/sizeof(x[0])  //custom macro for sizing motor/led arrays. 

/*GLOBAL APP EVENT FLAGS*/ 
volatile bool flag_scanBtns   = false;    //flag
volatile bool flag_10ms       = false;    //flag
volatile bool flag_100ms      = false;    //flag
volatile bool flag_1second    = false;    //flag

volatile bool flag_flickerLED = false;    //event
volatile bool flag_gotoSleep  = false;    //event
volatile bool flag_wake_init  = false;    //flag
volatile bool flag_crit_batt  = false;    //flag
volatile bool flag_low_batt   = false;    //event
volatile bool flag_updateDisp = false;    //event

//volatile bool uiflag_fastRate = true; 
volatile bool flag_blockSleep = false;    //set to true to disable sleep 
volatile bool disp_changed    = false; 
bool written_once             = false;

/*Global variables*/
volatile unsigned int btns_state  = 0;	// stores pins states
volatile unsigned int btns_cnt    = 0;	// stores pins states
unsigned int btns_stateLast       = 0;		// last pin states

volatile signed int drive_state   = 0;	// Current output duty of drive. 
volatile signed int req_mode      = 0;		// Not very useful- redundant- currently used to track first pass through event_manager- could use case_cnt
volatile signed int active_mode   = 99;	// MODE = USER SETTING or USER LEVEL
unsigned int led_on               = 0;				// counter to turn off led
unsigned int sleep_counter        = 0;         //
int timer_id                      = 0; 
unsigned int crit_batt_cntdown    = 0; 
unsigned int bod33_cnt            = 0; 

int temp_32; 

float batt_volts;
float avg_batt_volts = 4.0; 
String batt_soc; 

/* Instantiate Class Object Variables */
Timer app_timer;
Adafruit_NeoPixel serled(1, SER_LED, NEO_GRB + NEO_KHZ800);
#ifdef OLED_CONNECTED
  //Adafruit_SSD1306 display = Adafruit_SSD1306();
  Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &WIRE);
#endif


// Synch AC
static __inline__ void ACsync() __attribute__((always_inline, unused));
static void   ACsync() {
	while (AC->STATUSB.bit.SYNCBUSY == 1);
}
// Synch GCLK
static __inline__ void syncADC() __attribute__((always_inline, unused));
static void syncGCLK() {
	while (GCLK->STATUS.bit.SYNCBUSY == 1);
}

//this will be called when the BOD33 threshold is crossed
//once the controller comes out of reset (VAnna rises above threshold)
// so when the battery gets toward BOD33, it will begin interuppting the
//void PM_Handler(){
		//
//}
void SYSCTRL_Handler(){
	int temp_32;

	SYSCTRL->INTFLAG.bit.BOD33DET = 0x1; // clear flag to re-arm interrupt
	SYSCTRL->INTENCLR.bit.BOD33DET = 0x1; //disable interrupt
	//serled.setPixelColor(0, serled.Color(150, 0, 0));
	//serled.show();
	REG_SYSCTRL_INTFLAG |= 0x00000400;  //clear the interrupt flag
	flag_crit_batt = true;
	crit_batt_cntdown = 150;
  flag_low_batt = true; 
}

/*Power Manager interupt support routine - Not being used*/
//void PM_handler(void){
	//if(REG_PM_RCAUSE == 0x04){ //Brown Out Detected-- Goto Sleep
		//serled.setPixelColor(0, serled.Color(150, 0, 0));
		//serled.show();
		//app_timer.stop(timer_id);
		//delay(500);
		//serLed_off();
		//
		//timer_id = app_timer.every(APP_BASE_TIME, app_update);
		////LowPower.deepSleep();
	//}
//}

// the setup function runs once when you press reset or power the board
void setup() {
  /* Disable the brown-out detector during configuration,
    otherwise it might misbehave and reset the
    microcontroller. */
  SYSCTRL->BOD33.bit.ENABLE = 0;
  while (!SYSCTRL->PCLKSR.bit.B33SRDY) {};  //wait for sync

  /* Configure the brown-out detector so that the
    program can use it to watch the power supply
    voltage */
  SYSCTRL->BOD33.reg = (
  /* This sets the minimum voltage level to 3.0v - 3.2v.
      See datasheet table 37-21. */
  SYSCTRL_BOD33_LEVEL(48) |
  /* Since the program is waiting for the voltage to rise,
      don't reset the microcontroller if the voltage is too
      low. */
  SYSCTRL_BOD33_ACTION_INTERRUPT|
  
  /* Enable hysteresis to better deal with noisy power
      supplies and voltage transients. */
  SYSCTRL_BOD33_HYST);

  /* Enable the brown-out detector and then wait for
     the voltage level to settle. */
  SYSCTRL->BOD33.bit.ENABLE = 1;        

  SYSCTRL->INTENSET.bit.BOD33DET = 0x1; //enable BOD33 interrupt 
	
	NVIC_EnableIRQ(SYSCTRL_IRQn);   //enable global interupts
  // //END NEW BOD CODE HERE
	//REG_SYSCTRL_INTENSET |= 0x00000400; //enable BOD33 interrupt; 
	SYSCTRL->INTFLAG.bit.BOD33DET = 0x1;    //Enable BOD interrupt
	
	pinMode(13, OUTPUT);                //Board Simple RED LED
  pinMode(OLED_EN_PIN, OUTPUT);       //OLED Enable - Not used in current vers
  digitalWrite(OLED_EN_PIN, LOW);     //TURN ON DISPLAY EN
  pinMode(DRIVE_FLT_PIN, INPUT);      //FAULT I/O from motor drive PULLUP??
  pinMode(DRIVE_SLP_PIN, OUTPUT);     //Sleep I/O from motor drive
  digitalWrite(DRIVE_SLP_PIN, HIGH);  //TURN ON DRIVE 
  
	delay(200);                       //wait for OLED charge pumps etc
  pinMode(DRIVE_A1_PIN, OUTPUT);    //Drive A1 Output PWM PIn
 	pinMode(DRIVE_A2_PIN, OUTPUT);    //Drive A2 Output PWM PIn
  pinMode(DRIVE_B1_PIN, OUTPUT);    //Drive B1 Output PWM PIn 
	pinMode(DRIVE_B2_PIN, OUTPUT);    //Drive B2 Output PWM PIn
	
  pinMode(BTN_1_PIN, INPUT_PULLUP); //OLED Button - Not using Middle Button
	pinMode(BTN_2_PIN, INPUT_PULLUP); //OLED Button - Not using Middle Button

	set_drive(1,0);                   //Turn Off Drive Channel A
  set_drive(2,0);                   //Turn Off Drive Channel A
#ifdef DEBUG_PRINTS
	Serial.begin(115200);             //If Debug Defined- Console Prints
  //while(!Serial);
#endif
	analogWriteResolution(10);        //Increase Analog Write Res for PWM channels
  analogReadResolution(12);         //Increase Analog Read Res for Batt Measure and future motor measure
	serled.begin();                   //Init serial Pixel 
	serled.clear();                   //Set pixel to off
	serled.show();                    //send off command to pix
 #ifdef OLED_CONNECTED
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
    manage_disp(BOOT);                          // Display BOOT screen - version etc. 
#endif

#ifdef DEBUG_PRINTS
    Serial.print(F("FW Version:")); //Console version print
	  Serial.println(F(FW_VERSION));   
#endif 
    delay(2000);                    //Just wait for everything
  //write version;
	timer_id = app_timer.every(APP_BASE_TIME, app_update);  //Main app timer
	__enable_irq(); //enable global IRQ;                    //Enable Global Interuppts- Not sure if needed in arduino land
	flag_gotoSleep = true;            // Goto sleep right away
  flag_updateDisp = true;           // 
}


/*Initialize things before Sleep*/
void init_sleep(void){ //called before sleep call
  //DRIVE OFF
  set_drive(1, 0);                //TURN OFF MOTOR1
  set_drive(2, 0);                //TURN OFF MOTOR2
  //delayMicroseconds(20);
  //Set Sleep Pin
  pinMode(DRIVE_SLP_PIN, OUTPUT); 
  digitalWrite(DRIVE_SLP_PIN, LOW);
  pinMode(DRIVE_SLP_PIN, INPUT);

  //LEDS OFF
  pinMode(BRD_LED, OUTPUT);   //
  digitalWrite(BRD_LED, LOW); //TURN OFF FRAMEWORK LED
  serLed_off();               //TURN OFF SERIAL LED
  
  //SETUP WAKE BTNS
  pinMode(BTN_1_PIN, INPUT_PULLUP);
	pinMode(BTN_2_PIN, INPUT_PULLUP);

  //CLEAR AND DISABLE DISPLAY 
  manage_disp(INITSLEEP);
  delay(1);
  //DISABLE DISPLAY
  pinMode(OLED_EN_PIN, INPUT);  //PULL UP TURNS OFF

  //SETUP WAKE INTERUP 
  LowPower.attachInterruptWakeup(BTN_1_PIN, wake_ISR, FALLING);
	LowPower.attachInterruptWakeup(BTN_2_PIN, wake_ISR, FALLING);
  //__enable_irq();
}

/*Initialize things after Wakup*/
void init_wakeUp(void){ //called right after wakeup
  
	SYSCTRL->INTENSET.bit.BOD33DET = 0x1; //enable BOD33
	NVIC_EnableIRQ(SYSCTRL_IRQn);
	//REG_SYSCTRL_INTENSET |= 0x00000400; //enable BOD33 interrupt;
	SYSCTRL->INTFLAG.bit.BOD33DET = 0x1; //clear flag to re-arm interrupt

  //ENABLE DRIVE
  pinMode(DRIVE_SLP_PIN, OUTPUT);
  digitalWrite(DRIVE_SLP_PIN, HIGH);
  
  //ENABLE DISPLAY
  pinMode(OLED_EN_PIN, OUTPUT);
  digitalWrite(OLED_EN_PIN, LOW);  //set low to enable - pchannel hi-side fet. 

  delay(200); //allow charge pump to fill on OLED- Takes awhile
  manage_disp(INITWAKE);
  //__enable_irq();   //re-enable global interrupts
}

/*Button Interrupt- Can use. Or rely on app polling*/
void btn_ISR() {
  flag_scanBtns = true;
}
/*Wake Interrupt- Called for wakeup ISR */
void wake_ISR(){
  //__disable_irq();
	sleep_counter = 0; 
  flag_wake_init = true; 
	//bod33_cnt = 0; 
}

/*Application Timing*/
void app_update() { //runs every 10ms
  volatile static int btnScan_timer;
  volatile static int tenms_timer, hundredms_timer, oneSecond_timer;

  /*Used to Scope Timer functions- Can bury within timeslice loops-10ms,100ms,1s etc. */
  #ifdef ENABLE_TIMERSCOPE
    pinMode(BRD_LED, OUTPUT);
    digitalWrite(BRD_LED, LOW);
    pinMode(BRD_LED, INPUT_PULLUP);
  #endif

  /*Polling Button Flag */
  if (++btnScan_timer >= BTN_SCAN_PER) { //BTN_SCAN_PER = 4 so 40millisconds - 25Hz
    btnScan_timer = 0;
    flag_scanBtns = true;
  }

  if (tenms_timer == TEN_MS_PER) {
    tenms_timer = 1;
    flag_10ms = true;

    if (hundredms_timer == (TEN_MS_PER * 10)) {
      hundredms_timer = 1;
      flag_100ms = true;
      if (oneSecond_timer == (TEN_MS_PER * 10)) {
        flag_1second = true;
        oneSecond_timer = 1;
      }
      else
        oneSecond_timer++;
    }
    else
      hundredms_timer++;
  }
  else
    tenms_timer++;
}//end of function

/*simple btn scan function - loads byte with pin status- Reads buttons/Debounces/Manages state variables*/
unsigned int scan_btns() { //ENTER FUNCTION CALL PERIOD HERE: 20ms
  static unsigned int case_cnt;
  static unsigned int ui_static_cnt = 0;
  static unsigned int btn_release_event = 0;
  static bool ui_blocked = false;

	pinMode(BTN_1_PIN, INPUT_PULLUP);
	pinMode(BTN_2_PIN, INPUT_PULLUP);
  delayMicroseconds(20);
  //btns_state will be a byte holding multiple switch states 
  // for PCV4 = 2btns.  0|0|0|0|0|0|BTN2=UP|BTN1=DWN
  btns_state = ((int(!digitalRead(BTN_UP) << 1)) | (int(!digitalRead(BTN_DWN)))  );
  //int btn_state[2]   btn_state[1]: btn pressed, btn_state[2]: count

  /*TEST FOR BTNS CHANGED??*/
  if (btns_state != btns_stateLast) {   // ### TEST FOR CHANGE IN BTNS ###
    btns_cnt = 0; 
    case_cnt = 0; //btns have changed- reset case_cnt
    if (btns_state == 0){  //btns have changed, and are just released released 
      if(btn_release_event) btns_state = btn_release_event;
      btn_release_event = 0;
    } 
  }
  else    //!### NOTHING HAS CHANGED !###
    btns_cnt++;               //counting any press combo, case_cnt counts only defined cases. 
  
  /* TEST FOR BUTTONS PRESSED OR RELEASED */
  if (btns_state == 0) {
    ui_static_cnt++;
    ui_blocked = false;
  }
  else{
    ui_static_cnt = 0;
    sleep_counter = 0;
  }

  /*SAVE BUTTON STATE*/
  btns_stateLast = btns_state; //maybe move to end. Incorporate single click pending test up top of function
  

  if(ui_blocked) return btns_state;

  switch (btns_state) {   //Begin Case Statement User Interface - UI 
    case 0:
      case_cnt = 0;
    break;

    case BTN_UP_MASK:
      /*Begin testing for short First Click*/
      if (case_cnt == BTN_SHORTPRESS_TIME) {
      //case_cnt = 0;
        req_mode++; 
      }
      case_cnt++;  //case_cnt is an integer will rollover at 65k- Can play with location to make one-shot or repeated events
      if(case_cnt >= 20)
        case_cnt = 0; 
    break;

    case BTN_DWN_MASK:
      if (case_cnt == BTN_SHORTPRESS_TIME){
        req_mode--;
        //ui_blocked = true; 
        //case_cnt = 0; //reset case_cnt for sustained increment
      }      
      case_cnt++;
      if(case_cnt >= 20)
        case_cnt = 0; 
    break;

    case BTN_ALL_MASK:
      if (case_cnt == BTN_NORMPRESS_TIME+4) {
        case_cnt = 0;
        serLed_flicker(50); 
        //btn_release_event = BTN_ALL_MASK;
        ui_blocked = true; 
      }
      else 
        case_cnt++;
      
    break;

    default:
      case_cnt = 0;
      break;

  }//end switch(btns_state)

  return btns_state;
}//end scan_btns function

/*Manage Drive and LED - run every mode time slice = 10ms */
/* Should take time to make this a seperate class- for reuse: All cases same*/
void manage_modes(){
	static int case_cnt = 0;
	static int period_cnt = 0; 
	static int mode_indexA = 0;	//generic use motor channelA
	static int mode_indexB = 0;	//generic use motor channelB
  static unsigned int arraysizeA = 0;
  static unsigned int arraysizeB = 0;
	
	if(req_mode > 10)
		req_mode = 0;
  if(req_mode < 0)
    req_mode = 0;
	
	if(req_mode!=active_mode){
		mode_indexA=0;
		mode_indexB=0;
    flag_updateDisp = true; 
    #ifdef DEBUG_PRINTS
      Serial.println("Mode:"+String(req_mode));
    #endif
	}    

	switch(req_mode){
		case 0:
			//Idle ready for input or sleep timeout
			if(drive_state!=0)
				set_drive(1, 0);
        set_drive(2, 0);
			
			serLed_on(serLed_idleArray[period_cnt][0], serLed_idleArray[period_cnt][1], serLed_idleArray[period_cnt][2]);
			period_cnt++;
			if(period_cnt>(arrayLen(serLed_idleArray)-1))
				period_cnt = 0; 
			if(active_mode!=req_mode)
				active_mode = req_mode; 
			break;
		
		case 1:
			if(active_mode!=req_mode){//mode_counters should be zero
				serLed_on(0,0, 20);
				//serLed_off();
				active_mode = req_mode;
        arraysizeA = arrayLen(driveA_mode1_array);  //check array lengths
        arraysizeB = arrayLen(driveB_mode1_array);
        set_drive(1, driveA_mode1_array[0]);        //set drive in motion using first value in array
        set_drive(2, driveB_mode1_array[0]);
			}
      else{
        if(arraysizeA>1){                                     //if array is larger than 1, continue indexing
          set_drive(1, driveA_mode1_array[mode_indexA++]);
          if(mode_indexA>(arrayLen(driveA_mode1_array)-1))
            mode_indexA = 0;

        }
        if(arraysizeB>1){
          set_drive(2, driveB_mode1_array[mode_indexB++]);
          if(mode_indexB>(arrayLen(driveB_mode1_array)-1))
            mode_indexB = 0;
        }
      }   
			break;
		
		case 2:
			if(active_mode!=req_mode){//mode_counters should be zero
				serLed_on(0,0, 50);
				//serLed_off();
				active_mode = req_mode;
        arraysizeA = arrayLen(driveA_mode2_array);  //check array lengths
        arraysizeB = arrayLen(driveB_mode2_array);
        set_drive(1, driveA_mode2_array[0]);        //set drive in motion using first value in array
        set_drive(2, driveB_mode2_array[0]);
			}
      else{
        if(arraysizeA>1){                                     //if array is larger than 1, continue indexing
          set_drive(1, driveA_mode2_array[mode_indexA++]);
          if(mode_indexA>(arrayLen(driveA_mode2_array)-1))
            mode_indexA = 0;

        }
        if(arraysizeB>1){
          set_drive(2, driveB_mode2_array[mode_indexB++]);
          if(mode_indexB>(arrayLen(driveB_mode2_array)-1))
            mode_indexB = 0;
        }
      }   
			break;

		case 3:
			if(active_mode!=req_mode){//mode_counters should be zero
				serLed_on(0,0, 20);
				//serLed_off();
				active_mode = req_mode;
        arraysizeA = arrayLen(driveA_mode3_array);  //check array lengths
        arraysizeB = arrayLen(driveB_mode3_array);
        set_drive(1, driveA_mode3_array[0]);        //set drive in motion using first value in array
        set_drive(2, driveB_mode3_array[0]);
			}
      else{
        if(arraysizeA>1){                                     //if array is larger than 1, continue indexing
          set_drive(1, driveA_mode3_array[mode_indexA++]);
          if(mode_indexA>(arrayLen(driveA_mode1_array)-1))
            mode_indexA = 0;

        }
        if(arraysizeB>1){
          set_drive(2, driveB_mode3_array[mode_indexB++]);
          if(mode_indexB>(arrayLen(driveB_mode1_array)-1))
            mode_indexB = 0;
        }
      }   
			break;	
      
		case 4:
			if(active_mode!=req_mode){//mode_counters should be zero
				serLed_on(0,0, 20);
				//serLed_off();
				active_mode = req_mode;
        arraysizeA = arrayLen(driveA_mode4_array);  //check array lengths
        arraysizeB = arrayLen(driveB_mode4_array);
        set_drive(1, driveA_mode4_array[0]);        //set drive in motion using first value in array
        set_drive(2, driveB_mode4_array[0]);
			}
      else{
        if(arraysizeA>1){                                     //if array is larger than 1, continue indexing
          set_drive(1, driveA_mode4_array[mode_indexA++]);
          if(mode_indexA>(arrayLen(driveA_mode4_array)-1))
            mode_indexA = 0;

        }
        if(arraysizeB>1){
          set_drive(2, driveB_mode4_array[mode_indexB++]);
          if(mode_indexB>(arrayLen(driveB_mode4_array)-1))
            mode_indexB = 0;
        }
      }   
			break;

		case 5:
			if(active_mode!=req_mode){//mode_counters should be zero
				serLed_on(0,0, 20);
				//serLed_off();
				active_mode = req_mode;
        arraysizeA = arrayLen(driveA_mode5_array);  //check array lengths
        arraysizeB = arrayLen(driveB_mode5_array);
        set_drive(1, driveA_mode5_array[0]);        //set drive in motion using first value in array
        set_drive(2, driveB_mode5_array[0]);
			}
      else{
        if(arraysizeA>1){                                     //if array is larger than 1, continue indexing
          set_drive(1, driveA_mode5_array[mode_indexA++]);
          if(mode_indexA>(arrayLen(driveA_mode5_array)-1))
            mode_indexA = 0;
        }
        if(arraysizeB>1){
          set_drive(2, driveB_mode5_array[mode_indexB++]);
          if(mode_indexB>(arrayLen(driveB_mode5_array)-1))
            mode_indexB = 0;
        }
      }   
			break;

		case 6:
			if(active_mode!=req_mode){//mode_counters should be zero
				serLed_on(150,0, 150);
				//serLed_off();
				active_mode = req_mode;
        arraysizeA = arrayLen(driveA_mode6_array);  //check array lengths
        arraysizeB = arrayLen(driveB_mode6_array);
        set_drive(1, driveA_mode6_array[0]);        //set drive in motion using first value in array
        set_drive(2, driveB_mode6_array[0]);
			}
      else{
        if(arraysizeA>1){                                     //if array is larger than 1, continue indexing
          set_drive(1, driveA_mode6_array[mode_indexA++]);
          if(mode_indexA>(arrayLen(driveA_mode6_array)-1))
            mode_indexA = 0;
        }
        if(arraysizeB>1){
          set_drive(2, driveB_mode6_array[mode_indexB++]);
          if(mode_indexB>(arrayLen(driveB_mode6_array)-1))
            mode_indexB = 0;
        }
      }        
			break;

		case 7:
			if(active_mode!=req_mode){//mode_counters should be zero
				serLed_on(0,0, 20);
				//serLed_off();
				active_mode = req_mode;
        arraysizeA = arrayLen(driveA_mode7_array);  //check array lengths
        arraysizeB = arrayLen(driveB_mode7_array);
        set_drive(1, driveA_mode7_array[0]);        //set drive in motion using first value in array
        set_drive(2, driveB_mode7_array[0]);
			}
      else{
        if(arraysizeA>1){                                     //if array is larger than 1, continue indexing
          set_drive(1, driveA_mode7_array[mode_indexA++]);
          if(mode_indexA>(arrayLen(driveA_mode7_array)-1))
            mode_indexA = 0;

        }
        if(arraysizeB>1){
          set_drive(2, driveB_mode7_array[mode_indexB++]);
          if(mode_indexB>(arrayLen(driveB_mode7_array)-1))
            mode_indexB = 0;
        }
      }   
		  break; 
		
    case 8:
			if(active_mode!=req_mode){//mode_counters should be zero
				serLed_on(0,0, 20);
				//serLed_off();
				active_mode = req_mode;
        arraysizeA = arrayLen(driveA_mode8_array);  //check array lengths
        arraysizeB = arrayLen(driveB_mode8_array);
        set_drive(1, driveA_mode8_array[0]);        //set drive in motion using first value in array
        set_drive(2, driveB_mode8_array[0]);
			}
      else{
        if(arraysizeA>1){                                     //if array is larger than 1, continue indexing
          set_drive(1, driveA_mode8_array[mode_indexA++]);
          if(mode_indexA>(arrayLen(driveA_mode8_array)-1))
            mode_indexA = 0;

        }
        if(arraysizeB>1){
          set_drive(2, driveB_mode8_array[mode_indexB++]);
          if(mode_indexB>(arrayLen(driveB_mode8_array)-1))
            mode_indexB = 0;
        }
      }   
			break; 

		case 9:
			if(active_mode!=req_mode){//mode_counters should be zero
				serLed_on(0,0, 20);
				//serLed_off();
				active_mode = req_mode;
        arraysizeA = arrayLen(driveA_mode9_array);  //check array lengths
        arraysizeB = arrayLen(driveB_mode9_array);
        set_drive(1, driveA_mode9_array[0]);        //set drive in motion using first value in array
        set_drive(2, driveB_mode9_array[0]);
			}
      else{
        if(arraysizeA>1){                                     //if array is larger than 1, continue indexing
          set_drive(1, driveA_mode9_array[mode_indexA++]);
          if(mode_indexA>(arrayLen(driveA_mode9_array)-1))
            mode_indexA = 0;

        }
        if(arraysizeB>1){
          set_drive(2, driveB_mode9_array[mode_indexB++]);
          if(mode_indexB>(arrayLen(driveB_mode9_array)-1))
            mode_indexB = 0;
        }
      }   
			break;

		case 10:
			if(active_mode!=req_mode){//mode_counters should be zero
				serLed_on(0,0, 20);
				//serLed_off();
				active_mode = req_mode;
        arraysizeA = arrayLen(driveA_mode10_array);  //check array lengths
        arraysizeB = arrayLen(driveB_mode10_array);
        set_drive(1, driveA_mode10_array[0]);        //set drive in motion using first value in array
        set_drive(2, driveB_mode10_array[0]);
			}
      else{
        if(arraysizeA>1){                                     //if array is larger than 1, continue indexing
          set_drive(1, driveA_mode10_array[mode_indexA++]);
          if(mode_indexA>(arrayLen(driveA_mode10_array)-1))
            mode_indexA = 0;

        }
        if(arraysizeB>1){
          set_drive(2, driveB_mode10_array[mode_indexB++]);
          if(mode_indexB>(arrayLen(driveB_mode10_array)-1))
            mode_indexB = 0;
        }
      }  
			// if(active_mode!=req_mode){
			// 	serLed_on(75,37, 0);
			// 	set_drive(1, 0);
			// 	active_mode = req_mode;
      //   mode_indexB = arrayLen(drive_mode10_array)-1;
			// }
			// else{
			// 	set_drive(1, drive_mode10_array[mode_indexA++]);
      //   set_drive(2, drive_mode10_array[mode_indexB--]);
	
			// 	if(mode_indexA>(arrayLen(drive_mode10_array)-1))
			// 		mode_indexA = 0;
			// 	if(mode_indexB < 0)
			// 		mode_indexB = arrayLen(drive_mode10_array)-1;
          
			// }			
			break;
		
	} // end of switch
}


/**************** MAIN OLED DISPLAY FUNCTION****************/
/***************  CALLED EVERY xMS within app_update *******/
bool manage_disp(dispState_e disp_state){
bool retval=false;

#ifdef OLED_CONNECTED
  switch (disp_state) {
    case BOOT:
      display.display();
      display.clearDisplay();
        // text display tests
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.println("Hello World");
      display.setCursor(0,16);
      display.setTextSize(2);
      display.print("4V2:"+String(FW_VERSION));
      display.display();
    break;

    case INITSLEEP:
      //CLEAR AND DISABLE DISPLAY 
      display.clearDisplay(); 
      display.display();

      display.ssd1306_command(SSD1306_DISPLAYOFF);
    break;

    case INITWAKE:
      display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
      flag_updateDisp = true; 
      display.ssd1306_command(SSD1306_DISPLAYON);
    break;

    case LOWBATT:
      display.clearDisplay();
      display.drawRect(0,0, 40, 12, WHITE); //empty cell
      display.drawRect(40,3,5,6, WHITE); //button    
      display.setTextSize(2);
      display.setCursor(0,17);
      display.print("LOW BATT!");
      display.display();
    break;

    case USERMODE:
      if(flag_updateDisp){

        display.clearDisplay();

        display.drawRect(0,0, 40, 12, WHITE); //empty cell
        display.drawRect(40,3,5,6, WHITE); //button
        if(avg_batt_volts >= 4.0){    //full batt
          display.fillRect(0,0, 40, 12, WHITE);
        }
        else if(avg_batt_volts >= 3.8){
          display.fillRect(0,0, 30, 12, WHITE);
        }
        else if(avg_batt_volts >= 3.6){
          display.fillRect(0,0, 20, 12, WHITE);
        }
        else if(avg_batt_volts >= 3.5){
          display.fillRect(0,0, 10, 12, WHITE);
        }

        display.setCursor(60,2);
        display.setTextSize(1);
        display.print(String(avg_batt_volts)+" Vdc");
        display.setTextSize(2);
        display.setCursor(0,17);
        display.print("Mode: "+String(active_mode));
        display.display(); 
      }
      flag_updateDisp = false; 
    break;

  }
  retval = true;
#endif
return retval; 
}

/*Function Header*/
void loop() {

  app_timer.update();

  if(flag_wake_init){
    flag_wake_init = false; 
    init_wakeUp();
  }
    
  if(flag_gotoSleep){
	  if(!flag_blockSleep){
		  flag_gotoSleep = false;
		  flag_crit_batt = false;
      flag_low_batt = false;  //move to batt_read to verify above LVC?? 
      written_once = false; 
		  init_sleep();
		  //NVIC_SystemReset();
		  LowPower.deepSleep();
	  }
  }

  //app_timer.update();
  
  if(flag_flickerLED){
	  if(led_on == 0){
			serLed_off(); 
	  }
	  else
		led_on--; 
  }
  
  
  if((active_mode != 0) || (led_on>0)){ //Something is active??? Kick sleep counter.  
	  sleep_counter = 0; 
  }
  

  if (flag_scanBtns && !flag_crit_batt) {
    flag_scanBtns = false;
    scan_btns();
  }

  if (flag_10ms) {
    flag_10ms = false;

		if(flag_crit_batt){
			//flag_update_disp = true;
      if(!written_once)
			  serLed_on(120, 0, 0);

      written_once = true; 
      crit_batt_cntdown--;    //beat the sleep clock 
			if(!crit_batt_cntdown)
				flag_gotoSleep = true; 
		}
    else if(flag_low_batt == true){
      flag_low_batt = false; 
      manage_disp(LOWBATT);
      flag_crit_batt = true; 
      crit_batt_cntdown = 200;
      //warn User-
    }
		else
			manage_modes();
  }//end of 10ms

  if (flag_100ms) {
    flag_100ms = false;
    if(!flag_crit_batt)
      manage_disp(USERMODE);
    if(btns_state == 0)
      read_batt();
  }//end of 100ms

  if (flag_1second) {
    flag_1second = false;
   // scale_batt();
		if(sleep_counter == SLEEP_TIMEOUT) 	//move to moode_manager case0???
			flag_gotoSleep = true;
		else
			sleep_counter++;
  }//end of 1second
   
 
} //END OF MAIN() // ARDUINO LOOP() 




//BELOW CONVENIENCE FUNCTIONS

/*Uses set_drive to increment pwm duty by req amnt*/
void step_drive(int channel, bool dir, int step_size){
	int temp_state;
	
	temp_state = drive_state;
	if(dir){
		set_drive(1, temp_state+step_size);
	}
	else{
		set_drive(1, temp_state-step_size);
	
	}
}

/*Primary H-Bridge Motor Function*/ //setdrive(channel, duty))
void set_drive(int channel, int duty){ //(-1023 to 1023, 0 is off)
	int temp_pinA, temp_pinB; uint8_t pwm_channel; 

  if(channel == 1){
    temp_pinA = DRIVE_A1_PIN;
    temp_pinB = DRIVE_A2_PIN;
    //pwm_channel = A0; 
  }
  else if(channel == 2){
    temp_pinA = DRIVE_B1_PIN;
    temp_pinB = DRIVE_B2_PIN;   
    //pwm_channel = A1; 
  }
  else
    return; 

  pinMode(temp_pinA, OUTPUT);
  pinMode(temp_pinB, OUTPUT);
  //pinMode(pwm_channel, OUTPUT);
  
  if (duty != 0){
		if (duty > 1023)
		duty = 1023;
		if (duty < -1023)
		duty = -1023;
		
		if(duty>0){ //dir1
			digitalWrite(temp_pinA, HIGH);
			digitalWrite(temp_pinB, LOW);			
			analogWrite(temp_pinB, (1023-duty));
		}
		else if(duty<0){//dir2
			digitalWrite(temp_pinA, LOW);
			digitalWrite(temp_pinB, HIGH);
			analogWrite(temp_pinA, abs(1023-duty));
		}
		
		drive_state = duty;
	}
	else{ //duty = 0 - Turn off motors. 
		pinMode(temp_pinA, OUTPUT);
		pinMode(temp_pinB, OUTPUT);
		digitalWrite(temp_pinA, LOW);
		digitalWrite(temp_pinB, LOW);
		drive_state = 0; //OFF
	}
	//duty_last = duty;
}

/*Reads and averages battery measurement */
void read_batt(void){
  float temp_avg = 0;
  //static float avg_buffr[10];
  static float avg_buffr[10] = {4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0};
	static int i = 0; 

	pinMode(BTN_UP, OUTPUT);
  digitalWrite(BTN_UP, HIGH);
  pinMode(BTN_UP, INPUT);
	//delayMicroseconds(100);
	delay(1);
	float measuredvbat = analogRead(A7);
	//float measuredvbat = 3.3; 
	measuredvbat *= 2;    // we divided by 2, so multiply back
	measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
	measuredvbat /= 4096; // convert to voltage

	batt_volts = measuredvbat; 
  avg_buffr[i] = batt_volts;
  i++;
  if(i >= 10)
    i = 0;
  
  for(int j=0; j<=9; j++){
      temp_avg += avg_buffr[j];
  }
  temp_avg = temp_avg/10;

  if(abs(avg_batt_volts-temp_avg)>=0.05)
    flag_updateDisp = true;
  // if(avg_batt_volts != temp_avg)
  //   flag_updateDisp = true;

  avg_batt_volts = temp_avg;

  // if((abs(avg_batt_volts-APP_LVC) > 0.05) && (!flag_crit_batt))
  //   flag_low_batt = true;
  if((avg_batt_volts <= APP_LVC) && (!flag_crit_batt))
    flag_low_batt = true; 

  #ifdef DEBUG_PRINTS
    Serial.println(i);
    Serial.println(batt_volts); 
    Serial.println(avg_batt_volts); 
  #endif
  //average the voltage
}


/*Toggles Board LED - No BRD LED available on PC4V1*/
void toggle_brdled(bool en) {		//simple LED on pin 2 
  static bool state;

  if (en) {
    pinMode(BRD_LED, OUTPUT);
    digitalWrite(BRD_LED, state);
    state = !state;
  }
  else
    pinMode(BRD_LED, INPUT);
}

/*Shift Register LED driver*/
void serLed_on(int valr, int valg, int valb){
	serled.setPixelColor(0, serled.Color(valr, valg, valb));
	serled.show();
}

/*Shift Register LED Overide OFF*/
void serLed_off(void){
	serled.clear();
	serled.show();
	flag_flickerLED=false;
	led_on = 0;
	
}

/*Shift Register LED Simple Flicker Function*/
void serLed_flicker(unsigned int dur){
	serled.setPixelColor(0, serled.Color(0, 150, 150));
	serled.show();
	flag_flickerLED=true;
	led_on = dur; 
}
