/* C program code for timer0A to output to p1.6 PWM  	*
 * using timerA0 to flash LED 		*
 * 					*/

#include <msp430.h>

int a[5] = { 0, 8, 32, 64, 255 };
static volatile int i = 0;

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; 	/* To disable WATCHDOG TIMER */
	P1DIR |= BIT6;
	P1SEL |= BIT6;			/* to select the function TA0.1 */

	TA0CCR0 = 512-1;		/* the value 100 */

	TA0CCTL1 |= OUTMOD_7 ;		/* set up/down mode */
	TA0CCR1  = a[0];
	TA0CTL = TASSEL_2 + MC_1 + ID_3;	/* set the clock source ACLK(12KhZ) */


	TA1CCR0 |= 65530-1;
	TA1CCTL0 |= CCIE;
	TA1CTL  |= TASSEL_2 + MC_1;

	_bis_SR_register(LPM0_bits + GIE);

	while(1);
}

__attribute__((interrupt(TIMER1_A0_VECTOR))) void Timer1(void)
{

	TA0CCR1 = a[++i];

	if(i > 4)
	{
		i = 0;
	}
}

