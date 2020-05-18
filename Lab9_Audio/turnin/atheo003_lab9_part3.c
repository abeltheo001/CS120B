/*	Author: Abel Theodros
 * 	Partner(s) Name: n/a
 *	Lab Section: 024
 *	Assignment: Lab #9  Exercise #3
 *	Exercise Description: n/a
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <stdbool.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
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

enum States{start, press, release, beat1, beat2, beat3, beat4, beat5}state;

double note[8] = {261.63, 293.63,329.63,349.23,392,440,493.88,523.25}; 
unsigned char count = 0; 
int i = 0;
void Tick(){
	unsigned char button1 = ~PINA & 0x01;
	unsigned char stop = ~PINA & 0x02;
	switch(state) { //Transition
	
		case start: 
			state = press; 
			break;

		case press: 
			if (button1)
				state = release;
			else if (stop)
				state = press;
			else
				state = press;	
			break;

		case release:
			if (button1)
				state = release;
			else if (stop)
				state = press;
			else
				state = beat1;
			break;
		
		case beat1:
			if (stop)
				state = press;
			state = beat2; break;
		case beat2:
			if (stop) 
				state = press;
			state = beat3; break;
		case beat3:
			if (stop)
				state = press;
			state = beat4; break;
		case beat4:
			if (stop)
				state = press;
			state = beat5; break;
		case beat5:
			if (button1)
				state = release;
			else if (stop)
				state = press;
			else 
				state = press;
	 	default:
			state = start; break; 
	}

	switch(state) { //State 
		case start: break;
		case press: set_PWM(0); break;
		case release: break;
	
		case beat1:
			set_PWM(note[6]);
			break;	
		case beat2:
			set_PWM(note[1]);
			break;
		case beat3:
			set_PWM(note[5]);
			break;
		case beat4:
			set_PWM(note[3]);
			break;
		case beat5:
			set_PWM(note[4]);
			break;
			
	} 
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(650);
	TimerOn();
	PWM_on();
    /* Insert your solution below */
	while (1) {
		Tick();
		while (!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}
