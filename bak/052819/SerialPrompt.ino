
void PROMPT_I1Entry(){		
	SerialReceiveLoop();
	Iset1 = atoi(rxchars);	//Convert rxchars to an integer 
	fholder = Iset1/1000;		//Convert mA to A, ONLY used for displaying value in Amps via terminal
	if (Iset1 > 5000){
		Serial.println("\nMaximum allowed current is 5A. CURRENT NOT SET.\n");
		Serial.println("\n");
		exitflag = true;
		return;	
	}
	else if (Iset1 >= 0 && Iset1 <= 5000){
		//Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	}
	else {
		Serial.println("\nWe don't take kindly to invalid entries 'round these parts. CURRENT NOT SET.\n");
		Serial.println("\n");
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

void PROMPT_I2Entry(){			
	SerialReceiveLoop();
	Iset2 = atoi(rxchars);	//Convert rxchars to an integer 
	fholder = Iset2/1000;		//Convert mA to A, ONLY used for displaying value in Amps via terminal
	if (Iset2 > 5000){
		Serial.println("\nMaximum allowed current is 5A. CURRENT NOT SET.\n");
		Serial.println("\n");
		exitflag = true;
		delay(2000);
		return;		//Yes that's a goto. FRAK modern programming sensibilities. 
	}
	else if (Iset2 >= 0 && Iset2 <= 5000){
		//Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	}
	else {
		Serial.println("\nWe don't take kindly to invalid entries 'round these parts. CURRENT NOT SET.\n");
		Serial.println("\n");
		exitflag = true;		
		delay(2000);
		return;
	}
	//Integers stored in the EEPROM must be <=255, so we split our numbers in two, storing 2 digits in one EEPROM register and 2 in another.
	if (Iset2 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = Iset2;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(Iset2/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = Iset2 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_Iset2l, lhlf);   //write the left two digits of Iset1 to the address location given by addr_.
	EEPROM.write(addr_Iset2r, rhlf);	//etc.		
}

void PROMPT_minI1Entry(){		
	SerialReceiveLoop();
	minIset1 = atoi(rxchars);	//Convert rxchars to an integer 
	fholder = minIset1/1000;		//Convert mA to A, ONLY used for displaying value in Amps via terminal
	if (minIset1 > 5000){
		Serial.println("\nMaximum allowed current is 5A. CURRENT NOT SET.\n");
		Serial.println("\n");
		exitflag = true;
		return;	
	}
	else if (minIset1 >= 0 && minIset1 <= 5000){
		//Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	}
	else {
		Serial.println("\nInvalid entry. CURRENT NOT SET.\n");
		Serial.println("\n");
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
		Serial.println("\nMaximum allowed current is 5A. CURRENT NOT SET.\n");
		Serial.println("\n");
		exitflag = true;
		return;	
	}
	else if (minIset2 >= 0 && minIset2 <= 5000){
		//Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	}
	else {
		Serial.println("\nInvalid entry. CURRENT NOT SET.\n");
		Serial.println("\n");
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
		Serial.println("\nMaximum allowed multiplier is 1000. GAIN NOT SET.\n");
		Serial.println("Returning to main menu...\n\n");
		exitflag = true;
		delay(2000);
		return;		//Yes that's a goto. FRAK modern programming sensibilities. 
	}
	else if (gain1 == 0){
		Serial.println("\nWARNING: Setting gain to 0 is allowed, but the motor will not respont to commands.\n");
		delay(2000);
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
		Serial.println("\nMaximum allowed multiplier is 1000. GAIN NOT SET.\n");
		Serial.println("Returning to main menu...\n\n");
		exitflag = true;
		delay(2000);
		return;		//Yes that's a goto. FRAK modern programming sensibilities. 
	}
	else if (gain2 == 0){
		Serial.println("\nWARNING: Setting gain to 0 is allowed, but the motor will not respont to commands.\n");
		delay(2000);
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

void PROMPT_torqueprofile1Entry(){			
	SerialReceiveLoop();
	torqueprofile1 = atoi(rxchars);	//Convert rxchars to an integer 
	if (torqueprofile1 > 7){
		Serial.println("\nInvalid entry. Profile not set.\n");
		Serial.println("Returning to main menu...\n\n");
		exitflag = true;
		delay(2000);
		return;	
	}
	else if (rxchars[0] == 'm'){
		Serial.println("Returning to main menu...\n\n");
		exitflag = true;
		delay(2000);
		return;	
	}
	EEPROM.write(addr_torqueprofile1, torqueprofile1);   
}

void PROMPT_torqueprofile2Entry(){			
	SerialReceiveLoop();
	torqueprofile2 = atoi(rxchars);	//Convert rxchars to an integer 
	if (torqueprofile2 > 7){
		Serial.println("\nInvalid entry. Profile not set.\n");
		Serial.println("Returning to main menu...\n\n");
		exitflag = true;
		delay(2000);
		return;		
	}
	else if (rxchars[0] == 'm'){
		Serial.println("Returning to main menu...\n\n");
		exitflag = true;
		delay(2000);
		return;	
	}
	EEPROM.write(addr_torqueprofile2, torqueprofile2);   
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
	Serial.println("\n\n\n");
	Serial.println("~~~~Welcome to the NearZero Configuration Terminal~~~~");
	Serial.println("                Firmware version 1.0");
	Serial.println("       CHANGES TAKE EFFECT AFTER CYCLING POWER\n");
	Serial.println("1: Channel 1 configuration");
	Serial.println("2: Channel 2 configuration");
	Serial.println(" ");
	Serial.println("3: Control motors using this terminal");
	Serial.println("4: Test ch1 motor (rotate for about 25 seconds)");
	Serial.println("5: Test ch2 motor (rotate for about 25 seconds)");
	Serial.println("\n\n>>\n");
	SerialReceiveLoop();
	if (rxchars[0] == '1'){	
		ch1menu:
		exitflag = false;
		Serial.println("Channel 1 Configuration:\n");
		Serial.println("0: Set position sensor to NONE, ENCODER, or HALL");
		Serial.println("1: Set current (Amperes) to FIXED or DYNAMIC");
		Serial.println("2: Set resting current (Amperes)");
		Serial.println("3: Set command type to VELOCITY or POSITION");
		Serial.println("4: Set velocity/position GAIN");
		Serial.println("5: Set waveform TORQUE SMOOTHING");
		Serial.println("6: Display all present settings");
		Serial.println(" ");
		Serial.println("m: Return to main menu");
		//Serial.println("h: Help... what is all this?");
		Serial.println("\n\n>>");
		SerialReceiveLoop();
		//**0: Set position sensor to NONE, ENCODER, or HALL
		if (rxchars[0] == '0'){		
			Serial.println("Set channel 1 sensor type:");
			Serial.println("   n: NONE");
			Serial.println("   e: ENCODER (quadrature)");
			Serial.println("   h: HALL:");	
			Serial.println("\n\n>>");	
			SerialReceiveLoop();
			if (rxchars[0] == 'n' || rxchars[0] == 'N'){
				EEPROM.write(addr_sensortype1, 0);	//0 = none
				Serial.print("\nCHANNEL 1 SENSOR SET TO 'NONE'.");
			}			
			else if (rxchars[0] == 'e' || rxchars[0] == 'E'){
				EEPROM.write(addr_sensortype1, 1);	//1 = encoder
				Serial.print("\nCHANNEL 1 SENSOR SET TO 'ENCODER'.");
			}
			else if (rxchars[0] == 'h' || rxchars[0] == 'H'){
				EEPROM.write(addr_sensortype1, 2);	//2 = hall
				Serial.print("\nCHANNEL 1 SENSOR SET TO 'HALL'.");
			}
			else {
				Serial.println("\nInvalid entry.");
			}	
			Serial.println("\n\n\n\n");
			delay(2000);		
			goto ch1menu;
		}
		//**1: Set current (Amperes) to FIXED or DYNAMIC
		else if (rxchars[0] == '1'){	
			Serial.println("Set channel 1 current limiting mode. Resultant power depends on input voltage according to P = IV:");
			Serial.println("   f: Use FIXED current (default)");
			Serial.println("   d: Vary current DYNAMICALLY up to a maximum (encoder or hall sensor required)");
			Serial.println("\n\n>>\n");
			SerialReceiveLoop();
			if (rxchars[0] == 'f' || rxchars[0] == 'F'){
				EEPROM.write(addr_currentmode1, 0);	//0 = fixed 
				Serial.println("\n>>Enter channel 1 fixed current in mA (e.g. enter \"250\" for .25A):");	
				PROMPT_I1Entry();
				if (exitflag == true){
					delay(2000);
					goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
				}
				Serial.print("\nCHANNEL 1 FIXED CURRENT SET TO ");
				Serial.print(fholder);
				Serial.println("A.");
			}
			else if (rxchars[0] == 'd' || rxchars[0] == 'D'){
				sensortype1 = EEPROM.read(addr_sensortype1);
				if (sensortype1 == 1 || sensortype1 == 2){
					EEPROM.write(addr_currentmode1, 1);	//1 = dynamic 
					Serial.println("\n>>Enter channel 1 maximum dynamic current in mA (e.g. enter \"250\" for .25A):");
					PROMPT_I1Entry();
					if (exitflag == true){
						delay(2000);
						goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
					}
					Serial.print("\nCHANNEL 1 MAXIMUM CURRENT SET TO ");
					Serial.print(fholder);
					Serial.println("A.");
				}
				else {
					Serial.println("\nSensor type must be either ENCODER or HALL to use this mode.\n");
					Serial.println("MODE NOT SET.\n\n");
				}			
			}
			else {
				Serial.println("\nInvalid entry.");
			}
			Serial.println("\n\n\n\n");
			delay(2000);		
			goto ch1menu;
		}
		//**2: Set resting current (Amperes) for channel 1
		else if (rxchars[0] == '2'){
			Serial.println("Set channel 1 rest current (current when motor is static).");
			Serial.println("When in DYNAMIC current limiting mode this enforces a minimum current.");
			Serial.println("Default behavior for FIXED current limiting mode is to *not* reduce current when at rest.");
			Serial.println("Default behavior for DYNAMIC current limiting mode is 10%% of the maximum current.");  
			Serial.println("0 is a valid entry.");
			Serial.println("\n>>Enter channel 1 rest current in mA (e.g. enter \"250\" for .25A):");
			PROMPT_minI1Entry();
			if (exitflag == true){
				delay(2000);
				goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			Serial.print("\nCHANNEL 1 REST/MINIMUM CURRENT SET TO ");
			Serial.print(fholder);
			Serial.println("A.");
			Serial.println("\n\n\n\n");
			delay(2000);		
			goto ch1menu;
		}	
		//**3: Set command type to VELOCITY or POSITION
		else if (rxchars[0] == '3'){
			Serial.println("Set channel 1 input (command) type to velocity or position:");
			Serial.println("   v: VELOCITY (default)");
			Serial.println("   p: POSITION");
			Serial.println("\n\n>>\n");
			SerialReceiveLoop();
			if (rxchars[0] == 'v' || rxchars[0] == 'V'){
				EEPROM.write(addr_commandmode1, 0);	//0 = velocity
				Serial.print("\nCHANNEL 1 COMMAND INPUT SET TO 'VELOCITY'.");
			}			
			else if (rxchars[0] == 'p' || rxchars[0] == 'P'){
				EEPROM.write(addr_commandmode1, 1);	//1 = position
				Serial.print("\nCHANNEL 1 COMMAND INPUT SET TO 'POSITION'.");
			}
			else {
				Serial.println("\nInvalid entry.");
			}	
			Serial.println("\n\n\n\n");
			delay(2000);		
			goto ch1menu;
		}
		//**4: Set velocity/position GAIN for channel 1
		else if (rxchars[0] == '4'){	
			Serial.println("Enter gain (velocity/position scaling coefficient) for channel 1.");
			Serial.println("This is an integer between 1 and 1000 (arbitrary units). Default is 200.");		
			Serial.println("\n\n>>\n");
			PROMPT_gain1Entry();
			if (exitflag == true){
				delay(2000);
				goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			Serial.print("\nCHANNEL 1 GAIN SET TO ");
			Serial.print(gain1);
			Serial.println("\n\n\n\n");
			delay(2000);		
			goto ch1menu;
		}
		//**5: Set waveform TORQUE SMOOTHING
		else if (rxchars[0] == '5'){
			Serial.println("Set channel 1 commutation waveform for torque smoothing:");
			Serial.println("   0: NONE. Pure sine commutation (defualt)");
			Serial.println("\n\n>>");	
			SerialReceiveLoop();
			if (rxchars[0] == '0'){
				EEPROM.write(addr_torqueprofile1, 0);	//0 = sine
				Serial.print("\nCHANNEL 1 COMMUTATION WAVEFORM SET TO 'SINE'.");
			}			
			else {
				Serial.println("\nInvalid entry.");
			}	
			Serial.println("\n\n\n\n");
			delay(2000);		
			goto ch1menu;
		}
		//**6: Display all present settings for channel 1
		else if (rxchars[0] == '6'){
			CheckConfig();		//Read EEPROM
			Serial.println("Channel 1 Settings:");
			if (sensortype1 == 0){
				Serial.println("   Sensor type: NONE");
			}
			else if (sensortype1 == 1){
				Serial.println("   Sensor type: ENCODER");
			}
			else if (sensortype1 == 2){
				Serial.println("   Sensor type: HALL");
			}	

			fholder = Iset1/1000;		//Convert mA to A	
			if (currentmode1 == 0){
				Serial.print("   Current is FIXED at ");	
				Serial.print(fholder);
				Serial.println("A.");
			}
			else if (currentmode1 == 1){
				Serial.print("   Current is DYNAMIC with max set to ");
				Serial.print(fholder);
				Serial.println("A.");
			}

			fholder = minIset1/1000;		//Convert mA to A	
			Serial.print("   Rest/minimum current: ");			
			Serial.print(fholder);
			Serial.println("A.");

			if (commandmode1 == 0){
				Serial.println("   Command type: VELOCITY");
			}
			else if (commandmode1 == 1){
				Serial.println("   Command type: POSITION");
			}

			Serial.print("   command gain: ");
			Serial.println(gain1);

			if (torqueprofile1 == 0){
				Serial.println("   Commutation waveform: SINE");
			}
			Serial.println("\n\n\n\n");
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
		Serial.println("Channel 2 Configuration:\n");
		Serial.println("0: Set position sensor to NONE, ENCODER, or HALL");
		Serial.println("1: Set current (Amperes) to FIXED or DYNAMIC");
		Serial.println("2: Set resting current (Amperes)");
		Serial.println("3: Set command type to VELOCITY or POSITION");
		Serial.println("4: Set velocity/position GAIN");
		Serial.println("5: Set waveform TORQUE SMOOTHING");
		Serial.println("6: Display all present settings");
		Serial.println(" ");
		Serial.println("m: Return to main menu");
		//Serial.println("h: Help... what is all this?");
		Serial.println("\n\n>>");
		SerialReceiveLoop();
		//**0: Set position sensor to NONE, ENCODER, or HALL
		if (rxchars[0] == '0'){		
			Serial.println("Set channel 2 sensor type:");
			Serial.println("   n: NONE");
			Serial.println("   e: ENCODER (quadrature)");
			Serial.println("   h: HALL:");	
			Serial.println("\n\n>>");	
			SerialReceiveLoop();
			if (rxchars[0] == 'n' || rxchars[0] == 'N'){
				EEPROM.write(addr_sensortype2, 0);	//0 = none
				Serial.print("\nCHANNEL 2 SENSOR SET TO 'NONE'.");
			}			
			else if (rxchars[0] == 'e' || rxchars[0] == 'E'){
				EEPROM.write(addr_sensortype2, 1);	//1 = encoder
				Serial.print("\nCHANNEL 2 SENSOR SET TO 'ENCODER'.");
			}
			else if (rxchars[0] == 'h' || rxchars[0] == 'H'){
				EEPROM.write(addr_sensortype2, 2);	//2 = hall
				Serial.print("\nCHANNEL 2 SENSOR SET TO 'HALL'.");
			}
			else {
				Serial.println("\nInvalid entry.");
			}	
			Serial.println("\n\n\n\n");
			delay(2000);		
			goto ch2menu;
		}
		//**1: Set current (Amperes) to FIXED or DYNAMIC
		else if (rxchars[0] == '1'){	
			Serial.println("Set channel 2 current limiting mode. Resultant power depends on input voltage according to P = IV:");
			Serial.println("   f: Use FIXED current (default)");
			Serial.println("   d: Vary current DYNAMICALLY up to a maximum (encoder or hall sensor required)");
			Serial.println("\n\n>>\n");
			SerialReceiveLoop();
			if (rxchars[0] == 'f' || rxchars[0] == 'F'){
				EEPROM.write(addr_currentmode2, 0);	//0 = fixed 
				Serial.println("\n>>Enter channel 2 fixed current in mA (e.g. enter \"250\" for .25A):");	
				PROMPT_I2Entry();
				if (exitflag == true){
					delay(2000);
					goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
				}
				Serial.print("\nCHANNEL 2 FIXED CURRENT SET TO ");
				Serial.print(fholder);
				Serial.println("A.");
			}
			else if (rxchars[0] == 'd' || rxchars[0] == 'D'){
				sensortype2 = EEPROM.read(addr_sensortype2);
				if (sensortype2 == 1 || sensortype2 == 2){
					EEPROM.write(addr_currentmode2, 1);	//1 = dynamic 
					Serial.println("\n>>Enter channel 2 maximum dynamic current in mA (e.g. enter \"250\" for .25A):");
					PROMPT_I2Entry();
					if (exitflag == true){
						delay(2000);
						goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
					}
					Serial.print("\nCHANNEL 2 MAXIMUM CURRENT SET TO ");
					Serial.print(fholder);
					Serial.println("A.");
				}
				else {
					Serial.println("\nSensor type must be either ENCODER or HALL to use this mode.\n");
					Serial.println("MODE NOT SET.\n\n");
				}			
			}
			else {
				Serial.println("\nInvalid entry.");
			}
			Serial.println("\n\n\n\n");
			delay(2000);		
			goto ch2menu;
		}
		//**2: Set resting current (Amperes) for channel 2
		else if (rxchars[0] == '2'){
			Serial.println("Set channel 2 rest current (current when motor is static).");
			Serial.println("When in DYNAMIC current limiting mode this enforces a minimum current.");
			Serial.println("Default behavior for FIXED current limiting mode is to *not* reduce current when at rest.");
			Serial.println("Default behavior for DYNAMIC current limiting mode is 10%% of the maximum current.");  
			Serial.println("0 is a valid entry.");
			Serial.println("\n>>Enter channel 2 rest current in mA (e.g. enter \"250\" for .25A):");
			PROMPT_minI2Entry();
			if (exitflag == true){
				delay(2000);
				goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			Serial.print("\nCHANNEL 2 REST/MINIMUM CURRENT SET TO ");
			Serial.print(fholder);
			Serial.println("A.");
			Serial.println("\n\n\n\n");
			delay(2000);		
			goto ch2menu;
		}	
		//**3: Set command type to VELOCITY or POSITION
		else if (rxchars[0] == '3'){
			Serial.println("Set channel 2 input (command) type to velocity or position:");
			Serial.println("   v: VELOCITY (default)");
			Serial.println("   p: POSITION");
			Serial.println("\n\n>>\n");
			SerialReceiveLoop();
			if (rxchars[0] == 'v' || rxchars[0] == 'V'){
				EEPROM.write(addr_commandmode2, 0);	//0 = velocity
				Serial.print("\nCHANNEL 2 COMMAND INPUT SET TO 'VELOCITY'.");
			}			
			else if (rxchars[0] == 'p' || rxchars[0] == 'P'){
				EEPROM.write(addr_commandmode2, 1);	//1 = position
				Serial.print("\nCHANNEL 2 COMMAND INPUT SET TO 'POSITION'.");
			}
			else {
				Serial.println("\nInvalid entry.");
			}	
			Serial.println("\n\n\n\n");
			delay(2000);		
			goto ch2menu;
		}
		//**4: Set velocity/position GAIN
		else if (rxchars[0] == '4'){	
			Serial.println("Enter gain (velocity/position scaling coefficient) for channel 2.");
			Serial.println("This is an integer between 1 and 1000 (arbitrary units). Default is 200.");		
			Serial.println("\n\n>>\n");
			PROMPT_gain2Entry();
			if (exitflag == true){
				delay(2000);
				goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			Serial.print("\nCHANNEL 2 GAIN SET TO ");
			Serial.print(gain2);
			Serial.println("\n\n\n\n");
			delay(2000);		
			goto ch2menu;
		}
		//**5: Set waveform TORQUE SMOOTHING
		else if (rxchars[0] == '5'){
			Serial.println("Set channel 2 commutation waveform for torque smoothing:");
			Serial.println("   0: NONE. Pure sine commutation (defualt)");
			Serial.println("\n\n>>");	
			SerialReceiveLoop();
			if (rxchars[0] == '0'){
				EEPROM.write(addr_torqueprofile2, 0);	//0 = sine
				Serial.print("\nCHANNEL 2 COMMUTATION WAVEFORM SET TO 'SINE'.");
			}			
			else {
				Serial.println("\nInvalid entry.");
			}	
			Serial.println("\n\n\n\n");
			delay(2000);		
			goto ch2menu;
		}
		//**6: Display all present settings
		else if (rxchars[0] == '6'){
			CheckConfig();		//Read EEPROM
			Serial.println("Channel 2 Settings:");
			if (sensortype2 == 0){
				Serial.println("   Sensor type: NONE");
			}
			else if (sensortype2 == 1){
				Serial.println("   Sensor type: ENCODER");
			}
			else if (sensortype2 == 2){
				Serial.println("   Sensor type: HALL");
			}	

			fholder = Iset2/1000;		//Convert mA to A	
			if (currentmode2 == 0){
				Serial.print("   Current is FIXED at ");	
				Serial.print(fholder);
				Serial.println("A.");
			}
			else if (currentmode2 == 1){
				Serial.print("   Current is DYNAMIC with max set to ");
				Serial.print(fholder);
				Serial.println("A.");
			}

			fholder = minIset2/1000;		//Convert mA to A	
			Serial.print("   Rest/minimum current: ");			
			Serial.print(fholder);
			Serial.println("A.");

			if (commandmode2 == 0){
				Serial.println("   Command type: VELOCITY");
			}
			else if (commandmode2 == 1){
				Serial.println("   Command type: POSITION");
			}

			Serial.print("   command gain: ");
			Serial.println(gain2);

			if (torqueprofile2 == 0){
				Serial.println("   Commutation waveform: SINE");
			}
			Serial.println("\n\n\n\n");
			delay(2000);
			goto ch2menu;			
		}
		//**m: Return to main menu
		else if (rxchars[0] == 'm'){
			goto mainmenu;
		}
	}
	else if (rxchars[0] == '3'){
		Serial.println("Not available.");
	}
	else if (rxchars[0] == '4'){
		Serial.println("\nRunning motor on channel 1...");
		digitalWrite(U1_En, HIGH);    //Enable each phase
		digitalWrite(V1_En, HIGH);
		digitalWrite(W1_En, HIGH);
		phaseindex_1 = 0;
		TorqueCal1();
	}
	else if (rxchars[0] == '5'){
		Serial.println("\nRunning motor on channel 2...");
		digitalWrite(U2_En, HIGH);    //Enable each phase
		digitalWrite(V2_En, HIGH);
		digitalWrite(W2_En, HIGH);
		phaseindex_2 = 0;
		TorqueCal2();
	}

}




