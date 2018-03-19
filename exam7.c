/* C program code for performing P1.3 as ADC input  	*
 * and P1.6 as timer A output to drive a DC motor 	*
 * to learn the difference functions of ADC.		*/

#include <msp430.h>

void Setup_HW(void);

void main(void)
{
	Setup_HW();
	while(1);

}

/* TIMER0 interrupt service routine */
__attribute__((interrupt(TIMER0_A1_VECTOR))) void timer(void)
{
	TA0CTL &=~TAIFG;
	ADC10CTL0 |= ENC + ADC10SC; /* Start ADC Acquisition */
	TA0CCR1 = ADC10MEM;
	ADC10CTL0 &= ~ENC;
	__bis_SR_register_on_exit(CPUOFF);
}
void Setup_HW(void)
{
	WDTCTL = WDTPW + WDTHOLD;	/* Stop WDT */
	/* Load calibrated DCO values to set CPU clock to 1MHZ */
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	/* Configure ADC */
	/* 1.5V Ref, Ref on, Enable Ref Output, 16 Clocks for sample */
	ADC10CTL0 = SREF_0+ADC10ON+ADC10SHT_2;
	ADC10CTL1 = INCH_3; 	/* Input channel select ADC A3 */
	ADC10AE0 |= BIT3;	/* Set P1.3 as ADC inputs */

	/* Configure Output pin for LED */
	P1DIR |= BIT6;
	P1SEL |= BIT6;	/* Select output P1.6 to be timer ouput TA0.1 */
	P1SEL2 &= ~BIT6;

	/* Configure timer A as a PWM */
	/* SMCLK as input clock, count up to TA0CCR0, clock/1 */
	TA0CTL = TASSEL_2+MC_1+ID_3+TAIE;
	/* Set maximum count value to determine count frequency
	= SMCLK/ClockDivide/TACCRO(1MHZ/1/1024 = 0.97KHZ) */
	TA0CCR0 = 1024;
	/* Initialize counter compare value 100 to countrol duty Cycle
	 = TA0CCR1/TACCR0 (100/1000) = 10% */
	TA0CCR1 = 400;
	/* Set output to on when counter resets and off when counter 
	equals TACCR1. Normal PWM. */
	TA0CCTL1 = OUTMOD_7;
/*
	TA1CCR0 = 400;
	TA1CCTL0 = CCIE;
	TA1CTL = TASSEL_2+MC_1;
*/
	__bis_SR_register(LPM1_bits + GIE);/* Low Power Mode0, global interrupt */

}

