/*
 * HW4_2.c
 *
 * Created: 2023-05-21 7:00:21 PM
 * Author : LG
 */ 
#define F_CPU 16000000L
#define FORWARD 0
#define BACKWARD 1
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

// Using the volatile keyword to prevent the compiler from optimizing out this variable declaration.
volatile char flag = 0;

void INIT_PORT()
{
	DDRD = 0xFF;
	PORTD = 0x00;
}

ISR(TIMER1_COMPA_vect)
{
	TCNT1 = 0;  // Initialize the timer value to 0
	flag = 1;
}

int main(void)
{
	INIT_PORT();
	
	// Set prescaler to 1024
	TCCR1B |= (1 << CS12) | (1 << CS10); 
	
	// Since overflow occurs 32 times with 8-bit timer and takes about 0.5 seconds,
	// when using 16-bit timer TCNT1, set the baseline value to 256 * 32, which is 0x2000 in hexadecimal.
	OCR1A = 0x2000; 
	
	TIMSK1 |= (1 << OCIE1A);  // Enable compare match interrupt
	sei();
	
	bool direction = FORWARD;
	char output = 0x88;
	
    while (1) 
    {
		if (flag == 1)
		{
			if ((direction == FORWARD && output == 0x11) || (direction == BACKWARD && output == 0x88))
				direction = !direction;
			else
				output = direction == FORWARD ? output >> 1 : output << 1;
			flag = 0;
		}
		
		PORTD = output;
    }
}

