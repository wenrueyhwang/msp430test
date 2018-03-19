/* exam9_1.c for performing USCI UART using P1.1 and P1.2	*
 * as Rx and Tx. The USCI SMCLK is selected as clock 		*
 * source, and the clock prescaler value in the baud 		*
 * control register is set to 104 to have 1MHz and 		*
 * baud rate of 9600. 				 		*
 * to learn and understand the function of USCI UART.		*/

#include <msp430.h>

char UARTRxData[20];
volatile char UARTRxFlag = 0;
volatile char i = 0;

void Setup_HW(void);
void Setup_UART(void);

void UARTSendChar(char );
void UARTSendArray(char* , char);
void UARTSendString(char* );

void main(void)
{
	Setup_HW();
	Setup_UART();
	UARTSendChar('a');
	UARTSendString("Hello World");
	UARTSendString("\r\n");

	while(1)
	{
		if(UARTRxFlag)
		{
			UARTSendArray(UARTRxData, UARTRxFlag);
			UARTSendString("\r\n");
		}
	__bis_SR_register(CPUOFF+GIE);
	}
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
	UCA0BR1 = 0;
	/* Modulation Control Register UCBRS0 = 1 */
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
void UARTSendChar(char character)
{
	/* Send a single byte using the hardware UART 0 */
	/* Example usage: UARTSendChar('a'); */
	while(!(IFG2 & UCA0TXIFG)); 	/* Wait for TX buffer to be ready */
	UCA0TXBUF  = character;
}
void UARTSendArray(char *TxArray, char ArrayLength)
{
	/*Send number of bytes Specified in ArrayLength in the array
	at using the hardware UART 0.
	Example usage: UARTASendArray("Hello", 5);
	int data[2] = {1023, 235};
	UARTSendArray(data, 4); */
	while(ArrayLength--)
	{
		while(!(IFG2 & UCA0TXIFG));
		UCA0TXBUF = *TxArray++;
	}
}
void UARTSendString(char *TxArray)
{
	while(*TxArray)
	{
		while(!(IFG2 & UCA0TXIFG));
		UCA0TXBUF = *TxArray++;
	}
}
/* UART receive interrupt service routine */
/* #pragma vector = USCI0RX_VECTOR __interrupt void rart(void) */

__attribute__((interrupt(USCIAB0RX_VECTOR))) void uart(void)
{
        UARTRxData[i] = UCA0RXBUF; /* Read USART data register */
        if(UARTRxData[i++] == '\r')
        {
                UARTRxFlag = --i;
                i = 0;
                __bis_SR_register_on_exit(CPUOFF);
        }
	P1OUT ^= BIT6;
}


