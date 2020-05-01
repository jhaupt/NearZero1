void PROMPT_maxI1Entry(){			
	SerialReceiveLoop();
	maxIset1 = atoi(rxchars);	//Convert rxchars to an integer 
	fholder = maxIset1/1000;		//Convert mA to A, ONLY used for displaying value in Amps via terminal
	if (maxIset1 > 1000){
		CheckConfig();
		fholder = maxIset1/1000;		//Convert mA to A
		Serial.print(F("Maximum PWM current is 1A. CURRENT LEFT AT "));
		Serial.print(fholder);
		Serial.println(F("A"));
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;		//Yes that's a goto. FRAK modern programming sensibilities. 
	}
	else if (maxIset1 > 0 && maxIset1 <= 1000){} 	//Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		fholder = maxIset1/1000;		//Convert mA to A
		Serial.print(F("MAX PWM CURRENT LEFT AT "));
		Serial.print(fholder);
		Serial.println(F("A"));
		Serial.println(F("\n\n"));
		exitflag = true;		
		delay(2000);
		return;
	}
	//Integers stored in the EEPROM must be <=255, so we split our numbers in two, storing 2 digits in one EEPROM register and 2 in another.
	if (maxIset1 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = maxIset1;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(maxIset1/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = maxIset1 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_maxIset1l, lhlf);   //write the left two digits of Iset1 to the address location given by addr_.
	EEPROM.write(addr_maxIset1r, rhlf);	//etc.		
}

void PROMPT_maxI2Entry(){			
	SerialReceiveLoop();
	maxIset2 = atoi(rxchars);	//Convert rxchars to an integer 
	fholder = maxIset2/1000;		//Convert mA to A, ONLY used for displaying value in Amps via terminal
	if (maxIset2 > 1000){
		CheckConfig();
		fholder = maxIset2/1000;		//Convert mA to A
		Serial.print(F("Maximum PWM current is 1A. CURRENT LEFT AT "));
		Serial.print(fholder);
		Serial.println(F("A"));
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;		//Yes that's a goto. FRAK modern programming sensibilities. 
	}
	else if (maxIset2 > 0 && maxIset2 <= 1000){} 	//Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		fholder = maxIset2/1000;		//Convert mA to A
		Serial.print(F("MAX PWM CURRENT LEFT AT "));
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
	if (minIset1 > 1000){
		CheckConfig();
		fholder = minIset1/1000;		//Convert mA to A
		Serial.print(F("Maximum PWM current is 1A. CURRENT LEFT AT "));
		Serial.print(fholder);
		Serial.println(F("A"));
		Serial.println(F("\n\n"));
		exitflag = true;
		return;	
	}
	else if (minIset1 == 0) {minIset1 = 1;}	//If it was zero enter 1. We can't have zero, but 1 is almost zero so just as good.
	else if (minIset1 > 0 && minIset1 <= 1000){} //Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		minIset1 = maxIset1;		//The default is to make the resting current the same as the non-resting current.
		Serial.print(F("THE SAME CURRENT WILL BE USED FOR RESTING AND NON-RESTING PWM OPERATION"));
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
	if (minIset2 > 1000){
		CheckConfig();
		fholder = minIset2/1000;		//Convert mA to A
		Serial.print(F("Maximum current is 1A. CURRENT LEFT AT "));
		Serial.print(fholder);
		Serial.println(F("A"));
		Serial.println(F("\n\n"));
		exitflag = true;
		return;	
	}
	else if (minIset2 == 0) {minIset2 = 1;}	//If it was zero enter 1. We can't have zero, but 1 is almost zero so just as good.
	else if (minIset2 > 0 && minIset2 <= 1000){} //Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		minIset2 = maxIset2;		//The default is to make the resting current the same as the non-resting current.
		Serial.print(F("THE SAME CURRENT WILL BE USED FOR RESTING AND NON-RESTING PWM OPERATION"));
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
		CheckConfig();
		Serial.print(F("Maximum gain is 1000. PWM GAIN LEFT AT "));
		Serial.println(gain1);
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;		
	}
	else if (gain1 > 0 && gain1 <= 1000){} //Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		Serial.print(F("PWM GAIN LEFT AT "));
		Serial.println(gain1);
		Serial.println(F("\n\n"));
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
		Serial.print(F("Maximum gain is 1000. PWM GAIN LEFT AT "));
		Serial.println(gain2);
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;		
	}
	else if (gain2 > 0 && gain2 <= 1000){} //Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		Serial.print(F("PWM GAIN LEFT AT "));
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

void PROMPT_pwmoffset1Entry(){			
	SerialReceiveLoop();
	pwmoffset1 = atoi(rxchars);	//Convert rxchars to an integer 
	if (pwmoffset1 > 10000){
		CheckConfig();
		Serial.print(F("Maximum center offset is 10000. PWM CENTER LEFT AT "));
		Serial.println(pwmoffset1);
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;		
	}
	else if (pwmoffset1 >= 0 && pwmoffset1 <= 10000){} //Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.}
	else {
		CheckConfig();
		Serial.print(F("PWM CENTER OFFSET LEFT AT "));
		Serial.println(pwmoffset1);
		Serial.println(F("\n\n"));
		exitflag = true;		
		return;
	}
	if (pwmoffset1 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = pwmoffset1;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(pwmoffset1/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = pwmoffset1 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_pwmoffset1l, lhlf);   
	EEPROM.write(addr_pwmoffset1r, rhlf);	
}

void PROMPT_pwmoffset2Entry(){			
	SerialReceiveLoop();
	pwmoffset2 = atoi(rxchars);	//Convert rxchars to an integer 
	if (pwmoffset2 > 10000){
		CheckConfig();
		Serial.print(F("Maximum center offset is 10000. PWM CENTER LEFT AT "));
		Serial.println(pwmoffset2);
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;		
	}
	else if (pwmoffset2 >= 0 && pwmoffset2 <= 10000){} //Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		Serial.print(F("PWM CENTER OFFSET LEFT AT "));
		Serial.println(pwmoffset2);
		Serial.println(F("\n\n"));
		exitflag = true;		
		return;
	}
	if (pwmoffset2 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = pwmoffset2;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(pwmoffset2/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = pwmoffset2 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_pwmoffset2l, lhlf);   
	EEPROM.write(addr_pwmoffset2r, rhlf);	
}

void PROMPT_tsphase1Entry(){			
	SerialReceiveLoop();
	tsphase1 = atoi(rxchars);	//Convert rxchars to an integer 
	if (tsphase1 > 6283){
		CheckConfig();
		Serial.print(F("Maximum phase is 2pi (6283). PHASE LEFT AT "));
		Serial.println(tsphase1);
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;		
	}
	else if (tsphase1 > 0 && tsphase1 <= 6283){}		//Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		Serial.println(F("PHASE SET TO 0\n"));
		Serial.println(F("\n"));
		exitflag = true;		
		return;
	}
	if (tsphase1 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = tsphase1;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(tsphase1/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = tsphase1 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_tsphase1l, lhlf);   
	EEPROM.write(addr_tsphase1r, rhlf);	
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

void PROMPT_tscoeff1Entry(){			
	SerialReceiveLoop();
	tscoeff1 = atoi(rxchars);	//Convert rxchars to an integer 
	if (tscoeff1 > 254){
		CheckConfig();
		Serial.print(F("Maximum value is 254. AGGRESSIVENESS LEFT AT "));
		Serial.println(tscoeff1);
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;	
	}
	else if (tscoeff1 > 0 && tscoeff1 <255){		//If it's a valid evtry
		EEPROM.write(addr_tscoeff1, tscoeff1);   	
	}
	else {		//Then it's invalid in some other way
		CheckConfig();
		Serial.print(F("AGGRESSIVENESS LEFT AT "));
		Serial.println(tscoeff1);
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;	
	}	
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

void PROMPT_accel1Entry(){			
	SerialReceiveLoop();
	accel1 = atoi(rxchars);	//Convert rxchars to an integer 
	if (accel1 > 10000){
		CheckConfig();
		Serial.print(F("Maximum acceleration is 10,000. ACCELERATION LEFT AT "));
		Serial.println(accel1);
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;		
	}
	else if (accel1 > 0 && accel1 <= 10000){}	//Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		Serial.print(F("ACCELERATION LEFT AT "));
		Serial.println(accel1);
		Serial.println(F("\n\n"));
		exitflag = true;		
		return;
	}
	if (accel1 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = accel1;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(accel1/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = accel1 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_accel1l, lhlf);   
	EEPROM.write(addr_accel1r, rhlf);	
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

void PROMPT_maxslewvel1Entry(){			
	SerialReceiveLoop();
	maxslewvel1 = atoi(rxchars);	//Convert rxchars to an integer 
	if (maxslewvel1 > 1000){
		CheckConfig();
		Serial.print(F("Maximum MAX SLEW RATE is 1,000. MAX SLEW RATE LEFT AT "));
		Serial.println(maxslewvel1);
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;		
	}
	else if (maxslewvel1 > 0 && maxslewvel1 <= 1000){}	//Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		Serial.print(F("MAX SLEW RATE LEFT AT "));
		Serial.println(maxslewvel1);
		Serial.println(F("\n\n"));
		exitflag = true;		
		return;
	}
	if (maxslewvel1 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = maxslewvel1;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(maxslewvel1/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = maxslewvel1 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_maxslewvel1l, lhlf);   
	EEPROM.write(addr_maxslewvel1r, rhlf);	
}

void PROMPT_maxslewvel2Entry(){			
	SerialReceiveLoop();
	maxslewvel2 = atoi(rxchars);	//Convert rxchars to an integer 
	if (maxslewvel2 > 1000){
		CheckConfig();
		Serial.print(F("Maximum MAX SLEW RATE is 1,000. MAX SLEW RATE LEFT AT "));
		Serial.println(maxslewvel2);
		Serial.println(F("\n\n"));
		exitflag = true;
		delay(2000);
		return;		
	}
	else if (maxslewvel2 > 0 && maxslewvel2 <= 1000){}	//Don't do anything. This is just here so we can have the below "else" to detect an invalid entry.
	else {
		CheckConfig();
		Serial.print(F("MAX SLEW RATE LEFT AT "));
		Serial.println(maxslewvel2);
		Serial.println(F("\n\n"));
		exitflag = true;		
		return;
	}
	if (maxslewvel2 < 100){		//If it's a 1 or 2 digit number
		lhlf = 0;	//The left half of the four digit number is 0
		rhlf = maxslewvel2;		//And the right half is just the number
	}
	else {		//If it's a 3 or 4 digit number
		lhlf = floor(maxslewvel2/100);		//The left half is the leading digit of a 3 digit number or leading 2 digits of a 4 digit number.
		rhlf = maxslewvel2 - (lhlf*100);	//The right half is the trailing two digits of the number.
	}
	EEPROM.write(addr_maxslewvel2l, lhlf);   
	EEPROM.write(addr_maxslewvel2r, rhlf);	
}

void SerialReceiveLoop(){
	int ndx = 0;
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

	//MAIN MENU
	mainmenu:
	exitflag = false;
	Serial.println(F("\n\n\n"));
	Serial.println(F("~~~~Welcome to the NearZero Configuration Terminal~~~~"));
	Serial.println(F("         Firmware version 2.0 -- by J. Haupt"));
	Serial.println(F("       CHANGES TAKE EFFECT AFTER CYCLING POWER"));
	Serial.println(F("                   or RESETTING\n"));
	Serial.println(F("1: Channel 1 configuration"));
	Serial.println(F("2: Channel 2 configuration"));
	Serial.println(F("3: Set I2C Address"));
	Serial.println(F("4: List current settings"));
	Serial.println(F("5: Reset factory defaults"));
	Serial.println(F("q: Quit/restart (use after changing INPUT SELECT jumper as desired)"));
	Serial.println(F("\n\n>>\n"));
	SerialReceiveLoop();

	//CHANNEL 1 CONFIGURATION
	if (rxchars[0] == '1'){	
		ch1menu:
		exitflag = false;
		Serial.println(F("Channel 1 General Settings:"));
		Serial.println(F("1: Set SENSOR type (ENCODER or HALL)"));
		Serial.println(F("2: Set maximum position/servo SLEW RATE"));
		Serial.println(F("3: Set position/servo ACCELERATION")); 
		Serial.println(F("4: Set directionality (NORMAL or REVERSE)"));
		Serial.println(F("5: Set waveform TORQUE SMOOTHING (experimental)"));
		Serial.println(F(" "));
		Serial.println(F("Channel 1 PWM Settings:"));
		Serial.println(F("6: Set PWM running/resting CURRENT"));
		Serial.println(F("7: Set PWM command type (VELOCITY, POSITION, or SERVO)"));
		Serial.println(F("8: Set PWM command GAIN"));
		Serial.println(F("9: Set PWM center OFFSET or to center AUTO"));
		Serial.println(F(" "));
		Serial.println(F("t: Test/run channel 1 motor"));
		Serial.println(F("m: Return to main menu"));
		Serial.println(F("\n\n>>"));
		SerialReceiveLoop();
		exitflag =false;
		//**Set SENSOR TYPE
		if (rxchars[0] == '1'){
			Serial.println(F("Set channel 1 sensor type:"));
			Serial.println(F("   e: ENCODER -- reports quadrature ticks (signed integers counted since powerup"));
			Serial.println(F("      over I2C for external closed-loop control -- also needed for SERVO command mode"));
			Serial.println(F("   h: HALL -- reports hall ticks (signed integers counted since powerup)"));	
			Serial.println(F("      over I2C for external closed-loop control"));
			Serial.println(F("\n   (Or strike 'Enter' to leave as is)")); 
			Serial.println(F("\n\n>>"));	
			SerialReceiveLoop();
			if (rxchars[0] == 'e' || rxchars[0] == 'E'){
				EEPROM.write(addr_sensortype1, 0);	//0 = none
				Serial.println(F("CHANNEL 1 SENSOR SET TO 'ENCODER'"));
			}			
			else if (rxchars[0] == 'h' || rxchars[0] == 'H'){
				exitflag = false;
				EEPROM.write(addr_sensortype1, 1);	//1 = encoder
				Serial.print(F("CHANNEL 1 SENSOR SET TO 'HALL'"));	
			}
			else{
				Serial.print(F("SENSOR TYPE NOT CHANGED"));
			}	
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch1menu;
		}
		//**Set MAX SLEW RATE
		else if (rxchars[0] == '2'){	
			Serial.println(F("Set channel 1 position/servo maximum slew rate."));
			Serial.println(F("\n   This is an integer in arbitrary units between 1 and 1000. Must be low enough to both"));
			Serial.println(F("   prevent the motor from stalling and to allow accurate tracking of an encoder:")); 	
			Serial.println(F("   Strike 'Enter' to leave as is:")); 	
			Serial.println(F("\n\n>>"));
			PROMPT_maxslewvel1Entry();
			if (exitflag == true){
				delay(2000);
				goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			else {
				Serial.print(F("CHANNEL 1 MAXIMUM SLEW RATE SET TO "));
				Serial.print(maxslewvel1);
				Serial.println(F("\n\n\n\n"));
				delay(2000);		
				goto ch1menu;
			}
		}
		//**Set ACCELERATION
		else if (rxchars[0] == '3'){	
			Serial.println(F("Set channel 1 position/servo acceleration."));
			Serial.println(F("\n   This is an integer in arbitrary units between 1 and 9999."));
			Serial.println(F("   Enter 10000 to turn off acceleration dynamics entirely."));
			Serial.println(F("   Strike 'Enter' to leave as is:")); 	
			Serial.println(F("\n\n>>"));
			PROMPT_accel1Entry();
			if (exitflag == true){
				delay(2000);
				goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			if (accel1 == 10000){
				Serial.println(F("CHANNEL 1 ACCELERATION DYNAMICS TURNED OFF"));
				delay(2000);
				goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			else {
				Serial.print(F("CHANNEL 1 ACCELERATION SET TO "));
				Serial.print(accel1);
				Serial.println(F("\n\n\n\n"));
				delay(2000);		
				goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
		}
		//**Set directionality to NORMAL or REVERSE
		else if (rxchars[0] == '4'){
			Serial.println(F("Set channel 1 directionality:"));
			Serial.println(F("   n: NORMAL (default)"));
			Serial.println(F("   r: REVERSE "));
			Serial.println(F("\n   (Or strike 'Enter' to leave as is)")); 
			Serial.println(F("\n\n>>"));
			SerialReceiveLoop();
			if (rxchars[0] == 'n' || rxchars[0] == 'N'){
				EEPROM.write(addr_dir1, 0);	//0 = NORMAL
				Serial.print(F("CHANNEL 1 DIRECTIONALITY SET TO 'NORMAL'"));
			}			
			else if (rxchars[0] == 'r' || rxchars[0] == 'R'){
				EEPROM.write(addr_dir1, 1);	//1 = REVERSE
				Serial.print(F("CHANNEL 1 DIRECTIONALITY SET TO 'REVERSE'"));
			}
			else {
				Serial.println(F("DIRECTIONALITY NOT CHANGED"));
			}	
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch1menu;
		}
		//**Set waveform TORQUE SMOOTHING
		else if (rxchars[0] == '5'){
			Serial.println(F("Set channel 1 commutation waveform for torque smoothing:"));
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
				EEPROM.write(addr_torqueprofile1, 0);	//0 = sine
				Serial.print(F("CHANNEL 1 TORQUE SMOOTHING IS OFF (PURE SINE COMMUTATION)"));
			}	
			else if (rxchars[0] == '1'){
				EEPROM.write(addr_torqueprofile1, 1);	
				Serial.print(F("CHANNEL 1 TORQUE SMOOTHING IS ON"));
			}		
			else if (rxchars[0] == '2'){
				Serial.println(F("Enter aggressiveness parameter. This is an integer between 1 and 254 (arbitarary units)"));
				Serial.println(F("   or strike 'Enter' to leave as is:")); 
				Serial.println(F("\n\n>>"));
				PROMPT_tscoeff1Entry();
				if (exitflag == true){
					delay(2000);
					goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
				}
				Serial.print(F("CHANNEL 1 TORQUE SMOOTHING AGGRESSIVENESS SET TO "));
				Serial.println(tscoeff1);
				if (torqueprofile1 == 0){
					Serial.println(F("(only applies when torque smoothing is ON)"));
				}
			}
			else if (rxchars[0] == '3'){
				Serial.println(F("Enter phase/offset of correction function. This is a number between 0 and 2pi (6.283) in thousands"));
				Serial.println(F("(i.e. enter a number between 0 and 6283) or strike 'Enter' to leave as is:"));
				Serial.println(F("\n\n>>"));
				PROMPT_tsphase1Entry();
				if (exitflag == true){
					delay(2000);
					goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
				}
				Serial.print(F("CHANNEL 1 TORQUE SMOOTHING PHASE SET TO "));
				fholder = tsphase1/1000;		//Convert to decimal form
				Serial.println(fholder);
				if (torqueprofile1 == 0){
					Serial.println(F("(only applies when torque smoothing is ON)"));
				}				
			}
	
			else {
				Serial.println(F("TORQUE SMOOTHING NOT CHANGED"));
			}	
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch1menu;
		}

		//**Set Power
		else if (rxchars[0] == '6'){	
			Serial.println(F("Enter channel 1 PWM running current in mA (e.g. enter \"250\" for .25A)."));	
			Serial.println(F("\n   This sets the current for when the motion is in motion.")); 
			Serial.println(F("   Or strike 'Enter' to leave as is:")); 
			Serial.println(F("\n\n>>"));
			PROMPT_maxI1Entry();
			if (exitflag == true){
				delay(2000);
				goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			Serial.print(F("CHANNEL 1 RUNNING CURRENT SET TO "));
			Serial.print(fholder);
			Serial.println(F("A"));
			Serial.println(F("\nEnter channel 1 PWM resting current in mA (e.g. enter \"100\" for .1A)"));
			Serial.println(F("   Or strike 'Enter' to not reduce current while at rest:")); 
			Serial.println(F("\n\n>>"));
			PROMPT_minI1Entry();
			if (exitflag == true){
				delay(2000);
				goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			Serial.print(F("CHANNEL 1 PWM RESTING CURRENT SET TO "));
			Serial.print(fholder);
			Serial.println(F("A"));
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch1menu;
		}		
		//**Set PWM command type to VELOCITY or POSITION
		else if (rxchars[0] == '7'){
			Serial.println(F("Set channel 1 to interpret PWM input as either velocity, position, or servo commands:"));
			Serial.println(F("   v: VELOCITY"));
			Serial.println(F("   p: POSITION"));
			Serial.println(F("   s: SERVO (same as POSITION but with closed-loop control) -- requires encoder"));
			Serial.println(F("\n   (Or strike 'Enter' to leave as is)")); 
			Serial.println(F("\n\n>>"));
			SerialReceiveLoop();
			if (rxchars[0] == 'v' || rxchars[0] == 'V'){
				EEPROM.write(addr_commandmode1, 0);	//0 = velocity
				Serial.print(F("CHANNEL 1 PWM INPUT SET TO 'VELOCITY'"));
			}			
			else if (rxchars[0] == 'p' || rxchars[0] == 'P'){
				EEPROM.write(addr_commandmode1, 1);	//1 = position
				Serial.print(F("CHANNEL 1 PWM INPUT SET TO 'POSITION'"));
			}
			else if (rxchars[0] == 's' || rxchars[0] == 'S'){
				EEPROM.write(addr_commandmode1, 2);	//2 = servo
				Serial.print(F("CHANNEL 1 PWM INPUT SET TO 'SERVO'"));
			}
			else {
				Serial.println(F("COMMAND TYPE NOT CHANGED"));
			}	
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch1menu;
		}
		//**Set velocity/position GAIN
		else if (rxchars[0] == '8'){	
			Serial.println(F("Set channel 1 PWM command gain."));
			Serial.println(F("   This is the scaling coefficient for velocity or position inputs when in PWM-command mode."));
			Serial.println(F("\n   Enter an integer between 1 and 1000 (arbitrary units)"));	
			Serial.println(F("   Strike 'Enter' to leave as is:")); 	
			Serial.println(F("\n\n>>"));
			PROMPT_gain1Entry();
			if (exitflag == true){
				delay(2000);
				goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			Serial.print(F("\nCHANNEL 1 PWM GAIN SET TO "));
			Serial.print(gain1);
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch1menu;
		}
		//**Set PWM CENTER
		else if (rxchars[0] == '9'){	
			Serial.println(F("Set channel 1 PWM center offset."));
			Serial.println(F("   This defines the pulse width which sets the command-neutral position."));
			Serial.println(F("\n   Enter an integer between 1 and 9000."));	
			Serial.println(F("   Enter 0 to automatically use the first detected PWM value as the command-neutral position:")); 	
			Serial.println(F("   Strike 'Enter' to leave as is:")); 	
			Serial.println(F("\n\n>>"));
			PROMPT_pwmoffset1Entry();
			if (exitflag == true){
				delay(2000);
				goto ch1menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			if (pwmoffset1 == 0) {
				Serial.print(F("\nTHE INITIAL PWM INPUT WILL BE TAKEN AS COMMAND-NEUTRAL"));
			}
			else {
				Serial.print(F("\nCHANNEL 1 PWM CENTER SET TO "));
				Serial.print(pwmoffset1);
			}
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch1menu;
		}
		//Test Motor
		else if (rxchars[0] == 't' || rxchars[0] == 'T'){
			Serial.println(F("\nRunning motor on channel 1 for about 10s..."));
			TorqueCal1();
			goto ch1menu;
		}
		//**Return to main menu
		else if (rxchars[0] == 'm'){
			goto mainmenu;
		}
	}
	//CHANNEL 2 CONFIGURATION
	if (rxchars[0] == '2'){	
		ch2menu:
		exitflag = false;
		Serial.println(F("Channel 2 General Settings:"));
		Serial.println(F("1: Set SENSOR type (ENCODER or HALL)"));
		Serial.println(F("2: Set maximum position/servo SLEW RATE"));
		Serial.println(F("3: Set position/servo ACCELERATION")); 
		Serial.println(F("4: Set directionality (NORMAL or REVERSE)"));
		Serial.println(F("5: Set waveform TORQUE SMOOTHING (experimental)"));
		Serial.println(F(" "));
		Serial.println(F("Channel 2 PWM Settings:"));
		Serial.println(F("6: Set PWM running/resting CURRENT"));
		Serial.println(F("7: Set PWM command type (VELOCITY, POSITION, or SERVO)"));
		Serial.println(F("8: Set PWM command GAIN"));
		Serial.println(F("9: Set PWM center OFFSET or to center AUTO"));
		Serial.println(F(" "));
		Serial.println(F("t: Test/run channel 2 motor"));
		Serial.println(F("m: Return to main menu"));
		Serial.println(F("\n\n>>"));
		SerialReceiveLoop();
		exitflag =false;
		//**Set SENSOR TYPE
		if (rxchars[0] == '1'){
			Serial.println(F("Set channel 2 sensor type:"));
			Serial.println(F("   e: ENCODER -- reports quadrature ticks (signed integers counted since powerup)"));
			Serial.println(F("      over I2C for external closed-loop control -- also needed for SERVO command mode"));
			Serial.println(F("   h: HALL -- reports hall ticks (signed integers counted since powerup) over I2C"));	
			Serial.println(F("      over I2C for external closed-loop control"));
			Serial.println(F("\n   (Or strike 'Enter' to leave as is)")); 
			Serial.println(F("\n\n>>"));	
			SerialReceiveLoop();
			if (rxchars[0] == 'e' || rxchars[0] == 'E'){
				EEPROM.write(addr_sensortype2, 0);
				Serial.println(F("CHANNEL 2 SENSOR SET TO 'ENCODER'"));
			}			
			else if (rxchars[0] == 'h' || rxchars[0] == 'H'){
				exitflag = false;
				EEPROM.write(addr_sensortype2, 1);
				Serial.print(F("CHANNEL 2 SENSOR SET TO 'HALL'"));	
			}
			else{
				Serial.print(F("SENSOR TYPE NOT CHANGED"));
			}	
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch2menu;
		}
		//**Set MAX SLEW RATE
		else if (rxchars[0] == '2'){	
			Serial.println(F("Set channel 2 position/servo maximum slew rate."));
			Serial.println(F("\n   This is an integer in arbitrary units between 1 and 1000. Must be low enough to both"));
			Serial.println(F("   prevent the motor from stalling and to allow accurate tracking of an encoder:")); 	
			Serial.println(F("   Strike 'Enter' to leave as is:")); 	
			Serial.println(F("\n\n>>"));
			PROMPT_maxslewvel2Entry();
			if (exitflag == true){
				delay(2000);
				goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			else {
				Serial.print(F("CHANNEL 1 MAXIMUM SLEW RATE SET TO "));
				Serial.print(maxslewvel1);
				Serial.println(F("\n\n\n\n"));
				delay(2000);		
				goto ch2menu;
			}
		}
		//**Set ACCELERATION
		else if (rxchars[0] == '3'){	
			Serial.println(F("Set channel 2 position/servo acceleration."));
			Serial.println(F("\n   This is an integer in arbitrary units between 1 and 9999."));
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
				goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			else {
				Serial.print(F("CHANNEL 2 ACCELERATION SET TO "));
				Serial.print(accel2);
				Serial.println(F("\n\n\n\n"));
				delay(2000);		
				goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
		}
		//**Set directionality to NORMAL or REVERSE
		else if (rxchars[0] == '4'){
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
		//**Set waveform TORQUE SMOOTHING
		else if (rxchars[0] == '5'){
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

		//**Set Power
		else if (rxchars[0] == '6'){	
			Serial.println(F("Enter channel 2 PWM running current in mA (e.g. enter \"250\" for .25A)."));	
			Serial.println(F("\n   This sets the current for when the motion is in motion.")); 
			Serial.println(F("   Or strike 'Enter' to leave as is:")); 
			Serial.println(F("\n\n>>"));
			PROMPT_maxI2Entry();
			if (exitflag == true){
				delay(2000);
				goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			Serial.print(F("CHANNEL 2 RUNNING CURRENT SET TO "));
			Serial.print(fholder);
			Serial.println(F("A"));
			Serial.println(F("\nEnter channel 2 PWM resting current in mA (e.g. enter \"100\" for .1A)"));
			Serial.println(F("   Or strike 'Enter' to not reduce current while at rest:")); 
			Serial.println(F("\n\n>>"));
			PROMPT_minI2Entry();
			if (exitflag == true){
				delay(2000);
				goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			Serial.print(F("CHANNEL 2 PWM RESTING CURRENT SET TO "));
			Serial.print(fholder);
			Serial.println(F("A"));
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch2menu;
		}		
		//**Set PWM command type to VELOCITY or POSITION
		else if (rxchars[0] == '7'){
			Serial.println(F("Set channel 2 to interpret PWM input as either velocity, position, or servo commands:"));
			Serial.println(F("   v: VELOCITY"));
			Serial.println(F("   p: POSITION"));
			Serial.println(F("   s: SERVO (same as POSITION but with closed-loop control) -- requires encoder"));
			Serial.println(F("\n   (Or strike 'Enter' to leave as is)")); 
			Serial.println(F("\n\n>>"));
			SerialReceiveLoop();
			if (rxchars[0] == 'v' || rxchars[0] == 'V'){
				EEPROM.write(addr_commandmode2, 0);	//0 = velocity
				Serial.print(F("CHANNEL 2 PWM INPUT SET TO 'VELOCITY'"));
			}			
			else if (rxchars[0] == 'p' || rxchars[0] == 'P'){
				EEPROM.write(addr_commandmode2, 1);	//1 = position
				Serial.print(F("CHANNEL 2 PWM INPUT SET TO 'POSITION'"));
			}
			else if (rxchars[0] == 's' || rxchars[0] == 'S'){
				EEPROM.write(addr_commandmode2, 2);	//2 = servo
				Serial.print(F("CHANNEL 1 PWM INPUT SET TO 'SERVO'"));
			}
			else {
				Serial.println(F("COMMAND TYPE NOT CHANGED"));
			}	
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch1menu;
		}
		//**Set velocity/position GAIN
		else if (rxchars[0] == '8'){	
			Serial.println(F("Set channel 2 PWM command gain."));
			Serial.println(F("   This is the scaling coefficient for velocity or position inputs when in PWM-command mode."));
			Serial.println(F("\n   Enter an integer between 1 and 1000 (arbitrary units)"));	
			Serial.println(F("   Strike 'Enter' to leave as is:")); 	
			Serial.println(F("\n\n>>"));
			PROMPT_gain2Entry();
			if (exitflag == true){
				delay(2000);
				goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			Serial.print(F("\nCHANNEL 2 PWM GAIN SET TO "));
			Serial.print(gain2);
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch2menu;
		}
		//**Set PWM CENTER
		else if (rxchars[0] == '9'){	
			Serial.println(F("Set channel 2 PWM center offset."));
			Serial.println(F("   This defines the pulse width which sets the command-neutral position."));
			Serial.println(F("\n   Enter an integer between 1 and 9000."));	
			Serial.println(F("   Enter 0 to automatically use the first detected PWM value as the command-neutral position:")); 	
			Serial.println(F("   Strike 'Enter' to leave as is:")); 	
			Serial.println(F("\n\n>>"));
			PROMPT_pwmoffset2Entry();
			if (exitflag == true){
				delay(2000);
				goto ch2menu;		//Yes that's a goto. FRACK modern programming sensibilities. 
			}
			if (pwmoffset2 == 0) {
				Serial.print(F("\nTHE INITIAL PWM INPUT WILL BE TAKEN AS COMMAND-NEUTRAL"));
			}
			else {
				Serial.print(F("\nCHANNEL 2 PWM CENTER SET TO "));
				Serial.print(pwmoffset2);
			}
			Serial.println(F("\n\n\n\n"));
			delay(2000);		
			goto ch2menu;
		}
		//Test Motor
		else if (rxchars[0] == 't' || rxchars[0] == 'T'){
			Serial.println(F("\nRunning motor on channel 2 for about 10s..."));
			TorqueCal2();
			goto ch2menu;
		}
		//**Return to main menu
		else if (rxchars[0] == 'm'){
			goto mainmenu;
		}
	}

	//SET I2C ADDRESS
	if (rxchars[0] == '3'){	
		Serial.println(F("Enter an I2C address to use for this board as an integer between 3 and 119."));
		Serial.println(F("The hexadecimal version of this number may be used for specifying the address on the master device."));
		Serial.println(F("\n\n>>"));
		SerialReceiveLoop();
		iholder = atoi(rxchars);
		if (iholder >= 3 && iholder <= 119){
			i2caddress = iholder;
			EEPROM.write(addr_i2caddress, i2caddress);	
			Serial.print(F("I2C SET TO "));
			Serial.print(i2caddress);
			Serial.print(F(" [hex = 0x"));
			Serial.print(i2caddress, HEX);	//also display as a hex value
			Serial.println(F("]"));
		}
		else {
			Serial.print(F("I2C ADDRESS LEFT AS "));
			Serial.print(i2caddress);
			Serial.print(F(" [hex = 0x"));
			Serial.print(i2caddress, HEX);	//also display as a hex value
			Serial.println(F("]"));
		}	
		Serial.println(F("\n\n\n\n"));
		delay(2000);
		goto mainmenu;			
	}

	//LIST ALL SETTINGS
	if (rxchars[0] == '4'){	
		CheckConfig();
		DisplaySettings();
		delay(7000);
		goto mainmenu;			
	}

	//RESET FACTORY DEFAULTS
	if (rxchars[0] == '5'){	

		EEPROM.write(addr_maxIset1l, 255);  
		EEPROM.write(addr_maxIset1r, 255);	
		EEPROM.write(addr_maxIset2l, 255);  
		EEPROM.write(addr_maxIset2r, 255);	
		EEPROM.write(addr_minIset1l, 255);  
		EEPROM.write(addr_minIset1r, 255);	
		EEPROM.write(addr_minIset2l, 255);  
		EEPROM.write(addr_minIset2r, 255);	
		EEPROM.write(addr_gain1l, 255);  
		EEPROM.write(addr_gain1r, 255);	
		EEPROM.write(addr_gain2l, 255);  
		EEPROM.write(addr_gain2r, 255);	
		EEPROM.write(addr_pwmoffset1l, 255);  
		EEPROM.write(addr_pwmoffset1r, 255);	
		EEPROM.write(addr_pwmoffset2l, 255);  
		EEPROM.write(addr_pwmoffset2r, 255);	
		EEPROM.write(addr_pwmoffset1l, 255);  
		EEPROM.write(addr_pwmoffset1r, 255);	
		EEPROM.write(addr_torqueprofile1, 255);	
		EEPROM.write(addr_torqueprofile2, 255);	
		EEPROM.write(addr_tscoeff1, 255);	
		EEPROM.write(addr_tscoeff2, 255);	
		EEPROM.write(addr_commandmode1, 255);	
		EEPROM.write(addr_commandmode2, 255);	
		EEPROM.write(addr_sensortype1, 255);	
		EEPROM.write(addr_sensortype2, 255);	
		EEPROM.write(addr_dir1, 255);	
		EEPROM.write(addr_dir2, 255);	
		EEPROM.write(addr_accel1l, 255);	
		EEPROM.write(addr_accel1r, 255);	
		EEPROM.write(addr_accel2l, 255);	
		EEPROM.write(addr_accel2r, 255);	
		EEPROM.write(addr_maxslewvel1l, 255);	
		EEPROM.write(addr_maxslewvel1r, 255);	
		EEPROM.write(addr_maxslewvel2l, 255);	
		EEPROM.write(addr_maxslewvel2r, 255);	
		EEPROM.write(addr_i2caddress, 255);
		Serial.print(F("ALL CONFIGURATION SETTINGS HAVE BEEN RESET TO THE FACTORY DEFAULTS. REBOOTING..."));
		Serial.println(F("\n\n\n\n"));
		delay(2000);		
		softReset();
	}

	//RESTART	
	if (rxchars[0] == 'q' || rxchars[0] == 'Q'){	
		softReset();
	}
}
