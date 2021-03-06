/*	Author: Abel Theodros 
 *  	Partner(s) Name: n/a
 *	Lab Section: 024
 *	Assignment: Lab #11  Exercise #2
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

unsigned char i, y;
unsigned char line = 1; 

const unsigned char message[66] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','C','S','1','2','0','B',' ','i','s',' ','L','e','g','e','n','d','.','.','.','w','a','i','t',' ','f','o','r',' ','i','t',' ','D','A','R','Y','!',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};

enum LCD_states {start, scroll};
int LCD_scroll(int state) {
		
	switch(state) {
		case start:
			state = scroll;
			break;
		
		case scroll:
			for (i = 1; i <= 16; i++) {
				LCD_Cursor(i);
				LCD_WriteData(message[(i-2)+line]);
				if (line+1+i == 66)
					line = 1;
			}
			line++;
			state = scroll;

		
		default:
			state = start;
			break;
		
	}
		return state;
	
}


int main(void) {

	DDRA = 0xF0; PORTA = 0x0F; //keypad input 
	DDRB = 0xFF; PORTB = 0x00; //LED output

	DDRC = 0xFF; PORTC = 0x00; //LCD output
	DDRD = 0xFF; PORTD = 0x00; //LCD output
/*
	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	//Task #1
	task1.state = 0;
	task1.period = 2; 
	task1.elapsedTime = task1.period;
	task1.TickFct = &LCD_scroll;
	
	unsigned long GCD = tasks[0] -> period;
	for (int a = 0; a < numTasks; ++a) {
		GCD = findGCD(GCD, tasks[a] -> period);
	}	
	
	TimerSet(GCD);
	TimerOn();
	LCD_init();
	LCD_ClearScreen();
	
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
*/

	LCD_init();
	LCD_ClearScreen();
	while(1) {
		LCD_Cursor(1);
		LCD_ClearScreen();
		LCD_WriteData(99+'0');
	}
	return 0;
}


