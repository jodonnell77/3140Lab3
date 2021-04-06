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

process_t* process_queue = NULL;
process_t* current_process = NULL;


int process_create (void (*f)(void), int n) {
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; // Enable clock to PIT module
	PIT->MCR = (0 << 1); // Enable clock for MCR
	// Disable Global interrupts
	PIT->CHANNEL[0].TCTRL = 1;


	unsigned int * procPointer =  process_stack_init(*f, n);

	if(procPointer == NULL) {
		return -1;
	} else {
		//Initializes the process to be added onto the linked list
		process_t* elementPt;
		elementPt = malloc(n*sizeof(process_t));
		elementPt->sp = procPointer;
		elementPt->nextProcess = NULL;
		elementPt->size = n;

		//Make this the only element of the list if the list is empty
		if(process_queue == NULL) {
			process_queue = elementPt;

		} else {
			//Create a temporary process state, and keep updating it until it is the last element
			process_t* tempProcessPt;
			tempProcessPt = process_queue;
			while (tempProcessPt->nextProcess != NULL) {
				tempProcessPt = tempProcessPt->nextProcess;
			}
			tempProcessPt->nextProcess = elementPt;
		}

		// Re-enable global interrupts
		PIT->CHANNEL[0].TCTRL = 3;

		return 0;
	}

}

void process_start(void) {
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; // Enable clock to PIT module
	PIT->MCR = (0 << 1); // Enable clock for MCR
	PIT->CHANNEL[0].LDVAL = 0xFFF00; // Set load value of zeroth PIT
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK; // Enable timer for interrupts
	NVIC_EnableIRQ(PIT_IRQn);
	if(process_queue == NULL) { return; }
	process_begin();
}

unsigned int * process_select(unsigned int * cursp) {
	//check base cases
	if(process_queue == NULL) { return NULL; }
	else if(current_process == NULL) {
		current_process = process_queue;
		return current_process->sp;
	} else {
		//if there is no stack pointer, then there is no process
		if(cursp == NULL){
			process_stack_free(current_process->sp, current_process->size);
			free(current_process);
			//takes process off of queue by changing start
			process_queue = current_process->nextProcess;
		} else {
			//changes start of the list
			process_queue = current_process->nextProcess;
			//If the length of the list is 1 (next is NULL)
			if(process_queue == NULL) {
				current_process = process_queue;
			} else {
				//takes end of list and puts it at beginning
				process_t* tempProcessPt;
				while (tempProcessPt->nextProcess != NULL) {
					tempProcessPt = tempProcessPt->nextProcess;
				}
				tempProcessPt->nextProcess = current_process;
			}
		}
	}
	return current_process->sp;
}
