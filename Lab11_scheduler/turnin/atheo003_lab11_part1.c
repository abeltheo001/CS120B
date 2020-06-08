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
unsigned char x; 
enum firstTick_States {start, pressed};

int firstTick(int state)
{
	switch (state) {
		case start:
			state = pressed; break;

		case pressed:
			x = GetKeypadKey();
			switch(x) {
				case '\0': PORTB = 0x1F; break;
				case '1': PORTB = 0x01; break;
				case '2': PORTB = 0x02; break;
				case '3': PORTB = 0x03; break;
				case '4': PORTB = 0x04; break;
				case '5': PORTB = 0x05; break;
				case '6': PORTB = 0x06; break;
				case '7': PORTB = 0x07; break;
				case '8': PORTB = 0x08; break;
				case '9': PORTB = 0x09; break;
				case 'A': PORTB = 0x0A; break;
				case 'B': PORTB = 0x0B; break;
				case 'C': PORTB = 0x0C; break;
				case 'D': PORTB = 0x0D; break;
				case '*': PORTB = 0x0E; break;
				case '0': PORTB = 0x00; break;
				case '#': PORTB = 0x0F; break;
				default: PORTB = 0x1B; break; //middle LED off. 
			}
			state = pressed; 
			break;

		default:
			state = start; 
			break;
	}
		
	switch(state) {
		default:
			PORTB = ~PORTB; 
			break;
	}
	return state;
}

int main(void) {

	DDRA = 0xF0; PORTA = 0x0F; //keypad input 
	DDRB = 0xFF; PORTB = 0x00; //LED output

	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	//Task #1
	task1.state = 0;
	task1.period = 2; 
	task1.elapsedTime = task1.period;
	task1.TickFct = &firstTick;
	
	unsigned long GCD = tasks[0] -> period;
	for (int a = 0; a < numTasks; ++a) {
		GCD = findGCD(GCD, tasks[a] -> period);
	}	
	
	TimerSet(GCD);
	TimerOn();
	
	unsigned short i; 

    	while (1) {
		for (i = 0; i < numTasks; i++) {
			if (tasks[i] -> elapsedTime == tasks[i] -> period) {
				tasks[i] -> state = tasks[i] -> TickFct(tasks[i]->state);
				tasks[i] -> elapsedTime = 0;
			}
			tasks[i] -> elapsedTime += GCD;
		}
		while(!TimerFlag);
		TimerFlag = 0; 
	}
	return 0;	

}


