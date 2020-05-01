void Roll1_vel(){
	//SET POWER DUTY CYCLE IF IN PWM MODE
	if(pwm1active == true){
		if (vel1 == 0){		//Set power duty cycle based on whether or not motor is moving.
			duty1 = m1*minIset1 + b1;
		}
		else {
			duty1 = m1*maxIset1 + b1;
		}
	}

	//INCREMENT PHASE INDEX
	if (dir1 == 0) {	//If direction is NORMAL
		phaseindex1 = phaseindex1 + vel1;		
	}
	else if (dir1 == 1) {	//If direction is REVERSE
		phaseindex1 = phaseindex1 - vel1;
	}	

	//CALCULATE TORQUE SMOOTHING (IF ON)
	if (torqueprofile1 == 1){	//If the anti-cogging profile is set to 1, make the phase index be the phase index plus (minus) a correction which is a function of the phase index. The fuction 
		phaseindex1 = phaseindex1 - (tscoeff1*.00025*sin(2*phaseindex1+(.469+tsphase1)) + tscoeff1*.00005*sin(4*phaseindex1+(.491+tsphase1)));  
	}

	//RESET PHASE INDEX AFTER 2PI
	if (phaseindex1 >= 2*pi) {   //Reset phaseindex_U1 once it completes 2*180o in phase.
		phaseindex1 = 0;
	}
	else if (phaseindex1 <= 0){
		phaseindex1 = 2*pi - phaseindex1;
	}

	//WRITE OUTPUTS
	dutyU1 = (255/2)+(duty1*(sin(phaseindex1)));	   //The duty cycle varies with the sine function, which has output between -1 and 1. That is scaled by an amplitude variable, which effectively sets the motor power.This is all offset by half the maximum duty cycle so that the lowest instantaeous duty cycle is always positive.
	analogWrite(U1_High,dutyU1);	//Write to the PWM pins.
	dutyV1 = (255/2)+(duty1*(sin(phaseindex1+(2*pi/3))));
	analogWrite(V1_High,dutyV1);
	dutyW1 = (255/2)+(duty1*(sin(phaseindex1+(4*pi/3))));
	analogWrite(W1_High,dutyW1);
}

void Roll1_pos(){
	//Check for REVERSE directionality
	if (dir1 == 1) {  //If directionality is REVERSE
		pos1 = (-1)*pos1;
	}

	//TRACK THE MOTOR AGAINST THE COMMANDED POSITION
	slewvel1 = (accel1/100000)*(fabs(phaseindex1 - pos1));	//make this a signmoid func. instead of capping below
	if (slewvel1 > maxslewvel1) {	//cap the max slewrate	
		slewvel1 = maxslewvel1;
	}
	else if (slewvel1 < -maxslewvel1){
		slewvel1 = -maxslewvel1;
	}
	if ((phaseindex1 - .2) > pos1){		//the constant defines the deadzone
		phaseindex1 = phaseindex1 - slewvel1; 
		if(pwm1active == true){
			duty1 = m1*maxIset1 + b1;
		}
		j = 0;
	}
	else if ((phaseindex1 + .2) < pos1){
		phaseindex1 = phaseindex1 + slewvel1;
		if(pwm1active == true){
			duty1 = m1*maxIset1 + b1;
		}
		j = 0;
	}
	else {//they're equal, so don't move the phaseindex, but DO set the power to the low setting if in PWM mode
		j++;
		if (j >= 100){
			if(pwm1active == true){
				duty1 = m1*minIset1 + b1;
				j = 100;
			}
		}
	}

	//UPDATE MOTOR POSITION
	pos1_last = pos1;

	//CALCULATE TORQUE SMOOTHING (IF ON)
	if (torqueprofile1 == 1){	//If the anti-coffing profile is set to 1, make the phase index be the phase index plus (minus) a correction which is a function of the phase index. The fuction 
		phaseindex1 = phaseindex1 - (tscoeff1*.00025*sin(2*phaseindex1+(.469+tsphase1)) + tscoeff1*.00005*sin(4*phaseindex1+(.491+tsphase1)));  
	}

	//WRITE OUTPUTS
	dutyU1 = (255/2)+(duty1*(sin(phaseindex1)));	   //The duty cycle varies with the sine function, which has output between -1 and 1. That is scaled by an amplitude variable, which effectively sets the motor power.This is all offset by half the maximum duty cycle so that the lowest instantaeous duty cycle is always positive.
	analogWrite(U1_High,dutyU1);	//Write to the PWM pins.
	dutyV1 = (255/2)+(duty1*(sin(phaseindex1+(2*pi/3))));
	analogWrite(V1_High,dutyV1);
	dutyW1 = (255/2)+(duty1*(sin(phaseindex1+(4*pi/3))));
	analogWrite(W1_High,dutyW1);
}

void Roll1_servo(){
	//Check for REVERSE directionality
	if (dir1 == 1) {  //If directionality is REVERSE
		pos1 = (-1)*pos1;
	}

	//TRACK THE MOTOR AGAINST THE COMMANDED POSITION
	slewvel1 = (accel1/100000)*fabs(enc1_absticks - pos1);  //make this a signmoid func. instead of capping below
	if (slewvel1 > maxslewvel1) {	//cap the max slewrate	
		slewvel1 = maxslewvel1;
	}
	else if (slewvel1 < -maxslewvel1){
		slewvel1 = -maxslewvel1;
	}
	if ((enc1_absticks - .03) > pos1){		//the constant defines the deadzone
		phaseindex1 = phaseindex1 - slewvel1; 
		if(pwm1active == true){
			duty1 = m1*maxIset1 + b1;
		}
		j = 0;
	}
	else if ((enc1_absticks + .03) < pos1){
		phaseindex1 = phaseindex1 + slewvel1;
		if(pwm1active == true){
			duty1 = m1*maxIset1 + b1;
		}
		j = 0;
	}
	else {//they're equal, so don't move the phaseindex, but DO set the power to the low setting
		j++;
		if (j >= 100){
			if(pwm1active == true){
				duty1 = m1*minIset1 + b1;
				j = 100;
			}
		}
	}	

	//UPDATE MOTOR POSITION
	pos1_last = pos1;

	//CALCULATE TORQUE SMOOTHING (IF ON)
	if (torqueprofile1 == 1){	//If the anti-coffing profile is set to 1, make the phase index be the phase index plus (minus) a correction which is a function of the phase index. The fuction 
		phaseindex1 = phaseindex1 - (tscoeff1*.00025*sin(2*phaseindex1+(.469+tsphase1)) + tscoeff1*.00005*sin(4*phaseindex1+(.491+tsphase1)));  
	}

	//RESET PHASE INDEX AFTER 2PI
	if (phaseindex1 >= 2*pi) {   //Reset phaseindex_U1 once it completes 2*180o in phase.
		phaseindex1 = 0;
	}
	else if (phaseindex1 <= 0){
		phaseindex1 = 2*pi - phaseindex1;
	}

	//WRITE OUTPUTS
	dutyU1 = (255/2)+(duty1*(sin(phaseindex1)));	   //The duty cycle varies with the sine function, which has output between -1 and 1. That is scaled by an amplitude variable, which effectively sets the motor power.This is all offset by half the maximum duty cycle so that the lowest instantaeous duty cycle is always positive.
	analogWrite(U1_High,dutyU1);	//Write to the PWM pins.
	dutyV1 = (255/2)+(duty1*(sin(phaseindex1+(2*pi/3))));
	analogWrite(V1_High,dutyV1);
	dutyW1 = (255/2)+(duty1*(sin(phaseindex1+(4*pi/3))));
	analogWrite(W1_High,dutyW1);
}

void Roll2_vel(){
	//SET POWER DUTY CYCLE IF IN PWM MODE
	if(pwm2active == true){
		if (vel2 == 0){		//Set power duty cycle based on whether or not motor is moving.
			duty2 = m2*minIset2 + b2;
		}
		else {
			duty2 = m2*maxIset2 + b2;
		}
	}

	//INCREMENT PHASE INDEX
	if (dir2 == 0) {	//If direction is NORMAL
		phaseindex2 = phaseindex2 + vel2;		
	}
	else if (dir2 == 1) {	//If direction is REVERSE
		phaseindex2 = phaseindex2 - vel2;
	}	

	//CALCULATE TORQUE SMOOTHING (IF ON)
	if (torqueprofile2 == 1){	//If the anti-cogging profile is set to 1, make the phase index be the phase index plus (minus) a correction which is a function of the phase index. The fuction 
		phaseindex2 = phaseindex2 - (tscoeff2*.00025*sin(2*phaseindex2+(.469+tsphase2)) + tscoeff2*.00005*sin(4*phaseindex2+(.491+tsphase2)));  
	}

	//RESET PHASE INDEX AFTER 2PI
	if (phaseindex2 >= 2*pi) {   //Reset phaseindex_U1 once it completes 2*180o in phase.
		phaseindex2 = 0;
	}
	else if (phaseindex2 <= 0){
		phaseindex2 = 2*pi - phaseindex2;
	}

	//WRITE OUTPUTS
	dutyU2 = (255/2)+(duty2*(sin(phaseindex2)));	   //The duty cycle varies with the sine function, which has output between -1 and 1. That is scaled by an amplitude variable, which effectively sets the motor power.This is all offset by half the maximum duty cycle so that the lowest instantaeous duty cycle is always positive.
	analogWrite(U2_High,dutyU2);	//Write to the PWM pins.
	dutyV2 = (255/2)+(duty2*(sin(phaseindex2+(2*pi/3))));
	analogWrite(V2_High,dutyV2);
	dutyW2 = (255/2)+(duty2*(sin(phaseindex2+(4*pi/3))));
	analogWrite(W2_High,dutyW2);
}

void Roll2_pos(){
	//Check for REVERSE directionality
	if (dir2 == 1) {  //If directionality is REVERSE
		pos2 = (-1)*pos2;
	}

	//TRACK THE MOTOR AGAINST THE COMMANDED POSITION
	slewvel2 = (accel2/100000)*(fabs(phaseindex2 - pos2));	////make this a signmoid func. instead of capping below
	if (slewvel2 > maxslewvel2) {	//cap the max slewrate	
		slewvel2 = maxslewvel2;
	}
	else if (slewvel2 < -maxslewvel2){
		slewvel2 = -maxslewvel2;
	}
	if ((phaseindex2 - .2) > pos2){		//the constant defines the deadzone
		phaseindex2 = phaseindex2 - slewvel2; 
		if(pwm2active == true){
			duty2 = m2*maxIset2 + b2;
		}
		j = 0;
	}
	else if ((phaseindex2 + .2) < pos2){
		phaseindex2 = phaseindex2 + slewvel2;
		if(pwm2active == true){
			duty2 = m2*maxIset2 + b2;
		}
		j = 0;
	}
	else {	//they're equal, so don't move the phaseindex, but DO set the power to the low setting
		j++;
		if (j >= 100){
			if(pwm2active == true){
				duty2 = m2*minIset2 + b2;
				j = 100;
			}
		}
	}

	//UPDATE MOTOR POSITION
	pos2_last = pos2;

	//CALCULATE TORQUE SMOOTHING (IF ON)
	if (torqueprofile2 == 1){	//If the anti-coffing profile is set to 1, make the phase index be the phase index plus (minus) a correction which is a function of the phase index. The fuction 
		phaseindex2 = phaseindex2 - (tscoeff2*.00025*sin(2*phaseindex2+(.469+tsphase2)) + tscoeff2*.00005*sin(4*phaseindex2+(.491+tsphase2)));  
	}

	//WRITE OUTPUTS
	dutyU2 = (255/2)+(duty2*(sin(phaseindex2)));	   //The duty cycle varies with the sine function, which has output between -1 and 1. That is scaled by an amplitude variable, which effectively sets the motor power.This is all offset by half the maximum duty cycle so that the lowest instantaeous duty cycle is always positive.
	analogWrite(U2_High,dutyU2);	//Write to the PWM pins.
	dutyV2 = (255/2)+(duty2*(sin(phaseindex2+(2*pi/3))));
	analogWrite(V2_High,dutyV2);
	dutyW2 = (255/2)+(duty2*(sin(phaseindex2+(4*pi/3))));
	analogWrite(W2_High,dutyW2);
}

void Roll2_servo(){
	//Check for REVERSE directionality
	if (dir2 == 1) {  //If directionality is REVERSE
		pos2 = (-1)*pos2;
	}

	//TRACK THE MOTOR AGAINST THE COMMANDED POSITION
	slewvel2 = (accel2/100000)*fabs(enc2_absticks - pos2);  //make this a signmoid func. instead of capping below
	if (slewvel2 > maxslewvel2) {	//cap the max slewrate	
		slewvel2 = maxslewvel2;
	}	
	else if (slewvel2 < -maxslewvel2){
		slewvel2 = -maxslewvel2;
	}
	if ((enc2_absticks - .03) > pos2){		//the constant defines the deadzone
		phaseindex2 = phaseindex2 - slewvel2; 
		if(pwm2active == true){
			duty2 = m2*maxIset2 + b2;
		}
		j = 0;
	}
	else if ((enc2_absticks + .03) < pos2){
		phaseindex2 = phaseindex2 + slewvel2;
		if(pwm2active == true){
			duty2 = m2*maxIset2 + b2;
		}
		j = 0;
	}
	else {//they're equal, so don't move the phaseindex, but DO set the power to the low setting
		j++;
		if (j >= 100){
			if(pwm2active == true){
				duty2 = m2*minIset2 + b2;
				j = 100;
			}
		}
	}	

	//UPDATE MOTOR POSITION
	pos2_last = pos2;

	//CALCULATE TORQUE SMOOTHING (IF ON)
	if (torqueprofile2 == 1){	//If the anti-coffing profile is set to 1, make the phase index be the phase index plus (minus) a correction which is a function of the phase index. The fuction 
		phaseindex2 = phaseindex2 - (tscoeff2*.00025*sin(2*phaseindex2+(.469+tsphase2)) + tscoeff2*.00005*sin(4*phaseindex2+(.491+tsphase2)));  
	}

	//RESET PHASE INDEX AFTER 2PI
	if (phaseindex2 >= 2*pi) {   //Reset phaseindex once it completes 2*180o in phase.
		phaseindex2 = 0;
	}
	else if (phaseindex2 <= 0){
		phaseindex2 = 2*pi - phaseindex2;
	}

	//WRITE OUTPUTS
	dutyU2 = (255/2)+(duty2*(sin(phaseindex2)));	   //The duty cycle varies with the sine function, which has output between -1 and 1. That is scaled by an amplitude variable, which effectively sets the motor power.This is all offset by half the maximum duty cycle so that the lowest instantaeous duty cycle is always positive.
	analogWrite(U2_High,dutyU2);	//Write to the PWM pins.
	dutyV2 = (255/2)+(duty2*(sin(phaseindex2+(2*pi/3))));
	analogWrite(V2_High,dutyV2);
	dutyW2 = (255/2)+(duty2*(sin(phaseindex2+(4*pi/3))));
	analogWrite(W2_High,dutyW2);
}

