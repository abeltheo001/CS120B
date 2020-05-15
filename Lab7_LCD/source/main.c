/*	Author: Abel Theodros
 *      Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #6  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "io.h"
#include "timer.h"
#endif

enum States {start, select, plus, minus, reset}state;
unsigned char tempB = 0;
unsigned char timer = 0;

void Tick()
{
	unsigned char button1 = ~PINA & 0x01;
	unsigned char button2 = ~PINA & 0x02;

	switch(state) { //Transitions
		case start: 
			state = select;
			break;
		case select:
			if (button1)
				state = plus;
			if (button2)
				state = minus;	
			if (button1 && button2)
				state = reset;
			if (!button1 && !button2)
				state = select;
			break;
		case plus:
			if (button1) {
				state = plus; }
			else {
			state = select; }
			
			break;
		case minus:
			if (button2) {
				state = minus; }
			else {
			state = select; }
			break;
		case reset:
			state = select;
			break;
		default:
			state = start;
			break;
	
				
	}
	
	switch(state) { //States
		case start: 
			break;
		case select:
			break;
		case plus:
			if (tempB < 0x09)
			{	
				if (timer >= 10)
				{
					tempB++;
					timer = 0;
				}
				else 
				{
					timer++;
				}
						
			}
			break;
		case minus:
			if (tempB > 0x00)
			{
				if (timer >= 10)
				{
					tempB--;
					timer = 0;
				}
				else
				{
					timer++;
				}
			
			}
			break;	
		case reset: 
			tempB = 0;
			timer = 0;	
			break;

	}

}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */

	TimerSet(10);
	TimerOn();

	LCD_init();
	LCD_ClearScreen();
    while (1) {
//	LCD_Cursor(1);
	Tick();
	LCD_ClearScreen();
	LCD_WriteData(tempB + '0');
	while (!TimerFlag);
	TimerFlag = 0;
	continue;
    }
    return 1;
}
