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
#include "timer.h"
#endif

unsigned char temp = 0x03;
unsigned char direction = 0x00;
unsigned char check = 0x00;
enum States {start, select, plus, minus, reset}state;

unsigned char firstCheck = 0; 
unsigned char timer = 0; 

void Tick()
{
	unsigned char button1 = ~PINA & 0x01;
	unsigned char button2 = ~PINA & 0x02;
	switch(state) {
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
	
	switch(state) {
		case start: 
			break;
		case select:
			firstCheck = 0;
			break;
		case plus:
			if (PORTB < 0x09)
			{
				if (firstCheck == 0)
				{
					PORTB++;
					firstCheck = 1;
					timer++; 
				}
				else if (timer >= 10 && firstCheck == 1)
				{
					PORTB++;
					timer = 0;
				}
				else 
				{
					timer++;
				}
			}
			break;
		case minus:
			if (PORTB > 0x00)
			{
				if (firstCheck == 0)
				{
					PORTB--;
					firstCheck = 1;
					timer++; 
				}
				else if (timer >= 10 && firstCheck == 1)
				{
					PORTB--;
					timer = 0;
				}
				else 
				{
					timer++;
				}
			}
			break; 
		case reset: 
			firstCheck = 0;
			PORTB = 0x00;
			timer = 0;
			break;

	}

}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x07; 
    /* Insert your solution below */
	TimerSet(10);
	TimerOn();

    while (1) {

	Tick();
	while (!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
