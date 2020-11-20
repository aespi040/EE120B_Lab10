/*	Author: lab
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

typedef struct _task {

	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct)(int);
} task;

unsigned char GKK() {
	PORTC = 0xEF; //1110 1111
	asm("nop");
	if((PINC & 0x01) == 0x00) { return('1'); }
	if((PINC & 0x02) == 0x00) { return('4'); }
	if((PINC & 0x04) == 0x00) { return('7'); }
	if((PINC & 0x08) == 0x00) { return('*'); }

	PORTC = 0xDF; // column 2 1101 1111 
	asm("nop");
	if((PINC & 0x08) == 0x00) { return('2'); }
	if((PINC & 0x02) == 0x00) { return('5'); }
        if((PINC & 0x04) == 0x00) { return('8'); }
        if((PINC & 0x08) == 0x00) { return('0'); }

	PORTC = 0xBF; // column 3 1011 1111
        asm("nop");
        if((PINC & 0x08) == 0x00) { return('3'); }
        if((PINC & 0x02) == 0x00) { return('6'); }
        if((PINC & 0x04) == 0x00) { return('9'); }
        if((PINC & 0x08) == 0x00) { return('#'); }

	PORTC = 0x7F; // column 4 0111 1111
        asm("nop");
        if((PINC & 0x08) == 0x00) { return('A'); }
        if((PINC & 0x02) == 0x00) { return('B'); }
        if((PINC & 0x04) == 0x00) { return('C'); }
        if((PINC & 0x08) == 0x00) { return('D'); }

	return('\0'); //default value
}



enum buttons_states { B_start, B_press, B_release };
unsigned char light;
unsigned char input;

int buttons(int state) {
	switch (state) {
		case B_start:
			state = B_release;
		case B_press:
			input = GKK();
			if (input == '\0') { state = B_release; }
			else { state = B_press; }
			//state = B_press;
		case B_release:
			input = GKK();
			if(input != '\0') { state = B_press; }
			else { state = B_release; }
		default: state = B_start; break;
	}

	switch(state) {
		case B_start: break;
		case B_press:
			    light = 0x01;
			    break;
		case B_release:
			   light = 0x00;
			   break;
	}
	return state;
}

enum lightOF_states { L_start, L_ON, L_OFF };

int lightOF(int state) {
	switch (state) {
		case L_start:
			state = L_OFF;
		case L_ON:
			if (light == 0){ state = L_OFF; }
			else { state = L_ON: }
		case L_OFF:
			if (light == 1) { state = L_ON;}
			else { state = L_OFF; }
		default: state = L_start;
	}

	switch(state) {
		case L_start:
			PORTB = 0x00; 
                case L_ON:
			PORTB = 0x80;
                case L_OFF:
			PORTB = 0x00;
                default:
			PORTB = 0x00;
	}

	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; 
	DDRB = 0xFF; PORTB = 0x00; 

	/* Insert your solution below */
	static _task task1, task2;
	_task *tasks[] = { &task1, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = -1;

	//task 1 turn PB7 led on at any press
	task1.state = start;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &buttons;

	task2.state = start;
        task2.period = 50;
        task2.elapsedTime = task2.period;
        task2.TickFct = &lightOF;




   
    while (1) {
	    for ( i = 0; i < numTask; i++; ) {
		    if ( tasks[i] ->elapsedTime == tasks[i] ->period ) {
			    tasks[i] ->state = tasks[i] ->TickFct(tasks[i] ->state);
			    tasks[i] ->elapsedTime = 0;
		    }
		    tasks[i] ->elapsedTime += 50; // GCD of tasks' period 
	    }
	    while(!TimerFlag);
	    TimerFlag = 0;
    }
    return 0;
}
