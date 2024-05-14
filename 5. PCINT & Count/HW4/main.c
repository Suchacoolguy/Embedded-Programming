/*
 * HW4.c
 *
 * Created: 2023-05-15 2:07:39 AM
 * Author : Sangheon's LG Gram
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>
#include "UART.h"

// Using the volatile keyword to prevent the compiler from optimizing out this variable declaration.
volatile int count = 0;
volatile char flag = 0;

void INIT_PORT()
{
	DDRD = 0x00; // Button inputs
	PORTD = 0xA8; // Set the bit values of PORTD register corresponding to PD3, 5, 7 to 1 for using built-in pull-up resistors in input state.
}

void INIT_PCINT2()
{
	PCICR |= (1 << PCIE2);     // Enable PORTD pin change interrupts
	PCMSK2 |= (1 << PCINT19);    // PD3
	PCMSK2 |= (1 << PCINT21);    // PD5
	PCMSK2 |= (1 << PCINT23);    // PD7
	sei();   // Enable global interrupts
}

ISR(PCINT2_vect)
{	
	count++;
	
	// To ensure printing only once in main function, not repeatedly.
	flag = 1;
	
	// Solve switch bounce problem by waiting for 10 ms
	_delay_ms(10);
}

int main(void)
{
	INIT_PORT();
	UART_INIT();
	INIT_PCINT2();
	
	int local_count = 0;
    while (1) 
    {
		// Blocking interrupts during the assignment of a value to a local variable.
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) 
		{
			local_count = count;	
		}
		
		if ((flag != 0) && (local_count % 3 == 0))
		{
			UART_printString("current count: ");
			UART_printNumber(local_count);
			UART_printString("\n");
			flag = 0;
		}
    }
}
