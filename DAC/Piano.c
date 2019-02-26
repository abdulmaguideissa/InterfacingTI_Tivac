// Piano.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// There are four keys in the piano
// Daniel Valvano
// December 29, 2014

// Port E bits 3-0 have 4 piano keys

#include "Piano.h"
#include "..//tm4c123gh6pm.h"


// **************Piano_Init*********************
// Initialize piano key inputs
// Input: none
// Output: none
void Piano_Init(void){// unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000010;                       //set clock for port E
	 SYSCTL_RCGC2_R;                             //delay to stablize the port clock.
	GPIO_PORTE_PCTL_R &= ~0x0000FFFF;                   //regular functions for PE0-3.
	GPIO_PORTE_AMSEL_R &= ~0x0F;                        //disable port E analog function
	GPIO_PORTE_AFSEL_R &= ~0x0F;                        //disable port E alternative functions 
	GPIO_PORTE_DIR_R &= ~0x0F;                          //PE0-3 inputs.
	GPIO_PORTE_DEN_R |= 0x0F;                           //enable digital functions for port E.
}
// **************Piano_In*********************
// Input from piano key inputs from port E.
// Input: none 
// Output: 0 to 15 depending on keys
// 0x01 is key 0 pressed, 0x02 is key 1 pressed,
// 0x04 is key 2 pressed, 0x08 is key 3 pressed
//data inputs from port E decides the output frequency in main function.
unsigned long Piano_In(void){
	unsigned long data;
	data =  GPIO_PORTE_DATA_R&0x0F;
 return data;                       //return data from PE0-3.
}
