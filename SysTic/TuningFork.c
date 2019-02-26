// TuningFork.c Lab 12
// Runs on LM4F120/TM4C123
// Use SysTick interrupts to create a squarewave at 440Hz.  
// There is a positive logic switch connected to PA3, PB3, or PE3.
// There is an output on PA2, PB2, or PE2. The output is 
//   connected to headphones through a 1k resistor.
// The volume-limiting resistor can be any value from 680 to 2000 ohms
// The tone is initially off, when the switch goes from
// not touched to touched, the tone toggles on/off.
//                   |---------|               |---------|     
// Switch   ---------|         |---------------|         |------
//
//                    |-| |-| |-| |-| |-| |-| |-|
// Tone     ----------| |-| |-| |-| |-| |-| |-| |---------------
//
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015

 Copyright 2016 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */


#include "TExaS.h"
#include "..//tm4c123gh6pm.h"
//#include "PLL.h"


// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode
#define PA2 (*(volatile unsigned long *)0x40004010)
#define PA3 (*(volatile unsigned long *)0x40004020)
#define PRESSED 0x08                                  // for button at PA3
#define ON      1                                     // for current state


unsigned long H ,L; 
unsigned long cState, preState;
// input from PA3, output from PA2, SysTick interrupts
void Sound_Init(void){ volatile unsigned long delay;
	L = 22727;
	H = 90909;
	
	SYSCTL_RCGC2_R |= 0x00000001;             //activate port A
	delay = SYSCTL_RCGC2_R;                   //delay for clock to stablize.
	GPIO_PORTA_AMSEL_R &= ~0x04;              //clear analog functions for PA2, PA3
  GPIO_PORTA_PCTL_R &= ~0x00000F00;         // regular function
  GPIO_PORTA_AFSEL_R &= ~0x04;	            //clear alternate function for PA2, PA3
	GPIO_PORTA_DIR_R |= 0x04;                 //make PA2 as output
	GPIO_PORTA_DEN_R |= 0x04;
	GPIO_PORTA_DR8R_R |= 0x04;
	NVIC_ST_CTRL_R = 0x00;                    //clear control register.
	NVIC_ST_RELOAD_R = L - 1;                 //load for 2.27ms periodic interrupt
	NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R =NVIC_SYS_PRI3_R&0x00FFFFFF;      // priority 0
	NVIC_ST_CTRL_R |= 0x00000007;              //enable the core clock and the interrupts
	GPIO_PORTA_DATA_R &= ~0x04;                //clear output initially.
	//EnableInterrupts();
}
//Switch PA3
void Switch_Init(void){ volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000001;             //activate port A
	delay = SYSCTL_RCGC2_R;                   //delay for clock to stablize.
	GPIO_PORTA_AMSEL_R &= ~0x08;              //clear analog functions for PA2, PA3
  GPIO_PORTA_PCTL_R &= ~0x0000F000;         // regular function
  GPIO_PORTA_AFSEL_R &= ~0x08;	            //clear alternate function for PA2, PA3
	GPIO_PORTA_DIR_R  &= ~0x08;               //make PA3 as input
	GPIO_PORTA_DEN_R |= 0x08;
/*	GPIO_PORTA_PDR_R |= 0x08;                 //activate pull down resistor for PA3
	GPIO_PORTA_IS_R &= ~0x08;                 //clear interrupt sense
	GPIO_PORTA_IBE_R &= ~0x08;                //clear both edges
	GPIO_PORTA_IEV_R &= ~0x08;                //falling edge.
	GPIO_PORTA_ICR_R &= ~0x08;                //clear flags on PA, PA3
	GPIO_PORTA_IM_R |= 0x08;                  //arm the interrutps on PA2, PA3.
	NVIC_PRI0_R =(NVIC_PRI7_R&0xFF00FFFF)|0x00400000;     // priority 0
	NVIC_EN0_R = 0x40000000;*/
}
// called at 880 Hz
void SysTick_Handler(void){
	/*output is function of the input and the previous state*/
	if(PA3 == PRESSED && preState != PRESSED)
		cState ^= 0x01;                        //toggle current state
	if(cState == ON)
	  PA2 ^= 0x04;          //toggle PA2.
	else {
		PA2= ~0x04;
	}
	preState = PA3;
}

int main(void){// activate grader and set system clock to 80 MHz
	DisableInterrupts();
  TExaS_Init(SW_PIN_PA3, HEADPHONE_PIN_PA2,ScopeOn); 
  Sound_Init();         
	Switch_Init();
	cState = 0;
	preState = 0;
  EnableInterrupts();   // enable after all initialization are done
  while(1){
    // main program is free to perform other tasks
	//	SysTick_Handler();
    // do not use WaitForInterrupt() here, it may cause the TExaS to crash
  }
}
