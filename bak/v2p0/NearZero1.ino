/*
* NearZero Firmware
* Author: Justine Haupt
* Project started on 9/9/18
* Version 1.2 (released 4/1/2020)
*
*/

#include <avr/io.h>   //IO library for avr microcontrollers.
#include <EEPROM.h>   //We'll need to access the EEPROM for storing configuration data.
#include <PWM.h>    //For motor driver PWM commutation, include Sam Knight's PWM OUTPUT library, available at https://github.com/terryjmyers/PWM.git
#include <EnableInterrupt.h>
#include <Wire.h>

//Defime some constants
#define pwm1_in A15 //A15 is pin 82 on the package. This is the input for pwm1.
#define pwm2_in 14 //14 is pin 64 on the package. This is the input for pwm2.
#define pi 3.1415926

//Define some more constants for memeory locations
#define addr_maxIset1l 0	
#define addr_maxIset1r 1
#define addr_maxIset2l 2
#define addr_maxIset2r 3
#define addr_gain1l 4
#define addr_gain1r 5
#define addr_gain2l 6
#define addr_gain2r 7
#define addr_torqueprofile1 8
#define addr_torqueprofile2 9
#define addr_commandmode1 12
#define addr_commandmode2 13
#define addr_sensortype1 14
#define addr_sensortype2 15
#define addr_minIset1l 16
#define addr_minIset1r 17
#define addr_minIset2l 18
#define addr_minIset2r 19
#define addr_tscoeff1 20
#define addr_tscoeff2 21
#define addr_tsphase1l 22
#define addr_tsphase1r 23
#define addr_tsphase2l 24
#define addr_tsphase2r 25
#define addr_accel1l 26
#define addr_accel1r 27
#define addr_accel2l 28
#define addr_accel2r 29
#define addr_dir1 30
#define addr_dir2 31
#define addr_pwmoffset1l 45
#define addr_pwmoffset1r 46
#define addr_pwmoffset2l 47
#define addr_pwmoffset2r 48
#define addr_maxslewvel1l 49
#define addr_maxslewvel1r 50
#define addr_maxslewvel2l 51
#define addr_maxslewvel2r 52
#define addr_i2caddress 53

float phaseindex1 = pi;     //Start at pi radians so that the starting value of sin(phaseangle_U1) is 0.
float phaseindex2 = pi;     //Start at pi radians so that the starting value of sin(phaseangle_U1) is 0.
float phaseangle_U1; 
float phaseangle_V1;
float phaseangle_W1;
float phaseangle_U2;
float phaseangle_V2;
float phaseangle_W2;
float vel1 = 0;    //sets the velocity (positive or negative) or position. This is what increments the phaseangle for each phase.
float vel2 = 0;
float vel1_starting = 0;
float vel2_starting = 0;
float vel1_holder = 0;
float vel2_holder = 0;
float actualvel1 = 0;		//Intermediate variable used to track motor velocity up to vel1 according to accel1	
float actualvel2 = 0;
float pos1 = 0;	//used for positioning mode
float pos2 = 0;
float pos1_starting = 0;
float pos2_starting = 0;
float pos1_holder = 0;
float pos2_holder = 0;
float pos1_last;
float pos2_last;
float slewvel1;
float slewvel2;
float I_A;   //Current in amps after conversion from ADUs.
float I_qui;  //Stores currrent in amps before energizing motors in CalDuty() function.
float I_adu_av = 0;   //Averaged current in ADUs
float I_adu_sum = 0;   //Cumulative current in ADUs
float m1 = 0;	//Slope in the eq. of the line which determines relationship between current and duty cycle
float b1 = 0;	//Offset in the eq. of the line which determines relationship between current and duty cycle
float m2 = 0;
float b2 = 0;
int dutyU1;    //The current duty cycle of the gating signal for each phase of motor 1
int dutyV1;
int dutyW1;
int dutyU2;    //The current duty cycle of the gating signal for each phase of motor 2
int dutyV2;
int dutyW2;
int duty1 = 0;   //This is the gating PWM modulation amplitude. If it's zero the duty cycle will not vary. If it's 127, it will vary all the way from 0 to 255. Effectively sets the voltage going to the motor.
int duty2 = 0;
int dutycalpoint_high1;	//This is used to store the duty value for the requested current setting
int dutycalpoint_high2;
int dutycalpoint_low1;	//This is used to store the duty value for the requested static/rest current setting
int dutycalpoint_low2;
int pindex1_flag = 0;
int pindex2_flag = 0;
float encstep = .01;	//Hoe much the encoder tick increments with every change of encoder position. 
int enc1_state = 0;
int enc1_laststate = 0;
int enc2_state = 0;
int enc2_laststate = 0;
float enc1_absticks = 0;
float enc2_absticks = 0;
int j = 0;		//general purpose counter for the roll functions
int i = 1;    //A counter and divisor for calculating a running average for current-sensing the PubI() function.

//PWM input related variables
volatile uint16_t risemark1; 
volatile int16_t pwm1_t = 0;
volatile uint16_t risemark2; 
volatile int16_t pwm2_t = 0;
int pwmstate1 = 0;
int pwmstate2 = 0;
int32_t freq = 25000; //PWM frequency (in Hz). 3-phase bridge is rated to 50kHz. Allegro current sensor bandwidth is 80kHz.
bool pwmflag = false;    //Used to detect if PWM mode has just been entered, in which case the two PWM inputs will be checked for activity one time only.
bool pwm1active = false;    //Indicates whether a PWM source is connected to channel 1.
bool pwm2active = false;    //Indicates whether a PWM source is connected to channel 2.
	
//User setting variables
float Iset1;    	//I2C current draw set point
float Iset2;
float maxIset1;		//PWM running current draw set point in mA. 
float maxIset2; 
float minIset1;		//PWM resting current draw set point in mA.
float minIset2;
int gain1;		//Velocity multiplier.
int gain2;
int pwmoffset1;	//PWM center
int pwmoffset2;
int torqueprofile1;		//0 = OFF/sine, 1 = ON
int torqueprofile2;
int commandmode1;	//0 = VELOCITY command, 1 = POSITION command, 2 = SERVO command
int commandmode2;
int sensortype1;	//0 = ENCODER, 1 = HALL
int sensortype2; 	
int tscoeff1;		//Scaling coefficient for torque smoothing (ts) fourier series
int tscoeff2;
int dir1;		//0 = NORMAL, 1 = REVERSE
int dir2;
int i2caddress;
float tsphase1;		//Phase offset for torque smoothing (ts) fourier series
float tsphase2;
float accel1;	//Sets gain of position following velocity (sets servo aggressiveness)
float accel2;
float maxslewvel1;
float maxslewvel2;

//EEPROM address locations and related things
int lhlf;			//holder for the left half of an integer
int rhlf;			//holder for the right half of an integer

//Variables relating to serial terminal
const byte nchars = 31;	//Size of rxchars array
char rxchars[nchars];	//An array that holds up to "nchars" characters.	
bool newdata = false;	//New data flag
bool exitflag = false;	//Used to kill a function part-way through the terminal menu system
int iholder;		//integer for temporary use
float fholder;		//float for temporary use

//variables relating to I2C
int i2crxints[10];
char tickstx[12];
int ticksbuff1;		//Intermediate integer to hold enc1_absticks before conversion into tickstx1
int ticksbuff2;

// Declare pin names (fixed values, so we use the byte type):
const byte U1_High = 6;  //Gating signal for U phase
const byte V1_High = 7;
const byte W1_High = 8;
const byte U1_En = 17;   //Enable pin for U phase
const byte V1_En = 16;
const byte W1_En = 9;
const byte U2_High = 46;  //Gating signal pin for U phase
const byte V2_High = 45;
const byte W2_High = 44;
const byte U2_En = 47;
const byte V2_En = 48;
const byte W2_En = 49;
const byte Isense = 0;  //Current sense input pin
const byte mode_reset = 28;   //When this pin is grounded, USB input is selected.
const byte mode_config = 27;  //When this pin is grounded, I2C input is selected.
const byte mode_i2c = 26;  //When this pin is grounded, isp input is selected.
const byte enc1_pin3 = 3;
const byte enc1_pin4 = 2;
const byte enc1_pin5 = 5;
const byte enc1_pin6 = 53;
const byte enc2_pin3 = 38;
const byte enc2_pin4 = 18;
const byte enc2_pin5 = 19;
const byte enc2_pin6 = 43;

void setup(){
	enableInterrupt(pwm1_in, ISR_pwm1, CHANGE);
	enableInterrupt(pwm2_in, ISR_pwm2, CHANGE);

	InitTimersSafe(); //Initialize all timers except timer 0

	SetPinFrequencySafe(U1_High, freq);   //sets the frequency for the specified pin
	SetPinFrequencySafe(V1_High, freq);   //sets the frequency for the specified pin
	SetPinFrequencySafe(W1_High, freq);   //sets the frequency for the specified pin
	SetPinFrequencySafe(U2_High, freq);   //sets the frequency for the specified pin
	SetPinFrequencySafe(V2_High, freq);   //sets the frequency for the specified pin
	SetPinFrequencySafe(W2_High, freq);   //sets the frequency for the specified pin

	//Timer 4 outputs:
	pinMode(U1_High, OUTPUT); 
	pinMode(V1_High, OUTPUT);
	pinMode(W1_High, OUTPUT);

	//Timer 5 outputs:
	pinMode(U2_High, OUTPUT);
	pinMode(V2_High, OUTPUT);
	pinMode(W2_High, OUTPUT);

	//Other one time settings:
	pinMode(13, OUTPUT);    //indicator light
	pinMode(mode_reset, INPUT_PULLUP);
	pinMode(mode_i2c, INPUT_PULLUP);
	pinMode(mode_config, INPUT_PULLUP);
	pinMode(Isense, INPUT);
	pinMode(U1_En, OUTPUT);
	pinMode(V1_En, OUTPUT);
	pinMode(W1_En, OUTPUT);
	pinMode(U2_En, OUTPUT);
	pinMode(V2_En, OUTPUT);
	pinMode(W2_En, OUTPUT);
	pinMode(enc1_pin3, INPUT_PULLUP);
	pinMode(enc1_pin4, INPUT_PULLUP);
	pinMode(enc1_pin5, INPUT_PULLUP);
	pinMode(enc1_pin6, INPUT_PULLUP);
	pinMode(enc2_pin3, INPUT_PULLUP);
	pinMode(enc2_pin4, INPUT_PULLUP);
	pinMode(enc2_pin5, INPUT_PULLUP);
	pinMode(enc2_pin6, INPUT_PULLUP);
  	pinMode(pwm1_in, INPUT_PULLUP);  
  	pinMode(pwm2_in, INPUT_PULLUP);  

	digitalWrite(13, HIGH);   //Put the pin 13 LED high to indicate that the board is starting up.

	//The purpose of the next lines is to syncyrhonize all the timers so that the PWM duty cycles overlap completely:
	GTCCR = (1<<TSM)|(1<<PSRASY)|(1<<PSRSYNC);  // Halt all timers using the GTCCR register. We're setting the bit called "TSM" to 1, the bit called "PSRASY" to 1, etc. The names of each bit in each register is described in the ATMega2560 datasheet.

	// The TCNTn registers store the actual timer values (the actual number that counts up). These are 16bit timers, so they count up to 2^16=65535. With the timers halted with the previous line,
	// we can set these to start at any value between 0 and 65536. This effectively sets the relative phase of the timers. These timer registers are 16 bit, but the data bus in the ATmega is only 8 bit,
	// so it gets around this by using a separate 8-bit high byte and 8-bit low byte for each timer. The low and high bytes get written to the 16bit register in the same clock cycle, and this is triggered by
	// accessing the low byte. Writing to the high byte alone won't do anything. Also importantly, the high byte is stored in the same temporary 8-bit register that all the timers use, so every time a
	// value is written to the high byte, the low byte must be written before the high byte for another timer is written, lest we overwrite the temporarily stored high byte that we initially accessed.
	// The safe thing is to always write to a register's high byte followed by its low byte. To read a register, read its low byte first, followed by its high byte.
	// Note: The OCRnA/B/C registers do NOT use the temporary register. The registers can be written in hex (with 0x prefix) or binary (with 0b prefix). No prefix indicates decimal.
	TCNT3H = 0b00000000;  // set timer3 high byte to 0. This could be entered in decimal format as simply "0", which has the binary value we actually entered.
	TCNT3L = 0b00000000;  // set timer3 low byte to 0. With both the high and low bytes set to zero, we're saying that the counter's 16-bit value is 0. In binary it's 0000000000000000. Example: If we want 2,000 (as in 2,000 out of 65535), in binary 2,000 = 0000011111010000. So the high byte is 00000111 and the low byte is 11010000. 1,000 = 0000001111101000. 4,000 = 0000111110100000. 500 = 0000000111110100.
	TCNT4H = 0b00000000;
	TCNT4L = 0b00000000;
	TCNT5H = 0b00000000;
	TCNT5L = 0b00000000;
	GTCCR = 0; // release all timers by setting all bits back to 0.

	Serial.begin(9600);
	DisplaySettings();
	CalDuty();   //Set the 3-phase modulation amplitude to draw the amount of current dictated by the I_set_ variables.    
	digitalWrite(13, LOW);   //Put the pin 13 LED high to indicate that the board is starting up.
	
	if (digitalRead(mode_i2c) == LOW){	//This has to be the last thing that happens in setup() for some reason
		Wire.begin(i2caddress);                // Start I2C service with the given address
		Wire.onReceive(ISR_I2Crx); // An ISR: if I2C data comes in go to the ISR_I2C() function to handle it.
		Wire.onRequest(ISR_I2Ctx);	//An ISR: If data is requested, send it (for encoder ticks, etc)
	}
}

void loop(){
	//CHECK MODE SELECT HEADER
	CheckMode();		

	//READ ENCODER (OR HALL)
	if (sensortype1 == 0){		//If sensortype is set to encoder
		ReadEnc1();
	}
	else {		//If sensortype is set to hall
		ReadHall1();
	}
	if (sensortype2 == 0){		//If sensortype is set to encoder
		ReadEnc2();
	}
	else {		//If sensortype is set to hall
		ReadHall2();
	}

	//PROCESS ENCODER (OR HALL) TICKS FOR I2C TRANSMISSION
	ticksbuff1 = int(100*enc1_absticks);
	ticksbuff2 = int(100*enc2_absticks);
	sprintf(tickstx, "%06d%06d", ticksbuff1, ticksbuff2);

	//INCREMENT ROLL FUNCTIONS
	if (commandmode1 == 0) {	//if in velocity mode
		Roll1_vel();    
	}
	else if (commandmode1 == 1) { //if in position mode
		Roll1_pos();    
	}
	else {
		Roll1_servo();    
	}
	if (commandmode2 == 0) {	//if in velocity mode
		Roll2_vel();    
	}
	else if (commandmode2 == 1) { //if in position mode
		Roll2_pos();    
	}
	else {
		Roll2_servo();    
	}
}


