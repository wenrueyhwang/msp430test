/* C program code for performing 3 sepatate GPIO pins repeatedly*
 * from P1.0 to P1.2						*/

#include <msp430.h>

int adc[3] = {0};
int X_Axis, Y_Axis, Z_Axis;

/* function prototypes */
void Setup_HW(void);
void Read_Acc(void);

void main(void)
{
	Setup_HW();
	while(1)
	{

		Read_Acc();
	}

}

/* ADC10 interrupt service routine */
__attribute__((interrupt(ADC10_VECTOR))) void ADC10_ISR(void)
{
	__bis_SR_register_on_exit(CPUOFF);
	P1OUT  ^= BIT6;
}
void Setup_HW(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	ADC10CTL1 = INCH_2 + CONSEQ_1;
	/* for highest channel of sequence conversion */
	ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
	ADC10DTC1 = 0x03; 	/* 3 conversion */
	ADC10AE0 |= 0x03;
	P1DIR |= BIT6;

}
void Read_Acc(void)
{
	ADC10CTL0 &= ~ENC;
	while(ADC10CTL1 & BUSY);	/* Wait if ADC10 busy */
	ADC10SA = (unsigned int)adc;		/* Transfers data to next array */
					/* (DTC auto increment address) */
	ADC10CTL0 |= ENC + ADC10SC; 	/* Enable Conversion */
					/* and conversion start */
	X_Axis = adc[0];
	Y_Axis = adc[1];
	Z_Axis = adc[2];

	__bis_SR_register(CPUOFF + GIE);/* Low Power Mode0, ADC10_ISR */
}

