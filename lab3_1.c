/* This lab3_1.c is from Launchpad_EUSN
 * Running the CPU on the Crystal frequency 32768Hz, about 
 * three times faster than the VLO = 12000Hz.
 */
#include <msp430.h>

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; /* Stop watchdog timer */

	P1DIR = BIT6 + BIT0;
	P1OUT |= BIT0;

	BCSCTL3 |= LFXT1S_0;	/* LFXT1 = 32768 crystal */


	while(IFG1 & OFIFG == 1){

		IFG1 &= ~OFIFG;
		__delay_cycles(100000);
	}

/*	__delay_cycles(1000); 

	IFG1 &= ~OFIFG; */

	P1OUT &= ~BIT0;

	__bis_SR_register(SCG1 + SCG0);	/* Stop DCO */

	BCSCTL2 |= SELM_3 + DIVM_3;	/* MCLK = 32768/8 */

	while(1){
		P1OUT |= BIT6;
		__delay_cycles(20000);
		P1OUT &= ~BIT6;
		__delay_cycles(20960);
	}
}




