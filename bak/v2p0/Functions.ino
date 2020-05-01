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

void ISR_I2Crx(int nBytes) {		//Called by ISR when I2C data arrives
	for (int i = 0; i < nBytes; i++) {
		char c = Wire.read();
		i2crxints[i] = c - '0';	//Convert each char to an int
	}
	//If channel = 1
	if(i2crxints[1] == 1) {
		//Check command mode
		if(i2crxints[2] == 70) {	//70 means v
			commandmode1 = 0;	//set to velocity mode
		}
		else if(i2crxints[2] == 64) {	//64 means p
			commandmode1 = 1;	//set to position mode
		}
		else if(i2crxints[2] == 67) {	//67 means s
			commandmode1 = 2;	//set to servo mode
		}

		//Check command value... i2crxints digits are as follows: 1 = channel number, 2 = sign, 3-7 = value
		vel1 = (1*i2crxints[4])+(.1*i2crxints[5])+(.01*i2crxints[6])+(.001*i2crxints[7])+(.0001*i2crxints[8]);
		if(i2crxints[3] == -5) {	//-5 means a positive vel or pos value
			vel1 = 1*vel1;
		}
		else if(i2crxints[3] == -3) {	//-3 means a negative vel or pos value
			vel1 = -1*vel1;
		}
		pos1 = 100*vel1;

		//Set current/ duty cycle
		if(i2crxints[9] == 51) {	//check that there's a 'c' here. If not don't set current.
			Iset1 = (10000*i2crxints[10])+(1000*i2crxints[11])+(100*i2crxints[12])+(10*i2crxints[13])+(1*i2crxints[14]);
			if (Iset1 > 3000){
				Iset1 = 3000;
			}
			else if (Iset1 < 0){
				Iset1 = 0;
			}
		}
		else {
			Iset1 = 0;
			Serial.println(F("MALFORMATTED COMMAND: No 'c' in string index 8."));
		}
		duty1 = m1*Iset1 + b1;
		if (Iset1 == 0){
			duty1 = 0;
		}
	}
	
	//If channel 2
	else if(i2crxints[1] == 2) {		
		//Check command mode
		if(i2crxints[2] == 70) {	//70 means v
			commandmode2 = 0;	//set to velocity mode
		}
		else if(i2crxints[2] == 64) {	//64 means p
			commandmode2 = 1;	//set to position mode
		}
		else if(i2crxints[2] == 67) {	//67 means s
			commandmode2 = 2;	//set to servo mode
		}

		//Check command value... i2crxints digits are as follows: 1 = channel number, 2 = sign, 3-7 = value
		vel2 = (1*i2crxints[4])+(.1*i2crxints[5])+(.01*i2crxints[6])+(.001*i2crxints[7])+(.0001*i2crxints[8]);
		if(i2crxints[3] == -5) {	//-5 means a positive vel or pos value
			vel2 = 1*vel2;
		}
		else if(i2crxints[3] == -3) {	//-3 means a negative vel or pos value
			vel2 = -1*vel2;
		}
		pos2 = 100*vel2;

		//Set current/ duty cycle
		if(i2crxints[9] == 51) {	//check that there's a 'c' here. If not don't set current.
			Iset2 = (10000*i2crxints[10])+(1000*i2crxints[11])+(100*i2crxints[12])+(10*i2crxints[13])+(1*i2crxints[14]);
			if (Iset2 > 3000){
				Iset2 = 3000;
			}
			else if (Iset2 < 0){
				Iset2 = 0;
			}
		}
		else {
			Iset2 = 0;
			Serial.println(F("MALFORMATTED COMMAND: No 'c' in string index 8."));
		}
		duty2 = m2*Iset2 + b2;
		if (Iset2 == 0){
			duty2 = 0;
		}
	}
}

void ISR_I2Ctx() {		//Called by ISR when I2C data is requested
	Wire.write(tickstx);	
}

void CheckConfig(){

	//Read maxIset1
	lhlf = EEPROM.read(addr_maxIset1l);  
	rhlf = EEPROM.read(addr_maxIset1r);  
	maxIset1 = lhlf*100 + rhlf;    //Put the two halves back together
	if (maxIset1 > 1000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset Iset to a default value.
		maxIset1 = 300;
	}
	//Read maxIset2
	lhlf = EEPROM.read(addr_maxIset2l);  
	rhlf = EEPROM.read(addr_maxIset2r);  
	maxIset2 = lhlf*100 + rhlf;    //Put the two halves back together
	if (maxIset2 > 1000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset Iset to a default value.
		maxIset2 = 300;
	}
	//Read PWM minIset1
	lhlf = EEPROM.read(addr_minIset1l);  
	rhlf = EEPROM.read(addr_minIset1r);  
	minIset1 = lhlf*100 + rhlf;    //Put the two halves back together
	if (minIset1 > 1000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset Iset to default value.
		minIset1 = 200;	
	}
	//Read PWM minIset2
	lhlf = EEPROM.read(addr_minIset2l);  
	rhlf = EEPROM.read(addr_minIset2r);  
	minIset2 = lhlf*100 + rhlf;    //Put the two halves back together
	if (minIset2 > 1000){	//If the number is bigger than our maximum, assume that the initial current configuration was never done and reset Iset to default value.
		minIset2 = 200;	
	}
	//Read PWM gain1
	lhlf = EEPROM.read(addr_gain1l);  
	rhlf = EEPROM.read(addr_gain1r);
	gain1 = lhlf*100+rhlf;
	if (gain1 > 10000){	//If the number is bigger than our maximum, assume that the initial gain configuration was never done and reset gain to a default value.
		gain1 = 60;
	}
	//Read PWM gain2
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
		pwmoffset1 = 1500;	//0 for auto. Default = 1500
	}
	//Read pwmoffset2
	lhlf = EEPROM.read(addr_pwmoffset2l);  
	rhlf = EEPROM.read(addr_pwmoffset2r);
	pwmoffset2 = lhlf*100+rhlf;
	if (pwmoffset2 > 10000){	//If the number is bigger than our maximum, assume that the initial gain configuration was never done and reset gain to a default value.
		pwmoffset2 = 1500;	//0 for auto. Default = 1500
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
	//Read PWM commandmode1
	commandmode1 = EEPROM.read(addr_commandmode1);
	if (commandmode1 == 255){
		commandmode1 = 0;		//0 = VELOCITY control, 1 = POSITION control, 2 = SERVO control
	}
	//Read PWM commandmode2
	commandmode2 = EEPROM.read(addr_commandmode2);
	if (commandmode2 == 255){
		commandmode2 = 0;		//0 = VELOCITY control, 1 = POSITION control, 2 = SERVO control
	}
	//Read sensortype1
	sensortype1 = EEPROM.read(addr_sensortype1);
	if (sensortype1 == 255){
		sensortype1 = 0;		//0 = ENCODER, 1 = HALL
	}
	//Read sensortype2
	sensortype2 = EEPROM.read(addr_sensortype2);
	if (sensortype2 == 255){
		sensortype2 = 0;		//0 = ENCODER, 1 = HALL
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

	//Read i2caddress
	i2caddress = EEPROM.read(addr_i2caddress);
	if (i2caddress == 255){
		i2caddress = 64;		//Address is something between 3 and 119. Default picked randomly.
	}	
}

void DisplaySettings(){
	CheckConfig();		//Read EEPROM
	Serial.print(F("I2C address: "));
	Serial.print(i2caddress);
	Serial.print(F(" [= 0x"));
	Serial.print(i2caddress, HEX);	//also display as a hex value
	Serial.println(F("]"));
	Serial.println(F(" "));
	delay(1000);
	Serial.println(F("Channel 1 Settings:"));
	if (sensortype1 == 0){
		Serial.println(F("   Sensor type: ENCODER"));
	}
	else if (sensortype1 == 1){
		Serial.println(F("   Sensor type: HALL"));
	}

	fholder = maxslewvel1*1000;
	Serial.print(F("   Position/servo max slew rate: "));
	Serial.println(fholder);

	Serial.print(F("   Position/servo acceleration: "));
	Serial.println(accel1);

	if (dir1 == 0){
		Serial.println(F("   Directionality: NORMAL"));
	}
	else if (dir1 == 1){
		Serial.println(F("   Directionality: REVERSE"));
	}

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

	fholder = maxIset1/1000;		//Convert mA to A	
	Serial.print(F("   PWM Running Current: "));	
	Serial.print(fholder);
	Serial.println(F("A"));
	fholder = minIset1/1000;		//Convert mA to A	
	Serial.print(F("   PWM Rest current: "));			
	Serial.print(fholder);
	Serial.println(F("A"));
	if (commandmode1 == 0){
		Serial.println(F("   PWM command mode: VELOCITY"));
	}
	else if (commandmode1 == 1){
		Serial.println(F("   PWM command mode: POSITION"));
	}
	else if (commandmode1 == 2){
		Serial.println(F("   PWM command mode: SERVO"));
	}

	Serial.print(F("   PWM command gain: "));
	Serial.println(gain1);
	Serial.print(F("   PWM center offset: "));
	if (pwmoffset1 == 0){
		Serial.println("AUTO");
	}
	else {
		Serial.println(pwmoffset1);
	}

	delay(2000);
	Serial.println(F(""));
	Serial.println(F("Channel 2 Settings:"));
	if (sensortype2 == 0){
		Serial.println(F("   Sensor type: ENCODER"));
	}
	else if (sensortype2 == 1){
		Serial.println(F("   Sensor type: HALL"));
	}
	fholder = maxslewvel2*1000;
	Serial.print(F("   Position/servo max slew rate: "));
	Serial.println(fholder);
	Serial.print(F("   Position/servo acceleration: "));
	Serial.println(accel2);

	if (dir2 == 0){
		Serial.println(F("   Directionality: NORMAL"));
	}
	else if (dir2 == 1){
		Serial.println(F("   Directionality: REVERSE"));
	}
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

	
	fholder = maxIset2/1000;		//Convert mA to A	
	Serial.print(F("   PWM Running Current: "));	
	Serial.print(fholder);
	Serial.println(F("A"));
	fholder = minIset2/1000;		//Convert mA to A	
	Serial.print(F("   PWM Rest current: "));			
	Serial.print(fholder);
	Serial.println(F("A"));
	if (commandmode2 == 0){
		Serial.println(F("   PWM command mode: VELOCITY"));
	}
	else if (commandmode2 == 1){
		Serial.println(F("   PWM command mode: POSITION"));
	}
	else if (commandmode2 == 2){
		Serial.println(F("   PWM command mode: SERVO"));
	}
	Serial.print(F("   PWM command gain: "));
	Serial.println(gain2);

	Serial.print(F("   PWM center offset: "));
	if (pwmoffset1 == 0){
		Serial.println("AUTO");
	}
	else {
		Serial.println(pwmoffset1);
	}

	Serial.println(F("\n\n\n\n"));
}

void TorqueCal1(){ 		//Drive the motor a certain amount 
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

int CalDuty(){
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
	delay(2000);
	//Enable each ch 1 phase
	digitalWrite(U1_En, HIGH);   
	digitalWrite(V1_En, HIGH);
	digitalWrite(W1_En, HIGH);

	//Ramp up duty cycle on motor 1 until desired current is detected.
	i = 0;
	dutycalpoint_low1 = 0;
	do{
		duty1 = duty1 + 1;    //Ramp up the duty cycle with each iteration of this do-while loop.

		//WRITE OUTPUTS
		dutyU1 = (255/2)+(duty1*(sin(phaseindex1)));	   //The duty cycle varies with the sine function, which has output between -1 and 1. That is scaled by an amplitude variable, which effectively sets the motor power.This is all offset by half the maximum duty cycle so that the lowest instantaeous duty cycle is always positive.
		analogWrite(U1_High,dutyU1);	//Write to the PWM pins.
		dutyV1 = (255/2)+(duty1*(sin(phaseindex1+(2*pi/3))));
		analogWrite(V1_High,dutyV1);
		dutyW1 = (255/2)+(duty1*(sin(phaseindex1+(4*pi/3))));
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
			duty1 = 0;
			digitalWrite(U1_En, LOW);    //Disable each phase
			digitalWrite(V1_En, LOW); 
			digitalWrite(W1_En, LOW);
			break;
		}
		if (I_A < (.1 + I_qui)){	//Every iteration that the current is less than 200mA, save the duty cycle to dutycalpoint_low. When the routine is completed, this assures that dutycalpoint_low1 will contain the proper value.
			dutycalpoint_low1 = duty1;
		}
		i++;  
	} while (I_A < (.5 + I_qui)-.04); //As soon as I_A exceeds the user-determined current in amps, exit the loop, thereby maintaining the last duty cycle setting.
	dutycalpoint_high1 = duty1; //Now dutycalpoint_low1 = duty cycle corresponding to .1A and dutycalpoint_high1 = duty cycle corresponding to .5A

	if (duty1 == 0){
		Serial.println(F("Motor 1 not connected."));
	}
	else {
		Serial.println(F("Motor 1 current/power calibration complete.\n"));
		Serial.print(F("100mA duty cycle calibration value: "));
		Serial.println(dutycalpoint_low1);
		Serial.print(F("500mA duty cycle calibration value: "));
		Serial.println(dutycalpoint_high1);
		Serial.print(F("\n\n"));
	}	
	motor1error:
	if (exitflag == true) {
		Serial.print(F("\nERROR setting motor 1 current. Recommend cycling power to board."));
		dutycalpoint_high1 = 0;
		duty1 = 0;
	}
	//Calculate line parameters for current to duty cycle conversion
	m1 = (dutycalpoint_high1-dutycalpoint_low1)/400.00;
	b1 = dutycalpoint_low1 - (m1*100.00);

	duty1 = 0;

	//Write the new duty setting
	dutyU1 = (255/2)+(duty1*(sin(phaseindex1)));	   
	analogWrite(U1_High,dutyU1);	//Write to the PWM pins.
	dutyV1 = (255/2)+(duty1*(sin(phaseindex1+(2*pi/3))));
	analogWrite(V1_High,dutyV1);
	dutyW1 = (255/2)+(duty1*(sin(phaseindex1+(4*pi/3))));
	analogWrite(W1_High,dutyW1);

	//Disable each ch 1 phase
	digitalWrite(U1_En, LOW);    
	digitalWrite(V1_En, LOW);
	digitalWrite(W1_En, LOW);

	//Enable each ch 2 phase
	digitalWrite(U2_En, HIGH);    
	digitalWrite(V2_En, HIGH);
	digitalWrite(W2_En, HIGH);
  
	//Ramp up duty cycle on motor 2 until desired current is detected.
	i = 0;
	dutycalpoint_low2 = 0;
	do{
		duty2 = duty2 + 1;    //Ramp up the duty cycle with each iteration of this do-while loop.

		//WRITE OUTPUTS
		dutyU2 = (255/2)+(duty2*(sin(phaseindex2)));	   //The duty cycle varies with the sine function, which has output between -1 and 1. That is scaled by an amplitude variable, which effectively sets the motor power.This is all offset by half the maximum duty cycle so that the lowest instantaeous duty cycle is always positive.
		analogWrite(U2_High,dutyU2);	//Write to the PWM pins.
		dutyV2 = (255/2)+(duty2*(sin(phaseindex2+(2*pi/3))));
		analogWrite(V2_High,dutyV2);
		dutyW2 = (255/2)+(duty2*(sin(phaseindex2+(4*pi/3))));
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
			duty2 = 0;
			digitalWrite(U2_En, LOW);    //Disable each phase
			digitalWrite(V2_En, LOW); 
			digitalWrite(W2_En, LOW);
			break;
		}
		if (I_A < (.1 + I_qui)){	//Every iteration that the current is less than minIset, save the duty to dutycalpoint_low2. When the routine is completed, this assures that dutycalpoint_low1 will contain the proper value.
			dutycalpoint_low2 = duty2;
		}
		i++;  
	} while (I_A < (.5 + I_qui)-.04); //As soon as I_A exceeds the user-determined current in amps, exit the loop, thereby maintaining the last duty cycle setting.
	//delay(3000);		//Uncomment for diagnostic reaons (gives time to check displayed value against a meter)
	dutycalpoint_high2 = duty2;
	if (duty2 == 0){
		Serial.println(F("Motor 2 not connected."));
	}
	else {
		Serial.println(F("Motor 2 current/power calibration complete.\n"));
		Serial.print(F("100mA duty cycle calibration value: "));
		Serial.println(dutycalpoint_low2);
		Serial.print(F("500mA duty cycle calibration value: "));
		Serial.println(dutycalpoint_high2);
		Serial.print(F("\n\n"));
	}	
	motor2error:
	if (exitflag == true) {
		Serial.print(F("\nERROR setting motor 2 current. Recommend cycling power to board."));
		dutycalpoint_high2 = 0;
		duty2 = 0;
	}

	m2 = (dutycalpoint_high2-dutycalpoint_low2)/400.00;
	b2 = dutycalpoint_low2 - (m2*100.00);

	duty2 = 0;

	//Write the new duty setting
	dutyU2 = (255/2)+(duty2*(sin(phaseindex2)));	   
	analogWrite(U2_High,dutyU2);	//Write to the PWM pins.
	dutyV2 = (255/2)+(duty2*(sin(phaseindex2+(2*pi/3))));
	analogWrite(V2_High,dutyV2);
	dutyW2 = (255/2)+(duty2*(sin(phaseindex2+(4*pi/3))));
	analogWrite(W2_High,dutyW2);

	 //Re-enable each ch 1 phase
	digitalWrite(U1_En, HIGH);   
	digitalWrite(V1_En, HIGH); 
	digitalWrite(W1_En, HIGH);
	digitalWrite(13,LOW);  
}

void ReadEnc1(){
	if (digitalRead(enc1_pin3) == LOW && digitalRead(enc1_pin4) == LOW && enc1_state != 0){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc1_state = 0;
	}
	else if (digitalRead(enc1_pin3) == HIGH && digitalRead(enc1_pin4) == LOW && enc1_state != 1){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc1_state = 1;
	}
	else if (digitalRead(enc1_pin3) == HIGH && digitalRead(enc1_pin4) == HIGH && enc1_state != 2){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc1_state = 2;
	}
	else if (digitalRead(enc1_pin3) == LOW && digitalRead(enc1_pin4) == HIGH && enc1_state != 3){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc1_state = 3;
	}
	
	//ACCUMULATE ABSOLUTE TICKS USING THE QUADRATURE TICKS
	if (enc1_state == 0 && enc1_laststate == 3){
		enc1_absticks = enc1_absticks + encstep;		//Increment the absolute tracking counter
		enc1_laststate = enc1_state;
	}
	else if (enc1_state == 3 && enc1_laststate == 0){
		enc1_absticks = enc1_absticks - encstep;		//decrement the absolute tracking counter
		enc1_laststate = enc1_state;
	}
	else {
		enc1_absticks = enc1_absticks + (encstep*(enc1_state - enc1_laststate));		//Increment or decrement the absolute tracking counter depending on how big of a change was detected in the encoder state
		enc1_laststate = enc1_state;
	}		
}

void ReadHall1(){		
	if (digitalRead(enc1_pin3) == HIGH && digitalRead(enc1_pin4) == LOW && digitalRead(enc1_pin5) == LOW && enc1_state != 0){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc1_state = 0;
	}
	else if (digitalRead(enc1_pin4) == HIGH && digitalRead(enc1_pin3) == LOW && digitalRead(enc1_pin5) == LOW && enc1_state != 1){	
		enc1_state = 1;
	}
	else if (digitalRead(enc1_pin5) == HIGH && digitalRead(enc1_pin3) == LOW && digitalRead(enc1_pin4) == LOW && enc1_state != 2){	
		enc1_state = 2;
	}

	//ACCUMULATE ABSOLUTE TICKS USING THE HALL TICKS
	if (enc1_state == 0 && enc1_laststate == 2){
		enc1_absticks = enc1_absticks + encstep;		//Increment the absolute tracking counter
		enc1_laststate = enc1_state;
	}
	else if (enc1_state == 2 && enc1_laststate == 0){
		enc1_absticks = enc1_absticks - encstep;		//decrement the absolute tracking counter
		enc1_laststate = enc1_state;
	}
	else {
		enc1_absticks = enc1_absticks + (encstep*(enc1_state - enc1_laststate));		//Increment or decrement the absolute tracking counter depending on how big of a change was detected in the encoder state
		enc1_laststate = enc1_state;
	}		
}

void ReadEnc2(){
	if (digitalRead(enc2_pin3) == LOW && digitalRead(enc2_pin4) == LOW && enc2_state != 0){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc2_state = 0;
		//digitalWrite(13, HIGH); 		//DEBUG
	}
	else if (digitalRead(enc2_pin3) == HIGH && digitalRead(enc2_pin4) == LOW && enc2_state != 1){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc2_state = 1;
		//digitalWrite(13, LOW); 		//DEBUG
	}
	else if (digitalRead(enc2_pin3) == HIGH && digitalRead(enc2_pin4) == HIGH && enc2_state != 2){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc2_state = 2;
		//digitalWrite(13, HIGH); 		//DEBUG
	}
	else if (digitalRead(enc2_pin3) == LOW && digitalRead(enc2_pin4) == HIGH && enc2_state != 3){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc2_state = 3;
		//digitalWrite(13, LOW); 		//DEBUG
	}

	//ACCUMULATE ABSOLUTE TICKS USING THE QUADRATURE TICKS
	if (enc2_state == 0 && enc2_laststate == 3){
		enc2_absticks = enc2_absticks + encstep;		//Increment the absolute tracking counter
		enc2_laststate = enc2_state;
	}
	else if (enc2_state == 3 && enc2_laststate == 0){
		enc2_absticks = enc2_absticks - encstep;		//decrement the absolute tracking counter
		enc2_laststate = enc2_state;
	}
	else {
		enc2_absticks = enc2_absticks + (encstep*(enc2_state - enc2_laststate));		//Increment or decrement the absolute tracking counter depending on how big of a change was detected in the encoder state
		enc2_laststate = enc2_state;
	}		
}

void ReadHall2(){		
	if (digitalRead(enc2_pin3) == HIGH && digitalRead(enc2_pin4) == LOW && digitalRead(enc2_pin5) == LOW && enc2_state != 0){		//Trigger on rising edge only when this pin wasn't the last one triggered.
		enc2_state = 0;
	}
	else if (digitalRead(enc2_pin4) == HIGH && digitalRead(enc2_pin3) == LOW && digitalRead(enc2_pin5) == LOW && enc2_state != 1){	
		enc2_state = 1;
	}
	else if (digitalRead(enc2_pin5) == HIGH && digitalRead(enc2_pin3) == LOW && digitalRead(enc2_pin4) == LOW && enc2_state != 2){	
		enc2_state = 2;
	}

	//ACCUMULATE ABSOLUTE TICKS USING THE HALL TICKS
	if (enc2_state == 0 && enc2_laststate == 2){
		enc2_absticks = enc2_absticks + encstep;		//Increment the absolute tracking counter
		enc2_laststate = enc2_state;
	}
	else if (enc2_state == 2 && enc2_laststate == 0){
		enc2_absticks = enc2_absticks - encstep;		//decrement the absolute tracking counter
		enc2_laststate = enc2_state;
	}
	else {
		enc2_absticks = enc2_absticks + (encstep*(enc2_state - enc2_laststate));		//Increment or decrement the absolute tracking counter depending on how big of a change was detected in the encoder state
		enc2_laststate = enc2_state;
	}		
}

void CheckMode(){
	//********RESET**********
	if(digitalRead(mode_reset) == LOW){     //If in USB mode, get velocity data from the ROS node.
		softReset();
	}

    //********I2C**********
	if(digitalRead(mode_i2c) == LOW){
		//There's nothing here to set. The I2C "wire.begin" service is started in setup()
		pwmflag = false;
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
	}

	//********PWM**********
	if((digitalRead(mode_reset) == HIGH) && (digitalRead(mode_i2c) == HIGH) && (digitalRead(mode_config) == HIGH))  {     //If none of the mode pins are LOW, read the PWM inputs to control each motor's velocity independently.
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

			if (pwmoffset1 == 0) {		//If in pwm auto center offset mode
				vel1_starting = (pwm1_t-pwmoffset1)*.000002*gain1;	//Save the starting value of vel1 into vel1_starting
				pos1_starting = (pwm1_t-pwmoffset1)*.0002*gain1;
				vel2_starting = (pwm2_t-pwmoffset2)*.000002*gain2;	//Save the starting value of vel1 into vel1_starting
				pos2_starting = (pwm2_t-pwmoffset2)*.0002*gain2;
			}
			else {		
				vel1_starting = 0;
				pos1_starting = 0;
				vel2_starting = 0;
				pos2_starting = 0;
			}
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
	}
}

void softReset() {
	asm volatile ("  jmp 0");
}

void Diagnostics() {
	Serial.print("Commandmode1=");
	Serial.println(commandmode1);
	Serial.print("Commandmode2=");
	Serial.println(commandmode2);
	Serial.print("vel1=");
	Serial.println(vel1);
	Serial.print("vel2=");
	Serial.println(vel2);
	Serial.print("slewvel1=");
	Serial.println(slewvel1);
	Serial.print("slewvel2=");
	Serial.println(slewvel2);
	Serial.print("pos1=");
	Serial.println(pos1);
	Serial.print("pos2=");
	Serial.println(pos2);
	Serial.print("Iset1=");
	Serial.println(Iset1);
	Serial.print("Iset2=");
	Serial.println(Iset2);
	Serial.print("maxIset1=");
	Serial.println(maxIset1);
	Serial.print("maxIset2=");
	Serial.println(maxIset2);
	Serial.print("duty1=");
	Serial.println(duty1);
	Serial.print("duty2=");
	Serial.println(duty2);
	Serial.print("enc1_absticks=");
	Serial.println(enc1_absticks);
	Serial.print("enc2_absticks=");
	Serial.println(enc2_absticks);
	Serial.println(" ");
}
