/*	Author: Abel Theodros 
 *  	Partner(s) Name: n/a
 *	Lab Section: 024
 *	Assignment: Lab #11  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "bit.h"
#include "scheduler.h"
#include "io.h"
#include "timer.h"
#include "keypad.h"
#endif


int main(void) {
	
	unsigned char x;
	DDRB = 0xFF; PORTB = 0x00; //LED output
	DDRC = 0xF0; PORTC = 0x0F; //keypad input 


    	while (1) {
		x = GetKeypadKey();
		switch(x) {
			case '\0': PORTB = 0x1F; break;
			case '1': PORTB = 0x01; break;
			case '2': PORTB = 0x02; break;
			//....	
			case 'D': PORTB = 0x0D; break;
			case '*': PORTB = 0x0E; break;
			case '0': PORTB = 0x00; break;
			case '#': PORTB = 0x0F; break;
			default: PORTB = 0x1B; break; //middle LED off. 
		}

	}
	

}


