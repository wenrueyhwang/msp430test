/* This lab3.c is from Launchpad_EUSN
 * Running the CPU on the VLO(Very Low power/Low frequency Osc.
 */
#include <msp430.h>

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; /* Stop watchdog timer */
	P1DIR = BIT6;
	P1OUT = 0;

	BCSCTL3 |= LFXT1S_2;	/* LFXT1 = VLO */
	IFG1 &= ~OFIFG;
	__bis_SR_register(SCG1 + SCG0);	/* Stop DCO */
	BCSCTL2 |= SELM_3 + DIVM_3;	/* MCLK = VLO/8 */

	while(1){
		P1OUT |= BIT6;
		__delay_cycles(500);
		P1OUT &= ~BIT6;
		__delay_cycles(4000);
	}
}




