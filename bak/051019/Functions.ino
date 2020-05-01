int CalcMotion(){	//Not used in PWM control mode.
	wheel1_vel = (v_x*vcal_1*.001)+(v_th*.01);		//Convert v_x and v_th to wheel motion. The angular rate (V_th) is added for this wheel... 
	wheel2_vel = (v_x*vcal_2*.001)-(v_th*.01);		//...and subtracted from this one.
}

void CheckConfig(){
	//Read I_set_1
	lhlf = EEPROM.read(addr_Iset1l);  //read left half
	rhlf = EEPROM.read(addr_Iset1r);  //read right half 
	I_set_1 = lhlf*100 + rhlf;    //Put the two halves back together
	if (I_set_1 > 5000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset I_set to a default value.
		I_set_1 = 200;
	}
	//Read I_set_2
	lhlf = EEPROM.read(addr_Iset2l);  
	rhlf = EEPROM.read(addr_Iset2r);  
	I_set_2 = lhlf*100 + rhlf;    //Put the two halves back together
	if (I_set_2 > 5000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset I_set to a default value.
		I_set_2 = 200;
	}
	//Read vcal_1
	lhlf = EEPROM.read(addr_vcal1l);  
	rhlf = EEPROM.read(addr_vcal1r);
	vcal_1 = lhlf*100+rhlf;
	if (vcal_1 > 10000){	//If the number is bigger than our maximum, assume that the initial vcal configuration was never done and reset vcal_ to a default value.
		vcal_1 = 200;
	}
	//Read vcal_2
	lhlf = EEPROM.read(addr_vcal2l);  
	rhlf = EEPROM.read(addr_vcal2r);
	vcal_2 = lhlf*100+rhlf;
	if (vcal_2 > 10000){	//If the number is bigger than our maximum, assume that the initial vcal configuration was never done and reset vcal_ to a default value.
		vcal_2 = 200;
	}
	//Read cogprof_1
	cogprof_1 = EEPROM.read(addr_cogprof1);
	if (cogprof_1 == 255){
		cogprof_1 = 0;
	}
	//Read cogprof_2
	cogprof_2 = EEPROM.read(addr_cogprof2);
	if (cogprof_2 == 255){
		cogprof_2 = 0;
	}
	//Read motormode_1
	motormode_1 = EEPROM.read(addr_motormode1);
	if (motormode_1 == 255){
		motormode_1 = 0;
	}
	//Read motormode_2
	motormode_2 = EEPROM.read(addr_motormode2);
	if (motormode_2 == 255){
		motormode_2 = 0;
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

		Roll();
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

		Roll();
	}
	while(dummyphaseindex < 15*pi);
}   

void adu2amps(){		//Convert current sense value from ADUs to AMPERES.
	I_A = ((-1*I_adu_av+109)/-70)+.05;    //Convert raw current to Amps. Empirically derived. 
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
	adu2amps();
	I_qui = I_A;	
	Serial.print("\nDetected quiescent current (A): ");
	Serial.println(I_qui);
	Serial.print("\n");
	delay(2000);
	wheel1_vel = 0;   //Motors should not be moving.
	wheel2_vel = 0;
	digitalWrite(U1_En, HIGH);    //Enable each phase
	digitalWrite(V1_En, HIGH);
	digitalWrite(W1_En, HIGH);
	digitalWrite(U2_En, HIGH);
	digitalWrite(V2_En, HIGH);
	digitalWrite(W2_En, HIGH);
  
	//Ramp up power on motor 1 until desired current is detected.
	do{
		power_1 = power_1 + 1;    //Ramp up the power with each iteration of this do-while loop.
		Roll();     //Iterate on the 3-phase bridge outputs
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
		//Serial.print(j);
		//Serial.print("\nAmperes: ");
		//Serial.print(I_A);
		if ((j == 50) && (I_A < (I_qui+.02))){   //After several iterations, see if the current is still near the quiescent draw.
			power_1 = 0;
			digitalWrite(U1_En, LOW);    //Disable each phase
			digitalWrite(V1_En, LOW);
			digitalWrite(W1_En, LOW);
			break;
		}
		j++;             
	} while (I_A < (((I_set_1)/1000)+I_qui)); //As soon as I_A exceeds the user-determined current in amps, exit the loop, thereby maintaining the last power setting.
	if (power_1 == 0){
		Serial.println("Motor 1 not connected.");
	}
	else {
		Serial.print("\nMotor 1 current was set to ");
		Serial.print(I_A);	
		Serial.println("A.");
	}
	//delay(3000);		//Uncomment for diagnostic reaons (gives time to check displayed value against a meter)
	power_1_saved = power_1;      //Store the discovered power setting so we can set power_1 to zero to test the other motor, and so we can reduce power and come back to it in the future.
	power_1 = 0;    //Set power to zero so we can test the other motor.

	//Ramp up power on motor 2 until desired current is detected.
  j = 0;
	do{
		power_2 = power_2 + 1;    //Ramp up the power with each iteration of this do-while loop.
		Roll();     //Iterate on the 3-phase bridge outputs
		delay(5);   //Some settling time is needed for accuracy.
		I_adu_sum = 0;
		I_adu_av = 0;
		for (int i = 1; i <= 200; i++) {    //Check current draw. Sum and average number i times.
			I_adu_sum = I_adu_sum + analogRead(Isense);
			I_adu_av = (I_adu_av + I_adu_sum)/i;  
		}  
		adu2amps();		
		//Serial.print("\nIteration: ");
		//Serial.print(j);
		//Serial.print("\nAmperes: ");
		//Serial.print(I_A);        
		if ((j == 50) && (I_A < (I_qui+.02))){   //After several iterations, see if the current is still near the quiescent draw.
			power_2 = 0;
			digitalWrite(U2_En, LOW);    //Disable each phase
			digitalWrite(V2_En, LOW); 
			digitalWrite(W2_En, LOW);
			break;
		}
		j++;  
	} while (I_A < (((I_set_2)/1000)+I_qui)); //As soon as I_A exceeds the user-determined current in amps, exit the loop, thereby maintaining the last power setting.
	if (power_2 == 0){
		Serial.println("Motor 2 not connected.");
	}
	else {
		Serial.print("\nMotor 2 current was set to ");
		Serial.print(I_A);	
		Serial.println("A.");
	}	
	//delay(3000);		//Uncomment for diagnostic reaons (gives time to check displayed value against a meter)
	power_2_saved = power_2;
	power_1 = power_1_saved;    //Re-energize motor 1. Motor 2 is already powered at this point.
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
			pwm1_throttle = 0;
			pwm1_throttle = pulseIn(pwm1_in, HIGH); //Check for a PWM signal on this channel
			if(pwm1_throttle > 0){    //Was a signal detected?
				pwm1active = true;    //If so, we'll keep this channel on
			}
			else{
				pwm1active = false;   //If not, keep it off so the main loop doesn't get slowed down by an inactive PWM
				digitalWrite(U1_En, LOW);    //Disable this channel
				digitalWrite(V1_En, LOW);
				digitalWrite(W1_En, LOW);
			}

			pwm2_throttle = 0;
			pwm2_throttle = pulseIn(pwm2_in, HIGH);
			if(pwm2_throttle > 0){
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
			pwm1_throttle = pulseIn(pwm1_in, HIGH);    
			wheel1_vel = (pwm1_throttle-1500)*.000005*vcal_1; //Normal PWM servo pulsewidth goes from 800 to 2200ms. We subtract 1500ms, corresponding to the middle of that range, so that pulses in the range 800-1499 will have negative velocity, and 1501-2200 will be positive. 
		}

		if(pwm2active == true){    //Only get velocity data from this PWM channel if it was found to be connected above
			pwm2_throttle = pulseIn(pwm2_in, HIGH);
			wheel2_vel = (pwm2_throttle-1500)*.000005*vcal_2;
		}
		pwmflag = true;
	}
}
