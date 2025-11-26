default: build

build:
	avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os main.c -o main.elf
	avr-objcopy -O ihex main.elf main.hex
	avrdude -F -V -c arduino -p ATMEGA328P -c arduino -P /dev/ttyACM0 -b 115200 -U flash:w:main.hex
