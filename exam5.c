/* C program code for PWM generation 	*
 * using timer to generate a duty cycle	*
 * of a PWM.				*/

#include <msp430.h>

int a[5] = { 0, 32, 64, 128, 255};
int i = 0;

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; 	/* To disable WATCHDOG TIMER */
	P1DIR |= BIT6;
	P1SEL |= BIT6;			/* Green LED PWM */

	TA0CCR0 = 512;			/* PWM period */
	TA0CCR1 = a[0];			/* PWM duty cycle */
	TA0CCTL1 = OUTMOD_7;
	TA0CTL = TASSEL_2 + MC_1 + ID_3;

	TA1CCR0 = 65534;
	TA1CCTL0 = CCIE;
	TA1CTL = TASSEL_2 + MC_1;

	_bis_SR_register(LPM1_bits + GIE);
	while(1);
}
__attribute__((interrupt(TIMER1_A0_VECTOR))) void timer(void)
{

	TA0CCR1 = a[++i];
	if(i > 4)
	{
		 i = 0;
	}
}

