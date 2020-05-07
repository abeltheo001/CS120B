/*	Author: Abel Theodros
 *      Partner(s) Name: 
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
#include "timer.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00; 
    /* Insert your solution below */
	TimerSet(100);
	TimerOn();
	unsigned char tmp = 0x02; 
    while (1) {
	if (tmp == 0x02)
		PORTB = 0x01;
	else if (tmp == 0x01) 
		PORTB = 0x02;
	else if (tmp == 0x00) {
		PORTB = 0x04;
		tmp = 0x03;
		}
	while (!TimerFlag);
	TimerFlag = 0;
	tmp--;
    }
    return 1;
}
