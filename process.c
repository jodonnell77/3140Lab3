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
	unsigned int * sp;
	struct process_state* nextProcess;
};

struct process_t * listStart = NULL;


int process_create (void (*f)(void), int n) {
	unsigned int * procPointer =  process_stack_init(*f, n);

	process_t *tempProcess;
	process_t *element;
	if(procPointer == NULL) {
		return -1;
	} else {
		element->sp = procPointer;
		element->nextProcess = NULL;
		if(listStart == NULL) {
			listStart = element;
		} else {
			tempProcess = listStart;
		}
		return 0;
	}

}

void process_start(void) {

}

unsigned int * process_select(unsigned int * cursp){
	return 0;
}






