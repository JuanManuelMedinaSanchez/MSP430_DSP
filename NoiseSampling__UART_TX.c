*/Sampling noise audio signal with ADC12 module and MSP430f233/*
*/Audio processing, Acoustics and other sensoric and IoT implementations involved/*
*/Designed by Juan Manuel Medina, ingenieria@prismatiklab.com /*

#define ADC_RESULT_SIZE 8 // Number of samples to store (should match the number of output pins used)

void UART_init(void)
{
    // Configure UART pins (P3.4 = TXD)
    P3SEL |= BIT4;              // Set P3.4 to UART mode
    P3DIR |= BIT4;              // Set P3.4 as output

    // Configure UART settings (9600 baud, 8 data bits, no parity, 1 stop bit)
    UCA0CTL0 = 0;
    UCA0CTL1 |= UCSSEL_2;       // Use SMCLK as clock source
    UCA0BR0 = 104;              // Set baud rate to 9600 (SMCLK = 1MHz)
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;          // Modulation UCBRSx = 1

    UCA0CTL1 &= ~UCSWRST;       // Enable UART module
}

void UART_sendByte(unsigned char data)
{
    while (!(IFG2 & UCA0TXIFG));    // Wait until TX buffer is ready for new data
    UCA0TXBUF = data;               // Send data
}

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;          // Stop watchdog timer
    P6SEL |= 0x01;                     // Enable A/D channel A0
    ADC12CTL0 = ADC12ON + SHT0_2;      // Turn on ADC12, set sampling time
    ADC12CTL1 = SHP;                   // Use sampling timer
    ADC12MCTL0 = SREF_0;               // Vr+ = VREF+ (internal)
    ADC12CTL0 |= ENC;                  // Enable conversions

    unsigned int adcResults[ADC_RESULT_SIZE]; // Array to store ADC results
    unsigned int i = 0;                        // Index variable

    // Initialize UART
    UART_init();

    while (1)
    {
        ADC12CTL0 |= ADC12SC;               // Start conversion
        while ((ADC12IFG & ADC12BUSY) == 0)
            ;
        adcResults[i] = ADC12MEM0;          // Store ADC result in the array
        i = (i + 1) % ADC_RESULT_SIZE;      // Increment index with wrap-around

        // Send the ADC result over UART
        UART_sendByte(adcResults[i] & 0xFF);

        _NOP();                            // SET BREAKPOINT HERE
    }
}
