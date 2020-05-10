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

unsigned char temp = 0x03;
unsigned char direction = 0x00;

enum States {start, light, press, hold} state;

void Tick()
{
	unsigned char button = ~PINA & 0x01;
		switch(state) { //Transitions
		case start: 
			state = light;
			break;
		case light:
			if (button)
				state = press;
			else
				state = light;
			break;
		case press:
			if (button)
				state = press;
			else 
				state = hold;
			break;
		case hold: 
			if (button)
				state = light;
			else 
				state = hold;
			break; 
		}

	switch(state) { //state 
		case start:
			break;
		case light:
			temp--;
			if (direction == 0x00) {
				if (temp == 0x02) PORTB = 0x01;
				if (temp == 0x01) PORTB = 0x02;
				if (temp == 0x00) {
					PORTB = 0x04;
					temp = 0x03;
					direction = 0x01;
				}
			}
			else {
				if (temp == 0x02) PORTB = 0x04;
				if (temp == 0x01) PORTB = 0x02;
				if (temp == 0x00) {
					PORTB = 0x01;
					temp = 0x03;
					direction = 0x00;
				}
			
			}
			break;
		case press:
			break;
		case hold:
			if (button)
				break;
			else {
			if (temp == 0x02) PORTB = 0x01;
			if (temp == 0x01) PORTB = 0x02;
			if (temp == 0x00) {
				PORTB = 0x04;
				temp = 0x03;
			}
			}
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
