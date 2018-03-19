/* C program code for interfacing potentiometer 	*
 * using ADC10 converter to control p1.0 Red LED	*
 * P1.3 as ADC input ADC10AE0 |= BIT3			*
 * msp430G2553 launchpad from book Embedded System desing.*/

#include <msp430.h>

static volatile unsigned ADC_value = 0;

void ConfigureADC(void);
void InitialSet(void);

void main(void)
{
	InitialSet();
	ConfigureADC();
	__enable_interrupt();

	while(1)
	{
		__delay_cycles(1000);

		ADC10CTL0 |= ENC + ADC10SC; 	/* ONE SET OF enable conversion */
						/* and Start Conversion */
		__bis_SR_register(CPUOFF + GIE);
		ADC_value = ADC10MEM;
	}

}
/* ADC10 interrupt service routine */
__attribute__((interrupt(ADC10_VECTOR))) void ADC10_ISR(void)
{
	__bis_SR_register_on_exit(CPUOFF);
	P1OUT  ^= BIT0;
}

void InitialSet(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	BCSCTL2 &= ~(DIVS_3);
	P1DIR |= BIT0;
	
}
void ConfigureADC(void)
{
	ADC10CTL1 = INCH_3 + ADC10DIV_3;	/* Channel 3, ADC10CLK/3 */
	ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;
	ADC10AE0 |= BIT3;
}

