
//define local variable(s) here.  Use static keyword to keep local, e.g:
//   static int i;  // defines a local int named i



/*complete the state machine*/


void Ping(){
	switch(ping_state) { //Transitions
	
		case PInit: 
			ping_state = PInit;
			break;
			

	}
	
	switch(ping_state ) { //State actions 
		
		case PInit:
			if (i < 10) {
				i++;
				PORTB = PORTB & 0xFE;
			}
			else if (i == 10){
				PORTB = PORTB | 0x01; 
				i = 0;
			}
			else 
			 	i = 0; 
			break;

		default:
			break;

	}
}
