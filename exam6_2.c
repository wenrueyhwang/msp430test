/* C program code for performing a repeated read from GPIO P1.3 *
 * then using the DTC, it will copy the value into an array. 	*
 * The DTC automatically increments the address of the array.	*
 * The integer array containing the value is the added together *
 * and divided by 10 to give an average value, avg_adc.*/

#include <msp430.h>

int adc[10] = {0};
int avg_adc = 0;

/* function prototypes */
void adc_Setup(void);
void adc_Sam10(void);
void InitialSet(void);

void main(void)
{

	InitialSet();
	adc_Setup();

	while(1)
	{

		adc_Sam10();
		/* Add all the sampled data and divide by 10 to find average */
		avg_adc = adc[0]+adc[1]+adc[2]+adc[3]+adc[4]+ \
			adc[5]+adc[6]+adc[7]+adc[8]+adc[9];
	}

}
/* to find the average value */

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
	P1SEL |= BIT3;

}
/* ADC set-up function */
void adc_Setup(void)
{
	ADC10CTL1 = CONSEQ_2 + INCH_3 ;	/* Repeat single Channel 3 */
	ADC10CTL0 = SREF_0 + ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
	ADC10DTC1 = 0x0A; 		/* 10 conversions */
	ADC10AE0 |= BIT3;		/* P1.3 ADC Option select */
}
/* ADC sample conversion function */
void adc_Sam10(void)
{
	ADC10CTL0 &= ~ENC; 		/* Disable Conversion */
	while(ADC10CTL1 & BUSY);	/* Wait if ADC10 busy */
	ADC10SA = (int)adc;		/* Transfers data to next array */
					/* (DTC auto increment address) */
	ADC10CTL0 |= ENC + ADC10SC; 	/* Enable Conversion */
					/* and conversion start */
	__bis_SR_register(CPUOFF + GIE);/* Low Power Mode0, ADC10_ISR */
}

