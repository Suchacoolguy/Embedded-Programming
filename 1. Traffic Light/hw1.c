#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	DDRB = 0x01;
	DDRC = 0x04;
	DDRD = 0x20;

	while (1) {
		PORTB = 0x01;
		_delay_ms(500);
		PORTB &= ~(0x01);
		_delay_ms(500);
		PORTC = 0x04;
		_delay_ms(500);
		PORTC &= ~(0x04);
		_delay_ms(500);
		PORTD = 0x20;
		_delay_ms(500);
		PORTD &= ~(0x20);
		_delay_ms(500);
	}
		return 1;
}
