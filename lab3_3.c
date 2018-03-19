/* This lab3_3.c is from Launchpad_EUSN
 * Running the CPU on the DCO without a Crystal, we 
 * to calibrate it.
 */
#include <msp430.h>

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; /* Stop watchdog timer */

	if(CALBC1_1MHZ == 0xff || CALDCO_1MHZ == 0xff){
		while(1);
	}
	BCSCTL1 = CALBC1_1MHZ;	/* set range */
	DCOCTL = CALDCO_1MHZ;	/* set DCO step + modulation */

	P1DIR = BIT6 + BIT0;
	P1OUT |= BIT0;

	BCSCTL3 |= LFXT1S_2;	/* LFXT1 = VLO */

	IFG1 &= ~OFIFG;

	P1OUT &= ~BIT0;

	BCSCTL2 |= SELM_0 + DIVM_3;	/* MCLK = DCO/8 = 125kHz */

	while(1){
		P1OUT |= BIT6;
		__delay_cycles(600000);
		P1OUT &= ~BIT6;
		__delay_cycles(650000);
	}
}




