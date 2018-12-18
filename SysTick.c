// SysTick.c
// Runs on LM3S1968
// Provide functions that initialize the SysTick module, wait at least a
// designated number of clock cycles, and wait approximately a multiple
// of 10 milliseconds using busy wait.  After a power-on-reset, the
// LM3S1968 gets its clock from the 12 MHz internal oscillator, which
// can vary by +/- 30%.  If you are using this module, you probably need
// more precise timing, so it is assumed that you are using the PLL to
// set the system clock to 50 MHz.  This matters for the function
// SysTick_Wait10ms(), which will wait longer than 10 ms if the clock is
// slower.
// Daniel Valvano
// February 22, 2012

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011
   Program 2.11, Section 2.6

 Copyright 2011 by Jonathan W. Valvano, valvano@mail.utexas.edu
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

#include "FSM.h"
#include "lm3s1968.h"
#include "SysTick.h"

extern SM* StatePt; // Ptr to our FSM object which we will use to move between states
extern SM FSM[SIZE];   // our FSM object
extern unsigned int currentState;
extern unsigned int Switch3_Flag;

void SysTick_Wait(unsigned long delay);
void SysTick_Wait10ms(unsigned long delay);
void SysTick_Init(unsigned long reloadValue);
void Turn_Motor(const int Direction);
void PressSwitch1(void);
void PressSwitch2(void);
void PressSwitch3(void);
void PressSwitch12(unsigned long* count);
unsigned int GetButtonPress(void);


int cw_Switch1_Only;
int cw_Switch3_Only;
int ccw_Switch2_Only;
int cw_ccw_Switch_12;

void SysTick_Handler(void)
{
	static unsigned long count = 0;
	int data;
	int logic;
	DisableInterrupts();
	
	logic = GetButtonPress();
	
	// or switch statement
	switch(logic)
	{
		// need to add logic for when multiple ones are pressed.
		case 1:
			GPIO_PORTB_DATA_R |= 0x01;data = GPIO_PORTB_DATA_R;
			PressSwitch1();
			GPIO_PORTB_DATA_R &= ~0x01;data = GPIO_PORTB_DATA_R;
			break;
		case 2:
			GPIO_PORTB_DATA_R |= 0x02;data = GPIO_PORTB_DATA_R;
			PressSwitch2();
		  GPIO_PORTB_DATA_R &= ~0x02;data = GPIO_PORTB_DATA_R;
			break;
		case 3:
			GPIO_PORTB_DATA_R |= 0x40;data = GPIO_PORTB_DATA_R;
			PressSwitch3();
		  GPIO_PORTB_DATA_R &= ~0x40;data = GPIO_PORTB_DATA_R;
			break;
		case 12:
			GPIO_PORTB_DATA_R |= 0x03;data = GPIO_PORTB_DATA_R;
			PressSwitch12(&count);
		  GPIO_PORTB_DATA_R &= ~0x03;data = GPIO_PORTB_DATA_R;
			break;
		default:
			break;
	}		
	EnableInterrupts();
}

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(unsigned long reloadValue)
{
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = reloadValue;       // reload value ->___seconds
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
                                        // enable SysTick with core clock
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC+NVIC_ST_CTRL_INTEN;
}
// Time delay using busy wait.
// The delay parameter is in units of the core clock. (units of 20 nsec for 50 MHz clock)
void SysTick_Wait(unsigned long delay)
{
  volatile unsigned long elapsedTime;
  unsigned long startTime = NVIC_ST_CURRENT_R;
  do{
    elapsedTime = (startTime-NVIC_ST_CURRENT_R)&0x00FFFFFF;
  }
  while(elapsedTime <= delay);
}
// Time delay using busy wait.
// This assumes 50 MHz system clock.
void SysTick_Wait10ms(unsigned long delay)
	{
  unsigned long i;
		
  for(i=0; i<delay; i++)
	{
    SysTick_Wait(500000);  // wait 10ms (assumes 50 MHz clock)
  }
}

void Turn_Motor(const int Direction)
{
	GPIO_PORTD_DATA_R = FSM[currentState].output;
	SysTick_Wait10ms(FSM[currentState].delay);
	currentState = (currentState+SIZE+Direction)%SIZE; // keeps it in the range 0-3
}

void PressSwitch1(void)
{
	SysTick_Wait10ms(1); // for switch debouncing
		
		if(GetButtonPress() == 1)
		{
			Turn_Motor(CW);
  /*
			// send the current output through the ports
//			GPIO_PORTA_DATA_R = FSM[currentState].output;
//			SysTick_Wait10ms(FSM[currentState].delay);
//			currentState = (currentState+1)%SIZE; // keeps it in the range 0-3
//			// update to the next state for the next interrupt
	*/
		}
}


void PressSwitch2(void)
{
	SysTick_Wait10ms(1); // for switch debouncing
	if(GetButtonPress() == 2)
	{
		
/*
		// send the current output through the ports
		GPIO_PORTA_DATA_R = FSM[currentState].output;
		SysTick_Wait10ms(FSM[currentState].delay);
		// keeps it in the range 0-3, adding SIZE to the 
		// currentState keeps  from modding negative numbers.
		currentState = ((currentState+SIZE)-1)%SIZE; 
		// update to the next state for the next interrupt
*/
		Turn_Motor(CCW);
	}
}

void PressSwitch3(void)
{
	SysTick_Wait10ms(1); // for switch debouncing
		if(GetButtonPress() == 3)
		{
	/*		if(Switch3_Flag)
			{
				// do logic so that it waits to see that u release the switch
				// maybe use an interrupt that looks for the low-triggered interrupt
				// and changes the flag back to zero
				// what I want to do is enable an interrupt on switch3 pin when
				// I jump into this elif that then looks for the falling edge of 
				// the switch3 pin and then sets Switch3_Flag=0
				// what I will do instead is polling
				return;
				
			}
			
			// send the current output through the ports
			GPIO_PORTA_DATA_R = FSM[currentState].output;
			SysTick_Wait10ms(FSM[currentState].delay);
			currentState = (currentState+1)%SIZE; // keeps it in the range 0-3
	*/
			
			Turn_Motor(CW);
			// poll until the switch is released
			while(GetButtonPress() == 3)
			{}
		}
}


void PressSwitch12(unsigned long* countPtr)
{
	SysTick_Wait10ms(1); // for switch debouncing
	if(GetButtonPress() == 12)
	{
		if((*countPtr) < 8)
		{
			// rotate CW
			Turn_Motor(CW);
		}
		else if((*countPtr) < 16)
		{
			// rotate CCW
			Turn_Motor(CCW);				
		}
		(*countPtr)++;
		*(countPtr) %= 16; // keep count between 0-15			
	}
}

unsigned int GetButtonPress(void)
{	
	int s1,s2,s3;
	
	s1 = GPIO_PORTB_DATA_R & 0x10;
	s2 = GPIO_PORTB_DATA_R & 0x20;
	s3 = GPIO_PORTB_DATA_R & 0x08;
	
	if(s1 && s2 && !s3)
	{
		return 12;
	}
	else if(s1 && !s3 && !s2)
	{
		return 1;
	}
	else if(s2 && !s3 && !s1)
	{
		return 2;
	}
	else if(s3 && !s1 && !s2)
	{
		return 3;
	}
	else
	{
		return 0;
	}
}




