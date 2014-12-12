/***
 * Sean Gavan
 * 8 Dec. 14
 * Lab 7 -- Detecting walls with IR sensors
 * For this lab we were to use the IR transmitter/receiver pairs to detect when obstacles were near the bot.
 * Documentation: I asked Capt. Trimble on multiple occassion how to fix some issues with the green LED
 * always staying on.
 */
#include <msp430.h> 

/*
 * main.c
 */
int THRESHOLDLEFT = 0x3B0;		// Threshold voltage for wall prescence
int THRESHOLDRIGHT = 0x0FF;
int THRESHOLDCENTER = 0x3D0;

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;				  // Stop watchdog timer
    ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE; 		  // 64 ADC clks to combat loading, ADC10ON
    ADC10AE0 |= BIT1 + BIT2 + BIT4;           // P1.1, P1.2, P1.4 to ADC mode (A1,A2,A4)
    ADC10CTL1 |= ADC10SSEL1|ADC10SSEL0;       // Select SMCLK
    P1DIR |= BIT0 + BIT6;

    for (;;) {
    	ADC10CTL1 = INCH_4;				// Right receiver A4
    	ADC10CTL0 |= ENC + ADC10SC;
    	__bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
    	if (ADC10MEM > THRESHOLDRIGHT) {
        	P1OUT |= BIT6;				// P1.6 on
     	} else {
     		P1OUT &= ~BIT6;				// P1.6 off
     	}

    	ADC10CTL1 = INCH_1;				// Left receiver A1
    	ADC10CTL0 |= ENC + ADC10SC;     // Sampling and conversion start
    	 __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
    	if (ADC10MEM < THRESHOLDLEFT) {		// Wall close (less V when wall close)
    		P1OUT |= BIT0;             // P1.0 LED on
    		P1OUT &= ~BIT6;
    	} else {						// Wall close
    		P1OUT &= ~BIT0;              // P1.0 LED off
    		P1OUT &= ~BIT6;
    	}

    	ADC10CTL1 = INCH_2;				// Center receiver A2
    	ADC10CTL0 |= ENC + ADC10SC;
    	 __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
    	if (ADC10MEM < THRESHOLDCENTER) {
    		P1OUT |= BIT0 + BIT6;				// Both on
    	} else {
    		P1OUT &= ~(BIT0 + BIT6);			// Both off
    	}
   }
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	ADC10CTL0 &= ~ENC;
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}
