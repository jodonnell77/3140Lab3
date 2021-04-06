/*
 * lab3_t1.c
 *
 *  Created on: Apr 1, 2021
 *      Author: JohnO
 */


/*
 * Test case where process_start is called before processes have been created
 * This is a trivial case as nothing should happen.
 */


#include "3140_concur.h"
#include "utils.h"

void p1 (void)
{
	int i;
	for (i=0; i < 6; i++) {
		delay ();
    LEDRed_Toggle();
	}
}

void p2 (void)
{
	int i;
	for (i=0; i < 13; i++) {
		delay ();
    LEDBlue_Toggle();
	}
}


int main (void){

 LED_Initialize();
 LED_Off();

 process_start ();

 LEDGreen_Toggle();

 while (1) ;

 return 0;
}


