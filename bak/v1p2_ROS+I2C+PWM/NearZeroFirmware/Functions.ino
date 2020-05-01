void CheckConfig(){

	//Read maxIset1
	lhlf = EEPROM.read(addr_maxIset1l);  
	rhlf = EEPROM.read(addr_maxIset1r);  
	maxIset1 = lhlf*100 + rhlf;    //Put the two halves back together
	if (maxIset1 > 4000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset Iset to a default value.
		maxIset1 = 300;
	}
	//Read maxIset2
	lhlf = EEPROM.read(addr_maxIset2l);  
	rhlf = EEPROM.read(addr_maxIset2r);  
	maxIset2 = lhlf*100 + rhlf;    //Put the two halves back together
	if (maxIset2 > 4000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset Iset to a default value.
		maxIset2 = 300;
	}
	//Read minIset1
	lhlf = EEPROM.read(addr_minIset1l);  
	rhlf = EEPROM.read(addr_minIset1r);  
	minIset1 = lhlf*100 + rhlf;    //Put the two halves back together
	if (minIset1 > 4000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset Iset to default value.
		minIset1 = 200;	
	}
	//Read minIset2
	lhlf = EEPROM.read(addr_minIset2l);  
	rhlf = EEPROM.read(addr_minIset2r);  
	minIset2 = lhlf*100 + rhlf;    //Put the two halves back together
	if (minIset2 > 4000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset Iset to default value.
		minIset2 = 200;	
	}
	//Read gain1
	lhlf = EEPROM.read(addr_gain1l);  
	rhlf = EEPROM.read(addr_gain1r);
	gain1 = lhlf*100+rhlf;
	if (gain1 > 10000){	//If the number is bigger than our maximum, assume that the initial gain configuration was never done and reset gain to a default value.
		gain1 = 60;
	}
	//Read gain2
	lhlf = EEPROM.read(addr_gain2l);  
	rhlf = EEPROM.read(addr_gain2r);
	gain2 = lhlf*100+rhlf;
	if (gain2 > 10000){	//If the number is bigger than our maximum, assume that the initial gain configuration was never done and reset gain to a default value.
		gain2 = 60;
	}
	//Read pwmoffset1
	lhlf = EEPROM.read(addr_pwmoffset1l);  
	rhlf = EEPROM.read(addr_pwmoffset1r);
	pwmoffset1 = lhlf*100+rhlf;
	if (pwmoffset1 > 10000){	//If the number is bigger than our maximum, assume that the initial gain configuration was never done and reset gain to a default value.
		pwmoffset1 = 1500;
	}
	//Read pwmoffset2
	lhlf = EEPROM.read(addr_pwmoffset2l);  
	rhlf = EEPROM.read(addr_pwmoffset2r);
	pwmoffset2 = lhlf*100+rhlf;
	if (pwmoffset2 > 10000){	//If the number is bigger than our maximum, assume that the initial gain configuration was never done and reset gain to a default value.
		pwmoffset2 = 1500;
	}
	//Read wheelbasescale
	lhlf = EEPROM.read(addr_wheelbasescalel);  
	rhlf = EEPROM.read(addr_wheelbasescaler);
	wheelbasescale = lhlf*100+rhlf;
	if (wheelbasescale > 1000){	//If the number is bigger than our maximum, assume that the initial gain configuration was never done and reset gain to a default value.
		wheelbasescale = 50;
	}
	//Read wheeldiamscale
	lhlf = EEPROM.read(addr_wheeldiamscalel);  
	rhlf = EEPROM.read(addr_wheeldiamscaler);
	wheeldiamscale = lhlf*100+rhlf;
	if (wheeldiamscale > 1000){	//If the number is bigger than our maximum, assume that the initial gain configuration was never done and reset gain to a default value.
		wheeldiamscale = 50;
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
	//Read tscoeff1
	tscoeff1 = EEPROM.read(addr_tscoeff1);
	if (tscoeff1 == 255){
		tscoeff1 = 20;
	}
	//Read tscoeff2
	tscoeff2 = EEPROM.read(addr_tscoeff2);
	if (tscoeff2 == 255){
		tscoeff2 = 20;
	}
	//Read currentmode1
	currentmode1 = EEPROM.read(addr_currentmode1);
	if (currentmode1 == 255){
		currentmode1 = 0;		//0 = FIXED, 1 = DYNAMIC (NOT CURRENTLY SUPPORTED)
	}
	//Read currentmode2
	currentmode2 = EEPROM.read(addr_currentmode2);
	if (currentmode2 == 255){
		currentmode2 = 0;		//0 = FIXED, 1 = DYNAMIC (NOT CURRENTLY SUPPORTED)
	}
	//Read commandmode1
	commandmode1 = EEPROM.read(addr_commandmode1);
	if (commandmode1 == 255){
		commandmode1 = 0;		//0 = VELOCITY control, 1 = POSITION control, 2 = SERVO control
	}
	//Read commandmode2
	commandmode2 = EEPROM.read(addr_commandmode2);
	if (commandmode2 == 255){
		commandmode2 = 0;		//1 = VELOCITY control, 1 = POSITION control, 2 = SERVO control
	}
	//Read sensortype1
	sensortype1 = EEPROM.read(addr_sensortype1);
	if (sensortype1 == 255){
		sensortype1 = 0;		//0 = NONE, 1 = ENCODER, 2 = ENCLODER/SERVO, 3 = HALL
	}
	//Read sensortype2
	sensortype2 = EEPROM.read(addr_sensortype2);
	if (sensortype2 == 255){
		sensortype2 = 0;		//0 = NONE, 1 = ENCODER, 2 = ENCLODER/SERVO, 3 = HALL
	}
	//Read dir1
	dir1 = EEPROM.read(addr_dir1);
	if (dir1 == 255){
		dir1 = 0;		//0 = NORMAL, 1 = REVERSE
	}
	//Read dir2
	dir2 = EEPROM.read(addr_dir2);
	if (dir2 == 255){
		dir2 = 0;		//0 = NORMAL, 1 = REVERSE
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
	//Read maxslewvel1
	lhlf = EEPROM.read(addr_maxslewvel1l);  //read left half
	rhlf = EEPROM.read(addr_maxslewvel1r);  //read right half 
	maxslewvel1 = lhlf*100 + rhlf;    //Put the two halves back together
	if (maxslewvel1 > 1000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset Iset to a default value.
		maxslewvel1 = 30;
	}
	maxslewvel1 = maxslewvel1/1000;

	//Read maxslewvel2
	lhlf = EEPROM.read(addr_maxslewvel2l);  //read left half
	rhlf = EEPROM.read(addr_maxslewvel2r);  //read right half 
	maxslewvel2 = lhlf*100 + rhlf;    //Put the two halves back together
	if (maxslewvel2 > 1000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset Iset to a default value.
		maxslewvel2 = 30;
	}
	maxslewvel2 = maxslewvel2/1000;

	//Read commandtopic
	commandtopic = EEPROM.read(addr_commandtopic);
	if (commandtopic == 255){
		commandtopic = 1;		//0 = joint_commands, 1 = cmd_vel
	}	

	//Read i2caddress
	i2caddress = EEPROM.read(addr_i2caddress);
	if (i2caddress == 255){
		i2caddress = 78;		//Address is something between 3 and 119. Default picked randomly.
	}	

	//Read enctopic
	iholder = EEPROM.read(addr_enctopic);
	if (iholder == 255 || iholder == 1){enctopic[7] = '1';}
	else if (iholder == 2){enctopic[7] = '2';}
	else if (iholder == 3){enctopic[7] = '3';}
	else if (iholder == 4){enctopic[7] = '4';}
	else if (iholder == 5){enctopic[7] = '5';}
	else if (iholder == 6){enctopic[7] = '6';}
	else if (iholder == 7){enctopic[7] = '7';}
	else if (iholder == 8){enctopic[7] = '8';}
	else if (iholder == 9){enctopic[7] = '9';}
}

void DisplaySettings(){
	CheckConfig();		//Read EEPROM
	Serial.print(F("I2C address: "));
	Serial.print(i2caddress);
	Serial.print(F(" [= 0x"));
	Serial.print(i2caddress, HEX);	//also display as a hex value
	Serial.println(F("]"));
	Serial.println(F(" "));
	if (commandtopic == 0){
		Serial.println(F("USB/ROS topic for command input: /joint_commands"));
	}
	else if (commandtopic == 1){
		Serial.println(F("USB/ROS topic for command input: /cmd_vel"));
		Serial.print(F("Wheel base scale factor: "));
		Serial.println(wheelbasescale);
		Serial.print(F("Wheel diameter scale factor: "));
		Serial.println(wheeldiamscale);
	}

	delay(1000);
	Serial.println(F(""));
	Serial.println(F("Channel 1 Settings:"));
	if (commandtopic == 1) {
		if (commandmode1 == 0){
			Serial.println(F("   I2C/PWM command mode: VELOCITY"));
		}
		else if (commandmode1 == 1){
			Serial.println(F("   I2C/PWM command mode: POSITION"));
		}
		else if (commandmode1 == 2){
			Serial.println(F("   I2C/PWM command mode: SERVO"));
		}
		Serial.println(F("   USB/ROS command mode: DIFFERENTIAL DRIVE"));
	}
	else {
		if (commandmode1 == 0){
			Serial.println(F("   I2C/PWM/ROS command mode: VELOCITY"));
		}
		else if (commandmode1 == 1){
			Serial.println(F("   I2C/PWM/ROS command mode: POSITION"));
		}
		else if (commandmode1 == 2){
			Serial.println(F("   I2C/PWM/ROS command mode: SERVO"));
		}
	}
	if (sensortype1 == 0){
		Serial.println(F("   Sensor type: NONE"));
	}
	else if (sensortype1 == 1){
		Serial.println(F("   Sensor type: ENCODER"));
		Serial.print(F("   USB/ROS Topic for encoder ticks: "));
		Serial.println(enctopic);
	}
	else if (sensortype1 == 2){
		Serial.println(F("   Sensor type: HALL"));
		Serial.print(F("   USB/ROS Topic for hall ticks: "));
		Serial.println(enctopic);
	}	

	fholder = maxIset1/1000;		//Convert mA to A	
	Serial.print(F("   Current: "));	
	Serial.print(fholder);
	Serial.println(F("A"));
	fholder = minIset1/1000;		//Convert mA to A	
	Serial.print(F("   Rest current: "));			
	Serial.print(fholder);
	Serial.println(F("A"));

	if (dir1 == 0){
		Serial.println(F("   Directionality: NORMAL"));
	}
	else if (dir1 == 1){
		Serial.println(F("   Directionality: REVERSE"));
	}

	Serial.print(F("   PWM command gain: "));
	Serial.println(gain1);

	Serial.print(F("   PWM center offset: "));
	Serial.println(pwmoffset1);

	Serial.print(F("   Position/servo acceleration: "));
	Serial.println(accel1);

	fholder = maxslewvel1*1000;
	Serial.print(F("   Position/servo max slew rate: "));
	Serial.println(fholder);
	
	if (torqueprofile1 == 0){
		Serial.println(F("   Torque smoothing: OFF"));
	}
	else if (torqueprofile1 == 1){
		Serial.println(F("   Torque smoothing: ON"));
		Serial.print(F("   Torque smoothing aggressiveness: "));
		Serial.println(tscoeff1);
		Serial.print(F("   Torque smoothing phase: "));
		fholder = tsphase1/1000;		//Convert to decimal form
		Serial.println(fholder);
	}
	delay(2000);
	Serial.println(F(""));
	Serial.println(F("Channel 2 Settings:"));
	if (commandtopic == 1){
		if (commandmode2 == 0){
			Serial.println(F("   I2C/PWM command mode: VELOCITY"));
		}
		else if (commandmode2 == 1){
			Serial.println(F("   I2C/PWM command mode: POSITION"));
		}
		else if (commandmode2 == 2){
			Serial.println(F("   I2C/PWM command mode: SERVO"));
		}
		Serial.println(F("   USB/ROS command mode: DIFFERENTIAL DRIVE"));
	}
	else {
		if (commandmode2 == 0){
			Serial.println(F("   I2C/PWM/ROS command mode: VELOCITY"));
		}
		else if (commandmode2 == 1){
			Serial.println(F("   I2C/PWM/ROS command mode: POSITION"));
		}
		else if (commandmode2 == 2){
			Serial.println(F("   I2C/PWM/ROS command mode: SERVO"));
		}
	}
	if (sensortype2 == 0){
		Serial.println(F("   Sensor type: NONE"));
	}
	else if (sensortype2 == 1){
		Serial.println(F("   Sensor type: ENCODER"));
		Serial.print(F("   Topic for encoder ticks: "));
		Serial.println(enctopic);
	}
	else if (sensortype2 == 2){
		Serial.println(F("   Sensor type: HALL"));
		Serial.print(F("   Topic for hall ticks: "));
		Serial.println(enctopic);
	}	

	fholder = maxIset2/1000;		//Convert mA to A	
	Serial.print(F("   Current: "));	
	Serial.print(fholder);
	Serial.println(F("A"));
	fholder = minIset2/1000;		//Convert mA to A	
	Serial.print(F("   Rest current: "));			
	Serial.print(fholder);
	Serial.println(F("A"));

	if (dir2 == 0){
		Serial.println(F("   Directionality: NORMAL"));
	}
	else if (dir2 == 1){
		Serial.println(F("   Directionality: REVERSE"));
	}

	Serial.print(F("   PWM command gain: "));
	Serial.println(gain2);

	Serial.print(F("   PWM center offset: "));
	Serial.println(pwmoffset2);

	Serial.print(F("   Position/servo acceleration: "));
	Serial.println(accel2);

	fholder = maxslewvel2*1000;
	Serial.print(F("   Position/servo max slew rate: "));
	Serial.println(fholder);
	
	if (torqueprofile2 == 0){
		Serial.println(F("   Torque smoothing: OFF"));
	}
	else if (torqueprofile2 == 1){
		Serial.println(F("   Torque smoothing: ON"));
		Serial.print(F("   Torque smoothing aggressiveness: "));
		Serial.println(tscoeff2);
		Serial.print(F("   Torque smoothing phase: "));
		fholder = tsphase2/1000;		//Convert to decimal form
		Serial.println(fholder);
	}
	Serial.println(F("\n\n\n\n"));
}

void TorqueCal1(){ //Drive the motor a certain amount 
	float dummyphaseindex = 0;	
	digitalWrite(U1_En, HIGH);    
	digitalWrite(V1_En, HIGH);
	digitalWrite(W1_En, HIGH);	
	vel1 = .003;
	do {
		dummyphaseindex = dummyphaseindex + vel1;
		Roll1_vel();
	}
	while(dummyphaseindex < 15*pi);
	digitalWrite(U1_En, LOW);    
	digitalWrite(V1_En, LOW);
	digitalWrite(W1_En, LOW);
}   

void TorqueCal2(){ //Drive the motor a certain amount 
	float dummyphaseindex = 0;	
	digitalWrite(U2_En, HIGH);    
	digitalWrite(V2_En, HIGH);
	digitalWrite(W2_En, HIGH);	
	vel2 = .003;
	do {
		dummyphaseindex = dummyphaseindex + vel2;
		Roll2_vel();
	}
	while(dummyphaseindex < 15*pi);
	digitalWrite(U2_En, LOW);    
	digitalWrite(V2_En, LOW);
	digitalWrite(W2_En, LOW);
}   

void adu2amps(){		//Convert current sense value from ADUs to AMPERES.
	I_A = ((-1*I_adu_av+109)/-70)+.05;    //Convert raw current to Amps. Empirically derived. 
}

/*void PubI(){
	I_adu_sum = I_adu_sum + analogRead(Isense);
	I_adu_av = (I_adu_av + I_adu_sum)/i;
	I_A = (-1*I_adu_av+109)/-81.92;    //Convert raw current to Amps. Offset derrived empirically.   
	i++;  //Increment counter.
	if (i == 100){     //When the averaging cycle has been completed (as set by value of i here), publish the idraw message and reset the index.
		idraw_msg.data = I_A;
		pub_1.publish(&idraw_msg);
		I_adu_sum = 0;
		I_adu_av = 0;
		i = 1;
	}
}*/

int SetPower(){
	int i = 0;    //A counter for determining rate of current rise 
	digitalWrite(13, HIGH);
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
	if (digitalRead(mode_config) == LOW){	
		//Serial.print(F("\nDetected quiescent current (A): "));
		//Serial.println(I_qui);
		//Serial.print(F("\n"));
	}
	delay(2000);
	digitalWrite(U1_En, HIGH);    //Enable each phase
	digitalWrite(V1_En, HIGH);
	digitalWrite(W1_En, HIGH);
	digitalWrite(U2_En, HIGH);
	digitalWrite(V2_En, HIGH);
	digitalWrite(W2_En, HIGH);

	//Ramp up power on motor 1 until desired current is detected.
	i = 0;
	power1_low = 0;
	do{
		power1 = power1 + 1;    //Ramp up the power with each iteration of this do-while loop.

		//WRITE OUTPUTS
		dutyU1 = (255/2)+(power1*(sin(phaseindex1)));	   //The duty cycle varies with the sine function, which has output between -1 and 1. That is scaled by an amplitude variable, which effectively sets the motor power.This is all offset by half the maximum duty cycle so that the lowest instantaeous duty cycle is always positive.
		analogWrite(U1_High,dutyU1);	//Write to the PWM pins.
		dutyV1 = (255/2)+(power1*(sin(phaseindex1+(2*pi/3))));
		analogWrite(V1_High,dutyV1);
		dutyW1 = (255/2)+(power1*(sin(phaseindex1+(4*pi/3))));
		analogWrite(W1_High,dutyW1);

		delay(5);   //Some settling time is needed for accuracy.
		I_adu_sum = 0;
		I_adu_av = 0;
		for (int i = 1; i <= 200; i++) {    //Check current draw. Sum and average number i times.
			I_adu_sum = I_adu_sum + analogRead(Isense);
			I_adu_av = (I_adu_av + I_adu_sum)/i;  
		}  
		adu2amps();		
		if (I_A > 4000) {
			exitflag = true;
			goto motor1error;
		}

		if ((i == 50) && (I_A < (I_qui+.02))){   //After several iterations, see if the current is still near the quiescent draw.
			power1 = 0;
			digitalWrite(U1_En, LOW);    //Disable each phase
			digitalWrite(V1_En, LOW); 
			digitalWrite(W1_En, LOW);
			break;
		}
		if (I_A < (((minIset1)/1000)+I_qui)){	//Every iteration that the current is less than minIset, save the power to powerx_low. When the routine is completed, this assures that power1_low will contain the proper value.
			power1_low = power1;
		}
		if (I_A < ((200.00/1000)+I_qui)){	//Also detect another power level for doing the hall self-calibration.
			power1_diag = power1;
		}
		i++;  
	} while (I_A < (((maxIset1)/1000)+I_qui)-.04); //As soon as I_A exceeds the user-determined current in amps, exit the loop, thereby maintaining the last power setting.
	if (digitalRead(mode_ros) == LOW){
	}
	else {
		if (power1 == 0){
			Serial.println(F("Motor 1 not connected."));
		}
		else {
			Serial.print(F("\nMotor 1 max/fixed current was set to "));
			Serial.print(I_A);	
			Serial.println(F("A (as measured).\n"));
		}	
	}
	//delay(3000);		//Uncomment for diagnostic reasons (gives time to check displayed value against a meter)
	power1_high = power1;
	power1 = 0;
	//Write the new power setting
	dutyU1 = (255/2)+(power1*(sin(phaseindex1)));	   
	analogWrite(U1_High,dutyU1);	//Write to the PWM pins.
	dutyV1 = (255/2)+(power1*(sin(phaseindex1+(2*pi/3))));
	analogWrite(V1_High,dutyV1);
	dutyW1 = (255/2)+(power1*(sin(phaseindex1+(4*pi/3))));
	analogWrite(W1_High,dutyW1);
	motor1error:
	if (exitflag == true) {
		if (digitalRead(mode_config) == LOW){
			Serial.print(F("\nERROR setting motor 1 current. Recommend cycling power to board."));
			power1_high = 0;
			power1 = 0;
		}
	}
  
	//Ramp up power on motor 2 until desired current is detected.
	i = 0;
	power2_low = 0;
	do{
		power2 = power2 + 1;    //Ramp up the power with each iteration of this do-while loop.

		//WRITE OUTPUTS
		dutyU2 = (255/2)+(power2*(sin(phaseindex2)));	   //The duty cycle varies with the sine function, which has output between -1 and 1. That is scaled by an amplitude variable, which effectively sets the motor power.This is all offset by half the maximum duty cycle so that the lowest instantaeous duty cycle is always positive.
		analogWrite(U2_High,dutyU2);	//Write to the PWM pins.
		dutyV2 = (255/2)+(power2*(sin(phaseindex2+(2*pi/3))));
		analogWrite(V2_High,dutyV2);
		dutyW2 = (255/2)+(power2*(sin(phaseindex2+(4*pi/3))));
		analogWrite(W2_High,dutyW2);

		delay(5);   //Some settling time is needed for accuracy.
		I_adu_sum = 0;
		I_adu_av = 0;
		for (int i = 1; i <= 200; i++) {    //Check current draw. Sum and average number i times.
			I_adu_sum = I_adu_sum + analogRead(Isense);
			I_adu_av = (I_adu_av + I_adu_sum)/i;  
		}  
		adu2amps();		
		if (I_A > 4000) {
			exitflag = true;
			goto motor2error;
		}

		if ((i == 50) && (I_A < (I_qui+.02))){   //After several iterations, see if the current is still near the quiescent draw.
			power2 = 0;
			digitalWrite(U2_En, LOW);    //Disable each phase
			digitalWrite(V2_En, LOW); 
			digitalWrite(W2_En, LOW);
			break;
		}
		if (I_A < (((minIset2)/1000)+I_qui)){	//Every iteration that the current is less than minIset, save the power to powerx_low. When the routine is completed, this assures that power1_low will contain the proper value.
			power2_low = power2;
		}
		if (I_A < ((200.00/1000)+I_qui)){	//Also detect another power level for doing the hall self-calibration.
			power2_diag = power2;
		}
		i++;  
	} while (I_A < (((maxIset2)/1000)+I_qui)-.04); //As soon as I_A exceeds the user-determined current in amps, exit the loop, thereby maintaining the last power setting.
	if (digitalRead(mode_ros) == LOW){
	}
	else {
		if (power2 == 0){
			Serial.println(F("Motor 2 not connected."));
		}
		else {
			Serial.print(F("Motor 2 max/fixed current was set to "));
			Serial.print(I_A);	
			Serial.println(F("A (as measured).\n"));
		}	
	}
	//delay(3000);		//Uncomment for diagnostic reaons (gives time to check displayed value against a meter)
	power2_high = power2;
	power1 = power1_high;    //Re-energize motor 1. Motor 2 is already powered at this point.
	motor2error:
	if (exitflag == true) {
		if (digitalRead(mode_config) == LOW){
			Serial.print(F("\nERROR setting motor 2 current. Recommend cycling power to board."));
			power2_high = 0;
			power2 = 0;
		}
	}
	digitalWrite(U1_En, HIGH);    //Re-enable channel 1
	digitalWrite(V1_En, HIGH); 
	digitalWrite(W1_En, HIGH);
	digitalWrite(13,LOW);  
}

void ReadEnc1(){
	if (digitalRead(enc1_pin3) == LOW && digitalRead(enc1_pin4) == LOW && enc1_state != 0){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc1_state = 0;
		//digitalWrite(13, HIGH); 		//DEBUG
		Wire.write(enc1_state); 
		ticksarray[0] = enc1_state;
		ticks_msg.data = ticksarray;
		pub_2.publish(&ticks_msg);
	}
	else if (digitalRead(enc1_pin3) == HIGH && digitalRead(enc1_pin4) == LOW && enc1_state != 1){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc1_state = 1;
		//digitalWrite(13, LOW); 		//DEBUG
		Wire.write(enc1_state); 
		ticksarray[0] = enc1_state;
		ticks_msg.data = ticksarray;
		pub_2.publish(&ticks_msg);
	}
	else if (digitalRead(enc1_pin3) == HIGH && digitalRead(enc1_pin4) == HIGH && enc1_state != 2){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc1_state = 2;
		//digitalWrite(13, HIGH); 		//DEBUG
		Wire.write(enc1_state); 
		ticksarray[0] = enc1_state;
		ticks_msg.data = ticksarray;
		pub_2.publish(&ticks_msg);
	}
	else if (digitalRead(enc1_pin3) == LOW && digitalRead(enc1_pin4) == HIGH && enc1_state != 3){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc1_state = 3;
		//digitalWrite(13, LOW); 		//DEBUG
		Wire.write(enc1_state); 
		ticksarray[0] = enc1_state;
		ticks_msg.data = ticksarray;
		pub_2.publish(&ticks_msg);
	}
	ticks_msg.data_length = 2;
}

void ReadHall1(){		
	if (digitalRead(enc1_pin3) == HIGH && digitalRead(enc1_pin4) == LOW && digitalRead(enc1_pin5) == LOW && enc1_state != 0){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc1_state = 0;
		ticksarray[0] = enc1_state;
		ticks_msg.data = ticksarray;
		pub_2.publish(&ticks_msg);
	}
	else if (digitalRead(enc1_pin4) == HIGH && digitalRead(enc1_pin3) == LOW && digitalRead(enc1_pin5) == LOW && enc1_state != 1){	
		enc1_state = 1;
		ticksarray[0] = enc1_state;
		ticks_msg.data = ticksarray;
		pub_2.publish(&ticks_msg);
	}
	else if (digitalRead(enc1_pin5) == HIGH && digitalRead(enc1_pin3) == LOW && digitalRead(enc1_pin4) == LOW && enc1_state != 2){	
		enc1_state = 2;
		ticksarray[0] = enc1_state;
		ticks_msg.data = ticksarray;
		pub_2.publish(&ticks_msg);
	}
	ticks_msg.data_length = 2;
}

void ReadEnc2(){
	if (digitalRead(enc2_pin3) == LOW && digitalRead(enc2_pin4) == LOW && enc2_state != 0){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc2_state = 0;
		//digitalWrite(13, HIGH); 		//DEBUG
		ticksarray[1] = enc2_state;
		ticks_msg.data = ticksarray;
		pub_2.publish(&ticks_msg);
	}
	else if (digitalRead(enc2_pin3) == HIGH && digitalRead(enc2_pin4) == LOW && enc2_state != 1){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc2_state = 1;
		//digitalWrite(13, LOW); 		//DEBUG
		ticksarray[1] = enc2_state;
		ticks_msg.data = ticksarray;
		pub_2.publish(&ticks_msg);
	}
	else if (digitalRead(enc2_pin3) == HIGH && digitalRead(enc2_pin4) == HIGH && enc2_state != 2){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc2_state = 2;
		//digitalWrite(13, HIGH); 		//DEBUG
		ticksarray[1] = enc2_state;
		ticks_msg.data = ticksarray;
		pub_2.publish(&ticks_msg);
	}
	else if (digitalRead(enc2_pin3) == LOW && digitalRead(enc2_pin4) == HIGH && enc2_state != 3){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc2_state = 3;
		//digitalWrite(13, LOW); 		//DEBUG
		ticksarray[1] = enc2_state;
		ticks_msg.data = ticksarray;
		pub_2.publish(&ticks_msg);
	}
	ticks_msg.data_length = 2;
}

void ReadHall2(){		
	if (digitalRead(enc2_pin3) == HIGH && digitalRead(enc2_pin4) == LOW && digitalRead(enc2_pin5) == LOW && enc2_state != 0){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc2_state = 0;
		ticksarray[1] = enc2_state;
		ticks_msg.data = ticksarray;
		pub_2.publish(&ticks_msg);
	}
	else if (digitalRead(enc2_pin4) == HIGH && digitalRead(enc2_pin3) == LOW && digitalRead(enc2_pin5) == LOW && enc2_state != 1){	
		enc2_state = 1;
		ticksarray[1] = enc2_state;
		ticks_msg.data = ticksarray;
		pub_2.publish(&ticks_msg);
	}
	else if (digitalRead(enc2_pin5) == HIGH && digitalRead(enc2_pin3) == LOW && digitalRead(enc2_pin4) == LOW && enc2_state != 2){	
		enc2_state = 2;
		ticksarray[1] = enc2_state;
		ticks_msg.data = ticksarray;
		pub_2.publish(&ticks_msg);
	}
	ticks_msg.data_length = 2;
}

void CheckMode(){
	//********ROS**********
	if(digitalRead(mode_ros) == LOW){     //If in USB mode, get velocity data from the ROS node.
		if (commandtopic == 0){	//If subscribed to /joint_command...
			if (commandmode1 == 0){	//If in velocity input mode
				vel1 = vel1_holder;
			}
			else {	//If in position or servo input mode
				pos1 = pos1_holder;
			}
			if (commandmode2 == 0){	//If in velocity input mode
				vel2 = vel2_holder;
			}
			else {	//If in position or servo input mode
				pos2 = pos2_holder;
			}
		}
		else {	//If subcribed to /cmd_vel...			
			commandmode1 = 0;
			commandmode2 = 0;
			vel1 = (v_x*(float(wheeldiamscale)/1000))+(v_th*(float(wheelbasescale)/10000));		//Convert v_x and v_th to motor motion. The angular rate (V_th) is added for this motor... 
			vel2 = (v_x*(float(wheeldiamscale)/1000))-(v_th*(float(wheelbasescale)/10000));		//...and subtracted from this one.
		}
		//PubI();
		nh.spinOnce();    //Spin the node once (check for and process messages), hence retrieving values for v_x and v_th from the ROS. We use nh.spinOnce() instead of nh.spin() because this is already in a main loop and we want this thing to happen once every time through the loop.
		if (rosflag == false) {	//If we weren't already in ROS mode
			softReset();
		}
		pwmflag = false;
		rosflag = true;
	}

    //********I2C**********
	if(digitalRead(mode_i2c) == LOW){
		//Because this is handled entirely by an ISR, there's nothing here to set. The I2C "wire.begin" service is started in setup()
		pwmflag = false;
		rosflag = false;
	}

    //********Config**********
	while(digitalRead(mode_config) == LOW){     //Check for configuration mode
		digitalWrite(U1_En, LOW);    //Disable each phase
		digitalWrite(V1_En, LOW);
		digitalWrite(W1_En, LOW);
		digitalWrite(U2_En, LOW);
		digitalWrite(V2_En, LOW);
		digitalWrite(W2_En, LOW);
		SerialPrompt();   
		pwmflag = false;
		rosflag = false; 
	}

	//********PWM**********
	if((digitalRead(mode_ros) == HIGH) && (digitalRead(mode_i2c) == HIGH) && (digitalRead(mode_config) == HIGH))  {     //If none of the mode pins are LOW, read the PWM inputs to control each motor's velocity independently.
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

			vel1_starting = (pwm1_t-pwmoffset1)*.000002*gain1;	//Save the starting value of vel1 into vel1_starting
			vel2_starting = (pwm2_t-pwmoffset2)*.000002*gain2;	//Save the starting value of vel1 into vel1_starting
			pos1_starting = (pwm1_t-pwmoffset1)*.0002*gain1;
			pos2_starting = (pwm2_t-pwmoffset2)*.0002*gain2;
		}

		if(pwm1active == true){     //Only get velocity data from this PWM channel if it was found to be connected above    
			if (commandmode1 == 0){	//if in velocity mode
				vel1 = ((pwm1_t-pwmoffset1)*.000002*gain1) - vel1_starting;	//Normal PWM servo pulsewidth goes from 800 to 2200ms. We subtract 1500ms, corresponding to the middle of that range, so that pulses in the range 800-1499 will have negative velocity, and 1501-2200 will be positive. 
			}
			else {	//If in position or servo mode
				pos1 = (pwm1_t-pwmoffset1)*.0002*gain1 - pos1_starting;
			}
		}

		if(pwm2active == true){    //Only get velocity data from this PWM channel if it was found to be connected above
			if (commandmode2 == 0){
				vel2 = ((pwm2_t-pwmoffset2)*.000002*gain2) - vel2_starting;
			}
			else {	//If in position or servo mode
				pos2 = (pwm2_t-pwmoffset2)*.0002*gain2 - pos2_starting;
			}
		}
		pwmflag = true;
		rosflag = false;
	}
}

void receiveEvent (int nBytes) {		//Called by ISR when I2C data arrives
	for (int i = 0; i < nBytes; i++) {
		char c = Wire.read();
		i2crxints[i] = c - '0';	//Convert each char to an int
	}
	//i2crxints digits are as follows: 1 = channel number, 2 = sign, 3-7 = value
	vel = (.1*i2crxints[3])+(.01*i2crxints[4])+(.001*i2crxints[5])+(.0001*i2crxints[6])+(.00001*i2crxints[7]);
	if(i2crxints[2] == -5) {	//-5 means a positive vel or pos value
		vel = vel;
	}
	else if(i2crxints[2] == -3) {	//-3 means a negative vel or pos value
		vel = -1*vel;
	}
	pos = 1000*vel;
	if(i2crxints[1] == 1) {		//If channel 1
		vel1 = vel;
		pos1 = pos;
	}
	else if(i2crxints[1] == 2) {		//If channel 2
		vel2 = vel;
		pos2 = pos;
	}
}

void softReset() {
	asm volatile ("  jmp 0");
}
