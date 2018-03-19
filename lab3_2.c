/* This lab3_2.c is from Launchpad_EUSN
 * Running the CPU on the DCO and the Crystal, we 
 * to calibrate it.
 */
#include <msp430.h>

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; /* Stop watchdog timer */

	if(CALBC1_1MHZ == 0xff || CALDCO_1MHZ == 0xff){
		while(1);
	}
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	P1DIR = BIT6 + BIT0;
	P1OUT |= BIT0;

	BCSCTL3 |= LFXT1S_0;	/* LFXT1 = 32768 crystal */


	while(IFG1 & OFIFG == 1){
		IFG1 &= ~OFIFG;
		__delay_cycles(1000000);
	}
/*
	__delay_cycles(1000);

	IFG1 &= ~OFIFG;
*/
	P1OUT &= ~BIT0;

/*	__bis_SR_register(SCG1 + SCG0);	 */

	BCSCTL2 |= SELM_0 + DIVM_3;	/* MCLK = DCO/8 */

	while(1){
		P1OUT |= BIT6;
		__delay_cycles(1000000);
		P1OUT &= ~BIT6;
		__delay_cycles(1000000);
	}
}




