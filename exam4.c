/* C program code for interrupt Programming with GPIO */
/* exam4.c */
/* using p1.6 to output for green LED */
/* setting P1.3 as a input from switch of MSP-EXP430G2 */

#include <msp430.h>

/* static volatile int _blinking; */

void main(void)
{
	/* _blinking = 0; */

	WDTCTL = WDTPW + WDTHOLD; /* TO STOP WATCHDOG */
	P1DIR |= BIT6; /* setting P1.6 as an output of green LED */
	P1REN |= BIT3; /* pull-up resistor of input pin P1.3 */
	P1OUT |= BIT3;
	P1IES |= BIT3; /* P1.3 high to low edge */
	P1IFG &= ~BIT3;
	P1IE |= BIT3;

	_bis_SR_register(LPM4_bits + GIE); /* Enter LPM4 W/ interrupt */

/*
	while(_blinking > 0)
	{
		P1OUT ^= BIT6;
		__delay_cycles(5000);
	}
*/
	_no_operation(); /* for debugger */
}
__attribute__((interrupt(PORT1_VECTOR))) void Port_1(void)
{
/*	_blinking ^= 1; */

	P1OUT ^= BIT6;

	P1IFG &= ~BIT3;
}
 
