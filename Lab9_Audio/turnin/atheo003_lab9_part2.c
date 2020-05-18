/*	Author: Abel Theodros
 * 	Partner(s) Name: n/a
 *	Lab Section: 024
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: n/a
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <stdbool.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) {TCCR3B &= 0x08; }
		else {TCCR3B |= 0x03;}
		
		if (frequency < 0.954) {OCR3A = 0xFFFF;}
		else if (frequency > 31250) {OCR3A = 0x000;}
		else {OCR3A = (short){8000000 / (128 * frequency)} - 1;}

		TCNT3 = 0;
		current_frequency = frequency;
	}
}
void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum States{start, select, add, release, minus, on, offRelease, off}state;

double note[8] = {261.63, 293.63,329.63,349.23,392,440,493.88,523.25}; 
unsigned char count = 0; 

void Tick(){
	unsigned char button1 = ~PINA & 0x01;
	unsigned char button2 = ~PINA & 0x02;
	unsigned char button3 = ~PINA & 0x04;

	switch(state) { //Transition
	
		case start: 
			state = on; 
			break;

		case select: 
			if (button1)
				state = add;
			else if (button2)
				state = minus; 
			else if (button3)
				state = off; 
			else 
				state = select;
			break;

		case add:
			state = release; break;
		case release:
			if (!button1 && !button2)
				state = select;
			else
				state = release;
			break;

		case minus:	
			state = release; break;
		case off:
			if (button3)
				state = on;
			else
				state = off;
			break;
		case on:
			if (button3)
				state = on;
			else
				state = select;
			break;
	
	 	default:
			state = start; break; 
	}

	switch(state) { //State 
		case start: 
			count = 0;	
			break;
		case select:		
			 break;
		case add:
			if (count < 7)
				count++;
			set_PWM(note[count]);
			break;

		case release:
			break;

		case minus: 
			if (count > 0)
				count--;
			set_PWM(note[count]);
			break;

		case offRelease: 
			break;

		case on:
			PWM_on();
			break;

		case off:
			PWM_off();
			break;


		
			
	} 
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	PWM_on();
    /* Insert your solution below */
	while (1) {
		Tick();
	}
	return 1;
}
