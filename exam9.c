/* exam9.c for performing USCI UART using P1.1 and P1.2 *
 * as Rx and Tx. The USCI SMCLK is selected as clock 	*
 * source, and the clock prescaler value in the baud 	*
 * control register is set to 104 to have 1MHz and 	*
 * baud rate of 9600. 				 	*
 * to learn and understand the function of USCI UART.	*/

#include <msp430.h>

void Setup_HW(void);
void Setup_UART(void);

void main(void)
{
	Setup_HW();
	Setup_UART();
	while(1);

}

/* UART receive interrupt service routine */
__attribute__((interrupt(USCIAB0RX_VECTOR))) void uart(void)
{
	while(!(IFG2 & UCA0TXIFG)); /* USCI_A0 TX buffer ready? */
	UCA0TXBUF = UCA0RXBUF;
	P1OUT ^= BIT6;
	__bis_SR_register_on_exit(CPUOFF);
}
void Setup_HW(void)
{
	WDTCTL = WDTPW + WDTHOLD;	/* Stop WDT */
	if(CALBC1_1MHZ == 0xff) while(1);
	/* Load calibrated DCO values to set CPU clock to 1MHZ */
	DCOCTL = 0;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	/* Configure P1.1 AND P1.2 as Rx and Tx */
	P1SEL = BIT1 + BIT2;
	P1SEL2 = BIT1 + BIT2;
	/* Configure P1.6 as an indicator for receiving interrupt */
	P1DIR = BIT6;
	P1OUT = BIT6;
}
void Setup_UART(void)
{
	/* USCI clock source select. CLK = SMCLK */
	UCA0CTL1 |= UCSSEL_2;
	/* Baud Rate control register 0 */
	/* 1MHZ/9600 = 104 */
	/* 16-bit value of (UCA0BR0 + UCA0BR1 * 256) */
	/* 9600 = 1MHZ/104 */
	UCA0BR0 = 104;
	UCA0BR1 = 0x00;
	/* Modulation Control Register */
	UCA0MCTL = UCBRS0;
	/* Software reset enable */
	/* Disable -- USCI reset released for operation */
	UCA0CTL1 &= ~UCSWRST;
	/* Interrupt Enable register 2 */
	/* there are USCI_A0 transmit interrupt enable UCA0TXIE */
	/* USCI_A0 receive interrupt enable UCA0RXIE */
	IE2 |= UCA0RXIE;
	__bis_SR_register(LPM1_bits + GIE);/* Low Power Mode0, global interrupt */

}

