```c
/*
 * LED_test.c
 *
 * Created: 2023-06-18 3:42:10 AM
 * Author : LG
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "UART.h"
#include "LCD.h"


#define ROTATION_DELAY 	5000	// 1 second delay
#define PULSE_MIN		1300	// Minimum pulse length
#define PULSE_MAX		5000	// Maximum pulse length

volatile int count = 0;
volatile char start_count = 0;

#define PRESCALER 1024

// Servo motor + button from here
void INIT_PORT()
{
	DDRB = 0x00;
	PORTB = 0x08;
}

// Button interrupt settings
void INIT_PCINT0(void)
{
	PCICR |= (1 << PCIE0);
	// Use pin 11
	PCMSK0 |= (1 << PCINT3);
	sei();
}

// 5-second interrupt settings
void INIT_TIMER0()
{
	// Prescaler 1024
	TCCR0B |= (1 << CS02) | (1 << CS00);
	// Enable overflow interrupt
	TIMSK0 |= (1 << TOIE0);
	sei();
}

void INIT_TIMER1(void)
{
	// Fast PWM mode, TOP = ICR1
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	
	TCCR1B |= (1 << CS11);		// Prescaler 8, 2MHz
	
	ICR1 = 40000;				// 20ms period
	
	TCCR1A |= (1 << COM1A1);		// Non-inverting mode
	
	// Set OC1A (PB1, Arduino pin 9) as output
	DDRB |= (1 << PB1);
}

void timer_init(void)
{
	// Prescaler 1024
	TCCR2B |= (1 << CS22) | (1 << CS20);
}

uint8_t measure_distance(void)
{
	// Output pulse on trigger pin
	PORTC |= (1 << PC0); // Set HIGH
	_delay_us(10); // Wait for 10 microseconds
	PORTC &= ~(1 << PC0); // Set LOW
	// Wait until echo pin becomes HIGH
	TCNT2 = 0;
	while(!(PINC & 0x02))
	if(TCNT2 > 250) return 255; // No obstacle
	// Measure the time until echo pin becomes LOW
	TCNT2 = 0; // Reset the counter to 0
	while(PINC & 0x02){
		if (TCNT2 > 250){ // Measurement not possible
			TCNT2 = 0;
			break;
		}
	}
	// Calculate the pulse width of the echo pin in microseconds
	double pulse_width = TCNT2 * PRESCALER * 1000000.0 / F_CPU;
	return pulse_width / 58; // Return distance in centimeters
}

ISR(PCINT0_vect)
{
	// Button interrupt occurred
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		count = 0;
		start_count = 1;
	}
}

// Timer overflow interrupt occurred
ISR(TIMER0_OVF_vect)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		count++;
	}
	
}

void initializeAll()
{
	INIT_PORT();
	INIT_PCINT0();
	INIT_TIMER0();
	INIT_TIMER1();

	
	
	DDRC |= 0x01;
	DDRC &= 0xFD;
	DDRD |= 0x80;
	
	timer_init(); // Initialize timer

	LCD_init();
}


int main(void)
{
	uint8_t distance;
	char LCD_turned_on = 0;
	
	initializeAll();
	
	LCD_write_string("Initializing...");
	_delay_ms(2000);
	LCD_clear();

	while(1)
	{
		distance = measure_distance(); // Measure distance

		// Ultrasonic sensor detected
		if ((uint32_t) distance < (uint32_t)30)
		{
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
			{
				count = 0;	
			}
		}
			
		// Door opened
		if ((count <= 320 && start_count == 1))
		{
			OCR1A = PULSE_MAX;
			if (LCD_turned_on == 0)
			{
				LCD_write_string("Welcome!!!");	
				LCD_turned_on = 1;
			}
		}
		else
		{
			// Door closed
			OCR1A = PULSE_MIN;
			LCD_clear();
			LCD_turned_on = 0;
		}
	}
}
```