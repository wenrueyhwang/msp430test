/* This is a program to test the Tag_Length_Value from 0x10c2 	*
 * using memcpy(destination, soruce, size); */

#include <string.h>
#include <stdint.h>
#include <msp430.h>

static uint16_t *data;
static uint16_t _calculate_checksum(uint16_t *address, size_t len);
int tlv_verify(void);
void move(void);

void main(void)
{
	move();
	while(tlv_verify() != 0);
}
void move(void)
{
	memcpy(data, (uint16_t*)0x10c2, 31);
}
int tlv_verify(void)
{
	return(TLV_CHECKSUM + _calculate_checksum((uint16_t*)0x10c2, 62));
}

static uint16_t _calculate_checksum(uint16_t *data, size_t len)
{
	uint16_t crc=0;

	len = len/2;
	while(len-- >0)
	{
		crc ^= *(data++);
	}
	return crc;
}
