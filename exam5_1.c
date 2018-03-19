/* C program code for PWM generation 	*
 * using timer to flash LED 		*
 * 					*/

#include <msp430.h>

static volatile int control = 0;

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; 	/* To disable WATCHDOG TIMER */
	P1DIR |= BIT0;
	P1DIR |= BIT6;
	P1SEL &= ~BIT0;
	P1SEL &= ~BIT6;

	TA0CCR0 = 100-1;		/* the value 100 */
	TA0CCTL0 |= CCIE ;		/* turn interrupt on of TA0CCR0 */
	TA0CTL = TASSEL_1 + MC_1;	/* set the clock source ACLK(12KhZ) */

	_bis_SR_register(LPM0_bits + GIE);
	while(1);
}
__attribute__((interrupt(TIMER0_A0_VECTOR))) void timer(void)
{
	TA0CCTL0 &= ~CCIFG;
	control++;
	P1OUT ^= BIT0;
	if(control == 10)
	{
		P1OUT ^= BIT6;
		control = 0;
	}
}

