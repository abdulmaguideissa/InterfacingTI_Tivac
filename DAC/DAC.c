// DAC.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Implementation of the 4-bit digital to analog converter
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC

#include "DAC.h"
#include "..//tm4c123gh6pm.h"

// **************DAC_Init*********************
// Initialize 4-bit DAC 
// Input: none
// Output: none
void DAC_Init(void){//unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000002;                       //set clock for port B
	 SYSCTL_RCGC2_R;                             //delay to stablize the port clock.
	GPIO_PORTB_PCTL_R &= ~0x0000FFFF;                   //regular function for PB0-3
	GPIO_PORTB_AMSEL_R &= ~0x0F;                        //disable port B analog function
	GPIO_PORTB_AFSEL_R &= ~0x0F;                        //disable port B alternative functions 
	GPIO_PORTB_DIR_R |= 0x0F;                           //PB0-3 outputs.
	GPIO_PORTB_DR8R_R |= 0x0F;                          //set current driver for Port E.
	GPIO_PORTB_DEN_R |= 0x0F;                           //enable digital functions for port B.
}


// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned long data){
  GPIO_PORTB_DATA_R = data;          //set the data register with the input from sound drive.
}
