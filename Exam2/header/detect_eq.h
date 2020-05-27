//define local variable(s) here.  Use static keyword to keep local, e.g:
 static int i;  // defines a local int named i



/*complete the state machine*/

void Detect_EQ()
{
    switch(detect_eq_state)
    {
        case DEQInit:
        	if ((~PINA & 0xF8) != 0)
			detect_eq_state = DEQBegin;
		else 
			detect_eq_state = DEQInit;
            break;

	case DEQBegin:
		PORTB = PORTB | 0x02;
		if ((~PINA & 0xF8) != 0)
			detect_eq_state = DEQBegin;
		else
			detect_eq_state = DEQCheck;
		break;
	
	case DEQCheck:
		if (i < 10)
			i++;
		else {
			i = 0;
			PORTB = PORTB & 0xFD;
		
		}
        default:
            detect_eq_state = DEQInit;
            break;
    }
    switch(detect_eq_state)
    {
        case DEQInit:
            break;
        default:
            break;
    }
}



