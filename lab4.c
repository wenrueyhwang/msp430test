/* This lab4.c is from Launchpad_EUSN
 * Running the ADC from Temperature Sensor.
 */
#include <msp430.h>
#include <stddef.h>

volatile long tempRaw;

void FaultRoutine(void);

void main(void)
{
	volatile size_t i;

	WDTCTL = WDTPW + WDTHOLD; /* Stop watchdog timer */
	if(CALBC1_1MHZ == 0xff || CALDCO_1MHZ == 0xff)
		FaultRoutine();

	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	P1DIR = BIT6 + BIT0;
	P1OUT = 0;

	BCSCTL3 |= LFXT1S_2;	/* LFXT1 = VLO */
	IFG1 &= ~OFIFG;
	BCSCTL2 |= SELM_0 + DIVM_3;	/* MCLK = DCO/8 */

	while(1){
		ADC10CTL1 = INCH_10 + ADC10DIV_0;
		ADC10CTL0 = SREF_1 + ADC10SHT_3 +REFON+ADC10ON;
		__delay_cycles(5);

		ADC10CTL0 |= INCH_10 + ADC10DIV_0;
		P1OUT |= BIT6;
		__delay_cycles(100);

		ADC10CTL0 &= ~ENC;
		ADC10CTL0 &= ~(REFON + ADC10ON);
		tempRaw = ADC10MEM;
		P1OUT &= ~BIT6;
		__delay_cycles(125000);
	}
}
void FaultRoutine(void)
{
	P1OUT = BIT0;
	while(1);
}




