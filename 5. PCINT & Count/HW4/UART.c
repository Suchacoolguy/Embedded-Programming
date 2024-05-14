#include "UART.h"

void UART_INIT(void)
{
	// 2 times faster transmission
	UCSR0A |= _BV(U2X0);

	// setting baud rate
	UBRR0H = 0x00;
	UBRR0L = 207;

	// async, 8-bit data, no parity, 1 stop bit
	UCSR0C |= 0x06;

	// send & receive
	UCSR0B |= _BV(RXEN0);
	UCSR0B |= _BV(TXEN0);
}

unsigned char UART_receive(void)
{
	// waits to receive data;
	while ( !(UCSR0A & (1 << RXC0)) );
	return UDR0;
}

void UART_transmit(unsigned char data)
{
	while ( !(UCSR0A & (1 << UDRE0)) );
	UDR0 = data;
}

void UART_printString(char *str)
{
	for (int i = 0; str[i]; i++)
		UART_transmit(str[i]);
}

void UART_printNumber(uint32_t num)
{
	int i;
	char buf[10];
	
	for (i = 0; num > 0; ++i)
	{
		buf[i] = num % 10 + '0';
		num /= 10;
	}
	
	for (int j = i - 1; j >= 0; --j)
		UART_transmit(buf[j]);
}
