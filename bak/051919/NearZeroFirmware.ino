/*
* Fine Positioning Brushless Controller Firmware
* Created 9/9/18
* Edited:
*
* After roscore is started, do $rosrun rosserial_python serial_node.py /dev/ttyUSB[X] on the machine that the NearZero board is attached to, which will let NearZero interract with ROS.
* After that, with no additional configuration, NearZero will respond to velocity commands issued over the cmd_vel topic.
*
*/

/*
Todo: 
-Make TorqCal not rely on the phaseindex value
-Incorporate anti-cogging functions
-Move global variables to functions to save memory?

-Add I2C support and test with an Arduino.
-Design encoder and add publisher for the /odom topic. Reference http://wiki.ros.org/navigation/Tutorials/RobotSetup/Odom

*/

#include <avr/io.h>   //IO library for avr microcontrollers.
#include <EEPROM.h>   //We'll need to access the EEPROM for storing configuration data.
#include <PWM.h>    //Include Sam Knight's PWM library, available at https://github.com/terryjmyers/PWM.git
#include <ros.h>    //Let there be ROS! The ROS arduino libraries are installed according to http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup
#include <geometry_msgs/Twist.h>    //We're going to be subscribing to messages of type Twist, which contain 3-axis linear and angular motion commands.
#include <std_msgs/Float32.h>     //We're going to be publishing messages of type Float32, which holds a single float32 value. This is for reporting current draw.
#include <EnableInterrupt.h>
#define pwm1_in A15 //A15 is pin 82 on the package. This is the input for pwm1.
#define pwm2_in 14 //14 is pin 64 on the package. This is the input for pwm2.

const float pi = 3.1415926;
float phaseindex_1 = pi;     //Start at pi radians so that the starting value of sin(phaseangle_U1) is 0.
float phaseindex_2 = pi;     //Start at pi radians so that the starting value of sin(phaseangle_U1) is 0.
float phaseangle_U1; 
float phaseangle_V1;
float phaseangle_W1;
float phaseangle_U2;
float phaseangle_V2;
float phaseangle_W2;
float v_x;    //Linear velocity, as commanded over the ROS cmd_vel topic.
float v_th;   //Angular velocity, as commanded over the ROS cmd_vel topic.
float wheel1_vel = 0;    //sets the velocity (positive or negative). This is what increments the phaseangle for each phase.
float wheel2_vel = 0;
float I_A;   //Current in amps after conversion from ADUs.
float I_qui;  //Stores currrent in amps before energizing motors in SetPower() function.
float I_adu_av = 0;   //Averaged current in ADUs
float I_adu_sum = 0;   //Cumulative current in ADUs
int i = 1;    //A counter and divisor for calculating a running average for current-sensing the PubI() function.
int j = 0;    //A counter for determining rate of current rise in the SetPower() function.
int dutyU1;    //The current duty cycle of the gating signal for each phase of wheel 1
int dutyV1;
int dutyW1;
int dutyU2;    //The current duty cycle of the gating signal for each phase of wheel 2
int dutyV2;
int dutyW2;
int power_1 = 0;   //This is the gating PWM modulation amplitude. If it's zero the duty cycle will not vary. If it's 127, it will vary all the way from 0 to 255. Effectively sets the voltage going to the motor.
int power_2 = 0;
int power_1_saved = 0;
int power_2_saved = 0;

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
float I_set_1;    //Current draw in mA. Power is automatically ramped to this value at start time.
float I_set_2; 
int vcal_1;		//Velocity multiplier.
int vcal_2;
int cogprof_1;		//Anti-cogging profile.
int cogprof_2;
int motormode_1;	//Motor operating mode (open loop, closed loop, etc)
int motormode_2;

//EEPROM address locations and related things
const int addr_Iset1l = 0;	
const int addr_Iset1r = 1;
const int addr_Iset2l = 2;
const int addr_Iset2r = 3;
const int addr_vcal1l = 4;
const int addr_vcal1r = 5;
const int addr_vcal2l = 6;
const int addr_vcal2r = 7;
const int addr_cogprof1 = 8;
const int addr_cogprof2 = 9;
const int addr_motormode1 = 10;
const int addr_motormode2 = 11;
int lhlf;			//holder for the left half of an integer
int rhlf;			//holder for the right half of an integer

//Variables relating to serial terminal
const byte nchars = 10;	//Size of rxchars array
char rxchars[nchars];	//An array that holds up to "nchars" characters.	
bool newdata = false;	//New data flag
bool jumptomain = false;	//Used to kill a function part-way through the terminal menu system
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
const byte enc1_pin4 = 2;
const byte enc2_pin4 = 18;

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

void ISR_pwm2(){	//Just like above.
	if (pwmstate2 == 0){	
		risemark2 = micros();
		pwmstate2 = 1;
	}
	else {	//if flag was already 1...
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
	pinMode(enc1_pin4, INPUT_PULLUP);
	pinMode(enc2_pin4, INPUT_PULLUP);
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

void loop(){
	CheckMode();		
	Roll();    
}

void Roll(){

	//For reverse, the phaseangle will have to increment in the negative direction [REMOVE THIS LINE ONCE IMPLEMENTED]
	phaseindex_1 = phaseindex_1 + wheel1_vel;    //This is fed into the sine function to vary the pwm duty cycle. rate is the step size for incrementing phaseindex_U1. Small = slow, big = fast, 0 = static.
	phaseindex_2 = phaseindex_2 + wheel2_vel;    //This is fed into the sine function to vary the pwm duty cycle. rate is the step size for incrementing phaseindex_U1. Small = slow, big = fast, 0 = static.

	if(phaseindex_1 >= 2*pi) {   //Reset phaseindex_U1 once it completes 2*180o in phase.
		phaseindex_1 = 0;
	}
	if(phaseindex_2 >= 2*pi) {  
		phaseindex_2 = 0;
	}

	dutyU1 = (255/2)+(power_1*(sin(phaseindex_1)));	   //The duty cycle varies with the sine function, which has output between -1 and 1. That is scaled by an amplitude variable, which effectively sets the motor power.This is all offset by half the maximum duty cycle so that the lowest instantaeous duty cycle is always positive.
	analogWrite(U1_High,dutyU1);	//Write to the PWM pins.
	dutyV1 = (255/2)+(power_1*(sin(phaseindex_1+(2*pi/3))));
	analogWrite(V1_High,dutyV1);
	dutyW1 = (255/2)+(power_1*(sin(phaseindex_1+(4*pi/3))));
	analogWrite(W1_High,dutyW1);

	dutyU2 = (255/2)+(power_2*(sin(phaseindex_2)));
	analogWrite(U2_High,dutyU2);
	dutyV2 = (255/2)+(power_2*(sin(phaseindex_2+(2*pi/3))));
	analogWrite(V2_High,dutyV2);
	dutyW2 = (255/2)+(power_2*(sin(phaseindex_2+(4*pi/3))));
	analogWrite(W2_High,dutyW2);

}



 
