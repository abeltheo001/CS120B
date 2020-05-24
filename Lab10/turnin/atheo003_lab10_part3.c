/*	Author: Abel Theodros
 * 	Partner(s) Name: 
 *	Lab Section:024
 *	Assignment: Lab #10  Exercise #3 
 *	Exercise Description: n/a
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

typedef struct task {
	int state;		   // Task's current state
	unsigned long period;	   // Task period
	unsigned long elapsedTime; // Time elapsed since last tick
	int (*TickFct)(int);	   // Task tick function 
} task;

task tasks[4];
const unsigned short tasksNum = 4; 
unsigned char threeLEDS = 0x00;
unsigned char blinkingLED = 0x00;
unsigned short i, j = 0x00; 


enum TL_States {start, T0, T1, T2} TL_state;
void ThreeLEDsSM()
{
	switch(TL_state) { //Transitions
		
		case start:
			TL_state = T0; break;
		case T0:
			TL_state = T1;
			break;
		case T1:
			TL_state = T2;
			break;
		case T2: 
			TL_state = T0;
			break;			
		default:
			TL_state = start; 
			break;
	}

	switch(TL_state) { //State actions
	
		case start: 
			break;
		case T0:
			threeLEDS = 0x01;
			break;
		case T1:
			threeLEDS = 0x02;
			break;
		case T2: 
			threeLEDS = 0x04;
			break;		
	}

}

enum BL_States {BL_start, on, off} BL_state;
void BlinkLEDSM()
{
	switch(BL_state) { //Transitions
		case BL_start:
			BL_state = on;
			break;

		case on:
			BL_state = off; 
			break;

		case off:
			BL_state = on;
			break;
		
		default:
			BL_state = BL_start; 
			break;	
	}
	
	switch(BL_state) { //State actions
		case BL_start: break;	
		case on:
			blinkingLED = 0x08;
			break;
		case off:
			blinkingLED = 0x00;
			break;		

	}

	
}

enum SP_States {sp_start, sp_wait, sp_on, sp_off} sp_state;
unsigned char vocalCord = 0x00;
unsigned char t = 0x00;

void Speaker()
{
	unsigned char button_A2 = ~PINA & 0x04;
	switch(sp_state) {
		case sp_start:
			sp_state = sp_wait;
			break;
		case sp_wait:
			if (button_A2)
				sp_state = sp_on;
			else
				sp_state = sp_wait;
			break;
		case sp_on:
			if (button_A2)
				sp_state = sp_off;
			else
				sp_state = sp_wait;
			break;
		case sp_off:
			if (button_A2)
				sp_state = sp_on;
			else
				sp_state = sp_wait;
			break;
	
		default:
			sp_state = sp_start;
			break;

	}

	switch(sp_state) {
		case sp_start:
			break;

		case sp_wait:
			vocalCord = 0x00;
			break;

		case sp_on:
			vocalCord = 0x10;
			break;

		case sp_off:
			vocalCord = 0x00;
			break;
		default:
			vocalCord = 0x00;
			break;

	}
}

enum CM_States {CM_Start} CM_state;
void Tick_Combine()
{
	switch(CM_state) { //Transitions
		case CM_Start:
			CM_state = CM_Start;
			break;
	}
	switch(CM_state) { //State actions
		case CM_Start:
			PORTB = vocalCord|blinkingLED|threeLEDS;
			break;
	}
	
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	unsigned long blinkTime = 1000;
	unsigned long threeTime = 300;
	unsigned long speakerTime = 2;
	const unsigned long period = 1;
	TimerSet(period);
	TimerOn();

    while (1) {
	if ((~PINA & 0x01) == 0) {
		if (blinkTime >= 1000) {
			BlinkLEDSM();
			blinkTime = 0;
		}
		if (threeTime >= 300) {
			ThreeLEDsSM();
			threeTime = 0;
		}
		if (speakerTime >=2) {
			Speaker();
			speakerTime = 0;
		}
		Tick_Combine();
		}
	else 
		PORTB = 0x11;
	while (!TimerFlag);
	TimerFlag = 0;
	blinkTime += period;
	threeTime += period;
	speakerTime += period;
    }
    return 1;
}
