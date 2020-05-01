/*
* Fine Positioning Brushless Controller Firmware
* Author: Justine Haupt
* Project staby using meetuprted on 9/9/18
* Version 1.0 released 
*
*
* After roscore is started, do $rosrun rosserial_python serial_node.py /dev/ttyUSB[X] on the machine that the NearZero board is attached to, which will let NearZero interract with ROS.
* After that, with no additional configuration, NearZero will respond to velocity commands issued over the cmd_vel topic.
*
*
Todo: 
-Add servo ROS subscriber
-Add publisher for the /odom topic. Reference http://wiki.ros.org/navigation/Tutorials/RobotSetup/Odom
-Add I2C support for master-slave arrangement?
-Add ENCODER support
-Transfer channel 2 code over to channel 1
-Fix or patch runaway vel and startup stall?
*/

#include <avr/io.h>   //IO library for avr microcontrollers.
#include <EEPROM.h>   //We'll need to access the EEPROM for storing configuration data.
#include <PWM.h>    //For motor driver PWM commutation, include Sam Knight's PWM OUTPUT library, available at https://github.com/terryjmyers/PWM.git
#include <ros.h>    //Let there be ROS! The ROS arduino libraries are installed according to http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup
#include <geometry_msgs/Twist.h>    //We're going to be subscribing to messages of type Twist, which contain 3-axis linear and angular motion commands.
#include <std_msgs/Float32.h>     //We're going to be publishing messages of type Float32, which holds a single float32 value. This is for reporting current draw.
#include <EnableInterrupt.h>

//Defime some constants
#define pwm1_in A15 //A15 is pin 82 on the package. This is the input for pwm1.
#define pwm2_in 14 //14 is pin 64 on the package. This is the input for pwm2.
#define pi 3.1415926

//Define some more constants for memeory locations
#define addr_Iset1l 0	
#define addr_Iset1r 1
#define addr_maxIset2l 2
#define addr_maxIset2r 3
#define addr_gain1l 4
#define addr_gain1r 5
#define addr_gain2l 6
#define addr_gain2r 7
#define addr_torqueprofile1 8
#define addr_torqueprofile2 9
#define addr_currentmode1 10
#define addr_currentmode2 11
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
#define addr_c2TrigOffsetl 32
#define addr_c2TrigOffsetr 33
#define addr_c2vTcall 34
#define addr_c2vTcalr 35
#define addr_c2TrigSequence 36

float phaseindex1 = pi;     //Start at pi radians so that the starting value of sin(phaseangle_U1) is 0.
float phaseindex2 = pi;     //Start at pi radians so that the starting value of sin(phaseangle_U1) is 0.
unsigned long phase2_T = 0;	//Hall or encoder period since last phase trigger after compensation for offset of each phase
unsigned long phase2_T1 = 0;
unsigned long phase2_T2 = 0;
unsigned long phase2_T3 = 0;
float phase2flt_T1 = 0;
float phase2flt_T2 = 0;
float phase2flt_T3 = 0;
float phase2_T_deviation = 0;
float phase2_T_deviation_last = 0;
unsigned long c2Trig1;
unsigned long c2Trig2;
unsigned long c2Trig3;
unsigned long c2TrigOffset = 0;	//Calibration value for hall/encoder. Sets offset of trigger in phase index progression (fixes there being different numbers for time delay measurements forwards and reverse.
unsigned long c2vTcal = 0;	//Calibration value for hall/encoder. This is the vel*T_phase constant.
int c2TrigSequence = 0;	//Calibration value for hall.encoder. Sets order of phase triggering.
int c2TrigSequenceStore = 1;
float phaseangle_U1; 
float phaseangle_V1;
float phaseangle_W1;
float phaseangle_U2;
float phaseangle_V2;
float phaseangle_W2;
float v_x;    //Linear velocity, as commanded over the ROS cmd_vel topic.
float v_th;   //Angular velocity, as commanded over the ROS cmd_vel topic.
float vel1 = 0;    //sets the velocity (positive or negative) or position. This is what increments the phaseangle for each phase.
float vel2 = 0;
float actualvel1 = 0;		//Intermediate variable used to track motor velocity up to vel1 according to accel1	
float actualvel2 = 0;
float pos1 = 0;	//used for positioning mode
float pos2 = 0;
float posindexproxy1 = 0;
float posindexproxy2 = 0;
float pos1_last;
float pos2_last;
float servovel1;
float servovel2;
float I_A;   //Current in amps after conversion from ADUs.
float I_qui;  //Stores currrent in amps before energizing motors in SetPower() function.
float I_adu_av = 0;   //Averaged current in ADUs
float I_adu_sum = 0;   //Cumulative current in ADUs
int dutyU1;    //The current duty cycle of the gating signal for each phase of motor 1
int dutyV1;
int dutyW1;
int dutyU2;    //The current duty cycle of the gating signal for each phase of motor 2
int dutyV2;
int dutyW2;
int power1;   //This is the gating PWM modulation amplitude. If it's zero the duty cycle will not vary. If it's 127, it will vary all the way from 0 to 255. Effectively sets the voltage going to the motor.
int power2;
int power1_high;	//This is used to store the power value for the requested current setting
int power2_high;
int power1_low;	//This is used to store the power value for the requested static/rest current setting
int power2_low;
int power1_diag;	//This is used to store an intermediate power value (amount of A is hard coded in SetPower()) to use for the hall phase offset self calibration.
int power2_diag;
bool enc2_pin3_flag = false;
bool enc2_pin4_flag = false;
bool enc2_pin5_flag = false;
bool enc2_pin6_flag = false;
int pindex2_flag = 0;
int enc2_laststate = 0;
int enc2_lastlaststate = 0;
int j = 0;		//general purpose counter for the roll functions

//PWM-related variables
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
float Iset1;    //Maximum or constant current draw in mA. Power is automatically ramped to this value at start time.
float maxIset2; 
float minIset1;		//minimum or reduced current used when restmode = 1.
float minIset2;
int gain1;		//Velocity multiplier.
int gain2;
int torqueprofile1;		//0 = OFF/sine, 1 = ON
int torqueprofile2;
int currentmode1;	//0 = FIXED, 1 = DYNAMIC
int currentmode2;
int commandmode1;	//0 = VELOCITY control, 1 = POSITION control
int commandmode2;
int sensortype1;	//0 = NONE, 1 = ENCODER, 2 = HALL
int sensortype2; 	
int tscoeff1;		//Scaling coefficient for torque smoothing (ts) fourier series
int tscoeff2;
int dir1;		//0 = NORMAL, 1 = REVERSE
int dir2;
float tsphase1;		//Phase offset for torque smoothing (ts) fourier series
float tsphase2;
float accel1;	//Sets gain of position following velocity (sets servo aggressiveness)
float accel2;

//EEPROM address locations and related things
int lhlf;			//holder for the left half of an integer
int rhlf;			//holder for the right half of an integer

//Variables relating to serial terminal
const byte nchars = 10;	//Size of rxchars array
char rxchars[nchars];	//An array that holds up to "nchars" characters.	
bool newdata = false;	//New data flag
bool exitflag = false;	//Used to kill a function part-way through the terminal menu system
int iholder;		//integer for temporary use
float fholder;		//float for temporary use

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
const byte mode_usb = 28;   //When this pin is grounded, USB input is selected.
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

void ISR_pwm1(){	//Interrupt Service Routine. This function is run if a change is detected on pwm1_in
	if (pwmstate1 == 0){	
		risemark1 = micros();	//Record the time that pwm1_in went high
		pwmstate1 = 1;
	}
	else {	//if flag was already 1...
		pwm1_t = micros() - risemark1;		//If it's already gone high, then the present change must be the time that pwm1_in is going low. The pulse duration is the difference.
		pwmstate1 = 0;
	}
}

void ISR_pwm2(){
	if (pwmstate2 == 0){	
		risemark2 = micros();
		pwmstate2 = 1;
	}
	else {
		pwm2_t = micros() - risemark2;
		pwmstate2 = 0;
	}
}

void velCallback(  const geometry_msgs::Twist& vel){   //This is the heart of the subscriber. It's a callback function that reads the data in the message of type geometry_msgs::Twist, where we call the data "vel".
	v_x = vel.linear.x;    //Pull out the forward/backward velocity command.
	v_th = vel.angular.z;   //Pull out the angular velocity (turn) command.
}

//NOTE: Below, I don't understand why the formats for ros::Publisher and ros::Subscriber need to be so different.
std_msgs::Float32 idraw_msg;    //Declare a Float32 message type (from the std_msgs class) and call it current_msg.

ros::Publisher pub_I("idraw_1", &idraw_msg);   //Create a publisher and call the topic "idraw_1". The message published on this topic will be idraw_msg, defined above.
ros::Subscriber <geometry_msgs::Twist> sub("cmd_vel", velCallback);   //  Create a subscriber for the topic "cmd_vel" with message type "Twist" in the geometry_msgs class. Messages are passed to a callback function, called here "velCallback".
ros::NodeHandle nh;   //Create a Node Handler and call it nh.


void setup(){

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
	pinMode(mode_usb, INPUT_PULLUP);
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
	enableInterrupt(pwm1_in, ISR_pwm1, CHANGE);
	enableInterrupt(pwm2_in, ISR_pwm2, CHANGE);

	digitalWrite(13, HIGH);   //Put the pin 13 LED high to indicate that the board is starting up.

	nh.initNode();    //Start the node handler (i.e. start a node), which allows this program to create publishers or subscribers. This also handles serial comms. There can be only a single ROS node per serial device (i.e. a single node per device like this, which is fine because a node can both publish and subscribe).
	nh.subscribe(sub);    //Start listening.
	nh.advertise(pub_I);

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
	CheckConfig();  //Get configuration data
	SetPower();   //Set the 3-phase modulation amplitude to draw the amount of current dictated by the I_set_ variables.    
}

void(* resetFunc) (void) = 0; //This is the reset function

void loop(){
	CheckMode();		
	if (commandmode1 == 0){	//if in velocity mode
		Roll_vel1();    
	}
//	else if (commandmode1 == 1){	//if in position mode
//		Roll_pos1();    
//	}
	if (commandmode2 == 0){	//if in velocity mode
		Roll_vel2();    
	}
	else if (commandmode2 == 1){	//if in position mode
		Roll_pos2();    
	}
	//CHECK FOR RUNAWAY VELOCITY ISSUE
	if (vel2 > 5){
		Serial.println(F("Start-time error detected. RESETTING."));
		resetFunc();
	}
}

void Roll_vel1(){
	
	phaseindex1 = phaseindex1 + vel1;    //This is fed into the sine function to vary the pwm duty cycle. rate is the step size for incrementing phaseindex_U1. 

	if (torqueprofile1 == 1){	//If the anti-coffing profile is set to 1, make the phase index be the phase index plus (minus) a correction which is a function of the phase index. The fuction 
		phaseindex1 = phaseindex1 - (.05*sin(2*phaseindex1+(.469)) + .01*sin(4*phaseindex1+(.491)));  
	}

	if(fabs(phaseindex1) >= 100*pi) {   //Reset phaseindex_U1 once it completes some number of 2*180o in phase.
		phaseindex1 = 0;
	}

	dutyU1 = (255/2)+(power1*(sin(phaseindex1)));	   //The duty cycle varies with the sine function, which has output between -1 and 1. That is scaled by an amplitude variable, which effectively sets the motor power.This is all offset by half the maximum duty cycle so that the lowest instantaeous duty cycle is always positive.
	analogWrite(U1_High,dutyU1);	//Write to the PWM pins.
	dutyV1 = (255/2)+(power1*(sin(phaseindex1+(2*pi/3))));
	analogWrite(V1_High,dutyV1);
	dutyW1 = (255/2)+(power1*(sin(phaseindex1+(4*pi/3))));
	analogWrite(W1_High,dutyW1);
}

void Roll_vel2(){
	//CALCULATE ACCELERATION DYNAMICS
	if (accel2 == 10000) {	//If accel=10000, this means the accel variable shouldn't even be used
		actualvel2 = vel2;
	}
	else {	//If the accel variable is otherwise set, use it
		if ((vel2 - .0001) > actualvel2){		//the constant defines the deadzone	
			if (fabs(vel2) < .01){		//If the velocity is slow, don't use the acceleration parameter
				actualvel2 = vel2;
			}
			else {
				actualvel2 = actualvel2 + (accel2/1000000);
			}	
			if (currentmode2 == 0){	//If in fixed current mode set the current to high.
				power2 = power2_high;		
			}
		}
		else if ((vel2 + .0001) < actualvel2){		//the constant defines the deadzone
			if (fabs(vel2) < .01){		//If the velocity is slow, don't use the acceleration parameter
				actualvel2 = vel2;
			}
			else {
				actualvel2 = actualvel2 - (accel2/1000000);
			}	
			if (currentmode2 == 0){	//If in fixed current mode set the current to high.
				power2 = power2_high;		
			}
		}
		else if (vel2 == 0){	//if the motor isn't moving (is resting), set the power low as determined by the config settings
			power2 = power2_low;
		}
	}
	//-----IF NO SENSOR IS ATTACHED-----
	if (sensortype2 == 0){
		//WE CAN ONLY BE IN A 'FIXED' CURRENT MODE WHEN SENSOR TYPE = 0, SO NO CURRENTMODE CHECK NEEDED:
		if (actualvel2 == 0){		//Set power based on whether or not motor is moving.
			power2 = power2_low;		//This will be the same as power2_high when in the "simple" fixed current mode
		}
		else {
			power2 = power2_high;
		}
		//INCREMENT PHASE INDEX	
		if (dir2 == 0) {	//If direction is NORMAL
			phaseindex2 = phaseindex2 + actualvel2;    //This is fed into the sine function to vary the pwm duty cycle. rate is the step size for incrementing phaseindex_U1
		}
		else if (dir2 == 1) {	//If direction is REVERSE
			phaseindex2 = phaseindex2 - actualvel2;
		}			
	}

	//-----IF AN ENCODER IS ATTACHED-----
	if (sensortype2 == 1){
		if (currentmode2 == 0){
			//placeholder
		}
		if (currentmode2 == 2){
			//placeholder
		}
	}

	//-----IF A HALL SENSOR IS ATTACHED-----
	if (sensortype2 == 2){
		ReadHall2();
		//IF IN 'FIXED' CURRENT MODE
		if (currentmode2 == 0) {	
			if (actualvel2 == 0){		//Set power based on whether or not motor is moving.
				power2 = power2_low;		//This will be the same as power2_high when in the "simple" fixed current mode
			}
			else {
				power2 = power2_high;
			}
			//INCREMENT PHASE INDEX
			if (dir2 == 0) {	//If direction is NORMAL
				phaseindex2 = phaseindex2 + actualvel2;		//Scale actualvel2 by (vel2*phase2_T)-phasevelconst2
			}
			else if (dir2 == 1) {	//If direction is REVERSE
				phaseindex2 = phaseindex2 - actualvel2;
			}	
		}
		//IF WE'RE IN 'DYNAMIC' CURRENT MODE
		else if (currentmode2 == 1) {
			power2 = power2_low; //+ power2_low scaled by (vel2*phase2_T)-phasevelconst2... needs ((int) ) thing
			//Serial.println(vel2*phase2_T);
			if (power2 > power2_high){
				power2 = power2_high;
			}	
			if (power2 < power2_low){
				power2 = power2_low;
			}
			//Serial.println(power2);
			//INCREMENT PHASE INDEX
			if (dir2 == 0) {	//If direction is NORMAL
				phaseindex2 = phaseindex2 + actualvel2; 				
			}
			else if (dir2 == 1) {	//If direction is REVERSE
				phaseindex2 = phaseindex2 - actualvel2;
			}
		}
	}

	//CALCULATE TORQUE SMOOTHING (IF ON)
	if (torqueprofile2 == 1){	//If the anti-coffing profile is set to 1, make the phase index be the phase index plus (minus) a correction which is a function of the phase index. The fuction 
		phaseindex2 = phaseindex2 - (tscoeff2*.00025*sin(2*phaseindex2+(.469+tsphase1)) + tscoeff2*.00005*sin(4*phaseindex2+(.491+tsphase2)));  
	}

	//RESET PHASE INDEX AFTER 2PI
	if (phaseindex2 >= 2*pi) {   //Reset phaseindex_U1 once it completes 2*180o in phase.
		phaseindex2 = 0;
	}
	else if (phaseindex2 <= 0){
		phaseindex2 = 2*pi;
	}

	//WRITE OUTPUTS
	dutyU2 = (255/2)+(power2*(sin(phaseindex2)));	   //The duty cycle varies with the sine function, which has output between -1 and 1. That is scaled by an amplitude variable, which effectively sets the motor power.This is all offset by half the maximum duty cycle so that the lowest instantaeous duty cycle is always positive.
	analogWrite(U2_High,dutyU2);	//Write to the PWM pins.
	dutyV2 = (255/2)+(power2*(sin(phaseindex2+(2*pi/3))));
	analogWrite(V2_High,dutyV2);
	dutyW2 = (255/2)+(power2*(sin(phaseindex2+(4*pi/3))));
	analogWrite(W2_High,dutyW2);
}

void Roll_pos2(){

	//Check for REVERSE directionality
	if (dir2 == 1) {  //If directionality is REVERSE
		pos2 = (-1)*pos2;
	}

	//CALCULATE SERVO SLEW USING DELTA INPUT
	servovel2 = (accel2/100000)*(fabs(phaseindex2 - pos2));	

	//INCREMENT PHASE INDEX
	if ((posindexproxy2 - .2) > pos2){		//the constant defines the deadzone
		posindexproxy2 = posindexproxy2 - servovel2; 
		phaseindex2 = phaseindex2 - servovel2; 
		power2 = power2_high;
		j = 0;
	}
	else if ((posindexproxy2 + .2) < pos2){
		posindexproxy2 = posindexproxy2 + servovel2;
		phaseindex2 = phaseindex2 + servovel2;
		power2 = power2_high;
		j = 0;
	}
	else {//they're equal, so don't move the phaseindex, but DO set the power to the low setting
		j++;
		if (j == 100){
			power2 = power2_low;
		}
	}

	//UPDATE MOTOR POSITION
	pos2_last = pos2;

	//CALCULATE TORQUE SMOOTHING (IF ON)
	if (torqueprofile2 == 1){	//If the anti-coffing profile is set to 1, make the phase index be the phase index plus (minus) a correction which is a function of the phase index. The fuction 
		phaseindex2 = phaseindex2 - (tscoeff2*.00025*sin(2*phaseindex2+(.469+tsphase1)) + tscoeff2*.00005*sin(4*phaseindex2+(.491+tsphase2)));  
	}

	//WRITE OUTPUTS
	dutyU2 = (255/2)+(power2*(sin(phaseindex2)));	   //The duty cycle varies with the sine function, which has output between -1 and 1. That is scaled by an amplitude variable, which effectively sets the motor power.This is all offset by half the maximum duty cycle so that the lowest instantaeous duty cycle is always positive.
	analogWrite(U2_High,dutyU2);	//Write to the PWM pins.
	dutyV2 = (255/2)+(power2*(sin(phaseindex2+(2*pi/3))));
	analogWrite(V2_High,dutyV2);
	dutyW2 = (255/2)+(power2*(sin(phaseindex2+(4*pi/3))));
	analogWrite(W2_High,dutyW2);
}

