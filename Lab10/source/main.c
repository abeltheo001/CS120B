/*	Author: Abel Theodros
 * 	Partner(s) Name: 
 *	Lab Section:024
 *	Assignment: Lab #10  Exercise #4 
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

task tasks[5];
const unsigned short tasksNum = 5; 
unsigned char threeLEDS = 0x00;
unsigned char blinkingLED = 0x00;
unsigned short i, j = 0x00; 

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

enum FR_States {fr_start, fr_wait, fr_add, fr_sub, fr_pressed} fr_state;
unsigned char freq = 3;
void Frequency()
{
	unsigned char button1 = ~PINA & 0x01;
	unsigned char button2 = ~PINA & 0x02;
	switch(fr_state) { //Transitions 
		case fr_start:
			fr_state = fr_wait;
			break;
		case fr_wait:
			if (button1)
				fr_state = fr_add;
			else if (button2)
				fr_state = fr_sub;
			else
				fr_state = fr_wait;
			break;
		case fr_add:
			fr_state = fr_pressed;
			break;
		case fr_sub:
			fr_state = fr_pressed;
			break;
		case fr_pressed:
			if (!button1 && !button2)
				fr_state = fr_wait;
			else
				fr_state = fr_pressed;
			break;
		default:
			fr_state = fr_start;
			break;
	}

	switch(fr_state) { //State actions 
		case fr_start:
			break;
		case fr_wait:
			break;
		case fr_add:
			freq++;
			break;
		case fr_sub:
			if (freq >= 0)
				freq--;
			break;
		default:
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
	unsigned long frequencyTime = 500;
	const unsigned long period = 1;
	TimerSet(period);
	TimerOn();
	PWM_on();
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
		if (frequencyTime >= 500) {
			Frequency();
			frequencyTime = 0;
		}
		if (speakerTime >=2) {
			Speaker();
			speakerTime = 0;
		}
		Tick_Combine();
		}
	else 
		PORTB = 0x01;
	while (!TimerFlag);
	TimerFlag = 0;
	blinkTime += period;
	threeTime += period;
	speakerTime += period;
	frequencyTime += period;
    }
    return 1;
}
