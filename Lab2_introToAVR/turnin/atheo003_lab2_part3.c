/*	Author: abeltheo
 * 	Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #2  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; DDRC = 0xFF;
	PORTA = 0xFF; PORTC = 0x00;

	unsigned char tempA0, tempA1, tempA2, tempA3 = 0x00;
	unsigned char count = 0x00;

    while (1) {
	tempA0 = PINA & 0x01;
	tempA1 = PINA & 0x02;
	tempA2 = PINA & 0x04;
	tempA3 = PINA & 0x08;
	count = 0x00;

	tempA1 = tempA1 >> 1;
	tempA2 = tempA2 >> 2;
	tempA3 = tempA3 >> 3;

	count = tempA0 + tempA1 + tempA2 + tempA3;
	
	if (count == 0x00)
		PORTC = 0x80 | count;
	else
		PORTC = count; 
	
	
    }
    return 0;
}
