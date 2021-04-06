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
	unsigned int* curr_sp;
	unsigned int* init_sp;
	struct process_state* nextProcess;
};

process_t* process_queue = NULL;
process_t* current_process = NULL;


int process_create (void (*f)(void), int n) {
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; // Enable clock to PIT module
	PIT->MCR = (0 << 1); // Enable clock for MCR
	// Disable Global interrupts
	uint32_t m;
	m = __get_PRIMASK();
	__disable_irq();


	unsigned int * procPointer =  process_stack_init(*f, n);

	if(procPointer == NULL) {
		return -1;
	} else {
		//Initializes the process to be added onto the linked list
		process_t* elementPt;
		elementPt = malloc(sizeof(process_t));
		elementPt->curr_sp = procPointer;
		elementPt->init_sp = procPointer;
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
		__set_PRIMASK(m);

		return 0;
	}

}

void process_start(void) {
	NVIC_EnableIRQ(PIT_IRQn);
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; // Enable clock to PIT module
	PIT->MCR = (0 << 1); // Enable clock for MCR
	PIT->CHANNEL[0].LDVAL = 0x0FF00; // Set load value of zeroth PIT
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK; // Enable timer for interrupts
	process_begin();
}

unsigned int * process_select(unsigned int * cursp) {
	//check base cases
	if(process_queue == NULL) { return NULL; }
	else if(current_process == NULL) {
		current_process = process_queue;
		return current_process->curr_sp;
	} else {
		//if there is no stack pointer, then there is no process
		if(cursp == NULL){
			free(current_process);
			process_stack_free(current_process->init_sp, current_process->size);
			//takes process off of queue by changing start
			process_queue = current_process->nextProcess;
		} else {
			current_process->curr_sp = cursp;
			//changes start of the list
			process_queue = current_process->nextProcess;
			//If the length of the list is 1 (next is NULL)
			if(process_queue == NULL) {
				process_queue = current_process;
			} else {
				//takes end of list and puts it at beginning
				process_t* tempProcessPt;
				while (tempProcessPt->nextProcess != NULL) {
					tempProcessPt = tempProcessPt->nextProcess;
				}
				tempProcessPt->nextProcess = current_process;
			}
			current_process->nextProcess = NULL;
		}
	}
	return current_process->curr_sp;
}
