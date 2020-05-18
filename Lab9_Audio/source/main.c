/*	Author: Abel Theodros
 * 	Partner(s) Name: n/a
 *	Lab Section: 024
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: n/a
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
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

enum States{start, select, sound1, sound2, sound3}state;

void Tick(){
	unsigned char button1 = ~PINA & 0x01;
	unsigned char button2 = ~PINA & 0x02;
	unsigned char button3 = ~PINA & 0x04;

	switch(state) { //Transition
	
		case start: 
			state = select; break;
		case select: 
			if (button1)
				state = sound1;
			else if (button2)
				state = sound2; 
			else if (button3)
				state = sound3; 
			else 
				state = select;
			break;
		case sound1:
			if (button1)
				state = sound1;
			else
				state = select; 
			break; 
		case sound2:
			if (button2)
				state = sound2;
			else
				state = select;
			break;
		case sound3:
			if (button3)
				state = sound3;
			else
				state = select;
			break;
	 	default:
			state = start; break; 
	}

	switch(state) { //State 
		case start: break;
		
		case select: 
			set_PWM(0); break;
		case sound1: 
			set_PWM(261.63); break;
		case sound2:
			set_PWM(293.66); break;
		case sound3:
			set_PWM(329.63); break; 

		
	} 
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

    /* Insert your solution below */
	PWM_on();
	set_PWM(0);
	while (1) {
		Tick();
	}
	return 1;
}
