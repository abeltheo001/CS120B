/*	Author: Abel Theodros
 * 	Partner(s) Name: n/a 
 *	Lab Section: 024
 *	Assignment: Lab #8  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ACD_init(){
	ADCSRA |= (1<<ADEN)|(1<<ADSC)|(1<<ADATE);
}

int main(void) {
    /* Insert DDR and PORT initializations */
	//Faulty potentiometer. 
	//Video showing proof of completion is linked above. 
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	PORTB = 0x02;
	unsigned short x; 
	unsigned char tempB, tempD;

    /* Insert your solution below */
    while (1) {
	x = ADC;
	tempB = (char)x;
	tempD = (char)(x >> 8);
	PORTB = tempB; PORTD = tempD;
    }
    return 1;
}
