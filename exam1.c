/* MSP430G2 Lanuchpad */
/* to Blink an LED with GPIO */
#include <msp430.h>

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;
	P1DIR |= 0x41;

	P1OUT |= 0x01; 	/* Mask the p1.0 output = 1 */
	P1OUT &= ~0x40; /* Mask the p1.6 output = 0 */

	while(1)
	{
		volatile unsigned long i;
		P1OUT ^= 0x41;
		__delay_cycles(500000);
/*
		i = 500000;
		do
		{
			 i--;
		}
		while(i != 0);
*/
	}
}

