/* MSP430G2 Lanuchpad */
/* to Blink an LED with GPIO P1.0 and P1.6 */
/* using switch of PGIO P1.3 to control P1.0 and P1.6 */

#include <msp430.h>

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;
	P1DIR |= 0x41;
	P1REN |= BIT3;
	P1OUT |= BIT3;

	P1OUT |= 0x01; 	/* Mask the p1.0 output = 1 */
	P1OUT &= ~0x40; /* Mask the p1.6 output = 0 */

	while(1)
	{
		if((P1IN & BIT3))
		{

			P1OUT &= ~0x01;
			P1OUT |= 0x40;
		}
		else
		{
			P1OUT |= 0x01;
			P1OUT &= ~0x40;
		}

	}
}

