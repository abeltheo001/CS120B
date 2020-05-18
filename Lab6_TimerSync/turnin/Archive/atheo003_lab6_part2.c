/*	Author: Abel Theodros
 *      Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif
unsigned char temp = 0;
unsigned char direction = 0x00;
unsigned char pressed = 0; 
enum States {start, press, light1, light2, light3,hold} state;

void Tick()
{
	unsigned char button = ~PINA & 0x01;
	switch(state) { //Transitions
		case start: 
			temp = 0;
			state = light1;
			break;
		case light1:
			temp = 1; 
			direction = 1;
			if (button)
				state = press;
			else
				state = light2;
			break;
		case light2:
			temp = 2;
			if (button)
				state = press;
			else 
			{
				if (direction == 1)
					state = light3;
				else if (direction == 2)
					state = light1;
			}
			break;
		case light3:
			temp = 3;
			direction = 2;
			if (button)
				state = press;
			else 
				state = light2;
			break;
		case press:
			if (button)
				state = hold;
			else 
				state = press;
			break;
		case hold:
			if (!button)
				state = start;
			else
				state = hold;
			break;
		}

	switch(state) { //state 
		case start:
			break;
		case hold: 
			if (temp == 1) PORTB = 0x01;
			if (temp == 2) PORTB = 0x02;
			if (temp == 3) PORTB = 0x04;
			break;
	
		case press: 
			if (temp == 1) PORTB = 0x01;
			if (temp == 2) PORTB = 0x02;
			if (temp == 3) PORTB = 0x04;
			break;
		case light1:
			PORTB = 0x01;
			break;
		case light2:	
			PORTB = 0x02;
			break;
		case light3:
			PORTB  = 0x04;
			break;
			
		}
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00; 
    /* Insert your solution below */
	TimerSet(30);
	TimerOn();

    while (1) {
	Tick();
	while (!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
