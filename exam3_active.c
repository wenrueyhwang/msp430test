/* C program code for Active mode */
/* main_active.c */
#include <msp430.h>

#ifndef TIMER0_A1_VECTOR
#define TIMER0_A1_VECTOR TIMERA1_VECTOR
#define TIMER0_A0_VECTOR TIMERA0_VECTOR
#endif

volatile long tempRaw;
volatile unsigned int i;

void FaultRoutine(void);
void ConfigWDT(void);
void ConfigClocks(void);
void ConfigPins(void);
void ConfigADC10(void);
void ConfigTimerA2(void);

void main(void)
{
	ConfigWDT();
	ConfigClocks();
	ConfigPins();
	ConfigADC10();
	ConfigTimerA2();

	_BIS_SR(GIE);

	while(1)
	{
		for(i = 100; i>0; i--);
		for(i = 5000; i>0; i--);
	}
}
void ConfigWDT(void)
{
	WDTCTL = WDTPW + WDTHOLD;
}
void ConfigClocks(void)
{
	if(CALBC1_1MHZ == 0xff || CALDCO_1MHZ == 0xff)
		FaultRoutine();
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	BCSCTL3 |= LFXT1S_2;
	IFG1 &= ~OFIFG;
	BCSCTL2 |= SELM_0 + DIVM_3 + DIVS_3;
}
void FaultRoutine(void)
{
	P1OUT = BIT0;
	while(1);
}
void ConfigPins(void)
{
	P1DIR = ~BIT3;
	P1OUT = 0;

	P2SEL |= ~(BIT6 + BIT7);

	P2DIR |= BIT6 + BIT7;
	P2OUT = 0;
}
void ConfigADC10(void)
{
	ADC10CTL1 = INCH_10 + ADC10DIV_0;
}
void ConfigTimerA2(void)
{
	CCTL0 = CCIE;
	CCR0 = 36000;
	TACTL = TASSEL_1 + MC_2;
}

__attribute__((interrupt(TIMER0_A0_VECTOR))) void Timer_A(void)
{
	ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON;
	__delay_cycles(4);
	ADC10CTL0 |= ENC + ADC10SC;
	P1OUT |= BIT6;
	__delay_cycles(100);
	ADC10CTL0 &= ~ENC;
	ADC10CTL0 &= ~(REFON + ADC10ON);
	tempRaw = ADC10MEM;
	P1OUT &= ~BIT6;
	CCR0 += 36000;
}

