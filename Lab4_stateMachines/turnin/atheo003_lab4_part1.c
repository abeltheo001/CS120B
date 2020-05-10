/*	Author: abeltheo
 *  	Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
enum States {Start, Init, Press, Release,Wait} state;
#endif

void Light() {

	unsigned char A0 = PINA & 0x01;

	switch(state){ //Transitions
		case Start:
			state = Init; 
			break; 
		case Init: 
			if (A0 == 0x01)
				state = Press;
			else
				state = Init;
			break;
		case Press: 
			if (A0 == 0x01)
				state = Press;
			else 
				state = Release;
			break;
		case Release:
			if (A0 == 0x01)
				state = Wait;
			else
				state = Release;
			break;	
		case Wait:
			if (A0 == 0x01)
				state = Init;
			else
				state = Wait;
			break;

		default: 
			state = Start;
			break;

	}


	switch(state){ //State
		case Start:
			PORTB = 0x01;
			break;
		case Init:
			PORTB = 0x01;
			break;
		case Press:
			PORTB = 0x02;
			break;
		case Release:
			PORTB = 0x02;
			break;
		case Wait:
			break;
		default:
			PORTB = 0x01;
			break; 


	}
}


int main(void) {
	
	DDRA = 0x00; PORTA = 0xFF; 
	DDRB = 0xFF; PORTB = 0x00;
	
	state = Start;
	    
	while (1) { Light();} 

	return 1;
}
