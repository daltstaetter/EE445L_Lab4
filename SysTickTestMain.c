// SysTickTestMain.c
// Runs on LM3S1968
// Test the SysTick functions by activating the PLL, initializing the
// SysTick timer, and flashing an LED at a constant rate.
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

// PG2 is an output for debugging

#include "SysTick.h"
#include "PLL.h"
#include "lm3s1968.h"
#include "Switches.h"
#include "FSM.h"

//-------- All Global Variables are Here-----------------------
//order for next states are: clockwise, counterclockwise
//SM FSM[SIZE] = {
//	{ 0x21, 10, {six, nine}},
//	{ 0x81, 10, {ten, five}},
//	{ 0x84, 10, {nine, six}},
//	{ 0x24, 10, {five, ten}}
//};


//	{ 0x21, 10, {six, nine}},
//	{ 0x24, 10, {ten, five}},
//	{ 0x84, 10, {nine, six}},
//	{ 0x81, 10, {five, ten}}

#define DELAY 1
SM FSM[SIZE] = {
	{ 0x05, DELAY, {six, nine}},
	{ 0x06, DELAY, {ten, five}},
	{ 0x0A, DELAY, {nine, six}},
	{ 0x09, DELAY, {five, ten}}
};

//SM FSM[SIZE] = {
//  { 0x05, 4, {six, nine}},
//	{ 0x06, 4, {ten, five}},
//	{ 0x0A, 4, {nine, six}},
//	{ 0x09, 4, {five, ten}}
//};



SM *StatePt;
unsigned long currentState;
unsigned int Switch3_Flag;

//--------------------------------------------------------------

void FSM_Init(void)
{ 
	// initializes FSM
	return;
}

int main(void)
{
	DisableInterrupts();
	//GPIO_PortX_Init(GPIO_PORTA_DATA_BITS_R,0xAA,OUTPUT);
	//GPIO_PortX_Init(GPIO_PORTB_DATA_BITS_R,0xB0,INPUT);
	GPIO_Init();
	
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG; // activate port G
  PLL_Init();               // set system clock to 50 MHz
  //SysTick_Init(755000);    // Set interrrupt time at 10Hz. 1e6 => 2 milliseconds => 500Hz
	  SysTick_Init(600000);    // Set interrrupt time at 10Hz. 1e6 => 2 milliseconds => 500Hz
	
	
	
	
	currentState = 0; // begin with output 5.
	//Switch3_Flag = 0;
	
	GPIO_PORTD_DATA_R = FSM[currentState].output;
	//  GPIO_PORTG_DIR_R |= 0x04; // make PG2 out (built-in LED)
//  GPIO_PORTG_AFSEL_R &= ~0x04;// disable alt funct on PG2
//  GPIO_PORTG_DEN_R |= 0x04; // enable digital I/O on PG2
	EnableInterrupts();
  while(1)
	{
		// wait for periodic SysTick interrupt
		// All the action of this program will occure in the SysTick Interrupt
    // GPIO_PORTG_DATA_R = GPIO_PORTG_DATA_R^0x04; // toggle PG2

  }
}





