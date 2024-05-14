/*
 * test1.c
 *
 * Created: 2023-05-02 오후 6:59:10
 * Author : LG
 */ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include "UART.h"

void INIT_PORT(void)
{
	DDRB = 0x00;
}

int main(void)
{
	UART_INIT();
	INIT_PORT();
	while (1)
	{
		unsigned char input = PINB;
		for (char i = 0x02; i != 0x20; i = i << 1)
		{
			if ((input & i) != 0x00)
				UART_transmit('O');
			else
				UART_transmit('X');
			UART_transmit(' ');
		}
		
		UART_transmit('\n');
		_delay_ms(2000);
	}
	
}

