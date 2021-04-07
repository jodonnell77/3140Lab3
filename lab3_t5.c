/*
 * lab3_t5.c
 *
 *  Created on: Apr 5, 2021
 *      Author: JohnO
 */

/*
 * Test case where there are two process and both exit immediately.
 */

#include "3140_concur.h"
#include "utils.h"

void p1 (void)
{

}

void p2 (void)
{

}

int main (void){

 LED_Initialize();

 LEDBlue_On();

 if (process_create (p1,32) < 0) {
 	return -1;
 }

 if (process_create (p2,32) < 0) {
 	return -1;
 }

 process_start ();

 LEDGreen_Toggle();

 while (1) ;

 return 0;
}
