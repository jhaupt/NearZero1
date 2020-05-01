
void PROMPT_I1Entry(){		
	SerialReceiveLoop();
	Iset1 = atoi(rxchars);	//Convert rxchars to an integer 
	fholder = Iset1/1000;		//Convert mA to A, ONLY used for displaying value in Amps via terminal
	if (Iset1 > 5000){
		Serial.println(F("Maximum allowed current is 3A. CURRENT NOT CHANGED.\n"));
		Serial.println(F("\n"));
		exitflag = true;
		return;	
	}
	else if (Iset1 > 0 && Iset1 <= 5000){}	//Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		Serial.println(F("CURRENT NOT NOT CHANGED.\n"));
		Serial.println(F("\n"));
		exitflag = true;		
		return;
	}
	//Integers stored in the EEPROM must be <=255, so we split our numbers in two, storing 2 digits in one EEPROM register and 2 in another.
	if (Iset1 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = Iset1;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(Iset1/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = Iset1 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_Iset1l, lhlf);   //write the left two digits of Iset1 to the address location given by addr_.
	EEPROM.write(addr_Iset1r, rhlf);	//etc.		
}

void PROMPT_maxI2Entry(){			
	SerialReceiveLoop();
	maxIset2 = atoi(rxchars);	//Convert rxchars to an integer 
	fholder = maxIset2/1000;		//Convert mA to A, ONLY used for displaying value in Amps via terminal
	if (maxIset2 > 5000){
		CheckConfig();
		fholder = maxIset2/1000;		//Convert mA to A
		Serial.print(F("Maximum current is 3A. CURRENT LEFT AT "));
		Serial.print(fholder);
		Serial.println(F("A"));
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;		//Yes that's a goto. FRAK modern programming sensibilities. 
	}
	else if (maxIset2 > 0 && maxIset2 <= 5000){} 	//Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		fholder = maxIset2/1000;		//Convert mA to A
		Serial.print(F("CURRENT LEFT AT "));
		Serial.print(fholder);
		Serial.println(F("A"));
		Serial.println(F("\n\n"));
		exitflag = true;		
		delay(2000);
		return;
	}
	//Integers stored in the EEPROM must be <=255, so we split our numbers in two, storing 2 digits in one EEPROM register and 2 in another.
	if (maxIset2 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = maxIset2;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(maxIset2/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = maxIset2 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_maxIset2l, lhlf);   //write the left two digits of Iset1 to the address location given by addr_.
	EEPROM.write(addr_maxIset2r, rhlf);	//etc.		
}

void PROMPT_minI1Entry(){		
	SerialReceiveLoop();
	minIset1 = atoi(rxchars);	//Convert rxchars to an integer 
	fholder = minIset1/1000;		//Convert mA to A, ONLY used for displaying value in Amps via terminal
	if (minIset1 > 5000){
		CheckConfig();
		fholder = maxIset2/1000;		//Convert mA to A
		Serial.print(F("Maximum current is 3A. CURRENT LEFT AT "));
		Serial.print(fholder);
		Serial.println(F("A"));
		Serial.println(F("\n\n"));
		exitflag = true;
		return;	
	}
	else if (minIset1 > 0 && minIset1 <= 5000){}	//Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		fholder = maxIset2/1000;		//Convert mA to A
		Serial.print(F("CURRENT LEFT AT "));
		Serial.print(fholder);
		Serial.println(F("A"));
		Serial.println(F("\n\n"));
		exitflag = true;		
		return;
	}
	//Integers stored in the EEPROM must be <=255, so we split our numbers in two, storing 2 digits in one EEPROM register and 2 in another.
	if (minIset1 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = minIset1;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(minIset1/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = minIset1 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_minIset1l, lhlf);   //write the left two digits of Iset1 to the address location given by addr_.
	EEPROM.write(addr_minIset1r, rhlf);	//etc.		
}

void PROMPT_minI2Entry(){		
	SerialReceiveLoop();
	minIset2 = atoi(rxchars);	//Convert rxchars to an integer 
	fholder = minIset2/1000;		//Convert mA to A, ONLY used for displaying value in Amps via terminal
	if (minIset2 > 5000){
		CheckConfig();
		fholder = maxIset2/1000;		//Convert mA to A
		Serial.print(F("Maximum current is 3A. CURRENT LEFT AT "));
		Serial.print(fholder);
		Serial.println(F("A"));
		Serial.println(F("\n\n"));
		exitflag = true;
		return;	
	}
	else if (minIset2 == 0) {minIset2 = 1;}	//If it was zero enter 1. We can't have zero, but 1 is almost zero so just as good.
	else if (minIset2 > 0 && minIset2 <= 5000){} //Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		fholder = maxIset2/1000;		//Convert mA to A
		Serial.print(F("CURRENT LEFT AT "));
		Serial.print(fholder);
		Serial.println(F("A"));
		Serial.println(F("\n\n"));
		exitflag = true;		
		return;
	}
	//Integers stored in the EEPROM must be <=255, so we split our numbers in two, storing 2 digits in one EEPROM register and 2 in another.
	if (minIset2 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = minIset2;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(minIset2/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = minIset2 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_minIset2l, lhlf);   //write the left two digits of Iset1 to the address location given by addr_.
	EEPROM.write(addr_minIset2r, rhlf);	//etc.		
}

void PROMPT_gain1Entry(){			
	SerialReceiveLoop();
	gain1 = atoi(rxchars);	//Convert rxchars to an integer 
	if (gain1 > 1000){
		Serial.println(F("Maximum multiplier is 1000. GAIN NOT CHANGED.\n"));
		Serial.println(F("\n"));
		exitflag = true;
		delay(2000);
		return;		
	}
	else if (gain1 > 0 && gain1 <= 1000){}	//Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		Serial.println(F("GAIN NOT CHANGED.\n"));
		Serial.println(F("\n"));
		exitflag = true;		
		return;
	}
	if (gain1 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = gain1;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(gain1/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = gain1 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_gain1l, lhlf);   
	EEPROM.write(addr_gain1r, rhlf);	
}

void PROMPT_gain2Entry(){			
	SerialReceiveLoop();
	gain2 = atoi(rxchars);	//Convert rxchars to an integer 
	if (gain2 > 1000){
		CheckConfig();
		Serial.print(F("Maximum gain is 1000. GAIN LEFT AT "));
		Serial.println(gain2);
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;		
	}
	else if (gain2 > 0 && gain2 <= 1000){} //Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		Serial.print(F("GAIN LEFT AT "));
		Serial.println(gain2);
		Serial.println(F("\n\n"));
		exitflag = true;		
		return;
	}
	if (gain2 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = gain2;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(gain2/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = gain2 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_gain2l, lhlf);   
	EEPROM.write(addr_gain2r, rhlf);	
}

void PROMPT_c2vTcal_posvelEntry(){
	SerialReceiveLoop();
	iholder = atoi(rxchars);	//Convert rxchars to an integer 
	if (iholder >= 0 && iholder <= 10000){} //Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		Serial.print(F("Invalid entry. c2vTcal_posvel LEFT AS "));
		Serial.println(c2vTcal_posvel);
		Serial.println(F("\n\n"));
		exitflag = true;		
		return;
	}
	if (iholder < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = iholder;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(iholder/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = iholder - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_c2vTcal_posvell, lhlf);   
	EEPROM.write(addr_c2vTcal_posvelr, rhlf);	
}

void PROMPT_c2vTcal_negvelEntry(){
	SerialReceiveLoop();
	iholder = atoi(rxchars);	//Convert rxchars to an integer 
	if (iholder >= 0 && iholder <= 10000){} //Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		Serial.print(F("Invalid entry. c2vTcal_negvel LEFT AS "));
		Serial.println(c2vTcal_negvel);
		Serial.println(F("\n\n"));
		exitflag = true;		
		return;
	}
	if (iholder < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = iholder;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(iholder/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = iholder - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_c2vTcal_negvell, lhlf);   
	EEPROM.write(addr_c2vTcal_negvelr, rhlf);	
}

void PROMPT_c2TrigSequenceEntry(){
	SerialReceiveLoop();
	iholder = atoi(rxchars);	//Convert rxchars to an integer 
	if (iholder >= 1 && iholder <= 6){} //Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		Serial.print(F("Invalid entry. c2TrigSequence LEFT AS "));
		Serial.println(c2TrigSequence);
		Serial.println(F("\n\n"));
		exitflag = true;		
		return;
	}
	EEPROM.write(addr_c2TrigSequence, iholder);   
}

void PROMPT_tsphase2Entry(){			
	SerialReceiveLoop();
	tsphase2 = atoi(rxchars);	//Convert rxchars to an integer 
	if (tsphase2 > 6283){
		CheckConfig();
		Serial.print(F("Maximum phase is 2pi (6283). PHASE LEFT AT "));
		Serial.println(tsphase2);
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;		
	}
	else if (tsphase2 > 0 && tsphase2 <= 6283){}		//Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		Serial.println(F("PHASE SET TO 0\n"));
		Serial.println(F("\n"));
		exitflag = true;		
		return;
	}
	if (tsphase2 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = tsphase2;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(tsphase2/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = tsphase2 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_tsphase2l, lhlf);   
	EEPROM.write(addr_tsphase2r, rhlf);	
}

void PROMPT_tscoeff2Entry(){			
	SerialReceiveLoop();
	tscoeff2 = atoi(rxchars);	//Convert rxchars to an integer 
	if (tscoeff2 > 254){
		CheckConfig();
		Serial.print(F("Maximum value is 254. AGGRESSIVENESS LEFT AT "));
		Serial.println(tscoeff2);
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;	
	}
	else if (tscoeff2 > 0 && tscoeff2 <255){		//If it's a valid evtry
		EEPROM.write(addr_tscoeff2, tscoeff2);   	
	}
	else {		//Then it's invalid in some other way
		CheckConfig();
		Serial.print(F("AGGRESSIVENESS LEFT AT "));
		Serial.println(tscoeff2);
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;	
	}	
}

void PROMPT_accel2Entry(){			
	SerialReceiveLoop();
	accel2 = atoi(rxchars);	//Convert rxchars to an integer 
	if (accel2 > 10000){
		CheckConfig();
		Serial.print(F("Maximum acceleration is 10,000. ACCELERATION LEFT AT "));
		Serial.println(accel2);
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;		
	}
	else if (accel2 > 0 && accel2 <= 10000){}	//Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		Serial.print(F("ACCELERATION LEFT AT "));
		Serial.println(accel2);
		Serial.println(F("\n\n"));
		exitflag = true;		
		return;
	}
	if (accel2 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = accel2;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(accel2/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = accel2 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_accel2l, lhlf);   
	EEPROM.write(addr_accel2r, rhlf);	
}


void SerialReceiveLoop(){
	static byte ndx = 0;
	char endMarker = '\n';
	char rc;
	newdata = false;
	while (newdata == false){
		while (Serial.available() > 0) {
			rc = Serial.read();
			if (rc != endMarker) {
				rxchars[ndx] = rc;
				ndx++;
				if (ndx >= nchars) {
					ndx = nchars - 1;
				}
			}
			else {
				rxchars[ndx] = '\0'; // terminate the string
				ndx = 0;
				newdata = true;
			}
		}
	}
}

int SerialPrompt(){

	mainmenu:
	exitflag = false;
	Serial.println(F("\n\n\n"));
	Serial.println(F("~~~~Welcome to the NearZero Configuration Terminal~~~~"));
	Serial.println(F("                Firmware version 1.0"));
	Serial.println(F("       CHANGES TAKE EFFECT AFTER CYCLING POWER"));
	Serial.println(F("                   or RESTTING\n"));
	Serial.println(F("1: Channel 1 configuration"));
	Serial.println(F("2: Channel 2 configuration"));
	Serial.println(F("q: Quit/RESET (change INPUT SELECT jumper as desired FIRST"));
	Serial.println(F("\n\n>>\n"));
	SerialReceiveLoop();
	if (rxchars[0] == '1'){	
		ch1menu:
		exitflag = false;
		Serial.println(F("Channel 1 Configuration:\n"));
		Serial.println(F("0: Set position sensor to NONE, ENCODER, or HALL"));
		Serial.println(F("1: Set FIXED or DYNMAIC current (Amperes)"));
		Serial.println(F("2: Set resting current (Amperes)"));
		Serial.println(F("3: Set command type to VELOCITY or POSITION"));
		Serial.println(F("4: Set velocity/position GAIN"));
		Serial.println(F("5: Set waveform TORQUE SMOOTHING"));
		Serial.println(F("6: Display all present settings"));
		Serial.println(F(" "));
		Serial.println(F("m: Return to main menu"));
		//Serial.println("h: Help... what is all this?");
		Serial.println(F("\n\n>>"));
		SerialReceiveLoop();
		//**0: Set position sensor to NONE, ENCODER, or HALL
		if (rxchars[0] == '0'){		
			Serial.println(F("Set channel 1 sensor type:"));
			Serial.println(F("   n: NONE"));
			Serial.println(F("   e: ENCODER (quadrature)"));
			Serial.println(F("   h: HALL"));	
			Serial.println(F("\n\n>>"));	
			SerialReceiveLoop();
			if (rxchars[0] == 'n' || rxchars[0] == 'N'){
				EEPROM.write(addr_sensortype1, 0);	//0 = none
				Serial.print(F("\nCHANNEL 1 SENSOR SET TO 'NONE'."));
			}			
			else if (rxchars[0] == 'e' || rxchars[0] == 'E'){
				EEPROM.write(addr_sensortype1, 1);	//1 = encoder
				Serial.print(F("\nCHANNEL 1 SENSOR SET TO 'ENCODER'."));
			}
			else if (rxchars[0] == 'h' || rxchars[0] == 'H'){
				EEPROM.write(addr_sensortype1, 2);	//2 = hall
				Serial.print(F("\nCHANNEL 1 SENSOR SET TO 'HALL'."));
			}
			else {
				Serial.println(F("\nInvalid entry."));
			}	
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch1menu;
		}
		//**1: Set current (Amperes) to FIXED or DYNAMIC
		else if (rxchars[0] == '1'){	
			Serial.println(F("Set channel 1 current limiting mode. Resultant power depends on input voltage according to P = IV:"));
			Serial.println(F("   f: Use FIXED current (default)"));
			Serial.println(F("   d: Vary current DYNAMICALLY up to a maximum (encoder or hall sensor required)"));
			Serial.println(F("\n\n>>\n"));
			SerialReceiveLoop();
			if (rxchars[0] == 'f' || rxchars[0] == 'F'){
				EEPROM.write(addr_currentmode1, 0);	//0 = fixed 
				Serial.println(F("\n>>Enter channel 1 fixed current in mA (e.g. enter \"250\" for .25A) or RETURN for default:"));	
				PROMPT_I1Entry();
				if (exitflag == true){
					delay(2000);
					goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
				}
				Serial.print(F("\nCHANNEL 1 FIXED CURRENT SET TO "));
				Serial.print(fholder);
				Serial.println(F("A."));
			}
			else if (rxchars[0] == 'd' || rxchars[0] == 'D'){
				CheckConfig();		//Read EEPROM
				if (sensortype1 == 1 || sensortype1 == 2){
					EEPROM.write(addr_currentmode1, 1);	//1 = dynamic 
					Serial.println(F("\n>>Enter channel 1 maximum dynamic current in mA (e.g. enter \"250\" for .25A):"));
					PROMPT_I1Entry();
					if (exitflag == true){
						delay(2000);
						goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
					}
					Serial.print(F("\nCHANNEL 1 MAXIMUM CURRENT SET TO "));
					Serial.print(fholder);
					Serial.println(F("A."));
				}
				else {
					Serial.println(F("\nSensor type must be either ENCODER or HALL to use this mode.\n"));
					Serial.println(F("MODE NOT SET.\n\n"));
				}			
			}
			else {
				Serial.println(F("\nInvalid entry."));
			}
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch1menu;
		}
		//**2: Set resting current (Amperes) for channel 1
		else if (rxchars[0] == '2'){
			Serial.println(F("Set channel 1 rest current (current when motor is static)."));
			Serial.println(F("When in DYNAMIC current limiting mode this enforces a minimum current."));
			Serial.println(F("Default behavior for FIXED current limiting mode is to *not* reduce current when at rest."));
			Serial.println(F("Default behavior for DYNAMIC current limiting mode is 10%% of the maximum current."));  
			Serial.println(F("0 is a valid entry."));
			Serial.println(F("\n>>Enter channel 1 rest current in mA (e.g. enter \"250\" for .25A), or RETURN for default:"));
			PROMPT_minI1Entry();
			if (exitflag == true){
				delay(2000);
				goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			Serial.print(F("\nCHANNEL 1 REST/MINIMUM CURRENT SET TO "));
			Serial.print(fholder);
			Serial.println(F("A."));
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch1menu;
		}	
		//**3: Set command type to VELOCITY or POSITION
		else if (rxchars[0] == '3'){
			Serial.println(F("Set channel 1 input (command) type to velocity or position:"));
			Serial.println(F("   v: VELOCITY (default)"));
			Serial.println(F("   p: POSITION"));
			Serial.println(F("\n\n>>\n"));
			SerialReceiveLoop();
			if (rxchars[0] == 'v' || rxchars[0] == 'V'){
				EEPROM.write(addr_commandmode1, 0);	//0 = velocity
				Serial.print(F("\nCHANNEL 1 COMMAND INPUT SET TO 'VELOCITY'."));
			}			
			else if (rxchars[0] == 'p' || rxchars[0] == 'P'){
				EEPROM.write(addr_commandmode1, 1);	//1 = position
				Serial.print(F("\nCHANNEL 1 COMMAND INPUT SET TO 'POSITION'."));
			}
			else {
				Serial.println(F("\nInvalid entry."));
			}	
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch1menu;
		}
		//**4: Set velocity/position GAIN for channel 1
		else if (rxchars[0] == '4'){	
			Serial.println(F("Enter gain (velocity/position scaling coefficient) for channel 1."));
			Serial.println(F("This is an integer between 1 and 1000 (arbitrary units). Default is 200."));		
			Serial.println(F("\n\n>>\n"));
			PROMPT_gain1Entry();
			if (exitflag == true){
				delay(2000);
				goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			Serial.print(F("\nCHANNEL 1 GAIN SET TO "));
			Serial.print(gain1);
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch1menu;
		}
		//**5: Set waveform TORQUE SMOOTHING
		else if (rxchars[0] == '5'){
			Serial.println(F("Set channel 1 commutation waveform for torque smoothing:"));
			Serial.println(F("   0: NONE. Pure sine commutation (defualt)"));
			Serial.println(F("   1: BASIC. A two-term fourier is added to the sine waveform."));
			Serial.println(F("   May improve smoothness of some motors."));
			Serial.println(F("\n\n>>"));	
			SerialReceiveLoop();
			if (rxchars[0] == '0'){
				EEPROM.write(addr_torqueprofile1, 0);	//0 = sine
				Serial.print(F("\nCHANNEL 1 COMMUTATION WAVEFORM SET TO 'SINE'."));
			}	
			else if (rxchars[0] == '1'){
				EEPROM.write(addr_torqueprofile1, 1);	
				Serial.print(F("\nCHANNEL 1 COMMUTATION WAVEFORM SET FOR BASIC TORQUE SMOOTHING."));
			}			
			else {
				Serial.println(F("\nInvalid entry."));
			}	
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch1menu;
		}
		//**6: Display all present settings for channel 1
		else if (rxchars[0] == '6'){
			CheckConfig();		//Read EEPROM
			Serial.println(F("Channel 1 Settings:"));
			if (sensortype1 == 0){
				Serial.println(F("   Sensor type: NONE"));
			}
			else if (sensortype1 == 1){
				Serial.println(F("   Sensor type: ENCODER"));
			}
			else if (sensortype1 == 2){
				Serial.println(F("   Sensor type: HALL"));
			}	

			fholder = Iset1/1000;		//Convert mA to A	
			if (currentmode1 == 0){
				Serial.print(F("   Current is FIXED at "));	
				Serial.print(fholder);
				Serial.println(F("A"));
			}
			else if (currentmode1 == 1){
				Serial.print(F("   Current is DYNAMIC with max set to "));
				Serial.print(fholder);
				Serial.println(F("A"));
			}

			fholder = minIset1/1000;		//Convert mA to A	
			Serial.print(F("   Rest/minimum current: "));			
			Serial.print(fholder);
			Serial.println(F("A"));

			if (commandmode1 == 0){
				Serial.println(F("   Command type: VELOCITY"));
			}
			else if (commandmode1 == 1){
				Serial.println(F("   Command type: POSITION"));
			}

			Serial.print(F("   Command gain: "));
			Serial.println(gain1);

			if (torqueprofile1 == 0){
				Serial.println(F("   Commutation waveform: SINE"));
			}
			Serial.println(F("\n\n\n\n"));
			delay(2000);
			goto ch1menu;			
		}
		//**m: Return to main menu
		else if (rxchars[0] == 'm'){
			goto mainmenu;
		}
	}
	if (rxchars[0] == '2'){	
		ch2menu:
		exitflag = false;
		Serial.println(F("Channel 2 Configuration:\n"));
		Serial.println(F("0: Set position sensor (NONE, ENCODER, or HALL)"));
		Serial.println(F("1: Set FIXED or DYNAMIC power"));
		Serial.println(F("2: Set command type (VELOCITY or POSITION)"));
		Serial.println(F("3: Set directionality (NORMAL or REVERSE)"));
		Serial.println(F("4: Display all present settings"));
		Serial.println(F("m: Return to main menu"));
		Serial.println(F(" "));
		Serial.println(F("Advanced settings:"));
		Serial.println(F("5: Set velocity/position GAIN"));
		Serial.println(F("6: Set ACCELERATION"));
		Serial.println(F("7: Set waveform TORQUE SMOOTHING"));
		Serial.println(F("8: Test/run channel 2 motor"));
		//Serial.println("h: Help... what is all this?");
		Serial.println(F("\n\n>>"));
		SerialReceiveLoop();
		sensormenu2:
		//**0: Set position sensor to NONE, ENCODER, or HALL
		exitflag =false;
		if (rxchars[0] == '0'){		
			Serial.println(F("Set channel 2 sensor type:"));
			Serial.println(F("   n: NONE (default)"));
			Serial.println(F("   e: Quadrature ENCODER"));
			Serial.println(F("   h: HALL"));	
			Serial.println(F("\n   (Or strike 'Enter' to leave as is)")); 
			Serial.println(F("\n\n>>"));	
			SerialReceiveLoop();
			if (rxchars[0] == 'n' || rxchars[0] == 'N'){
				EEPROM.write(addr_sensortype2, 0);	//0 = none
				Serial.println(F("CHANNEL 2 SENSOR SET TO 'NONE'"));
				Serial.println(F("POWER MODE WILL REVERT TO 'FIXED'"));
				EEPROM.write(addr_currentmode2, 0);	//0 = fixed 
			}			
			else if (rxchars[0] == 'e' || rxchars[0] == 'E'){
				//Copy and modify from hall section below
			}
			else if (rxchars[0] == 'h' || rxchars[0] == 'H'){
				hallmenu2:	
				exitflag = false;
				Serial.println(F("\nThe hall sensor must be calibrated for each new make/model of motor used:"));
				Serial.println(F("   1: Perform self-calibration (motor must be free and unloaded)"));
				Serial.println(F("   2: Enter previously-measured calibration value"));
				Serial.println(F("   3: Leave as is"));
				Serial.println(F("   4: Go back to previous menu without changing sensor type")); 
				Serial.println(F("\n\n>>"));
				SerialReceiveLoop();
				if (rxchars[0] == '1'){
					Serial.println(F("Measuring hall sensor calibration value (motor should be turning)..."));	
					HallCal2();	
					if (exitflag == true){
						Serial.println(F("\nHALL MODE NOT SET"));
						delay(2000);
						goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. You know what I mean?
					}		
				
					if (sensdelay2_deviation_lowest > 30) {	//Don't allow setting HALL mode if the best deviation was too high
						Serial.println(F("\nWARNING: The measured calibration values suggest that dynamic current"));
						Serial.println(F("control is not possible with this motor's hall sensors."));
						Serial.println(F("that re-running the self-calibration will result in a better calibration measurement."));
						Serial.println(F("\nHALL MODE NOT SET."));
						Serial.println(F("\n\n\n\n"));
						delay(2000);		
						goto ch2menu;
					}
					else if (sensdelay2_deviation_lowest > 15) {	//Throw warning
						Serial.println(F("WARNING: The measured calibration values suggest that dynamic current"));
						Serial.println(F("control may behave sporadically with this motor's hall sensors. It's possible"));
						Serial.println(F("that re-running the self-calibration will result in a better calibration measurement."));
					}
					if (c2vTcal_posvel == c2vTcal_negvel) {
						Serial.println(F("\nOops, there was an error. This happens. Run self-calibration again."));
						goto hallmenu2;
					}
					else {
						Serial.println(F("\nMeasured hall calibration values for channel 2: "));
						Serial.print(F("   c2TrigSequence = "));
						Serial.println(c2TrigSequence);
						Serial.print(F("   c2vTcal_posvel = "));
						Serial.println(c2vTcal_posvel);
						Serial.print(F("   c2vTcal_negvel = "));
						Serial.println(c2vTcal_negvel);
						Serial.println(F("\nUse these values and write to EEPROM? (Y/n)"));
						Serial.println(F("\n\n>>"));	
						SerialReceiveLoop();
						if (rxchars[0] == 'n' || rxchars[0] == 'N'){
							CheckConfig();		//Needed to reset hall/encoder configs back to original value.
							Serial.println(F("VALUE NOT SET"));
							Serial.println(F("\n\n\n\n"));
							delay(2000);		
							goto hallmenu2;		
						}
						else {		//If the user pressed anything but 'n', go ahead and commit the settings
							EEPROM.write(addr_sensortype2, 2);	//2 = hall	
							iholder = c2vTcal_posvel*100;	//Convert to integer for floating point storage
							if (iholder < 100){		//If it's a 1 or 2 digit number
								lhlf = 0;	//The left half of the four digit number is 0
								rhlf = iholder;		//And the right half is just the number
							}
							else {		//If it's a 3 or 4 digit number
								lhlf = floor(iholder/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
								rhlf = iholder - (lhlf*100);	//The right half is the trailing two digits of the number.
							}
							EEPROM.write(addr_c2vTcal_posvell, lhlf);   //write the left two digits of iholder to the address location given by addr_.
							EEPROM.write(addr_c2vTcal_posvelr, rhlf);	//etc.	
							iholder = c2vTcal_negvel*100;
							if (iholder < 100){		//If it's a 1 or 2 digit number
								lhlf = 0;	//The left half of the four digit number is 0
								rhlf = iholder;		//And the right half is just the number
							}
							else {		//If it's a 3 or 4 digit number
								lhlf = floor(iholder/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
								rhlf = iholder - (lhlf*100);	//The right half is the trailing two digits of the number.
							}
							EEPROM.write(addr_c2vTcal_negvell, lhlf);   //write the left two digits of iholder to the address location given by addr_.
							EEPROM.write(addr_c2vTcal_negvelr, rhlf);	//etc.		
						
							EEPROM.write(addr_c2TrigSequence, c2TrigSequence);	//etc.		
							Serial.print(F("CHANNEL 2 SUCCESSFULLY CONFIGURED WITH HALL SENSOR"));						
						}
					}
				}
				else if (rxchars[0] == '2'){
					Serial.println(F("Enter c2vTcal_posvel calibration value in hundreths (e.g. 123 for 1.23)"));	
					Serial.println(F("   or strike 'Enter' to leave as is:")); 
					Serial.println(F("\n\n>>"));
					PROMPT_c2vTcal_posvelEntry();
					if (exitflag == true){
						delay(2000);
						goto hallmenu2;		//Yes that's a goto. FRACK modern programming sensibilities. You know what I mean?
					}
					CheckConfig();
					Serial.print(F("c2vTcal_posvel SET TO "));
					Serial.println(c2vTcal_posvel);
					Serial.println(F("\n "));

					Serial.println(F("Enter c2vTcal_negvel calibration value in hundreths (e.g. 123 for 1.23)"));	
					Serial.println(F("   or strike 'Enter' to leave as is:")); 
					Serial.println(F("\n\n>>"));
					PROMPT_c2vTcal_negvelEntry();
					if (exitflag == true){
						delay(2000);
						goto hallmenu2;		//Yes that's a goto. FRACK modern programming sensibilities. You know what I mean?
					}
					CheckConfig();
					Serial.print(F("c2vTcal_negvel SET TO "));
					Serial.print(c2vTcal_negvel);
					Serial.println(F("\n "));

					Serial.println(F("Enter c2TrigSequence calibration value"));	
					Serial.println(F("   or strike 'Enter' to leave as is:")); 
					Serial.println(F("\n\n>>"));
					PROMPT_c2TrigSequenceEntry();
					if (exitflag == true){
						delay(2000);
						goto hallmenu2;		//Yes that's a goto. FRACK modern programming sensibilities. You know what I mean?
					}
					CheckConfig();
					Serial.print(F("c2TrigSequence SET TO "));
					Serial.print(c2TrigSequence);
					Serial.println(F("\n "));

					EEPROM.write(addr_sensortype2, 2);	//2 = hall
				}
				else if (rxchars[0] == '3'){
					Serial.print(F("c2vTcal_posvel LEFT AS "));
					Serial.println(c2vTcal_posvel);
					Serial.print(F("c2vTcal_negvel LEFT AS "));
					Serial.println(c2vTcal_negvel);
					Serial.print(F("c2TrigSequence LEFT AS "));
					Serial.println(c2TrigSequence);
					EEPROM.write(addr_sensortype2, 2);	//2 = hall
				}
				else if (rxchars[0] == '4'){
					goto sensormenu2;
				}
			}
			else {
				Serial.println(F("SENSOR NOT CHANGED"));
			}	
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch2menu;
		}
		//**1: Set current (Amperes) to FIXED or DYNAMIC
		else if (rxchars[0] == '1'){	
			Serial.println(F("Set channel 2 fixed or dynamic power (actually, you set the current in mA"));
			Serial.println(F("and the resultant power depends on input voltage according to P = IV):"));
			Serial.println(F("\n   f: Use FIXED power"));
			Serial.println(F("   r: Use FIXED power and use lower RESTING power when commanded velocity is 0"));
			Serial.println(F("   d: Vary power DYNAMICALLY based on mechanical resistance (encoder or hall sensor required & does not work in 'POSITION' command mode)"));
			Serial.println(F("\n   (Or strike 'Enter' to leave as is)")); 
			Serial.println(F("\n\n>>"));
			SerialReceiveLoop();
			if (rxchars[0] == 'f' || rxchars[0] == 'F'){
				EEPROM.write(addr_currentmode2, 0);	//0 = fixed 
				Serial.println(F("Enter channel 2 fixed current in mA (e.g. enter \"250\" for .25A)"));	
				Serial.println(F("   or strike 'Enter' to leave as is:")); 
				Serial.println(F("\n\n>>"));
				PROMPT_maxI2Entry();
				if (exitflag == true){
					delay(2000);
					goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
				}
				//--------SET minI2 = maxI2-----------
				minIset2 = maxIset2;
				if (minIset2 < 100){		//If it's a 1 or 2 digit number
					lhlf = 0;	//The left half of the four digit number is 0
					rhlf = minIset2;		//And the right half is just the number
				}
				else {		//If it's a 3 or 4 digit number
					lhlf = floor(minIset2/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
					rhlf = minIset2 - (lhlf*100);	//The right half is the trailing two digits of the number.
				}
				EEPROM.write(addr_minIset2l, lhlf);   //write the left two digits of Iset1 to the address location given by addr_.
				EEPROM.write(addr_minIset2r, rhlf);	//etc.
				//------------------------------------

				Serial.print(F("CHANNEL 2 FIXED CURRENT SET TO "));
				Serial.print(fholder);
				Serial.println(F("A"));
			}
			else if (rxchars[0] == 'r' || rxchars[0] == 'R'){
				EEPROM.write(addr_currentmode2, 0);	//0 = fixed 
				Serial.println(F("Enter channel 2 fixed current in mA (e.g. enter \"250\" for .25A)"));	
				Serial.println(F("   or strike 'Enter' to leave as is:")); 
				Serial.println(F("\n\n>>"));
				PROMPT_maxI2Entry();
				if (exitflag == true){
					delay(2000);
					goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
				}
				Serial.print(F("CHANNEL 2 FIXED CURRENT SET TO "));
				Serial.print(fholder);
				Serial.println(F("A"));
				Serial.println(F("\nEnter channel 2 resting current in mA (e.g. enter \"100\" for .1A)"));
				Serial.println(F("   or strike 'Enter' to leave as is:")); 
				Serial.println(F("\n\n>>"));
				PROMPT_minI2Entry();
				if (exitflag == true){
					delay(2000);
					goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
				}
				Serial.print(F("CHANNEL 2 RESTING CURRENT SET TO "));
				Serial.print(fholder);
				Serial.println(F("A"));
			}
			else if (rxchars[0] == 'd' || rxchars[0] == 'D'){
				CheckConfig();		//Read EEPROM
				if (sensortype2 == 1 || sensortype2 == 2){
					EEPROM.write(addr_currentmode2, 1);	//1 = dynamic 
					Serial.println(F("Enter channel 2 maximum dynamic current in mA (e.g. enter \"250\" for .25A)"));
					Serial.println(F("   or strike 'Enter' to leave as is:")); 
					Serial.println(F("\n\n>>"));
					PROMPT_maxI2Entry();
					if (exitflag == true){
						delay(2000);
						goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
					}
					Serial.print(F("CHANNEL 2 MAXIMUM CURRENT SET TO "));
					Serial.print(fholder);
					Serial.println(F("A"));
					Serial.println(F("\nEnter channel 2 minimum/resting dynamic current in mA (e.g. enter \"100\" for .1A)"));
					Serial.println(F("   or strike 'Enter' to leave as is:")); 
					Serial.println(F("\n\n>>"));
					PROMPT_minI2Entry();
					if (exitflag == true){
						delay(2000);
						goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
					}
					Serial.print(F("CHANNEL 2 MINIMUM/RESTING CURRENT SET TO "));
					Serial.print(fholder);
					Serial.println(F("A"));
				}
				else {
					Serial.println(F("Sensor type must be either ENCODER or HALL to use this mode.\n"));
					Serial.println(F("MODE NOT SET\n\n"));
				}			
			}
			else {
				Serial.println(F("POWER SETTINGS NOT CHANGED"));
			}
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch2menu;
		}
		//**2: Set command type to VELOCITY or POSITION
		else if (rxchars[0] == '2'){
			Serial.println(F("Set channel 2 input (command) type to velocity or position:"));
			Serial.println(F("   v: VELOCITY (default)"));
			Serial.println(F("   p: POSITION / servo mode "));
			Serial.println(F("\n   (Or strike 'Enter' to leave as is)")); 
			Serial.println(F("\n\n>>"));
			SerialReceiveLoop();
			if (rxchars[0] == 'v' || rxchars[0] == 'V'){
				EEPROM.write(addr_commandmode2, 0);	//0 = velocity
				Serial.print(F("CHANNEL 2 COMMAND INPUT SET TO 'VELOCITY'"));
			}			
			else if (rxchars[0] == 'p' || rxchars[0] == 'P'){
				EEPROM.write(addr_commandmode2, 1);	//1 = position
				Serial.print(F("CHANNEL 2 COMMAND INPUT SET TO 'POSITION' (SERVO MODE)"));
			}
			else {
				Serial.println(F("COMMAND TYPE NOT CHANGED"));
			}	
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch2menu;
		}
		//**3: Set directionality to NORMAL or REVERSE
		else if (rxchars[0] == '3'){
			Serial.println(F("Set channel 2 directionality:"));
			Serial.println(F("   n: NORMAL (default)"));
			Serial.println(F("   r: REVERSE "));
			Serial.println(F("\n   (Or strike 'Enter' to leave as is)")); 
			Serial.println(F("\n\n>>"));
			SerialReceiveLoop();
			if (rxchars[0] == 'n' || rxchars[0] == 'N'){
				EEPROM.write(addr_dir2, 0);	//0 = NORMAL
				Serial.print(F("CHANNEL 2 DIRECTIONALITY SET TO 'NORMAL'"));
			}			
			else if (rxchars[0] == 'r' || rxchars[0] == 'R'){
				EEPROM.write(addr_dir2, 1);	//1 = REVERSE
				Serial.print(F("CHANNEL 2 DIRECTIONALITY SET TO 'REVERSE'"));
			}
			else {
				Serial.println(F("DIRECTIONALITY NOT CHANGED"));
			}	
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch2menu;
		}
		//**4: Display all present settings
		else if (rxchars[0] == '4'){
			CheckConfig();
			DisplaySettings2();
			delay(2000);
			goto ch2menu;			
		}
		//**m: Return to main menu
		else if (rxchars[0] == 'm'){
			goto mainmenu;
		}
		//**5: Set velocity/position GAIN
		else if (rxchars[0] == '5'){	
			Serial.println(F("Set channel 2 gain."));
			Serial.println(F("   This is the scaling coefficient for velocity or position inputs."));
			Serial.println(F("\n   Enter an integer between 1 and 1000 (arbitrary units)"));	
			Serial.println(F("   or strike 'Enter' to leave as is:")); 	
			Serial.println(F("\n\n>>"));
			PROMPT_gain2Entry();
			if (exitflag == true){
				delay(2000);
				goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			Serial.print(F("\nCHANNEL 2 GAIN SET TO "));
			Serial.print(gain2);
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch2menu;
		}
		//**6: Set ACCELERATION
		else if (rxchars[0] == '6'){	
			Serial.println(F("Set channel 2 acceleration."));
			Serial.println(F("   In VELOCITY input mode, this affects how aggressively the motor ramps velocity to the commanded velocity."));
			Serial.println(F("   In POSITION input mode, this affects how aggressively the motor's position follows the commanded position."));
			Serial.println(F("\n   Enter acceleration (an integer between 1 an 9999, arbitary units)."));
			Serial.println(F("   Enter 10000 to turn off acceleration dynamics entirely."));
			Serial.println(F("   Strike 'Enter' to leave as is:")); 	
			Serial.println(F("\n\n>>"));
			PROMPT_accel2Entry();
			if (exitflag == true){
				delay(2000);
				goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			if (accel2 == 10000){
				Serial.println(F("CHANNEL 2 ACCELERATION DYNAMICS TURNED OFF"));
				delay(2000);
				goto ch2menu;
			}
			else {
				Serial.print(F("CHANNEL 2 ACCELERATION SET TO "));
				Serial.print(accel2);
				Serial.println(F("\n\n\n\n"));
				delay(2000);		
				goto ch2menu;
			}
		}
		//**7: Set waveform TORQUE SMOOTHING
		else if (rxchars[0] == '7'){
			Serial.println(F("Set channel 2 commutation waveform for torque smoothing:"));
			Serial.println(F("   Torque smoothing may improve motion smoothness with some motors."));
			Serial.println(F("   If so inclined, the aggressiveness and phase may be tweaked by trial and error.\n"));
			Serial.println(F("   0: OFF. Pure sine commutation"));
			Serial.println(F("   1: ON. A correction function (two-term fourier series) is added to the sine waveform."));
			Serial.println(F("   2: Set aggressiveness of correction function (when ON)"));
			Serial.println(F("   3: Set phase of correction function (when ON)"));
			Serial.println(F("\n   (Or strike 'Enter' to leave as is)")); 
			Serial.println(F("\n\n>>"));	
			SerialReceiveLoop();
			if (rxchars[0] == '0'){
				EEPROM.write(addr_torqueprofile2, 0);	//0 = sine
				Serial.print(F("CHANNEL 2 TORQUE SMOOTHING IS OFF (PURE SINE COMMUTATION)"));
			}	
			else if (rxchars[0] == '1'){
				EEPROM.write(addr_torqueprofile2, 1);	
				Serial.print(F("CHANNEL 2 TORQUE SMOOTHING IS ON"));
			}		
			else if (rxchars[0] == '2'){
				Serial.println(F("Enter aggressiveness parameter. This is an integer between 1 and 254 (arbitarary units)"));
				Serial.println(F("   or strike 'Enter' to leave as is:")); 
				Serial.println(F("\n\n>>"));
				PROMPT_tscoeff2Entry();
				if (exitflag == true){
					delay(2000);
					goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
				}
				Serial.print(F("CHANNEL 2 TORQUE SMOOTHING AGGRESSIVENESS SET TO "));
				Serial.println(tscoeff2);
				if (torqueprofile2 == 0){
					Serial.println(F("(only applies when torque smoothing is ON)"));
				}
			}
			else if (rxchars[0] == '3'){
				Serial.println(F("Enter phase/offset of correction function. This is a number between 0 and 2pi (6.283) in thousands"));
				Serial.println(F("(i.e. enter a number between 0 and 6283) or strike 'Enter' to leave as is:"));
				Serial.println(F("\n\n>>"));
				PROMPT_tsphase2Entry();
				if (exitflag == true){
					delay(2000);
					goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
				}
				Serial.print(F("CHANNEL 2 TORQUE SMOOTHING PHASE SET TO "));
				fholder = tsphase2/1000;		//Convert to decimal form
				Serial.println(fholder);
				if (torqueprofile2 == 0){
					Serial.println(F("(only applies when torque smoothing is ON)"));
				}				
			}
	
			else {
				Serial.println(F("TORQUE SMOOTHING NOT CHANGED"));
			}	
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch2menu;
		}
		else if (rxchars[0] == '8'){
			Serial.println(F("\nRunning motor on channel 2 for about 10s..."));
			TorqueCal2();
			goto ch2menu;
		}
	}
	else if (rxchars[0] == 'q' || rxchars[0] == 'Q'){	
		softReset();
	}
	/*else if (rxchars[0] == '3'){
		Serial.println(F("w increments channel 1 forward"));
		Serial.println(F("x increments channel 1 backward"));
		Serial.println(F("a increments channel 2 forward"));
		Serial.println(F("d increments channel 2 backward"));
		Serial.println(F("Strike 'm' at any time to return to main menu"));		
	}*/
}

