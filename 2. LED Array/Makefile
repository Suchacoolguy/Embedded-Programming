default:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o file.o file.c
	avr-gcc -o file.bin file.o
	avr-objcopy -O ihex -R .eeprom file.bin file.hex
	sudo avrdude -F -V -c arduino -p ATMEGA328P -P /dev/tty.usbmodem101 -b 115200 -U flash:w:file.hex
