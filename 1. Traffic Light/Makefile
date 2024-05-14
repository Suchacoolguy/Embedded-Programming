default:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o hw1.o hw1.c
	avr-gcc -o hw1.bin hw1.o
	avr-objcopy -O ihex -R .eeprom hw1.bin hw1.hex
	sudo avrdude -F -V -c arduino -p ATMEGA328P -P /dev/tty.usbmodem1101 -b 115200 -U flash:w:hw1.hex
