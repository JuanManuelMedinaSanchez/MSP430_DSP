
 /*ADC TEST -> OUTPUT PORT DIGITAL ACTIVITY WHEN ADC12 IS ENABLE
 * Processor: MSP430F233
 * Compiler:  GCC for MSP430
 * Scripted by Juan Manuel Medina -> ingenieria@prismatiklab.com */


#include  <msp430f233.h>
#define ADC_RESULT_SIZE 8 // Number of samples to store (should match the number of output pins used)

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

  // Configure P1 as output
  P1DIR |= 0xFF; // P1.0 to P1.7 as output

  while (1)
  {
    ADC12CTL0 |= ADC12SC;               // Start conversion
    while ((ADC12IFG & ADC12BUSY) == 0)
      ;
    adcResults[i] = ADC12MEM0;          // Store ADC result in the array
    i = (i + 1) % ADC_RESULT_SIZE;      // Increment index with wrap-around

    // Output the ADC result on P1 pins
    P1OUT = adcResults[i];

    _NOP();                            // SET BREAKPOINT HERE
  }
}
