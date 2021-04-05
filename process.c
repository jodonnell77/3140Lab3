/*
 * process.c
 *
 *  Created on: Apr 1, 2021
 *      Author: JohnO
 */


#include <stdlib.h>
#include <MKL25Z4.h>
#include "3140_concur.h"

struct process_state {
	unsigned int size;
	unsigned int* sp;
	struct process_state* nextProcess;
};

process_t* listStart = NULL;


int process_create (void (*f)(void), int n) {
	unsigned int * procPointer =  process_stack_init(*f, n);

	if(procPointer == NULL) {
		return -1;
	} else {
		//Initializes the process to be added onto the linked list
		process_t* elementPt;
		elementPt = malloc(sizeof(process_t));
		elementPt->sp = procPointer;
		elementPt->nextProcess = NULL;
		elementPt->size = n;

		//Make this the only element of the list if the list is empty
		if(listStart == NULL) {
			listStart = elementPt;

		} else {
			//Create a temporary process state, and keep updating it until it is the last element
			process_t* tempProcessPt;
			tempProcessPt = listStart;
			while (tempProcessPt->nextProcess != NULL) {
				tempProcessPt = tempProcessPt->nextProcess;
			}
			tempProcessPt->nextProcess = elementPt;
		}
		return 0;
	}

}

void process_start(void) {
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; // Enable clock to PIT module
	PIT->MCR = (0 << 1); // Enable clock for MCR
	PIT->CHANNEL[0].LDVAL = 0xFFF00; // Set load value of zeroth PIT
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK; // Enable timer for interrupts
	NVIC_EnableIRQ(PIT_IRQn);
	if(listStart == NULL) { return; }
	process_begin();
}

unsigned int * process_select(unsigned int * cursp){
	return 0;
}
