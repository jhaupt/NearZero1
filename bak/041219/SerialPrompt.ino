void PROMPT_I1Entry(){			
	SerialReceiveLoop();
	I_set_1 = atoi(rxchars);	//Convert rxchars to an integer 
	fholder = I_set_1/1000;		//Convert mA to A, ONLY used for displaying value in Amps via terminal
	if (I_set_1 > 5000){
		Serial.println("\nMaximum allowed current is 5A. CURRENT NOT SET.\n");
		Serial.println("Returning to main menu...\n\n");
		jumptomain = true;
		delay(2000);
		return;	
	}
	else if (I_set_1 == 0){
		Serial.println("\nWe don't take kindly to invalid entries 'round these parts. CURRENT NOT SET.\n");
		Serial.println("Returning to main menu...\n\n");
		jumptomain = true;		
		delay(2000);
		return;
	}
	//Integers stored in the EEPROM must be <=255, so we split our numbers in two, storing 2 digits in one EEPROM register and 2 in another.
	if (I_set_1 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = I_set_1;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(I_set_1/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = I_set_1 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_Iset1l, lhlf);   //write the left two digits of I_set_1 to the address location given by addr_.
	EEPROM.write(addr_Iset1r, rhlf);	//etc.		
}

void PROMPT_I2Entry(){			
	SerialReceiveLoop();
	I_set_2 = atoi(rxchars);	//Convert rxchars to an integer 
	fholder = I_set_2/1000;		//Convert mA to A, ONLY used for displaying value in Amps via terminal
	if (I_set_2 > 5000){
		Serial.println("\nMaximum allowed current is 5A. CURRENT NOT SET.\n");
		Serial.println("Returning to main menu...\n\n");
		jumptomain = true;
		delay(2000);
		return;		//Yes that's a goto. FRAK modern programming sensibilities. 
	}
	else if (I_set_2 == 0){
		Serial.println("\nInvalid entry. CURRENT NOT SET.\n");
		Serial.println("Returning to main menu...\n\n");
		jumptomain = true;		
		delay(2000);
		return;
	}
	//Integers stored in the EEPROM must be <=255, so we split our numbers in two, storing 2 digits in one EEPROM register and 2 in another.
	if (I_set_2 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = I_set_2;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(I_set_2/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = I_set_2 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_Iset2l, lhlf);   //write the left two digits of I_set_1 to the address location given by addr_.
	EEPROM.write(addr_Iset2r, rhlf);	//etc.		
}

void PROMPT_vcal1Entry(){			
	SerialReceiveLoop();
	vcal_1 = atoi(rxchars);	//Convert rxchars to an integer 
	if (vcal_1 > 1000){
		Serial.println("\nMaximum allowed multiplier is 1000. GAIN NOT SET.\n");
		Serial.println("Returning to main menu...\n\n");
		jumptomain = true;
		delay(2000);
		return;		//Yes that's a goto. FRAK modern programming sensibilities. 
	}
	else if (vcal_1 == 0){
		Serial.println("\nWARNING: Settubg gain to 0 is allowed, but the motor will not respont to commands.\n");
		delay(2000);
	}
	if (vcal_1 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = vcal_1;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(vcal_1/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = vcal_1 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_vcal1l, lhlf);   
	EEPROM.write(addr_vcal1r, rhlf);	
}

void PROMPT_vcal2Entry(){			
	SerialReceiveLoop();
	vcal_2 = atoi(rxchars);	//Convert rxchars to an integer 
	if (vcal_2 > 1000){
		Serial.println("\nMaximum allowed multiplier is 1000. GAIN NOT SET.\n");
		Serial.println("Returning to main menu...\n\n");
		jumptomain = true;
		delay(2000);
		return;		//Yes that's a goto. FRAK modern programming sensibilities. 
	}
	else if (vcal_2 == 0){
		Serial.println("\nWARNING: Settubg gain to 0 is allowed, but the motor will not respont to commands.\n");
		delay(2000);
	}
	if (vcal_2 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = vcal_2;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(vcal_2/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = vcal_2 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_vcal2l, lhlf);   
	EEPROM.write(addr_vcal2r, rhlf);	
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
	jumptomain = false;
	Serial.println("\n\n\n");		//padding
	Serial.println("~~Welcome to the NearZero Configuration Terminal~~");
	Serial.println("              Firmware version 1.0\n");
	Serial.println("Configuration Settings:\n");
	Serial.println("0: Configuration Wizard (verbose walk-through of options 1-6)");
	Serial.println("1: Set or check ch1 mode & power");
	Serial.println("2: Set or check ch2 mode & power");
	Serial.println("3: Set or check ch1 anti-cogging profile");
	Serial.println("4: Set or check ch2 anti-cogging profile");
	Serial.println("5: Set or check ch1 velocity multiplier (gain)");
	Serial.println("6: Set or check ch2 velocity multiplier (gain)");
	Serial.println("\nOther Options:\n");
	Serial.println("7: Control motors using this terminal");
	Serial.println("8: Demo mode");
	Serial.println("\n\n>>\n");
	SerialReceiveLoop();
	//**0: Configuration Wizard
	if (rxchars[0] == '0'){
		Serial.println("Wizard:"); 
	}
	//**1: Set or check ch1 mode & power
	else if (rxchars[0] == '1'){	
		Serial.println("Set or check channel 1 mode:\n");
		Serial.println("1: Use open-loop control with fixed current (no encoder)");
		Serial.println("2: Use closed-loop control with fixed current (with encoder)");
		Serial.println("3: Use closed-loop control and vary current dynamically (with encoder)");
		Serial.println("4: Display present current setting for channel 1");
		Serial.println("m: Return to main menu\n\n>>");
		SerialReceiveLoop();
		//****1:1: Use open-loop control with fixed current (no encoder)
		if (rxchars[0] == '1'){	
			Serial.println("\n\nOPEN LOOP MODE SET FOR CHANNEL 1.\n");
			//stuff to set mode
			Serial.println(">>Enter fixed current for channel 1 in mA (e.g. enter \"250\" for .25A):");		
			PROMPT_I1Entry();	
			if (jumptomain == true){
				goto mainmenu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			Serial.print("\nCHANNEL 1 FIXED CURRENT SET TO ");
			Serial.print(fholder);
			Serial.println(" A.\n\nReturning to main menu...\n\n");
			delay(2000);		
			goto mainmenu;
		}
		//****1:2: Use closed-loop control with fixed current (with encoder feedback)
		else if (rxchars[0] == '2'){
			Serial.println("1>> CLOSED LOOP MODE SET FOR CHANNEL 1. ");		
			//stuff to set mode
			Serial.println(">>Enter fixed current for channel 1 in mA (e.g. enter \"250\" for .25A):");		
			PROMPT_I1Entry();	
			if (jumptomain == true){
				goto mainmenu;
			}
			Serial.print("\nCHANNEL 1 FIXED CURRENT SET TO ");
			Serial.print(fholder);
			Serial.println(" A.\n\nReturning to main menu...\n\n");
			delay(2000);		
			goto mainmenu;
		}
		//****1:3: Use closed-loop control and vary current dynamically (with encoder feedback)
		else if (rxchars[0] == '3'){
			Serial.println("1>> CLOSED LOOP MODE SET FOR CHANNEL 1. ");		
			//stuff to set mode
			Serial.println(">>Enter never-exceed current for channel 1 in mA (e.g. enter \"250\" for .25A):");		
			PROMPT_I1Entry();	
			if (jumptomain == true){
				goto mainmenu;
			}
			Serial.print("\nCHANNEL 1 DYNAMIC CURRENT SET TO MAX OUT AT ");
			Serial.print(fholder);
			Serial.println(" A.\n\nReturning to main menu...\n\n");
			delay(2000);		
			goto mainmenu;
		}
		//****1:4: Display present current setting for channel 1
		else if (rxchars[0] == '4'){
			CheckConfig();		//Read EEPROM
			fholder = I_set_1/1000;		//Convert mA to A	
			Serial.print("\n1>> Channel 1 fixed/max current is presently set to ");		
			Serial.print(fholder);
			Serial.print("A.");
			delay(2000);
			goto mainmenu;			
		}
		//****1:m: Return to main menu
		else if (rxchars[0] == 'm'){
			goto mainmenu;
		}
	}
	//***"2: Set or check ch2 mode & power"
	else if (rxchars[0] == '2'){
		Serial.println("Set or check channel 2 mode:\n");
		Serial.println("1: Use open-loop control with fixed current (no encoder)");
		Serial.println("2: Use closed-loop control with fixed current (with encoder)");
		Serial.println("3: Use closed-loop control and vary current dynamically (with encoder)");
		Serial.println("4: Display present current setting for channel 2");
		Serial.println("m: Return to main menu\n\n>>");
		SerialReceiveLoop();
		//****2:1: Use open-loop control with fixed current (no encoder)
		if (rxchars[0] == '1'){	
			Serial.println("\n\nOPEN LOOP MODE SET FOR CHANNEL 2.\n");
			//stuff to set mode
			Serial.println(">>Enter fixed current for channel 2 in mA (e.g. enter \"250\" for .25A):");		
			PROMPT_I2Entry();	
			if (jumptomain == true){
				goto mainmenu;		
			}
			Serial.print("\nCHANNEL 2 FIXED CURRENT SET TO ");
			Serial.print(fholder);
			Serial.println(" A.\n\nReturning to main menu...\n\n");
			delay(2000);		
			goto mainmenu;
		}
		//****2:2: Use closed-loop control with fixed current (with encoder feedback)
		else if (rxchars[0] == '2'){
			Serial.println("1>> CLOSED LOOP MODE SET FOR CHANNEL 2. ");		
			//stuff to set mode
			Serial.println(">>Enter fixed current for channel 2 in mA (e.g. enter \"250\" for .25A):");		
			PROMPT_I2Entry();	
			if (jumptomain == true){
				goto mainmenu;
			}
			Serial.print("\nCHANNEL 2 FIXED CURRENT SET TO ");
			Serial.print(fholder);
			Serial.println(" A.\n\nReturning to main menu...\n\n");
			delay(2000);		
			goto mainmenu;
		}
		//****2:3: Use closed-loop control and vary current dynamically (with encoder feedback)
		else if (rxchars[0] == '3'){
			Serial.println("1>> CLOSED LOOP MODE SET FOR CHANNEL 2. ");		
			//stuff to set mode
			Serial.println(">>Enter never-exceed current for channel 2 in mA (e.g. enter \"250\" for .25A):");		
			PROMPT_I2Entry();	
			if (jumptomain == true){
				goto mainmenu;
			}
			Serial.print("\nCHANNEL 2 DYNAMIC CURRENT SET TO MAX OUT AT ");
			Serial.print(fholder);
			Serial.println(" A.\n\nReturning to main menu...\n\n");
			delay(2000);		
			goto mainmenu;
		}
		//****2:4: Display present current setting for channel 2
		else if (rxchars[0] == '4'){
			CheckConfig();		//Read EEPROM
			fholder = I_set_2/1000;		//Convert mA to A	
			Serial.print("\n1>> Channel 2 fixed/max current is presently set to ");		
			Serial.print(fholder);
			Serial.print("A.");
			delay(2000);
			goto mainmenu;			
		}
		//****2:m: Return to main menu
		else if (rxchars[0] == 'm'){
			goto mainmenu;
		}
	}
	//***"3: Set or check ch1 anti-cogging profile"
	else if (rxchars[0] == '3'){
		Serial.println("Select anti-cogging profile for channel 1:");
	}
	//***"4: Set or check ch2 anti-cogging profile"
	else if (rxchars[0] == '4'){
		Serial.println("Select anti-cogging profile for channel 2:");
	}
	//***"5: Set or check ch1 velocity multiplier"
	else if (rxchars[0] == '5'){
		Serial.println("Set or check channel 1 velocity multiplier (gain):\n");
		Serial.println("1: Set the velocity multiplier for channel 1");
		Serial.println("2: Display present velocity multiplier for channel 1");
		Serial.println("m: Return to main menu\n\n>>");
		SerialReceiveLoop();
		//****5:1: Set the velocity multiplier for channel 1
		if (rxchars[0] == '1'){	
			Serial.println(">>Enter velocity multiplier for channel 1. This is an integer between 1 and 1000 which sets the velocity gain. Default is 200:");		
			PROMPT_vcal1Entry();	
			if (jumptomain == true){
				goto mainmenu;		
			}
			Serial.print("\nCHANNEL 1 VELOCITY MULTIPLIER SET TO ");
			Serial.print(vcal_1);
			Serial.println("\n\nReturning to main menu...\n\n");
			delay(2000);		
			goto mainmenu;
		}
		//****5:2: Display present velocity for channel 1
		else if (rxchars[0] == '2'){
			CheckConfig();		//Read EEPROM
			Serial.print("\n1>> Channel 1 velocity multiplier presently set to ");		
			Serial.print(vcal_1);
			delay(2000);
			goto mainmenu;			
		}
		//****5:m: Return to main menu
		else if (rxchars[0] == 'm'){
			goto mainmenu;
		}		
	}
	//***"6: Set or check ch2 velocity multiplier"
	else if (rxchars[0] == '6'){
		Serial.println("Set or check channel 2 velocity multiplier (gain):\n");
		Serial.println("1: Set the velocity multiplier for channel 2");
		Serial.println("2: Display present velocity multiplier for channel 2");
		Serial.println("m: Return to main menu\n\n>>");
		SerialReceiveLoop();
		//****6:1: Set the velocity multiplier for channel 2
		if (rxchars[0] == '1'){	
			Serial.println(">>Enter velocity multiplier for channel 2. This is an integer between 1 and 1000 which sets the velocity gain. Default is 200:");		
			PROMPT_vcal2Entry();	
			if (jumptomain == true){
				goto mainmenu;		
			}
			Serial.print("\nCHANNEL 2 VELOCITY MULTIPLIER SET TO ");
			Serial.print(vcal_2);
			Serial.println("\n\nReturning to main menu...\n\n");
			delay(2000);		
			goto mainmenu;
		}
		//****6:2: Display present velocity for channel 2
		else if (rxchars[0] == '2'){
			CheckConfig();		//Read EEPROM
			Serial.print("\n>> Channel 2 velocity multiplier presently set to ");		
			Serial.print(vcal_2);
			delay(2000);
			goto mainmenu;			
		}
		//****6:m: Return to main menu
		else if (rxchars[0] == 'm'){
			goto mainmenu;
		}		
	}
	//***"7: Control motors using this terminal"		<<MAYBE NOT FOR 1ST RELEASE
	else if (rxchars[0] == '7'){
		Serial.println("stuff");
	}
	//***"8: Demo mode"									<<MAYBE NOT FOR 1ST RELEASE
	else if (rxchars[0] == '8'){
		Serial.println("stuff");
	}
	//***"42: (hidden) Calibration capture menu
	else if (rxchars[0] == '4' && rxchars[1] == '2'){
		Serial.println("\nRunning motor on channel 1 for external velocity capture");
		digitalWrite(U1_En, HIGH);    //Enable each phase
		digitalWrite(V1_En, HIGH);
		digitalWrite(W1_En, HIGH);
		phaseindex_U1 = 0;
		TorqueCal();
	}
}


