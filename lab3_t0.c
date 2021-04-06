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
