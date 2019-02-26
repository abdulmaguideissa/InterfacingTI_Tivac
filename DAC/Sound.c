// Sound.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// This routine calls the 4-bit DAC

#include "Sound.h"
#include "DAC.h"
#include "..//tm4c123gh6pm.h"

const unsigned char sine[64] = {8,8,9,10,10,11,12,12,13,13,14,14,14,15,15,15,15,15,15,15,14,14,14,13,13,12,11,11,10,9,
																9,8,7,6,6,5,4,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,3,4,5,5,6,7,8};
unsigned long i;
// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Also calls DAC_Init() to initialize DAC
// Input: none
// Output: none
void Sound_Init(void){
	//systick is not compeletly initialized.
	//the reload value is in sound tone function.
	DAC_Init();                                                           //call DAC initialization 
	i = 0;
	NVIC_ST_CTRL_R = 0x00;                                                //1)clear control register.
	NVIC_ST_CURRENT_R = 0;                                                //2)set current register to 0.
	NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF;                         //3) set the sysTick priority.
	NVIC_ST_CTRL_R = 0x00000007;                                          //4)enable clock, counter, interrupts
}

// **************Sound_Tone*********************
// Change Systick periodic interrupts to start sound output
// Input: interrupt period
//           Units of period are 12.5ns
//           Maximum is 2^24-1
//           Minimum is determined by length of ISR
// Output: none
// period = System clock / (required frequency / n)
// n in this case  = 16.
void Sound_Tone(unsigned long period){
// this routine sets the RELOAD and starts SysTick
	NVIC_ST_CTRL_R = 0x00;
	NVIC_ST_CURRENT_R = 0x00;
  NVIC_ST_RELOAD_R = period-1;
	NVIC_ST_CTRL_R = 0x07;
}


// **************Sound_Off*********************
// stop outputing to DAC
// Output: none
void Sound_Off(void){
 // this routine stops the sound output
	DAC_Out(0x00);
}


// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
   i = (i+1)%64;   //increment from 0 to 15
	DAC_Out(sine[i]);  //pick the sine wave value from the global table.
}
