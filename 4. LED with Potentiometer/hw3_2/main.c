/*
 * hw3_2.c
 *
 * Created: 2023-05-02 오후 10:49:34
 * Author : LG
 */ 
#define F_CPU 1600000L
#include <avr/io.h>

void ADC_INIT(unsigned char channel)
{
	ADMUX |= 0x40;
	
	ADCSRA |= 0x07;
	ADCSRA |= (0x01 << ADEN);
	ADCSRA |= (1 << ADATE);
	
	ADMUX |= ((ADMUX & 0xE0) | channel);
	ADCSRA |= (1 << ADSC);
}

int read_ADC(void)
{
	while (!(ADCSRA & (1 << ADIF)));
	return ADC;
}

void PORT_INIT()
{
	DDRD = 0xFF;
	PORTD = 0x00;
}

int main(void)
{
	int value;
	uint8_t on_off;
	
	ADC_INIT(0);
	PORT_INIT();
    while (1) 
    {
		value = read_ADC() >> 7;
		on_off = (0x01 << value);
		PORTD = on_off;
    }
	
}

