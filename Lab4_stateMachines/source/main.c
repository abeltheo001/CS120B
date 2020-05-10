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
enum States {lock, pause, pause2, unlock1, unlock2} state;
#endif
void Light();

int main(void) {

	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	state = lock;
	while (1) { 	
		Light();
	} 

	return 1;
}

void Light() {

	unsigned char X = PINA & 0x01;
	unsigned char Y = PINA & 0x02;
	unsigned char NUM = PINA & 0x04;
	unsigned char INSIDE = PINA & 0x80;

	switch(state){ //Transitions
	
		case lock: 
			if (INSIDE) 
				state = lock;
			
			else {
				if (NUM && !Y && !X)
					state = pause;
				else 
					state = lock;
			}

			break;

		case pause:
			state = unblock1;
			break;
		case unlock1:
			if (INSIDE) 
				state = lock;
			
			else {
				if (!NUM && Y && !X)
					state = unlock2;
				else 
					state = lock;
			}

			break;
		case pause2:
			state = unblock2;
			break;

		case unlock2: 
			if (INSIDE) 
				state = lock;
			
			else {
				state = unlock2;
			}

			break;

		default: 
			state = lock;
			break;

	}


	switch(state){ //State

		case lock:
			PORTB = 0x00;
			break;
		case pause:
			break;
		case unlock1:
			break;
		case unlock2: 
			PORTB = 0x01;
			break;
		default:
			break; 


	}
}


