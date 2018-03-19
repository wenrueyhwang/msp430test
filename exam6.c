/* C program code for interfacing potentiometer 	*
 * using ADC10 converter to control p1.0 Red LED	*
 * msp430G2553 launchpad from book Embedded System desing.*/

#include <msp430.h>


void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; 	/* To disable WATCHDOG TIMER */
	P1DIR |= BIT0;			/* P1.0 RED LED */

	ADC10CTL0 = SREF_0 + ADC10SHT_2 + ADC10ON;
	ADC10CTL1 = INCH_3;		/* Input A3 is P1.3 */
	ADC10AE0 |= BIT3;		/* Analog input enable A3 */

	while(1)
	{
		ADC10CTL0 |= ENC + ADC10SC; 	/* ONE SET OF enable conversion */
						/* and Start Conversion */
		if( ADC10MEM < 512)
			P1OUT &= ~BIT0;
		else
			P1OUT |= BIT0;
	}
}

