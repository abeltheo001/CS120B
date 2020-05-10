/*	Author: abeltheo
 *  	Partner(s) Name: 
 *	Lab Section:
 	Assignment: Lab #4  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
enum States {start, init, increase, decrease, reset, wait} state;
#endif

void Light() {
	unsigned char A1 = PINA & 0x02;
	unsigned char A0 = PINA & 0x01;  

	switch(state){ //Transitions
		case start: 
			state = init;
			break;

		case init: 
			if (A0 && !A1)
				state = increase;
			else if (A1 && !A0)
				state = decrease;
			else if (A0 && A1)
				state = reset;
			else
				state = wait;
			break;

		case increase: 
			state = wait;
			break;

		case decrease: 
			state = wait;
			break;

		case wait:
			if (A0 && A1)
				state = reset;
			else if (!A0 && !A1)
				state = init;
			else
				state = wait;
			break;

		case reset: 
			if (A0 && A1)
				state = reset;
			else
				state = wait;
			break;

		default: 
			state = start;
			break;

	}


	switch(state){ //State
		case start: 
			PORTC = 0x07;
			break;
		
		case init:
			PORTC = 0x07;
			break;		
	
		case increase: 
			if (PORTC < 0x09)
				PORTC++;
			else 
				PORTC = 0x09;
			break;
		case decrease: 
			if (PORTC > 0x00)
				PORTC--;
			else
				PORTC = 0x00;
			break;
		case wait:
			break;
		case reset:
			PORTC = 0x00;
			break;
		default:
			PORTC = 0x07;
		break; 


	}
}


int main(void) {

	DDRA = 0x00; PORTA = 0xFF; 
	DDRC = 0xFF; PORTC = 0x00;
	state = start;
	    
	while (1) { Light();} 

	return 1;
}
