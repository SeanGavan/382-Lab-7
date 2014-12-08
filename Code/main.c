#include <msp430.h> 

/*
 * main.c
 */
int THRESHOLDLEFT = 0x3B0;		// Threshold voltage for wall prescence
int THRESHOLDRIGHT = 0x120;
int THRESHOLDCENTER = 0x3D0;
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;				  // Stop watchdog timer
    ADC10CTL0 = ADC10SHT_3 + ADC10ON; 		  // 64 ADC clks to combat loading, ADC10ON
    ADC10AE0 |= BIT1 + BIT2 + BIT4;           // P1.1, P1.2, P1.4 to ADC mode (A1,A2,A4)
    ADC10CTL1 |= ADC10SSEL1|ADC10SSEL0;       // Select SMCLK
    P1DIR = BIT0 + BIT6;
    P1OUT &= ~BIT0;
    P1OUT &= ~BIT6;

    for (;;) {
    	ADC10CTL1 = INCH_1;				// Left receiver A1
    	ADC10CTL0 |= ENC + ADC10SC;     // Sampling and conversion start
    	if (ADC10MEM < THRESHOLDLEFT) {		// Wall not close (less V when wall close)
    		P1OUT |= BIT0;             // P1.0 LED off
    	} else {						// Wall close
    		P1OUT &= ~BIT0;              // P1.0 LED on
    	}
    	ADC10CTL0 &= ~ENC;

    	ADC10CTL1 = INCH_4;				// Right receiver A4
    	ADC10CTL0 |= ENC + ADC10SC;
    	if (ADC10MEM > THRESHOLDRIGHT) {	// Wall not close (more V when wall close)
    		P1OUT |= BIT6;             // P1.3 LED off
    	} else {
    		P1OUT &= ~BIT6;              // P1.3 LED on
    	}
    	ADC10CTL0 &= ~ENC;

    	ADC10CTL1 = INCH_2;				// Center receiver A2
    	ADC10CTL0 |= ENC + ADC10SC;
    	if (ADC10MEM < THRESHOLDCENTER) {
    		P1OUT |= BIT0 + BIT6;				// Both off
    	} else {
    		P1OUT &= ~BIT0 + ~BIT6;				// Both on
    	}
    	ADC10CTL0 &= ~ENC;

   }
}
