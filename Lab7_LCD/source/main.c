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
#include "io.h"
#endif
unsigned char temp = 0;
unsigned char direction = 0x00;
unsigned char total = 5; 
unsigned char pressed = 0; 
enum States {start, press, light1, light2, light3,hold} state;

void Tick()
{
	unsigned char button = ~PINA & 0x01;
	switch(state) { //Transitions
		case start: 
			temp = 0;
			LCD_Cursor(1);
			LCD_ClearScreen();
			LCD_WriteData(total + '0');
			state = light1;
			break;
		case light1:
			temp = 1; 
			direction = 1;
			if (button){
				if (total > 0)
				{
					total--;
					LCD_Cursor(1);
					LCD_ClearScreen();
					LCD_WriteData(total+'0');
				}
				if (total == 0)
				{
					LCD_Cursor(1);
					LCD_ClearScreen();
					LCD_WriteData(total+'0');

				}

				state = press;}
			else
				state = light2;
			break;
		case light2:
			temp = 2;
			if (button){
				if (total < 9)
				{
					total++;
					LCD_Cursor(1);
					LCD_ClearScreen();
					LCD_WriteData(total+'0');

				}
				if (total == 9)
				{
					LCD_Cursor(1);
					LCD_ClearScreen();
					LCD_DisplayString(3, "Congrats!");
				}
				state = press; }
			else {
				if (direction == 1)
					state = light3;
				else if (direction == 2)
					state = light1;
			}
			break;
		case light3:
			temp = 3;
			direction = 2;
			if (button) {
				if (total > 0){
					total--;
					LCD_Cursor(1);
					LCD_ClearScreen();
					LCD_WriteData(total+'0');

				}
				if (total == 0)
				{
					LCD_Cursor(1);
					LCD_ClearScreen();
					LCD_WriteData(total+'0');

				}

				state = press;
				}
			else 
				state = light2;
			break;
		case press:
			if (button)
				state = hold;
			else 
				state = press;
			break;
		case hold:
			if (!button)
				state = start;
			else
				state = hold;
			break;
		 default:
			state = start;
			total = 5;
			break;	
		}

	switch(state) { //state 
		case start:
			break;
		case hold: 
			if (temp == 1) PORTB = 0x01;
			if (temp == 2) PORTB = 0x02;
			if (temp == 3) PORTB = 0x04;
			break;
	
		case press: 
			if (temp == 1) PORTB = 0x01;
			if (temp == 2) PORTB = 0x02;
			if (temp == 3) PORTB = 0x04;
			break;
		case light1:
			PORTB = 0x01;
			break;
		case light2:	
			PORTB = 0x02;
			break;
		case light3:
			PORTB  = 0x04;
			break;
			
		}
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00; 
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
	TimerSet(300);
	TimerOn();
	LCD_init();
	LCD_ClearScreen();

    while (1) {
	Tick();
	while (!TimerFlag);
	TimerFlag = 0;
	continue;
    }
    return 1;
}
