#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/crc16.h>
#include <stdio.h>

// generate and store program data:
uint16_t data[10];
void storeData()
{
	for(int i = 0; i < 10; i++)
	{
		if(i % 2 == 0)
		{
			data[i] PROGMEM = 0;
		}
		else
		{
			data[i] PROGMEM = (i & 0xFF);
		}
	}
}

// load program data into temp buffer:
uint16_t buffer[10];
void readData()
{
	for(int i = 0; i < 10; i++)
	{
		buffer[i] = (uint16_t)pgm_read_byte(&(data[i]));
	}
}

uint16_t crc16_update(uint16_t crc, uint8_t a)
{
	int i;
	crc ^= a;
	for(i = 0; i < 8; i++)
	{
		if(crc & 1)
		    crc = (crc >> 1) ^ 0xA001;
		else
		    crc = (crc >> 1);
	}
	return crc;
}

// CRC driver:
int checkcrc()
{
	uint8_t crc = 0, i;
	for(i = 0; i < sizeof buffer[0]; i++)
	{
		crc = crc16_update(crc, buffer[i]);
	}
	return crc;
}

int main(void)
{
	storeData();
	readData();
	printf("%d", checkcrc());
	while(1)
	{
	}
}
