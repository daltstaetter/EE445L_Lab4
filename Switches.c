// Dalton Altstaetter
// 2/11/14
// Switches.c
// Switches module for the stepper motor
// Meant for the LM3S1968

// Use PB4,PB5,PB7 for the input switches bc of their location on the board
// Use PA0,PA2,PA4,PA6 for the outputs bc of their location on the board


#include "lm3s1968.h"
#include "Switches.h"


/*static unsigned long DeterminePortX(volatile unsigned long* baseAddress)
{
	volatile unsigned long SYSCTL_RCGC2_GPIOX = 0;
	
	if(baseAddress == GPIO_PORTA_DATA_BITS_R)
	{
		SYSCTL_RCGC2_GPIOX = SYSCTL_RCGC2_GPIOA;
	}
	else if(baseAddress == GPIO_PORTB_DATA_BITS_R)
	{
		SYSCTL_RCGC2_GPIOX = SYSCTL_RCGC2_GPIOB;
	}
	else if(baseAddress == GPIO_PORTC_DATA_BITS_R)
	{
		SYSCTL_RCGC2_GPIOX = SYSCTL_RCGC2_GPIOC;
	}
	else if(baseAddress == GPIO_PORTD_DATA_BITS_R)
	{
		SYSCTL_RCGC2_GPIOX = SYSCTL_RCGC2_GPIOD;
	}
	else if(baseAddress == GPIO_PORTE_DATA_BITS_R)
	{
		SYSCTL_RCGC2_GPIOX = SYSCTL_RCGC2_GPIOE;
	}
	else if(baseAddress == GPIO_PORTF_DATA_BITS_R)
	{
		SYSCTL_RCGC2_GPIOX = SYSCTL_RCGC2_GPIOF;
	}
	else if(baseAddress == GPIO_PORTG_DATA_BITS_R)
	{
		SYSCTL_RCGC2_GPIOX = SYSCTL_RCGC2_GPIOG;
	}
	else if(baseAddress == GPIO_PORTH_DATA_BITS_R)
	{
		SYSCTL_RCGC2_GPIOX = SYSCTL_RCGC2_GPIOH;
	}
	
	return SYSCTL_RCGC2_GPIOX;
}
*/

static void Delay(unsigned long count)
{
	while(count)
	{
		count--;
	}
}



void GPIO_PortX_Init(volatile unsigned long* baseAddress, unsigned long bits, unsigned int input_output)
{
	// Let X be the letter of the port you wish to initialize
	// Enables PortX
	SYSCTL_RCGC2_R |= (SYSCTL_RCGC2_GPIOA + SYSCTL_RCGC2_GPIOB); //DeterminePortX(baseAddress);
	Delay(100); // give it time to enable the port
	
	if(input_output == OUTPUT)
	{
		*(baseAddress+0x0400) |= bits; // makes them output pins
	}
	else
	{
		*(baseAddress+0x0400) &= ~bits; // makes them input pins
	}
	
	*(baseAddress+0x051C) |= bits;    // digital enabled 
	*(baseAddress+0x0420) &= ~bits; // disable alternate function
}




void PortB_Init(void)
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
	
	// dummy instructions to allow the port time to initialize
	Delay(100);
	
	GPIO_PORTB_DIR_R &= ~0x38;   // make PB3,4,5,7 inputs
	GPIO_PORTB_DEN_R |= 0x38;    // digital enable
	GPIO_PORTB_AFSEL_R &= ~0x38; // disable alternate function
	
	GPIO_PORTB_DIR_R |= 0x47;   // make PB0-3,6 inputs
	GPIO_PORTB_DEN_R |= 0x47;    // digital enable
	GPIO_PORTB_AFSEL_R &= ~0x47; // disable alternate function
}

void PortD_Init(void)
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
	
	// dummy instructions to allow the port time to initialize
	Delay(100);
	
	GPIO_PORTD_DIR_R |= 0x0F;   // make outputs
	GPIO_PORTD_DEN_R |= 0x0F;    // digital enable
	GPIO_PORTD_AFSEL_R &= ~0x0F; // disable alternate function
}

void PortA_Init(void)
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	
	// dummy instructions to allow the port time to initialize
	Delay(100);
	
	GPIO_PORTA_DIR_R |= 0xAA;    // make PA1,3,5,7 outputs
	GPIO_PORTA_DIR_R &= ~0x55;    // make PA1,3,5,7 outputs
	GPIO_PORTA_DEN_R |= 0xFF;    // digital enable
	GPIO_PORTA_AFSEL_R &= ~0xFF; // disable alternate function
}

void GPIO_Init(void)
{
	//PortA_Init();
	PortB_Init();	
	PortD_Init();
}

