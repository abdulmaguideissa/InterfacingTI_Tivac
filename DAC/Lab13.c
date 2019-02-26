// Lab13.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// edX Lab 13 
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC
// Port E bits 3-0 have 4 piano keys

#include "..//tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"
unsigned long data;
// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Debug_Init(void);
void delay(unsigned long msec);

int main(void){ // Real Lab13
	DisableInterrupts();
	// for the real board grader to work 
	// you must connect PD3 to your DAC output
  TExaS_Init(SW_PIN_PE3210, DAC_PIN_PB3210,ScopeOn); // activate grader and set system clock to 80 MHz
// PortE used for piano keys, PortB used for DAC        
  Sound_Init(); // initialize SysTick timer and DAC
  Piano_Init();
	Debug_Init();
  EnableInterrupts();  // enable after all initialization are done
  while(1){    
// input from keys to select tone
		data = Piano_In();     
		if(data == 0x01){          //PE0 is Pushed
			Sound_Tone(2389);        //generate 523.257 Hz (C) note
			//GPIO_PORTF_DATA_R ^= 0x02;
			//delay(1000);
		}else if(data == 0x02){    //PE1 is Pushed
			Sound_Tone(2128);        //generate 587.33 HZ (D) note.
			//delay(1000);
				//		GPIO_PORTF_DATA_R ^= 0x02;
		}else if(data == 0x04){    //PE2 is Pushed
			Sound_Tone(1896);        //generate 659.255 HZ (E) note.
			//delay(1000);
					//				GPIO_PORTF_DATA_R ^= 0x02;
		}else if(data == 0x08){    //PE3 is Pushed
			Sound_Tone(1594);        //generate 783.991 HZ (G) note.
			//delay(1000);
						//			GPIO_PORTF_DATA_R ^= 0x02;
		}else {                    //Turn off sound if otherwise.
			Sound_Off();
		}
		 delay(10);
  }       
}
/*-------------------------------------------*/
/* ------------- Declarations ------------- */
void Debug_Init(void){//unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000020;                       //set clock for port F
	SYSCTL_RCGC2_R;                             //delay to stablize the port clock.
	GPIO_PORTF_PCTL_R &= ~0x000000F0;                   //regular function for PF0-3
	GPIO_PORTF_AMSEL_R &= ~0x02;                        //disable port F analog function
	GPIO_PORTF_AFSEL_R &= ~0x02;                        //disable port F alternative functions 
	GPIO_PORTF_DIR_R |= 0x02;                           //PF1 outputs.
	GPIO_PORTF_DEN_R |= 0x02;                           //enable digital functions for port F.
	
	/*---------------------------------*/
	SYSCTL_RCGC2_R |= 0x00000008;                       //set clock for port D
	SYSCTL_RCGC2_R;                             //delay to stablize the port clock.
	GPIO_PORTD_PCTL_R &= ~0x0000F000;                   //regular function for PD3
	GPIO_PORTD_AMSEL_R &= ~0x08;                        //disable port D analog function
	GPIO_PORTD_AFSEL_R &= ~0x08;                        //disable port D alternative functions 
	GPIO_PORTD_DIR_R &= ~0x08;                          //PD3 inputs.
	GPIO_PORTD_DEN_R |= 0x08;                           //enable digital functions for port D.
}
// Inputs: Number of msec to delay
// Outputs: None
void delay(unsigned long msec){ 
  unsigned long count;
  while(msec > 0 ) {  // repeat while there are still delay
    count = 16000;    // about 1ms
    while (count > 0) { 
      count--;
    } // This while loop takes approximately 3 cycles
    msec--;
  }
}


