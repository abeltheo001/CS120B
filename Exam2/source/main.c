/*	Author: Abel Theodros
 *	Lab Section: 24
 * 	Exam #2
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "timer.h"

/* SM state declarations --- fill in as needed */
typedef enum ping_states {PInit} ping_states;
typedef enum detect_eq_states { DEQInit, DEQBegin, DEQCheck} detect_eq_states;
typedef enum detect_max_amp_states { DMAInit,                               } detect_max_amp_states;
typedef enum detect_zc_states { DZCInit,                                    } detect_zc_states;
typedef enum transmit_states {TInit,                                        } transmit_states;

/* shared variables --- fill in as needed */
unsigned short i,j = 0x00;
unsigned char tempB;  


/* state variables --- do not alter */
ping_states ping_state;
detect_eq_states detect_eq_state;
detect_max_amp_states detect_max_amp_state;
detect_zc_states detect_zc_state;
transmit_states transmit_state;

/* SM definitions --- complete each task as a SM in the appropriate file.
 * Alternatively, you can remove the #include statement and insert your
 *   SM implementation directly. 
 */
#include "ping.h"
#include "detect_eq.h"
#include "detect_max_amp.h"
#include "detect_zc.h"
#include "transmit.h"

void Ping(){
	switch(ping_states) { //Transitions
	
		case PInit: 
			ping_states = PInit;
			break;
			

	}
	
	switch(ping_states) { //State actions 
		
		case PInit:
			if (i < 10) {
				i++;
				PORTB = PORTB & 0xFE;
			}
			else if (i == 10){
				PORTB = PORTB | 0x01; 
				i = 0;
			}
			else 
			 	i = 0; 
			break;

	}
}

void Detect_EQ(){
	switch(detect_eq_states) { //Transitions
		
		case DEQInit:
			if ((~PINA & 0xF8) != 0) {
				detect_eq_states = DEQBegin; }
			else {
				detect_eq_states = DEQInit; 
			}
			break;

		case DEQBegin:
			if ((~PINA & 0xF8) != 0) {
				detect_eq_states = DEQBegin; }
			else {
				detect_eq_states = DEQCheck; 
			}
			break;

		case DEQCheck:
			if (j < 10)
			{
				j++;
				detect_eq_states = DEQCheck;
			}
			else if (j == 10)
			{
				j = 0;
				PORTB = PORTB & 0xF8;
				detect_eq_states = DEQInit;
			}
			break;
	}
	
	switch(detect_eq_states) { //State actions 
		
		case DEQBegin:
			PORTB = PORTB | 0x02;  
			break;
		default:
			break;
	}
}


void Detect_Max_Amp(){
	switch(detect_max_amp_states) { //Transitions

	}
	
	switch(detect_max_amp_states) { //State actions
 
	}
}

void Detect_ZC(){
	switch(detect_zc_states) { //Transitions

	}
	
	switch(detect_zc_states) { //State actions
 
	}
}

void Transmit(){
	switch(transmit_states) { //Transitions

	}
	
	switch(transmit_states) { //State actions 

	}
}




/* main function --- do not alter */
int main(void) {
    /* DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    // Common period for all tasks set to 100ms
    TimerSet(100);
    TimerOn();

    // init state vars
    ping_state = PInit;
    detect_eq_state = DEQInit;
    detect_max_amp_state = DMAInit;
    detect_zc_state = DZCInit;
    transmit_state = TInit;

    while (1) {
        Ping();
        Detect_EQ();
        Detect_Max_Amp();
        Detect_ZC();
        Transmit();
        while (!TimerFlag) { }
        TimerFlag = 0;
    }
    return 1;
}
