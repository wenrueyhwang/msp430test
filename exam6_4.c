/* C program code for performing P1.4 as reference+ output 	*
 * and P1.5 as ADC input 					*
 * to learn the difference functions of ADC.			*/

#include <msp430.h>

void Setup_HW(void);

void main(void)
{
	Setup_HW();
	while(1)
	{

	}

}

/* ADC10 interrupt service routine */
__attribute__((interrupt(ADC10_VECTOR))) void ADC10_ISR(void)
{
	/* read ADC value (Note this is a 10 bit value stored in 16bit */
	TA0CCR1 = ADC10MEM;
	ADC10CTL0 &= ~ENC;
	__bis_SR_register_on_exit(CPUOFF);
}
/* TIMER0 interrupt service routine */
__attribute__((interrupt(TIMER0_A0_VECTOR))) void Timer_A0_ISR(void)
{
	ADC10CTL1 = INCH_5; /* Select ADC Channel */
	ADC10CTL0 |= ENC + ADC10SC; /* Start ADC Acquisition */
	__bis_SR_register_on_exit(CPUOFF);
}
void Setup_HW(void)
{
	WDTCTL = WDTPW + WDTHOLD;	/* Stop WDT */
	/* Load calibrated DCO values to set CPU clock to 1MHZ */
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	/* Configure ADC */
	/* 1.5V Ref, Ref on, Enable Ref Output, 64 Clocks for sample */
	ADC10CTL0 = SREF_1+REFON+REFOUT+ADC10ON+ADC10SHT_3+ADC10IE;
	ADC10CTL1 = ADC10DIV_3; 	/* clock/4 */
	ADC10AE0 |= BIT4 + BIT5;	/* Set P1.4 and P1.5 as ADC inputs */

	/* Configure Output pin for LED */
	P1DIR |= BIT6;
	P1SEL |= BIT6;	/* Select output P1.6 to be timer ouput TA0.1 */
	P1SEL2 &= ~BIT6;

	/* Configure timer A as a PWM */
	/* SMCLK as input clock, count up to TA0CCR0, clock/1 */
	TA0CTL = TASSEL_2+MC_1+ID_0;
	/* Set maximum count value to determine count frequency
	= SMCLK/ClockDivide/TACCRO(1MHZ/1/1024 = 0.97KHZ) */
	TA0CCR0 = 1000;
	/* Set output mode 0, enable CCR0 interrupt */
	TA0CCTL0 = OUTMOD_0 + CCIE;
	/* Initialize counter compare value 100 to countrol duty Cycle
	 = TA0CCR1/TACCR0 (100/1000) = 10% */
	TA0CCR1 = 100;
	/* Set output to on when counter resets and off when counter 
	equals TACCR1. Normal PWM. */
	TA0CCTL1 = OUTMOD_7;
	__bis_SR_register(CPUOFF + GIE);/* Low Power Mode0, ADC10_ISR */
}

