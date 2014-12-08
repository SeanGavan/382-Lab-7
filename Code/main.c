#include <msp430.h> 

/*
 * main.c
 */
int THRESHOLD = 0x0200;		// Threshold voltage for wall prescence (2.5v)
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;				  // Stop watchdog timer
    ADC10CTL0 = ADC10SHT_3 + ADC10ON; 		  // 64 ADC clks to combat loading, ADC10ON
    ADC10AE0 |= BIT1 + BIT2 + BIT4;           // P1.1, P1.2, P1.4 to ADC mode (A1,A2,A4)
    ADC10CTL1 |= ADC10SSEL1|ADC10SSEL0;       // Select SMCLK
    P1DIR |= 0x01 + 0x08;                     // Set P1.0, P1.3 to output direction

    for (;;) {
    	ADC10CTL1 = INCH_1;				// Left receiver A1
    	ADC10CTL0 |= ENC + ADC10SC;     // Sampling and conversion start
    	if (ADC10MEM < THRESHOLD) {		// Wall not close
    		P1OUT &= ~0x01;             // P1.0 LED off
    	} else {						// Wall close
    		P1OUT |= 0x01;              // P1.0 LED on
    	}

    	ADC10CTL1 = INCH_2;				// Right receiver A2
    	ADC10CTL0 |= ENC + ADC10SC;
    	if (ADC10MEM < THRESHOLD) {
    		P1OUT &= ~0x08;             // P1.3 LED off
    	} else {
    		P1OUT |= 0x08;              // P1.3 LED on
    	}

    	ADC10CTL1 = INCH_4;				// Center receiver A4
    	ADC10CTL0 |= ENC + ADC10SC;
    	if (ADC10MEM < THRESHOLD) {
    		P1OUT &= ~0x01;				// Both off
    		P1OUT &= ~0x08;
    	} else {
    		P1OUT |= 0x01;				// Both on
    		P1OUT |= 0X08;
     }
   }
}
