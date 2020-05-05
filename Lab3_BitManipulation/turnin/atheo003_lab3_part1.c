/*	Author: abeltheo
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
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
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tmpA, tmpB; 
	unsigned char count,i = 0x00;

    /* Insert your solution below */
    while (1) {
	count = 0; 
	tmpA = PINA;
	tmpB = PINB; 
	
	for (i = 0; i<8; i++)
	{
		if ((tmpA >> i & 0x01) == 0x01)
			count++;
		if ((tmpB >> i & 0x01) == 0x01)
			count++;
	}


	PORTC = count; 

	


    }
    return 1;
}
