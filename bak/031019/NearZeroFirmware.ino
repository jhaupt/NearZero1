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
-Add configuration mode.
-Fine-tune quiescent current comparison in SetPower(). Check minimum permissible values for I_userset variables. Should be able to go less than 100mA.

-Add I2C support and test with an Arduino.
-Design encoder and add publisher for the /odom topic. Reference http://wiki.ros.org/navigation/Tutorials/RobotSetup/Odom

*/

#include <avr/io.h>   //IO library for avr microcontrollers.
#include <EEPROM.h>   //We'll need to access the EEPROM for storing configuration data.
#include <PWM.h>    //Include Sam Knight's PWM library, available at https://github.com/terryjmyers/PWM.git
#include <ros.h>    //Let there be ROS! The ROS arduino libraries are installed according to http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup
#include <geometry_msgs/Twist.h>    //We're going to be subscribing to messages of type Twist, which contain 3-axis linear and angular motion commands.
#include <std_msgs/Float32.h>     //We're going to be publishing messages of type Float32, which holds a single float32 value. This is for reporting current draw.

float pi = 3.1415926;
float phaseangle_U1 = pi;     //Start at pi radians so that the starting value of sin(phaseangle_U1) is 0.
float phaseangle_V1;
float phaseangle_W1;
float phaseangle_U2 = pi;
float phaseangle_V2;
float phaseangle_W2;
float v_x;    //Linear velocity, as commanded over the ROS cmd_vel topic.
float v_th;   //Angular velocity, as commanded over the ROS cmd_vel topic.
float wheel1_vel = 0;    //sets the velocity (positive or negative). This is what increments the phaseangle for each phase.
float wheel2_vel = 0;
float I_A;   //Current in amps after conversion from ADUs.
float I_qui;  //Stores currrent in amps before energizing motors in SetPower() function.
float I_adu_av = 0;   //Averaged current in ADUs
int I_adu_sum = 0;   //Cumulative current in ADUs
float I_userset_1 = 100;    //The user-determined current draw in mA. Power is automatically ramped to this value at start time.
float I_userset_2 = 100; 
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
int pwm1_throttle;   //Holds PWM pulse width value. Only used when in "PWM input" mode.
int pwm2_throttle;
bool pwmflag = false;    //Used to detect if PWM mode has just been entered, in which case the two PWM inputs will be checked for activity one time only.
bool pwm1active = false;    //Indicates whether a PWM source is connected to channel 1.
bool pwm2active = false;    //Indicates whether a PWM source is connected to channel 2.

int32_t freq = 25000; //PWM frequency (in Hz). 3-phase bridge is rated to 50kHz. Allegro current sensor bandwidth is 80kHz.

char rx_byte = 0;

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
const byte pwm1_in = 24;  //pwm input for channel 1
const byte pwm2_in = 25;  //pwm in put for channel 2
const byte enc1_pin4 = 2;
const byte enc2_pin4 = 18;

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
  pinMode(pwm1_in, INPUT);
  pinMode(pwm2_in, INPUT);
  pinMode(Isense, INPUT);
  pinMode(U1_En, OUTPUT);
  pinMode(V1_En, OUTPUT);
  pinMode(W1_En, OUTPUT);
  pinMode(U2_En, OUTPUT);
  pinMode(V2_En, OUTPUT);
  pinMode(W2_En, OUTPUT);
  pinMode(enc1_pin4, INPUT_PULLUP);
  pinMode(enc2_pin4, INPUT_PULLUP);

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

  CheckConfig();  //Get configuration data
  SetPower();   //Set the 3-phase modulation amplitude to draw the amount of current dictated by the I_userset variables.    
}

void loop(){
    CheckMode();
    Roll();    
}

void Roll(){
  
    //For reverse, the phaseangle will have to increment in the negative direction [REMOVE THIS LINE ONCE IMPLEMENTED]
    phaseangle_U1 = phaseangle_U1 + wheel1_vel;    //This is fed into the sine function to vary the pwm duty cycle. rate is the step size for incrementing phaseangle_U1. Small = slow, big = fast, 0 = static.
    phaseangle_V1 = phaseangle_U1 + (2*pi/3);   //phaseangle_V1 is 1/3th phase ahead of phaseangle_U1
    phaseangle_W1 = phaseangle_V1 + (2*pi/3);   //phaseangle_W1 is 1/3th phase ahead of phaseangle_V1
    phaseangle_U2 = phaseangle_U2 + wheel2_vel;    //This is fed into the sine function to vary the pwm duty cycle. rate is the step size for incrementing phaseangle_U1. Small = slow, big = fast, 0 = static.
    phaseangle_V2 = phaseangle_U2 + (2*pi/3);   //phaseangle_V1 is 1/3th phase ahead of phaseangle_U1
    phaseangle_W2 = phaseangle_V2 + (2*pi/3);   //phaseangle_W1 is 1/3th phase ahead of phaseangle_V1

    if(phaseangle_U1 >= 2*pi)   //Reset phaseangle_U1 once it completes 360o in phase.
    {
      phaseangle_U1 = 0;
    }
    if(phaseangle_U2 >= 2*pi)   //Reset phaseangle_U1 once it completes 360o in phase.
    {
      phaseangle_U2 = 0;
    }

    //                       Fundamental         same imprint on each phase   fixed with motor angle
    dutyU1 = (255/2)+(power_1*(sin(phaseangle_U1)+(.05*sin(3*phaseangle_U1))+(.1*sin(7*phaseangle_U1+2.5))));   //The duty cycle varies with the sine function, which has output between -1 and 1. That is scaled by an amplitude variable, which effectively sets the motor power.This is all offset by half the maximum duty cycle so that the lowest instantaeous duty cycle is always positive.
    dutyV1 = (255/2)+(power_1*(sin(phaseangle_V1)+(.05*sin(3*phaseangle_V1))+(.1*sin(7*phaseangle_U1+2.5))));
    dutyW1 = (255/2)+(power_1*(sin(phaseangle_W1)+(.05*sin(3*phaseangle_W1))+(.1*sin(7*phaseangle_U1+2.5))));
    
    dutyU2 = (255/2)+(power_2*(sin(phaseangle_U2)+(.05*sin(3*phaseangle_U2))+(.0*sin(3*phaseangle_U2))));
    dutyV2 = (255/2)+(power_2*(sin(phaseangle_V2)+(.05*sin(3*phaseangle_V2))+(.0*sin(3*phaseangle_U2))));
    dutyW2 = (255/2)+(power_2*(sin(phaseangle_W2)+(.05*sin(3*phaseangle_W2))+(.0*sin(3*phaseangle_U2))));

    //analogWrite(13,(50*(sin(phaseangle_U1)+1)));
    analogWrite(U1_High,dutyU1);
    analogWrite(V1_High,dutyV1);
    analogWrite(W1_High,dutyW1);
    analogWrite(U2_High,dutyU2);
    analogWrite(V2_High,dutyV2);
    analogWrite(W2_High,dutyW2);
}


int CheckMode(){
    
    if(digitalRead(mode_usb) == LOW){     //If in USB mode, get velocity data from the ROS node.
      //Serial.end();
      nh.spinOnce();    //Spin the node once (check for and process messages), hence retrieving values for v_x and v_th from the ROS. We use nh.spinOnce() instead of nh.spin() because this is already in a main loop and we want this thing to happen once every time through the loop.
      CalcMotion();   //Take the raw velocity obtained according to the current mode setting (ROS or I2C) and convert to forward rate (v_x) and angular rate (a_z), and then calculate the rates needed for each wheel.
      PubI();
      pwmflag = false;
    }
    if(digitalRead(mode_i2c) == LOW){     //Check for I2C mode
      //stuff
      CalcMotion();   //Take the raw velocity obtained according to the current mode setting (ROS or I2C) and convert to forward rate (v_x) and angular rate (a_z), and then calculate the rates needed for each wheel.
      pwmflag = false;
    }
    if(digitalRead(mode_config) == LOW){     //Check for configuration (EEPROM write) mode
      Serial.begin(9600);

      if (Serial.available() > 0) {    // is a character available?
      rx_byte = Serial.read();       // get the character
  
      // check if a number was received
      if ((rx_byte >= '0') && (rx_byte <= '9')) {
        Serial.print("Number received: ");
        Serial.println(rx_byte);
        }
      else {
        Serial.println("Not a number.");
        }
      } 
      //value1 = EEPROM.read(0);   //Read current configuration value
      //value2 = EEPROM.read(1);
      //EEPROM.write(0, i);   //write the integer to address location 0 out of 255.
      pwmflag = false; 
    }
    
    if((digitalRead(mode_usb) == HIGH) && (digitalRead(mode_i2c) == HIGH) && (digitalRead(mode_config) == HIGH))  {     //If none of the mode pins are LOW, read the PWM inputs to control each motor's velocity independently.
      if(pwmflag == false) {  //if we weren't already in this mode before we need to check which PWM ports have signals coming in.
        
        pwm1_throttle = 0;
        pwm1_throttle = pulseIn(pwm1_in, HIGH); //Check for a PWM signal on this channel
        if(pwm1_throttle > 0){    //Was a signal detected?
          pwm1active = true;    //If so, we'll keep this channel on
        }
        else{
          pwm1active = false;   //If not, keep it off so the main loop doesn't get slowed down by an inactive PWM
        }
        
        pwm2_throttle = 0;
        pwm2_throttle = pulseIn(pwm2_in, HIGH);
        if(pwm2_throttle > 0){
          pwm2active = true;
        }
        else{
          pwm2active = false;
        }
       
      }

      if(pwm1active == true){     //Only get velocity data from this PWM channel if it was found to be connected above
        pwm1_throttle = pulseIn(pwm1_in, HIGH);    
        wheel1_vel = (pwm1_throttle-1500)*.001; //Normal PWM servo pulsewidth goes from 800 to 2200ms. We subtract 1500ms, corresponding to the middle of that range, so that pulses in the range 800-1499 will have negative velocity, and 1501-2200 will be positive. 
      }

      if(pwm2active == true){    //Only get velocity data from this PWM channel if it was found to be connected above
        pwm2_throttle = pulseIn(pwm2_in, HIGH);
        wheel2_vel = (pwm2_throttle-1500)*.001;
      }
      pwmflag = true;
    }
}

int CheckConfig(){
  //value = EEPROM.read(0);
}

int PubI(){
    I_adu_sum = I_adu_sum + analogRead(Isense);
    I_adu_av = (I_adu_av + I_adu_sum)/i;
    I_A = (-1*I_adu_av+109)/-81.92;    //Convert raw current to Amps. Offset derrived empirically.   
    i++;  //Increment counter.
    if (i == 100){     //When the averaging cycle has been completed (as set by value of i here), publish the idraw message and reset the index.
      idraw_msg.data = I_A;
      pub_I.publish(&idraw_msg);
      I_adu_sum = 0;
      I_adu_av = 0;
      i = 1;
    }
}

int SetPower(){

   //Check quiescent current draw:
    delay(500);   //Wait for inrush current to settle.
    I_adu_sum = 0;
    I_adu_av = 0;
    for (int i = 1; i <= 200; i++) {    //Check current draw. Sum and average number i times.
      I_adu_sum = I_adu_sum + analogRead(Isense);
      I_adu_av = (I_adu_av + I_adu_sum)/i;  
    }
    I_qui = (-1*I_adu_av+109)/-75;    //Convert raw current to Amps. Empirically derived.   

    wheel1_vel = 0;   //Motors should not be moving.
    wheel2_vel = 0;
    digitalWrite(U1_En, HIGH);    //Enable each phase
    digitalWrite(V1_En, HIGH);
    digitalWrite(W1_En, HIGH);
    digitalWrite(U2_En, HIGH);
    digitalWrite(V2_En, HIGH);
    digitalWrite(W2_En, HIGH);
    Serial.begin(9600);
    //Ramp up power on motor 1 until desired current is detected.
    do{
      power_1 = power_1 + 2;    //Ramp up the power with each iteration of this do-while loop.
      Roll();     //Iterate on the 3-phase bridge outputs
      delay(5);   //Some settling time is needed for accuracy.
      I_adu_sum = 0;
      I_adu_av = 0;
      for (int i = 1; i <= 200; i++) {    //Check current draw. Sum and average number i times.
        I_adu_sum = I_adu_sum + analogRead(Isense);
        I_adu_av = (I_adu_av + I_adu_sum)/i;  
      }
      I_A = (-1*I_adu_av+109)/-75;    //Convert raw current to Amps. Empirically derived.   
      if ((j == 50) && (I_A < (I_qui+.02))){   //After several iterations, see if the current is still near the quiescent draw.
        Serial.println("Motor 1 not connected.");
        power_1 = 0;
        digitalWrite(U1_En, LOW);    //Disable each phase
        digitalWrite(V1_En, LOW);
        digitalWrite(W1_En, LOW);
        break;
      }
      j++;  
                 
    } while (I_A < (((I_userset_1)/1000)+I_qui)); //As soon as I_A exceeds the user-determined current in amps, exit the loop, thereby maintaining the last power setting.
    
    power_1_saved = power_1;      //Store the discovered power setting so we can set power_1 to zero to test the other motor, and so we can reduce power and come back to it in the future.
    power_1 = 0;    //Set power to zero so we can test the other motor.
    
    //Ramp up power on motor 2 until desired current is detected.
    j = 0;
    do{
      power_2 = power_2 + 2;    //Ramp up the power with each iteration of this do-while loop.
      Roll();     //Iterate on the 3-phase bridge outputs
      delay(5);   //Some settling time is needed for accuracy.
      I_adu_sum = 0;
      I_adu_av = 0;
      for (int i = 1; i <= 200; i++) {    //Check current draw. Sum and average number i times.
        I_adu_sum = I_adu_sum + analogRead(Isense);
        I_adu_av = (I_adu_av + I_adu_sum)/i;  
      }
      I_A = (-1*I_adu_av+109)/-75;    //Convert raw current to Amps. Empirically derived.             
      if ((j == 50) && (I_A < (I_qui+.02))){   //After several iterations, see if the current is still near the quiescent draw.
        Serial.println("Motor 2 not connected.");
        power_2 = 0;
        digitalWrite(U2_En, LOW);    //Disable each phase
        digitalWrite(V2_En, LOW); 
        digitalWrite(W2_En, LOW);
        break;
      }
      j++;  
    
    } while (I_A < (((I_userset_2)/1000)+I_qui)); //As soon as I_A exceeds the user-determined current in amps, exit the loop, thereby maintaining the last power setting.
    power_2_saved = power_2;
    power_1 = power_1_saved;    //Re-energize motor 1. Motor 2 is already powered at this point.
    digitalWrite(13,LOW);   //Put the pin 13 LED low to indicate the board is ready to go.
}

int CalcMotion(){
    //TODO: Replace coefficients with variables names like "linear_multiplier" and "angular_multiplier".
    wheel1_vel = (v_x*.25)+(v_th*.01);    //Convert to wheel motion. The angular rate must be subtracted for the other wheel.
    wheel2_vel = (v_x*.25)-(v_th*.01);
}

int ReadEncoders(){
  
    if (digitalRead(enc1_pin4) == LOW){
      digitalWrite(13,HIGH);
      delay(500);
      digitalWrite(13,LOW);
    }
    
    if (digitalRead(enc2_pin4) == LOW){
      digitalWrite(13,HIGH);
      delay(500);
      digitalWrite(13,LOW);
    }
}
