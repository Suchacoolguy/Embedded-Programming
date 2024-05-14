#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

int main(void) {
	int bits = 8;
	DDRD = 0xFF;

	while (1) {
		char a = 0x10;
		char b = 0x08;
		char direction = 1;

		for (int i = 0, j = bits - 2; i < bits, j > 0; i++, j--) {
			if (direction == 1)
				PORTD = a << i | b >> i;
			else if (direction == -1)
				PORTD = a << j | b >> j;

			if ((a << i | b >> i) & 0x81 == 0x81)
				direction = -1;

			_delay_ms(500);
		}
	}
	return 1;

}
