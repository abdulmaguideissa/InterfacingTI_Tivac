// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// January 15, 2016
//      Jon Valvano and Ramesh Yerraballi

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"
#include "main.h"
// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void PortEInit(void);        //initialize port E
//void Delay(unsigned long delay);

unsigned long SW;
// ***** 3. Subroutines Section *****

// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).
int main(void){ 
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);  // activate grader and set system clock to 80 MHz
  PortEInit();                   //intialize port E.
  EnableInterrupts();           // enable interrupts for the grader
	
  while(1){
   SysTick_Wait10ms(10);                    //2) Dealy 100ms.
	 SW = GPIO_PORTE_DATA_R&0x01;  //read pin 0 of port E to SW.
   if(SW == 0x01){                //3) Check if the SW is pressed.
		 GPIO_PORTE_DATA_R ^= 0x02;   //4) Toggle the LED if it is.
		// Delay(1);
	 } else{                         //5) Turn the LED on if not.
      GPIO_PORTE_DATA_R |= 0x02;
	}		 
 }
}


void PortEInit(void){volatile unsigned long Delay;
	SYSCTL_RCGC2_R |= 0x10;            // 1) E clock
  Delay = SYSCTL_RCGC2_R;            // delay to stablize the register.
	GPIO_PORTE_DIR_R  |= 0x02;         //PE1 is output
	GPIO_PORTE_DIR_R  &= ~0x01;        //PE0 is input.
 	GPIO_PORTE_AMSEL_R &= ~0x03;       //Disable analog functions 
  GPIO_PORTE_AFSEL_R &= ~0x03;	     //Disable alternate functions
	GPIO_PORTE_PCTL_R &= ~0x000000FF;  
	GPIO_PORTE_DEN_R  |= 0x03;         //Enable digital functions.
	GPIO_PORTE_DATA_R |= 0x02;     //1) LED is on.
}

/*void Delay(unsigned long delay){
	volatile unsigned long loops;
	while(delay > 0){
		loops = 615384;
		while(loops > 0){
			loops--;
		}
		delay--;
	}
}*/
