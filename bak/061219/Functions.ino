int CalcMotion(){	//Not used in PWM control mode.
	wheel1_vel = (v_x*gain1*.001)+(v_th*.01);		//Convert v_x and v_th to wheel motion. The angular rate (V_th) is added for this wheel... 
	wheel2_vel = (v_x*gain2*.001)-(v_th*.01);		//...and subtracted from this one.
}

void CheckConfig(){
	//Read Iset1
	lhlf = EEPROM.read(addr_Iset1l);  //read left half
	rhlf = EEPROM.read(addr_Iset1r);  //read right half 
	Iset1 = lhlf*100 + rhlf;    //Put the two halves back together
	if (Iset1 > 5000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset Iset to a default value.
		Iset1 = 200;
	}
	//Read Iset2
	lhlf = EEPROM.read(addr_Iset2l);  
	rhlf = EEPROM.read(addr_Iset2r);  
	Iset2 = lhlf*100 + rhlf;    //Put the two halves back together
	if (Iset2 > 5000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset Iset to a default value.
		Iset2 = 700;
	}
	//Read minIset1
	lhlf = EEPROM.read(addr_minIset1l);  //read left half
	rhlf = EEPROM.read(addr_minIset1r);  //read right half 
	minIset1 = lhlf*100 + rhlf;    //Put the two halves back together
	if (minIset1 > 5000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset Iset to default value.
		minIset1 = Iset1;
	}
	//Read minIset2
	lhlf = EEPROM.read(addr_minIset2l);  
	rhlf = EEPROM.read(addr_minIset2r);  
	minIset2 = lhlf*100 + rhlf;    //Put the two halves back together
	if (minIset2 > 5000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset Iset to default value.
		minIset2 = 100;	//Iset2;
	}
	//Read gain1
	lhlf = EEPROM.read(addr_gain1l);  
	rhlf = EEPROM.read(addr_gain1r);
	gain1 = lhlf*100+rhlf;
	if (gain1 > 10000){	//If the number is bigger than our maximum, assume that the initial gain configuration was never done and reset gain to a default value.
		gain1 = 200;
	}
	//Read gain2
	lhlf = EEPROM.read(addr_gain2l);  
	rhlf = EEPROM.read(addr_gain2r);
	gain2 = lhlf*100+rhlf;
	if (gain2 > 10000){	//If the number is bigger than our maximum, assume that the initial gain configuration was never done and reset gain to a default value.
		gain2 = 600;
	}
	//Read torqueprofile1
	torqueprofile1 = EEPROM.read(addr_torqueprofile1);
	if (torqueprofile1 == 255){
		torqueprofile1 = 0;		//0 = OFF/sine, 1 = ON
	}
	//Read torqueprofile2
	torqueprofile2 = EEPROM.read(addr_torqueprofile2);
	if (torqueprofile2 == 255){
		torqueprofile2 = 0;		//0 = OFF/sine, 1 = ON
	}
	//Read tscoeff2
	tscoeff2 = EEPROM.read(addr_tscoeff2);
	if (tscoeff2 == 255){
		tscoeff2 = 20;
	}
	//Read tsphase2
	lhlf = EEPROM.read(addr_tsphase2l);  //read left half
	rhlf = EEPROM.read(addr_tsphase2r);  //read right half 
	tsphase2 = lhlf*100 + rhlf;    //Put the two halves back together
	if (tsphase2 > 5000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset Iset to a default value.
		tsphase2 = 0;
	}
	//Read currentmode1
	currentmode1 = EEPROM.read(addr_currentmode1);
	if (currentmode1 == 255){
		currentmode1 = 0;		//0 = FIXED, 1 = DYNAMIC
	}
	//Read currentmode2
	currentmode2 = EEPROM.read(addr_currentmode2);
	if (currentmode2 == 255){
		currentmode2 = 1;		//0 = FIXED, 1 = DYNAMIC
	}
	//Read commandmode1
	commandmode1 = EEPROM.read(addr_commandmode1);
	if (commandmode1 == 255){
		commandmode1 = 0;		//0 = VELOCITY control, 1 = POSITION control
	}
	//Read commandmode2
	commandmode2 = EEPROM.read(addr_commandmode2);
	if (commandmode2 == 255){
		commandmode2 = 0;		//0 = VELOCITY control, 1 = POSITION control
	}
	//Read sensortype1
	sensortype1 = EEPROM.read(addr_sensortype1);
	if (sensortype1 == 255){
		sensortype1 = 0;		//0 = NONE, 1 = ENCODER, 2 = HALL
	}
	//Read sensortype2
	sensortype2 = EEPROM.read(addr_sensortype2);
	if (sensortype2 == 255){
		sensortype2 = 2;		//0 = NONE, 1 = ENCODER, 2 = HALL
	}
	//Read accel1
	lhlf = EEPROM.read(addr_accel1l);  //read left half
	rhlf = EEPROM.read(addr_accel1r);  //read right half 
	accel1 = lhlf*100 + rhlf;    //Put the two halves back together
	if (accel1 > 10000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset Iset to a default value.
		accel1 = 3000;
	}
	//Read accel2
	lhlf = EEPROM.read(addr_accel2l);  //read left half
	rhlf = EEPROM.read(addr_accel2r);  //read right half 
	accel2 = lhlf*100 + rhlf;    //Put the two halves back together
	if (accel2 > 10000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset Iset to a default value.
		accel2 = 3000;
	}
}

void TorqueCal1(){ //Drive the motor a certain amount 
	float dummyphaseindex = 0;	
	do {
		wheel1_vel = .003;
		wheel2_vel = 0;
		dummyphaseindex = dummyphaseindex + wheel1_vel;
		if(dummyphaseindex >= 14*pi){   //Disable the drive after some number of half commutation cycles. If this number equals the number of magnets, the motor will make one full rotation.
			digitalWrite(U1_En, LOW);    
			digitalWrite(V1_En, LOW);
			digitalWrite(W1_En, LOW);
			break;
		}

		Roll_vel1();
	}
	while(dummyphaseindex < 15*pi);
}   

void TorqueCal2(){ //Drive the motor a certain amount 
	float dummyphaseindex = 0;	
	do {
		wheel1_vel = 0;
		wheel2_vel = .003;
		dummyphaseindex = dummyphaseindex + wheel2_vel;
		if(dummyphaseindex >= 14*pi){   //Disable the drive after some number of half commutation cycles. If this number equals the number of magnets, the motor will make one full rotation.
			digitalWrite(U2_En, LOW);    
			digitalWrite(V2_En, LOW);
			digitalWrite(W2_En, LOW);
			break;
		}

		Roll_vel2();
	}
	while(dummyphaseindex < 15*pi);
}   

void adu2amps(){		//Convert current sense value from ADUs to AMPERES.
	I_A = ((-1*I_adu_av+109)/-70)+.05;    //Convert raw current to Amps. Empirically derived. 
}

int PubI(){
	int i = 1;    //A counter and divisor for calculating a running average for current-sensing the PubI() function.
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
	int i = 0;    //A counter for determining rate of current rise 

	//Check quiescent current draw:
	delay(500);   //Wait for inrush current to settle.
	I_adu_sum = 0;
	I_adu_av = 0;
	for (int i = 1; i <= 200; i++) {    //Check current draw. Sum and average number i times.
		I_adu_sum = I_adu_sum + analogRead(Isense);
		I_adu_av = (I_adu_av + I_adu_sum)/i;  
	}
	adu2amps();
	I_qui = I_A;	
	Serial.print(F("\nDetected quiescent current (A): "));
	Serial.println(I_qui);
	Serial.print(F("\n"));
	delay(2000);
	digitalWrite(U1_En, HIGH);    //Enable each phase
	digitalWrite(V1_En, HIGH);
	digitalWrite(W1_En, HIGH);
	digitalWrite(U2_En, HIGH);
	digitalWrite(V2_En, HIGH);
	digitalWrite(W2_En, HIGH);
  
	//Ramp up power on motor 1 until desired current is detected.
	power_1_low = 0;
	do{
		power_1 = power_1 + 1;    //Ramp up the power with each iteration of this do-while loop.

		//WRITE OUTPUTS
		dutyU1 = (255/2)+(power_1*(sin(phaseindex_1)));	   
		analogWrite(U1_High,dutyU1);	//Write to the PWM pins.
		dutyV1 = (255/2)+(power_1*(sin(phaseindex_1+(2*pi/3))));
		analogWrite(V1_High,dutyV1);
		dutyW1 = (255/2)+(power_1*(sin(phaseindex_1+(4*pi/3))));
		analogWrite(W1_High,dutyW1);

		delay(5);   //Some settling time is needed for accuracy.
		I_adu_sum = 0;
		I_adu_av = 0;
		for (int i = 1; i <= 200; i++) {    //Check current draw. Sum and average number i times.
			I_adu_sum = I_adu_sum + analogRead(Isense);
			if (i == 200){			
				I_adu_av = I_adu_sum/i;
			}  
		} 
		adu2amps();		
		//Serial.print("\nIteration: ");
		//Serial.print(i);
		//Serial.print("\nAmperes: ");
		//Serial.print(I_A);
		if ((i == 50) && (I_A < (I_qui+.02))){   //After several iterations, see if the current is still near the quiescent draw.
			power_1 = 0;
			digitalWrite(U1_En, LOW);    //Disable each phase
			digitalWrite(V1_En, LOW);
			digitalWrite(W1_En, LOW);
			break;
		}
		if (I_A < (((minIset1)/1000)+I_qui)){	//Every iteration that the current is less than minIset, save the power to power_x_low. When the routine is completed, this assures that power_1_low will contain the proper value.
			power_1_low = power_1;
		}
		i++;             
	} while (I_A < (((Iset1)/1000)+I_qui)-.04); //As soon as I_A exceeds the user-determined current in amps, exit the loop, thereby maintaining the last power setting.
	if (power_1 == 0){
		Serial.println(F("Motor 1 not connected."));
	}
	else {
		Serial.print(F("\nMotor 1 current was set to "));
		Serial.print(I_A);	
		Serial.println(F("A (as measured)"));
	}
	//delay(3000);		//Uncomment for diagnostic reaons (gives time to check displayed value against a meter)
	power_1_high = power_1;      //Store the discovered power setting so we can set power_1 to zero to test the other motor, and so we can reduce power and come back to it in the future.
	power_1 = 0;    //Set power to zero so we can test the other motor.

	//Ramp up power on motor 2 until desired current is detected.
	i = 0;
	power_2_low = 0;
	do{
		power_2 = power_2 + 1;    //Ramp up the power with each iteration of this do-while loop.

		//WRITE OUTPUTS
		dutyU2 = (255/2)+(power_2*(sin(phaseindex_2)));	   //The duty cycle varies with the sine function, which has output between -1 and 1. That is scaled by an amplitude variable, which effectively sets the motor power.This is all offset by half the maximum duty cycle so that the lowest instantaeous duty cycle is always positive.
		analogWrite(U2_High,dutyU2);	//Write to the PWM pins.
		dutyV2 = (255/2)+(power_2*(sin(phaseindex_2+(2*pi/3))));
		analogWrite(V2_High,dutyV2);
		dutyW2 = (255/2)+(power_2*(sin(phaseindex_2+(4*pi/3))));
		analogWrite(W2_High,dutyW2);

		delay(5);   //Some settling time is needed for accuracy.
		I_adu_sum = 0;
		I_adu_av = 0;
		for (int i = 1; i <= 200; i++) {    //Check current draw. Sum and average number i times.
			I_adu_sum = I_adu_sum + analogRead(Isense);
			I_adu_av = (I_adu_av + I_adu_sum)/i;  
		}  
		adu2amps();		
		//Serial.print("\nIteration: ");
		//Serial.print(i);
		//Serial.print("\nAmperes: ");
		//Serial.print(I_A);        
		Serial.println(power_2);
		if ((i == 50) && (I_A < (I_qui+.02))){   //After several iterations, see if the current is still near the quiescent draw.
			power_2 = 0;
			digitalWrite(U2_En, LOW);    //Disable each phase
			digitalWrite(V2_En, LOW); 
			digitalWrite(W2_En, LOW);
			break;
		}
		if (I_A < (((minIset2)/1000)+I_qui)){	//Every iteration that the current is less than minIset, save the power to power_x_low. When the routine is completed, this assures that power_1_low will contain the proper value.
			power_2_low = power_2;
		}
		i++;  
	} while (I_A < (((Iset2)/1000)+I_qui)-.04); //As soon as I_A exceeds the user-determined current in amps, exit the loop, thereby maintaining the last power setting.
	if (power_2 == 0){
		Serial.println(F("Motor 2 not connected."));
	}
	else {
		Serial.print(F("\nMotor 2 current was set to "));
		Serial.print(I_A);	
		Serial.println(F("A (as measured)"));
	}	
	//delay(3000);		//Uncomment for diagnostic reaons (gives time to check displayed value against a meter)
	power_2_high = power_2;
	power_1 = power_1_high;    //Re-energize motor 1. Motor 2 is already powered at this point.
	digitalWrite(13,LOW);   //Put the pin 13 LED low to indicate the board is ready to go.
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

int CheckMode(){
	//********USB**********
	if(digitalRead(mode_usb) == LOW){     //If in USB mode, get velocity data from the ROS node.
		//Serial.end();
		nh.spinOnce();    //Spin the node once (check for and process messages), hence retrieving values for v_x and v_th from the ROS. We use nh.spinOnce() instead of nh.spin() because this is already in a main loop and we want this thing to happen once every time through the loop.
		CalcMotion();   //Take the raw velocity obtained according to the current mode setting (ROS or I2C) and convert to forward rate (v_x) and angular rate (a_z), and then calculate the rates needed for each wheel.
		PubI();
		pwmflag = false;
	}

    //********I2C**********
	if(digitalRead(mode_i2c) == LOW){     //Check for I2C stuff  
		//mode
		//CalcMotion();	
		pwmflag = false;
	}

    //********Config**********
	while(digitalRead(mode_config) == LOW){     //Check for configuration mode
		digitalWrite(U1_En, LOW);    //Enable each phase
		digitalWrite(V1_En, LOW);
		digitalWrite(W1_En, LOW);
		digitalWrite(U2_En, LOW);
		digitalWrite(V2_En, LOW);
		digitalWrite(W2_En, LOW);
		SerialPrompt();   
		pwmflag = false; 
	}

	//********PWM**********
	if((digitalRead(mode_usb) == HIGH) && (digitalRead(mode_i2c) == HIGH) && (digitalRead(mode_config) == HIGH))  {     //If none of the mode pins are LOW, read the PWM inputs to control each motor's velocity independently.
		if(pwmflag == false) {    //if we weren't already in this mode before we need to check which PWM ports have signals coming in.
			if(pwm1_t > 0){    //Is the pwm active?
				pwm1active = true;    //If so, we'll keep this channel on
			}
			else{
				pwm1active = false;   //If not, keep it off so the main loop doesn't get slowed down by an inactive PWM
				digitalWrite(U1_En, LOW);    //Disable this channel
				digitalWrite(V1_En, LOW);
				digitalWrite(W1_En, LOW);
			}
			if(pwm2_t > 0){
				pwm2active = true;
			}
			else{
				pwm2active = false;
				digitalWrite(U2_En, LOW);    //Disable this channel
				digitalWrite(V2_En, LOW);
				digitalWrite(W2_En, LOW);
			}
		}

		if(pwm1active == true){     //Only get velocity data from this PWM channel if it was found to be connected above    
			if (commandmode1 == 0){	//if in velocity mode
				wheel1_vel = (pwm1_t-1500)*.0000006*gain1;	//Normal PWM servo pulsewidth goes from 800 to 2200ms. We subtract 1500ms, corresponding to the middle of that range, so that pulses in the range 800-1499 will have negative velocity, and 1501-2200 will be positive. 
			}
			else if (commandmode1 == 1){
				wheel1_pos = (pwm1_t-1500)*.00005*gain1;
			}
		}

		if(pwm2active == true){    //Only get velocity data from this PWM channel if it was found to be connected above
			if (commandmode2 == 0){
				wheel2_vel = (pwm2_t-1500)*.0000006*gain2;
			}
			else if (commandmode2 == 1){
				wheel2_pos = (pwm2_t-1500)*.00005*gain2;
			}
		}
		pwmflag = true;
	}
}
